#!/usr/bin/env bash
# test_smoke_sn26_csn_bridge_b.sh — smoke test for SN-26-csn-bridge-b.
#
# Validates that the four monitor_emit_* sites in CSNOBOL4 (ASGNVV, SJSRV1,
# DEFF18, DEFF20) all fire when xsnobol4 runs a SNOBOL4 program with
# MONITOR_READY_PIPE set.  Companion to test_smoke_sn26_csn_bridge.sh,
# which validates the standalone C IPC plumbing.  This script drives
# xsnobol4 itself via the corpus probe at
#   programs/snobol4/demo/csn_bridge_b/probe_b.sno
#
# Per RULES.md self-contained scripts: paths derived from $0; corpus path
# hardcoded; oracle paths hardcoded; SKIP cleanly if dependencies missing.
set -e
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CSNOBOL4="${CSNOBOL4:-/home/claude/csnobol4/snobol4}"
CORPUS="${CORPUS:-/home/claude/corpus}"
MONITOR_DIR="${MONITOR_DIR:-$HERE/monitor}"
PROBE="$CORPUS/programs/snobol4/demo/csn_bridge_b/probe_b.sno"

EXPECTED_RECORDS=12  # SN-26-bridge-coverage-f: +5 LABEL records (DEFINE,SQR_END,3×top-level + SQR body)

# --- preflight -----------------------------------------------------------
if [ ! -x "$CSNOBOL4" ]; then
    echo "SKIP $CSNOBOL4 missing — run build_csnobol4_oracle.sh"
    exit 0
fi
if [ ! -f "$PROBE" ]; then
    echo "SKIP $PROBE missing — corpus checkout incomplete"
    exit 0
fi
if [ ! -f "$MONITOR_DIR/read_one_wire.py" ]; then
    echo "SKIP $MONITOR_DIR/read_one_wire.py missing"
    exit 0
fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

RFIFO="$WORK/r.fifo"
GFIFO="$WORK/g.fifo"
NAMES="$WORK/names.out"
CTRL_LOG="$WORK/ctrl.log"
PART_OUT="$WORK/part.out"

# --- run xsnobol4 against the wire reader -------------------------------
( python3 "$MONITOR_DIR/read_one_wire.py" "$RFIFO" "$GFIFO" "$NAMES" \
        > "$CTRL_LOG" 2>&1 ) &
CTRL_PID=$!

for _ in 1 2 3 4 5 6 7 8 9 10; do
    [ -p "$RFIFO" ] && [ -p "$GFIFO" ] && break
    sleep 0.1
done

MONITOR_READY_PIPE="$RFIFO" MONITOR_GO_PIPE="$GFIFO" MONITOR_NAMES_OUT="$NAMES" \
    timeout 8 "$CSNOBOL4" -bf "$PROBE" > "$PART_OUT" 2>&1 < /dev/null
PART_RC=$?

wait "$CTRL_PID"
CTRL_RC=$?

if [ "$PART_RC" -ne 0 ] || [ "$CTRL_RC" -ne 0 ]; then
    echo "FAIL participant rc=$PART_RC controller rc=$CTRL_RC"
    cat "$CTRL_LOG"
    exit 1
fi

N_REC=$(grep -c '^\[ctrl\] #' "$CTRL_LOG" || true)
if [ "$N_REC" -ne "$EXPECTED_RECORDS" ]; then
    echo "FAIL expected $EXPECTED_RECORDS records, got $N_REC"
    cat "$CTRL_LOG"
    exit 1
fi

# Verify each expected record-shape appears.  We pin record indices to
# catch a re-ordering regression — bridge sites must fire in this order.
# Use fixed-string grep so STRING(11)/INTEGER(49) parens aren't ERE groups.
while IFS= read -r spec; do
    [ -z "$spec" ] && continue
    if ! grep -qF "$spec" "$CTRL_LOG"; then
        echo "FAIL missing/misordered record: $spec"
        cat "$CTRL_LOG"
        exit 1
    fi
done <<'EOF_SPECS'
#000 kind=LABEL name_id=4294967295 INTEGER(2)
#001 kind=LABEL name_id=4294967295 INTEGER(5)
#002 kind=VALUE name_id=0 STRING(11)=b'hello world'
#003 kind=LABEL name_id=4294967295 INTEGER(6)
#004 kind=VALUE name_id=0 STRING(11)=b'hello there'
#005 kind=LABEL name_id=4294967295 INTEGER(7)
#006 kind=CALL name_id=1
#007 kind=LABEL name_id=4294967295 INTEGER(3)
#008 kind=VALUE name_id=1 INTEGER(49)
#009 kind=RETURN name_id=1 STRING(6)=b'RETURN'
#010 kind=VALUE name_id=2 INTEGER(49)
#011 kind=END
EOF_SPECS

# Verify names sidecar contains S, SQR, N (in the order they were interned).
if [ ! -f "$NAMES" ]; then
    echo "FAIL names sidecar not written"
    exit 1
fi
N_NAMES=$(wc -l < "$NAMES")
if [ "$N_NAMES" -ne 3 ]; then
    echo "FAIL expected 3 names in sidecar, got $N_NAMES"
    cat "$NAMES"
    exit 1
fi
for nm in S SQR N; do
    if ! grep -qx "$nm" "$NAMES"; then
        echo "FAIL name '$nm' missing from sidecar"
        cat "$NAMES"
        exit 1
    fi
done

# --- no-op fallback test (env vars unset) -------------------------------
unset MONITOR_READY_PIPE MONITOR_GO_PIPE MONITOR_NAMES_OUT
if ! timeout 4 "$CSNOBOL4" -bf "$PROBE" > /dev/null 2>&1 < /dev/null; then
    echo "FAIL silent no-op fallback (env vars unset) returned non-zero"
    exit 1
fi

echo "PASS  $N_REC records (ASGNVV+SJSRV1+DEFF18+DEFF20+ASGNVV+ASGNVV+END), $N_NAMES names, no-op fallback OK"
echo "PASS=1 FAIL=0"
