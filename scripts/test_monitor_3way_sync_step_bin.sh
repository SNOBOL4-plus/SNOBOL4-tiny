#!/usr/bin/env bash
# test_monitor_3way_sync_step_bin.sh — binary-protocol sync-step monitor (3-way).
#
# Runs CSNOBOL4 (oracle, participant 0), SPITBOL x64 (participant 1), and
# scrip --ir-run (participant 2) on the same instrumented .sno file.  Each
# participant emits fixed-size binary records on the wire and waits for an
# ACK byte from the controller before continuing.  The Python controller
# compares records as tuples; first divergence stops everyone cleanly.
#
# Wire format: see scripts/monitor/monitor_wire.h.
#
# Usage:  bash test_monitor_3way_sync_step_bin.sh <file.sno> [tracepoints_conf]
# Exit:   0 = all participants agreed
#         1 = divergence
#         2 = timeout / EOF on a participant
#         3 = protocol error
#
# Requires (run once per session):
#   bash scripts/install_system_packages.sh
#   bash scripts/build_scrip.sh
#   bash scripts/build_spitbol_oracle.sh
#   bash scripts/build_csnobol4_oracle.sh
#   bash scripts/build_monitor_ipc_bin_libraries.sh

set -uo pipefail

SNO=${1:?Usage: test_monitor_3way_sync_step_bin.sh <file.sno> [tracepoints_conf]}
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MON_DIR="$HERE/monitor"
CONF=${2:-$MON_DIR/tracepoints_bin.conf}

X64_DIR="${X64_DIR:-/home/claude/x64}"
SPITBOL="$X64_DIR/bin/sbl"
CSNOBOL4="/home/claude/csnobol4/snobol4"
SCRIP="${SCRIP:-$HERE/../scrip}"
INC="${INC:-/home/claude/corpus/programs/include}"

CSN_SO="$MON_DIR/monitor_ipc_bin_csn.so"
SPL_SO="$X64_DIR/monitor_ipc_bin_spl.so"

TIMEOUT="${MONITOR_TIMEOUT:-15}"

# Prerequisites
[[ -x "$SPITBOL"  ]] || { echo "FAIL spitbol not built: $SPITBOL";   exit 2; }
[[ -x "$CSNOBOL4" ]] || { echo "FAIL csnobol4 not built: $CSNOBOL4"; exit 2; }
[[ -x "$SCRIP"    ]] || { echo "FAIL scrip not built: $SCRIP";       exit 2; }
[[ -f "$CSN_SO"   ]] || { echo "FAIL CSN .so missing: $CSN_SO   — run build_monitor_ipc_bin_libraries.sh"; exit 2; }
[[ -f "$SPL_SO"   ]] || { echo "FAIL SPL .so missing: $SPL_SO   — run build_monitor_ipc_bin_libraries.sh"; exit 2; }
[[ -f "$MON_DIR/inject_traces_bin.py" ]] || { echo "FAIL inject_traces_bin.py missing"; exit 2; }
[[ -f "$MON_DIR/monitor_sync_bin.py"  ]] || { echo "FAIL monitor_sync_bin.py missing";  exit 2; }
[[ -f "$CONF" ]] || { echo "FAIL tracepoints_bin.conf missing: $CONF"; exit 2; }

TMP=$(mktemp -d /tmp/monitor_3way_bin_XXXXXX)

base="$(basename "$SNO" .sno)"
STDIN_SRC="/dev/null"
[[ -f "${SNO%.sno}.input" ]] && STDIN_SRC="${SNO%.sno}.input"

echo "[3way-bin] program: $base"
echo "[3way-bin] tmp:     $TMP"

# ── Step 1: inject traces; emit names file ──────────────────────────────
# inject_traces_bin.py adds MON_OPEN + LOAD()ed wrappers around assignments
# for the LOAD-able participants (CSN, SPL).  scrip ignores the injection
# overhead — its comm_var hook already fires inside the C runtime, and it
# reads the same names file directly via MONITOR_NAMES_FILE.
NAMES_FILE="$TMP/names.txt"
SNO_LIB="$INC" python3 "$MON_DIR/inject_traces_bin.py" \
    "$SNO" "$CONF" --names-out="$NAMES_FILE" > "$TMP/instr.sno"
inj_lines=$(wc -l < "$TMP/instr.sno")
orig_lines=$(wc -l < "$SNO")
n_names=$(wc -l < "$NAMES_FILE")
echo "[3way-bin] inject:  $orig_lines source -> $inj_lines instrumented lines"
echo "[3way-bin] names:   $n_names entries in $NAMES_FILE"

