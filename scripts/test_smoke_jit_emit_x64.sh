#!/usr/bin/env bash
# test_smoke_jit_emit_x64.sh — gate for GOAL-MODE4-EMIT EM-1 + EM-2
#
# Single growing gate.  Each EM-N rung extends the test set; previous
# rungs' invariants still hold.
#
# EM-1 contract (wiring + libscrip_rt.so skeleton):
#   - --jit-emit + --x64 flag pair selects asm emission to stdout.
#   - All three flag-validation paths error correctly.
#   - Bare emit (any program) produces asm with a main: label and
#     PLT calls into scrip_rt_init / scrip_rt_finalize.
#
# EM-2 contract (SM_HALT + SM_PUSH_LIT_I codegen):
#   - Synthetic SM_PUSH_LIT_I 42 + SM_HALT program emits, links,
#     runs, exits with rc=42 — proves PUSH_LIT_I + HALT codegen,
#     SM stack push/pop in libscrip_rt.so, halt-rc surfacing
#     through finalize.
#   - Synthetic program containing an opcode the emitter does not yet
#     bake (SM_ADD) emits, links, runs, and aborts loudly with the
#     unhandled-op trap diagnostic on stderr.
#   - Real-frontend program (`OUTPUT = "hi"`) emits successfully (the
#     emit itself returns 0); the resulting asm assembles cleanly.
#     Runtime behavior of the real-frontend binary is not checked
#     here — most ops are still unhandled in EM-2; that's expected
#     and verified by the synthetic unhandled-op test above.
#
# EM-3+ rungs add tests in this file; the runtime test for the real
# frontend grows as more opcodes leave the unhandled set.
#
# Idempotent.  Safe to run multiple times.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIP="$ROOT/scrip"
RT_SO="$ROOT/out/libscrip_rt.so"
HARNESS="$ROOT/out/sm_codegen_x64_emit_test"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

[ -x "$SCRIP" ]   || { echo "FAIL scrip not built — run scripts/build_scrip.sh"; exit 1; }
[ -f "$RT_SO" ]   || { echo "FAIL libscrip_rt.so not built — run: make libscrip_rt"; exit 1; }
[ -x "$HARNESS" ] || { echo "FAIL test harness not built — run: make out/sm_codegen_x64_emit_test"; exit 1; }

# ── Test 1: SM_PUSH_LIT_I 42 + SM_HALT → rc=42 ─────────────────────────
"$HARNESS" "$TMP/em2_a.s" >/dev/null
gcc -no-pie "$TMP/em2_a.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em2_a" 2> "$TMP/em2_a.err" || {
    echo "FAIL em2_a link"; cat "$TMP/em2_a.err"; exit 1; }
set +e
"$TMP/em2_a"
RC=$?
set -e
if [ "$RC" -ne 42 ]; then
    echo "FAIL em2_a expected rc=42 got rc=$RC"; exit 1
fi
# Asm-content sanity: PUSH_LIT_I and HALT emitter blocks both present.
grep -q "SM_PUSH_LIT_I  42"           "$TMP/em2_a.s" || { echo "FAIL no PUSH_LIT_I 42 marker"; exit 1; }
grep -q "SM_HALT (rc <- TOS"          "$TMP/em2_a.s" || { echo "FAIL no SM_HALT marker"; exit 1; }
grep -q "scrip_rt_pop_int@PLT"        "$TMP/em2_a.s" || { echo "FAIL no pop_int call"; exit 1; }
grep -q "scrip_rt_halt@PLT"           "$TMP/em2_a.s" || { echo "FAIL no halt call"; exit 1; }
grep -q "movabs *rdi, 42"             "$TMP/em2_a.s" || { echo "FAIL no literal-42 movabs"; exit 1; }
echo "  PASS PUSH_LIT_I+HALT  (rc=42; emit shape correct)"

