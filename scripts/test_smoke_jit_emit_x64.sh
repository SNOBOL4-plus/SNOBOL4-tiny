#!/usr/bin/env bash
# test_smoke_jit_emit_x64.sh — gate for GOAL-MODE4-EMIT EM-1
#
# Verifies the literal-zero stub: scrip --jit-emit --x64 produces an
# asm file that, when assembled and linked against libscrip_rt.so,
# loads, runs, and exits 0. This is the wiring proof for EM-1.
#
# Subsequent EM-N rungs extend this script (or replace it) to exercise
# real codegen — first SM_HALT with a non-zero rc (EM-2), then
# arithmetic (EM-3), control flow (EM-4), chunks (EM-5), etc.
#
# Idempotent. Safe to run multiple times.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIP="$ROOT/scrip"
RT_SO="$ROOT/out/libscrip_rt.so"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

[ -x "$SCRIP" ] || { echo "FAIL scrip not built — run scripts/build_scrip.sh"; exit 1; }
[ -f "$RT_SO" ] || { echo "FAIL libscrip_rt.so not built — run: make libscrip_rt"; exit 1; }

cat > "$TMP/em1_test.sno" <<'EOF'
	OUTPUT = "hi"
END
EOF

# Step 1: emit asm
"$SCRIP" --jit-emit --x64 "$TMP/em1_test.sno" > "$TMP/em1_test.s" 2> "$TMP/em1_emit.err" || {
    echo "FAIL scrip --jit-emit --x64 returned $?"
    cat "$TMP/em1_emit.err"
    exit 1
}
[ -s "$TMP/em1_test.s" ] || { echo "FAIL emitted asm is empty"; exit 1; }
grep -q "main:" "$TMP/em1_test.s" || { echo "FAIL no main: label in emitted asm"; exit 1; }
grep -q "scrip_rt_init"     "$TMP/em1_test.s" || { echo "FAIL no scrip_rt_init call"; exit 1; }
grep -q "scrip_rt_finalize" "$TMP/em1_test.s" || { echo "FAIL no scrip_rt_finalize call"; exit 1; }
echo "  PASS emit  (asm produced, main + ABI calls present)"

# Step 2: assemble + link
gcc -no-pie "$TMP/em1_test.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em1_prog" 2> "$TMP/em1_link.err" || {
    echo "FAIL gcc link returned $?"
    cat "$TMP/em1_link.err"
    exit 1
}
[ -x "$TMP/em1_prog" ] || { echo "FAIL emitted binary not executable"; exit 1; }
echo "  PASS link  (binary built against libscrip_rt.so)"

# Step 3: run
set +e
"$TMP/em1_prog"
RC=$?
set -e
if [ "$RC" -ne 0 ]; then
    echo "FAIL emitted binary exited with rc=$RC (expected 0)"
    exit 1
fi
echo "  PASS run   (binary exited 0)"

# Step 4: error paths
set +e
"$SCRIP" --jit-emit "$TMP/em1_test.sno" >/dev/null 2>&1
[ $? -eq 1 ] || { echo "FAIL bare --jit-emit should error"; exit 1; }
"$SCRIP" --x64 "$TMP/em1_test.sno" >/dev/null 2>&1
[ $? -eq 1 ] || { echo "FAIL bare --x64 should error"; exit 1; }
"$SCRIP" --jit-emit --x64 --sm-run "$TMP/em1_test.sno" >/dev/null 2>&1
[ $? -eq 1 ] || { echo "FAIL --jit-emit --x64 --sm-run mutex should error"; exit 1; }
set -e
echo "  PASS errors (flag validation)"

echo
echo "PASS=4 FAIL=0  (EM-1 wiring + libscrip_rt.so skeleton)"