# ── Step 2: create FIFO pairs (one pair per participant) ────────────────
for p in csn spl scr; do
    mkfifo "$TMP/$p.ready"
    mkfifo "$TMP/$p.go"
done

# ── Step 3: launch participants ─────────────────────────────────────────
# CSNOBOL4 — participant 0 (oracle)
MONITOR_READY_PIPE="$TMP/csn.ready" \
MONITOR_GO_PIPE="$TMP/csn.go" \
MONITOR_NAMES_FILE="$NAMES_FILE" \
MONITOR_SO="$CSN_SO" \
    timeout "$((TIMEOUT*2))" "$CSNOBOL4" -bf -P256k -S 64k -I"$INC" "$TMP/instr.sno" \
    < "$STDIN_SRC" > "$TMP/csn.out" 2>"$TMP/csn.err" &
CSN_PID=$!

# SPITBOL x64 — participant 1
MONITOR_READY_PIPE="$TMP/spl.ready" \
MONITOR_GO_PIPE="$TMP/spl.go" \
MONITOR_NAMES_FILE="$NAMES_FILE" \
MONITOR_SO="$SPL_SO" \
SETL4PATH=".:$INC" \
    timeout "$((TIMEOUT*2))" "$SPITBOL" -bf "$TMP/instr.sno" \
    < "$STDIN_SRC" > "$TMP/spl.out" 2>"$TMP/spl.err" &
SPL_PID=$!

# scrip --ir-run — participant 2.  Runs the SAME instrumented source as
# csn/spl: scrip pre-registers MON_OPEN / MON_PUT_*_VALUE / MON_PUT_*_RETURN /
# MON_PUT_CALL / MON_CLOSE as C builtins, so the LOAD()'d preamble resolves
# to those builtins instead of dlopen'd .so symbols.  scrip's stub LOAD
# returns success for any "MON_*" prototype.  MONITOR_SO=builtin is a
# sentinel non-empty string so the preamble's `IDENT(MON_SO_) :S(MON_NOOP_)`
# branches *forward* into the LOAD chain (which all succeed via the stub).
# This way all three participants cross MON_OPEN at the same source line —
# automatic startup synchronization, no special-case code in the controller.
MONITOR_READY_PIPE="$TMP/scr.ready" \
MONITOR_GO_PIPE="$TMP/scr.go" \
MONITOR_NAMES_FILE="$NAMES_FILE" \
MONITOR_SO="builtin" \
SNO_LIB="$INC" \
    timeout "$((TIMEOUT*2))" "$SCRIP" --ir-run "$TMP/instr.sno" \
    < "$STDIN_SRC" > "$TMP/scr.out" 2>"$TMP/scr.err" &
SCR_PID=$!

# ── Step 4: launch controller (oracle is participant listed first) ──────
python3 "$MON_DIR/monitor_sync_bin.py" \
    "$NAMES_FILE" \
    "csn:$TMP/csn.ready:$TMP/csn.go" \
    "spl:$TMP/spl.ready:$TMP/spl.go" \
    "scr:$TMP/scr.ready:$TMP/scr.go" > "$TMP/ctrl.out" 2>&1 &
CTRL_PID=$!

# ── Step 5: wait + reap ────────────────────────────────────────────────
wait $CTRL_PID
CTRL_RC=$?

for pid in $CSN_PID $SPL_PID $SCR_PID; do kill "$pid" 2>/dev/null || true; done
wait 2>/dev/null || true

# ── Step 6: report ─────────────────────────────────────────────────────
echo
echo "── controller output ──"
cat "$TMP/ctrl.out"

for p in csn spl scr; do
    if [[ -s "$TMP/$p.out" ]]; then
        echo
        echo "── $p stdout (head) ──"
        head -20 "$TMP/$p.out"
    fi
    if [[ -s "$TMP/$p.err" ]]; then
        echo "── $p stderr (head) ──"
        head -20 "$TMP/$p.err"
    fi
done

# Preserve a copy of the run for forensic inspection.
rm -rf /tmp/monitor_3way_bin_last
cp -a "$TMP" /tmp/monitor_3way_bin_last
echo
echo "[3way-bin] artefacts: /tmp/monitor_3way_bin_last/"
echo "[3way-bin] exit:      $CTRL_RC"

exit $CTRL_RC
