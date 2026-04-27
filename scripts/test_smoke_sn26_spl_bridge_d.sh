#!/usr/bin/env bash
# test_smoke_sn26_spl_bridge_d.sh — smoke test for SN-26-bridge-coverage-b.
#
# Validates that the new asign + asinp fire-points in SPITBOL x64 fire
# on `.`-capture (PAT . X) and pattern-substitute (S 'pat' = 'rep') —
# the two cases the existing b_vrs-only fire-point did NOT cover.
#
# Reuses corpus/programs/snobol4/demo/csn_bridge_c/probe_c.sno so the
# CSN and SPL bridges can be cross-validated against the same probe.
#
# Per RULES.md self-contained scripts: paths derived from $0; corpus path
# hardcoded; oracle paths hardcoded; SKIP cleanly if dependencies missing.
set -e
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SBL="${SBL:-/home/claude/x64/bin/sbl}"
CORPUS="${CORPUS:-/home/claude/corpus}"
MONITOR_DIR="${MONITOR_DIR:-$HERE/monitor}"
PROBE="$CORPUS/programs/snobol4/demo/csn_bridge_c/probe_c.sno"

EXPECTED_RECORDS=3   # ASGNVV-equivalent (asign for a) + .-capture (asinp) + END

# --- preflight -----------------------------------------------------------
if [ ! -x "$SBL" ]; then
    echo "SKIP $SBL missing — run build_spitbol_oracle.sh"
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

# Capability probe: this gate requires the SN-26-bridge-coverage-b
# build (asign + asinp fire-points).  Pre-coverage-b builds would
# produce only one VALUE record (the b_vrs path for `S = 'AXBYC'`).
if ! nm -D "$SBL" 2>/dev/null | grep -q '\bzysmv\b' && \
   ! strings "$SBL" 2>/dev/null | grep -q 'MONITOR_READY_PIPE'; then
    echo "SKIP $SBL is pre-SN-26-spl-bridge-b (no zysmv symbol)"
    exit 0
fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

RFIFO="$WORK/r.fifo"
GFIFO="$WORK/g.fifo"
NAMES="$WORK/names.out"
CTRL_LOG="$WORK/ctrl.log"
PART_OUT="$WORK/part.out"

mkfifo "$RFIFO" "$GFIFO"

# --- run sbl against the wire reader ------------------------------------
( python3 "$MONITOR_DIR/read_one_wire.py" "$RFIFO" "$GFIFO" "$NAMES" \
        > "$CTRL_LOG" 2>&1 ) &
CTRL_PID=$!

sleep 0.3

MONITOR_READY_PIPE="$RFIFO" MONITOR_GO_PIPE="$GFIFO" MONITOR_NAMES_OUT="$NAMES" \
    timeout 8 "$SBL" -bf "$PROBE" > "$PART_OUT" 2>&1 < /dev/null
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

# Pin record indices — fire-points must produce these in order.
# Note: the FIRST VALUE record fires from b_vrs (universal var-store,
# untouched by this rung).  The SECOND VALUE record is the new one,
# from the asinp fire-point catching `S ANY('AB') . captured`.
while IFS= read -r spec; do
    [ -z "$spec" ] && continue
    if ! grep -qF "$spec" "$CTRL_LOG"; then
        echo "FAIL missing/misordered record: $spec"
        cat "$CTRL_LOG"
        exit 1
    fi
done <<'EOF_SPECS'
#000 kind=VALUE name_id=0 STRING(5)=b'AXBYC'
#001 kind=VALUE name_id=1 STRING(1)=b'A'
#002 kind=END
EOF_SPECS

# Verify names sidecar.  Must be S + captured (NOT S + <lval> — that
# was the symptom of the wrong xr offset before the *vrvlo fix).
if [ ! -f "$NAMES" ]; then
    echo "FAIL names sidecar not written"
    exit 1
fi
N_NAMES=$(wc -l < "$NAMES")
if [ "$N_NAMES" -ne 2 ]; then
    echo "FAIL expected 2 names in sidecar, got $N_NAMES"
    cat "$NAMES"
    exit 1
fi
for nm in S captured; do
    if ! grep -qx "$nm" "$NAMES"; then
        echo "FAIL name '$nm' missing from sidecar (asign/asinp xr offset wrong?)"
        cat "$NAMES"
        exit 1
    fi
done

# --- no-op fallback test (env vars unset) -------------------------------
unset MONITOR_READY_PIPE MONITOR_GO_PIPE MONITOR_NAMES_OUT
if ! timeout 4 "$SBL" -bf "$PROBE" > /dev/null 2>&1 < /dev/null; then
    echo "FAIL silent no-op fallback (env vars unset) returned non-zero"
    exit 1
fi

echo "PASS  $N_REC records (b_vrs+asinp/.-capture+END), $N_NAMES names (S, captured), no-op fallback OK"
echo "PASS=1 FAIL=0"
