#!/usr/bin/env bash
# test_monitor_2way_sync_step_bin.sh — binary-protocol sync-step monitor (2-way).
#
# Same shape as test_monitor_2way_sync_step.sh but uses the binary protocol.
# Runs CSNOBOL4 (oracle, participant 0) and SPITBOL x64 (participant 1) on
# the same instrumented .sno file.  Each participant LOAD()s its ABI's
# binary IPC .so and emits fixed-size binary records on the wire.  The
# Python controller compares records as tuples, no string processing.
#
# Wire format: see scripts/monitor/monitor_wire.h.
#
# Usage:  bash test_monitor_2way_sync_step_bin.sh <file.sno> [tracepoints_conf]
# Exit:   0 = all participants agreed
#         1 = divergence
#         2 = timeout / EOF on a participant
#         3 = protocol error

set -uo pipefail

SNO=${1:?Usage: test_monitor_2way_sync_step_bin.sh <file.sno> [tracepoints_conf]}
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MON_DIR="$HERE/monitor"
CONF=${2:-$MON_DIR/tracepoints_bin.conf}

X64_DIR="${X64_DIR:-/home/claude/x64}"
SPITBOL="$X64_DIR/bin/sbl"
CSNOBOL4="/home/claude/csnobol4/snobol4"
INC="${INC:-/home/claude/corpus/programs/include}"

CSN_SO="$MON_DIR/monitor_ipc_bin_csn.so"
SPL_SO="$X64_DIR/monitor_ipc_bin_spl.so"

TIMEOUT="${MONITOR_TIMEOUT:-10}"

# Prerequisites
[[ -x "$SPITBOL"  ]] || { echo "FAIL spitbol not built: $SPITBOL";   exit 2; }
[[ -x "$CSNOBOL4" ]] || { echo "FAIL csnobol4 not built: $CSNOBOL4"; exit 2; }
[[ -f "$CSN_SO"   ]] || { echo "FAIL CSN .so missing: $CSN_SO   — run build_monitor_ipc_bin_libraries.sh"; exit 2; }
[[ -f "$SPL_SO"   ]] || { echo "FAIL SPL .so missing: $SPL_SO   — run build_monitor_ipc_bin_libraries.sh"; exit 2; }
[[ -f "$MON_DIR/inject_traces_bin.py" ]] || { echo "FAIL inject_traces_bin.py missing"; exit 2; }
[[ -f "$MON_DIR/monitor_sync_bin.py"  ]] || { echo "FAIL monitor_sync_bin.py missing";  exit 2; }
[[ -f "$CONF" ]] || { echo "FAIL tracepoints_bin.conf missing: $CONF"; exit 2; }

TMP=$(mktemp -d /tmp/monitor_2way_bin_XXXXXX)

base="$(basename "$SNO" .sno)"
STDIN_SRC="/dev/null"
[[ -f "${SNO%.sno}.input" ]] && STDIN_SRC="${SNO%.sno}.input"

echo "[2way-bin] program: $base"
echo "[2way-bin] tmp:     $TMP"

# ── Step 1: inject traces; emit names file ──────────────────────────────
NAMES_FILE="$TMP/names.txt"
SNO_LIB="$INC" python3 "$MON_DIR/inject_traces_bin.py" \
    "$SNO" "$CONF" --names-out="$NAMES_FILE" > "$TMP/instr.sno"
inj_lines=$(wc -l < "$TMP/instr.sno")
orig_lines=$(wc -l < "$SNO")
n_names=$(wc -l < "$NAMES_FILE")
echo "[2way-bin] inject:  $orig_lines source -> $inj_lines instrumented lines"
echo "[2way-bin] names:   $n_names entries in $NAMES_FILE"

# ── Step 2: create FIFO pairs ───────────────────────────────────────────
for p in csn spl; do
    mkfifo "$TMP/$p.ready"
    mkfifo "$TMP/$p.go"
done

# ── Step 3: launch participants ─────────────────────────────────────────
MONITOR_READY_PIPE="$TMP/csn.ready" \
MONITOR_GO_PIPE="$TMP/csn.go" \
MONITOR_NAMES_FILE="$NAMES_FILE" \
MONITOR_SO="$CSN_SO" \
    timeout "$((TIMEOUT*2))" "$CSNOBOL4" -bf -P256k -S 64k -I"$INC" "$TMP/instr.sno" \
    < "$STDIN_SRC" > "$TMP/csn.out" 2>"$TMP/csn.err" &
CSN_PID=$!

MONITOR_READY_PIPE="$TMP/spl.ready" \
MONITOR_GO_PIPE="$TMP/spl.go" \
MONITOR_NAMES_FILE="$NAMES_FILE" \
MONITOR_SO="$SPL_SO" \
SETL4PATH=".:$INC" \
    timeout "$((TIMEOUT*2))" "$SPITBOL" -bf "$TMP/instr.sno" \
    < "$STDIN_SRC" > "$TMP/spl.out" 2>"$TMP/spl.err" &
SPL_PID=$!

# ── Step 4: launch controller ───────────────────────────────────────────
python3 "$MON_DIR/monitor_sync_bin.py" \
    "$NAMES_FILE" \
    "csn:$TMP/csn.ready:$TMP/csn.go" \
    "spl:$TMP/spl.ready:$TMP/spl.go" > "$TMP/ctrl.out" 2>&1 &
CTRL_PID=$!

# ── Step 5: wait + reap ────────────────────────────────────────────────
wait $CTRL_PID
CTRL_RC=$?

for pid in $CSN_PID $SPL_PID; do kill "$pid" 2>/dev/null || true; done
wait 2>/dev/null || true

# ── Step 6: report ─────────────────────────────────────────────────────
echo ""
echo "── controller output ──"
cat "$TMP/ctrl.out"
echo ""
echo "── csn stdout (head) ──"; head -20 "$TMP/csn.out" 2>/dev/null
echo "── csn stderr (head) ──"; head -10 "$TMP/csn.err" 2>/dev/null
echo "── spl stdout (head) ──"; head -20 "$TMP/spl.out" 2>/dev/null
echo "── spl stderr (head) ──"; head -10 "$TMP/spl.err" 2>/dev/null

cp -r "$TMP" /tmp/monitor_2way_bin_last 2>/dev/null
echo ""
echo "[2way-bin] artefacts: /tmp/monitor_2way_bin_last/"
echo "[2way-bin] exit:      $CTRL_RC"
exit $CTRL_RC
