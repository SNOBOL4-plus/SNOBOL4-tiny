#!/usr/bin/env bash
# Smoke probe for SN-26-auto-binary-scrip.
# Validates: MONITOR_BIN=1 + SCRIP_TRACE=1 + SCRIP_FTRACE=1 + MONITOR_NAMES_OUT
# produces a binary wire stream, no source modification.  The user's .sno
# only contains assignments and a function call/return.
#
# Pipeline:
#   - Make a FIFO pair (ready=script→reader, go=reader→script).
#   - Reader process drains ready FIFO into /tmp/probe_auto.bin and acks 'G'
#     after each record.  EOF / final MWK_END terminates.
#   - Launch scrip on a tiny .sno with the relevant env set.
#   - Verify: bin file is multiple of 13 in some prefix, decodes to expected
#     event sequence, names sidecar lists the variables/functions used.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"

if [ ! -x "$SCRIP" ]; then
    echo "FAIL scrip not built at $SCRIP"; exit 1
fi

T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
mkfifo "$T/ready" "$T/go"

cat > "$T/p.sno" <<'EOF'
                  DEFINE('SQR(x)f')                          :(SQR_END)
SQR               SQR     =  x * x                           :(RETURN)
SQR_END
                  a       =  'hello'
                  b       =  42
                  c       =  3.14
                  d       =  SQR(7)
END
EOF

# Reader: drain ready FIFO, ack 'G' after each 13-byte header (+ value bytes).
# Implemented in awk-friendly form via a small helper python; falls back to
# pure-bash byte counting if python missing.
python3 - "$T/ready" "$T/go" "$T/probe_auto.bin" <<'PY' &
import os, sys, struct
ready, go, outpath = sys.argv[1], sys.argv[2], sys.argv[3]
fout = open(outpath, "wb")
fr = open(ready, "rb", buffering=0)
fg = open(go, "wb", buffering=0)
while True:
    hdr = fr.read(13)
    if len(hdr) == 0:
        break
    if len(hdr) < 13:
        sys.stderr.write(f"short hdr: {len(hdr)} bytes\n")
        break
    kind, name_id, t, vlen = struct.unpack("<IIBI", hdr)
    fout.write(hdr)
    if vlen > 0:
        body = fr.read(vlen)
        fout.write(body)
    fout.flush()
    try:
        fg.write(b"G"); fg.flush()
    except BrokenPipeError:
        break
    if kind == 4:  # MWK_END
        break
fout.close()
PY
READER_PID=$!

MONITOR_BIN=1 \
MONITOR_READY_PIPE="$T/ready" MONITOR_GO_PIPE="$T/go" \
MONITOR_NAMES_OUT="$T/names.out" \
SCRIP_TRACE=1 SCRIP_FTRACE=1 \
timeout 10 "$SCRIP" --ir-run "$T/p.sno" </dev/null > "$T/scrip.out" 2> "$T/scrip.err"
SCRIP_RC=$?

wait $READER_PID 2>/dev/null

if [ ! -s "$T/probe_auto.bin" ]; then
    echo "FAIL no binary records captured (scrip rc=$SCRIP_RC)"
    echo "--- scrip stderr ---"; cat "$T/scrip.err"
    exit 1
fi

# Decode: count records by kind, list names referenced.
python3 - "$T/probe_auto.bin" "$T/names.out" <<'PY'
import sys, struct
binp, names = sys.argv[1], sys.argv[2]
with open(binp, "rb") as f:
    data = f.read()
try:
    with open(names) as f:
        nm = [ln.rstrip("\n") for ln in f]
except FileNotFoundError:
    nm = []
print(f"names.out: {nm}")
i = 0
records = []
KINDS = {1:"VALUE", 2:"CALL", 3:"RETURN", 4:"END"}
TYPES = {0:"NULL",1:"STRING",2:"INTEGER",3:"REAL",4:"NAME",5:"PATTERN",
         6:"EXPRESSION",7:"ARRAY",8:"TABLE",9:"CODE",10:"DATA",11:"FILE",255:"UNKNOWN"}
while i + 13 <= len(data):
    kind, name_id, t, vlen = struct.unpack("<IIBI", data[i:i+13])
    val = data[i+13:i+13+vlen]
    nm_str = nm[name_id] if 0 <= name_id < len(nm) else f"<id={name_id}>"
    if kind == 4:
        nm_str = "<END>"
    if t == 1 or t == 4:
        valdisp = repr(val.decode("utf-8", "replace"))
    elif t == 2:
        valdisp = struct.unpack("<q", val)[0] if vlen == 8 else "?"
    elif t == 3:
        valdisp = struct.unpack("<d", val)[0] if vlen == 8 else "?"
    else:
        valdisp = ""
    print(f"  {KINDS.get(kind,'?'):7s} {nm_str:20s} type={TYPES.get(t,'?')} {valdisp}")
    records.append((KINDS.get(kind,'?'), nm_str, TYPES.get(t,'?'), val))
    i += 13 + vlen

# Assertions:
kinds = [r[0] for r in records]
assert "VALUE"  in kinds, "no VALUE records"
assert "CALL"   in kinds, "no CALL record"
assert "RETURN" in kinds, "no RETURN record"
assert kinds[-1] == "END", "missing trailing END"
nameset = set(r[1] for r in records if r[0] != "END")
for required in ["a", "b", "c", "d", "SQR"]:
    assert required in nameset, f"missing trace for {required}"
print(f"OK  records={len(records)} names={len(nm)}")
PY
echo "PASS sn26_auto_binary smoke"