# ── Test 2: unhandled-op trap fires on SM_ADD ──────────────────────────
# Build a tiny inline harness that emits SM_PUSH_LIT_I + SM_PUSH_LIT_I
# + SM_ADD + SM_HALT.  Run; expect non-zero rc + diagnostic on stderr.
cat > "$TMP/unh.c" <<'CEOF'
#include "sm_prog.h"
#include "sm_codegen_x64_emit.h"
#include <stdio.h>
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    SM_Program *p = sm_prog_new();
    sm_emit_i(p, SM_PUSH_LIT_I, 1);
    sm_emit_i(p, SM_PUSH_LIT_I, 2);
    sm_emit(p, SM_ADD);
    sm_emit(p, SM_HALT);
    FILE *f = fopen(argv[1], "w");
    sm_codegen_x64_emit(p, f);
    fclose(f);
    sm_prog_free(p);
    return 0;
}
CEOF
gcc -O0 -g -w -I"$ROOT/src" -I"$ROOT/src/runtime/x86" -I"$ROOT/src/runtime" \
    "$TMP/unh.c" \
    "$ROOT/src/runtime/x86/sm_codegen_x64_emit.c" \
    "$ROOT/src/runtime/x86/sm_prog.c" \
    -o "$TMP/unh_emitter" 2> "$TMP/unh_build.err" || {
    echo "FAIL unhandled-op harness build"; cat "$TMP/unh_build.err"; exit 1; }
"$TMP/unh_emitter" "$TMP/unh.s" >/dev/null
gcc -no-pie "$TMP/unh.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/unh_prog" 2> "$TMP/unh_link.err" || {
    echo "FAIL unhandled-op link"; cat "$TMP/unh_link.err"; exit 1; }
set +e
"$TMP/unh_prog" 2> "$TMP/unh.err"
RC=$?
set -e
if [ "$RC" -eq 0 ]; then
    echo "FAIL unhandled-op program should abort, got rc=0"; exit 1
fi
grep -q "EM-2 emitter does not yet bake" "$TMP/unh.err" || {
    echo "FAIL no unhandled-op diagnostic on stderr"; cat "$TMP/unh.err"; exit 1; }
echo "  PASS UNHANDLED_OP trap (rc=$RC; diagnostic present)"

# ── Test 3: real-frontend emit returns 0 + asm assembles ───────────────
cat > "$TMP/real.sno" <<'EOF'
	OUTPUT = "hi"
END
EOF
"$SCRIP" --jit-emit --x64 "$TMP/real.sno" > "$TMP/real.s" 2> "$TMP/real_emit.err" || {
    echo "FAIL real-frontend emit"; cat "$TMP/real_emit.err"; exit 1; }
[ -s "$TMP/real.s" ] || { echo "FAIL real-frontend emit produced empty asm"; exit 1; }
# Just assemble (don't link or run — it'd hit unhandled-op as expected).
gcc -c "$TMP/real.s" -o "$TMP/real.o" 2> "$TMP/real_as.err" || {
    echo "FAIL real-frontend asm doesn't assemble"; cat "$TMP/real_as.err"; exit 1; }
echo "  PASS real frontend  (emit ok; asm assembles)"

# ── Test 4: EM-1 wiring still solid (regression guard) ────────────────
# All three flag-validation paths should still error out as in EM-1.
set +e
"$SCRIP" --jit-emit "$TMP/real.sno" >/dev/null 2>&1
[ $? -eq 1 ] || { echo "FAIL bare --jit-emit should error"; exit 1; }
"$SCRIP" --x64 "$TMP/real.sno" >/dev/null 2>&1
[ $? -eq 1 ] || { echo "FAIL bare --x64 should error"; exit 1; }
"$SCRIP" --jit-emit --x64 --sm-run "$TMP/real.sno" >/dev/null 2>&1
[ $? -eq 1 ] || { echo "FAIL mutex with --sm-run should error"; exit 1; }
set -e
echo "  PASS EM-1 errors    (flag validation regression-clean)"

echo
echo "PASS=4 FAIL=0  (EM-1 wiring + EM-2 codegen for SM_HALT + SM_PUSH_LIT_I)"
