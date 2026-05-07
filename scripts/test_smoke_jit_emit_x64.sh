#!/usr/bin/env bash
# test_smoke_jit_emit_x64.sh — gate for GOAL-MODE4-EMIT EM-1..EM-4
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
#     bake (SM_CONCAT) emits, links, runs, and aborts loudly with the
#     unhandled-op trap diagnostic on stderr.
#   - Real-frontend program (`OUTPUT = "hi"`) emits successfully (the
#     emit itself returns 0); the resulting asm assembles cleanly.
#     Runtime behavior of the real-frontend binary is not checked
#     here — most ops are still unhandled in EM-2; that's expected
#     and verified by the synthetic unhandled-op test above.
#
# EM-3 contract (typed stack + arithmetic):
#   - Synthetic (2+3)*4 program emits, links, runs, exits rc=20.
#
# EM-4 contract (control flow):
#   - 6a: synthetic 15-op program exercises SM_JUMP forward (skip dead
#     code), SM_JUMP_F not-taken (last_ok=1 default), SM_JUMP_S taken
#     (last_ok=1 default).  Expected rc=42; asm shape verified
#     (jmp/jz/jnz forms all present).
#   - 6b: synthetic 6-op countdown body uses SM_JUMP_F backward.  A
#     thin C driver overrides scrip_rt_last_ok to return 0 twice then
#     1, proving the backward branch lands on its .Lpc<top> label.
#     Expected rc=0.
#
# EM-5+ rungs add tests in this file; the runtime test for the real
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
# asm-content sanity: check for key instruction sequences in new 3-col format
grep -q "movabs  rdi, 42"             "$TMP/em2_a.s" || { echo "FAIL no literal-42 movabs"; exit 1; }
grep -q "scrip_rt_push_int@PLT"       "$TMP/em2_a.s" || { echo "FAIL no push_int call"; exit 1; }
grep -q "scrip_rt_halt_tos@PLT"       "$TMP/em2_a.s" || { echo "FAIL no halt_tos call"; exit 1; }
echo "  PASS PUSH_LIT_I+HALT  (rc=42; emit shape correct)"

# ── Test 2: unhandled-op trap fires on SM_INCR ─────────────────────────
# Build a tiny inline harness that emits SM_PUSH_LIT_I + SM_INCR + SM_HALT.
# Run; expect non-zero rc + diagnostic on stderr.
# (SM_CONCAT was used pre-EM-7 — replaced with SM_INCR which remains unhandled.)
cat > "$TMP/unh.c" <<'CEOF'
#include "sm_prog.h"
#include "sm_codegen_x64_emit.h"
#include <stdio.h>
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    SM_Program *p = sm_prog_new();
    sm_emit_i(p, SM_PUSH_LIT_I, 1);
    sm_emit_i(p, SM_INCR, 1);
    sm_emit(p, SM_HALT);
    FILE *f = fopen(argv[1], "w");
    sm_codegen_x64_emit(p, f, NULL);
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
grep -q "unhandled SM opcode" "$TMP/unh.err" || {
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


# -- Test 5: EM-3 gate -- (2 + 3) * 4 = 20 ----------------------------------
# Build the EM-3 synthetic program (6-op: PUSH 2, PUSH 3, ADD, PUSH 4, MUL, HALT).
# The harness now accepts up to four asm output paths;
#   argv[1]=EM-2, argv[2]=EM-3, argv[3]=EM-4a (forward+conditional shapes),
#   argv[4]=EM-4b (backward-loop body, driven by override below).
"$HARNESS" "$TMP/em2_a.s" "$TMP/em3.s" "$TMP/em4a.s" "$TMP/em4b.s" "$TMP/em5.s" "$TMP/em5b.s" >/dev/null
gcc -no-pie "$TMP/em3.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em3_prog" 2> "$TMP/em3_link.err" || {
    echo "FAIL em3 link"; cat "$TMP/em3_link.err"; exit 1; }
set +e
"$TMP/em3_prog"
RC=$?
set -e
if [ "$RC" -ne 20 ]; then
    echo "FAIL em3 expected rc=20 got rc=$RC"; exit 1
fi
# Asm-content sanity: ADD and MUL emitter blocks both present.
grep -q "SM_ADD"   "$TMP/em3.s" || { echo "FAIL no SM_ADD marker in em3 asm"; exit 1; }
grep -q "SM_MUL"   "$TMP/em3.s" || { echo "FAIL no SM_MUL marker in em3 asm"; exit 1; }
grep -q "scrip_rt_arith@PLT" "$TMP/em3.s" || { echo "FAIL no arith PLT call"; exit 1; }
echo "  PASS EM-3 arithmetic  ((2+3)*4=20; emit->link->run verified)"

# -- Test 6a: EM-4 forward jump + conditional shapes -----------------------
# Synthetic 15-op program: SM_JUMP forward over dead code, then
# SM_JUMP_F (not taken since last_ok=1) and SM_JUMP_S (taken since last_ok=1)
# steer execution to the final HALT.  Expected rc=42.
gcc -no-pie "$TMP/em4a.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em4a_prog" 2> "$TMP/em4a_link.err" || {
    echo "FAIL em4a link"; cat "$TMP/em4a_link.err"; exit 1; }
set +e
"$TMP/em4a_prog"
RC=$?
set -e
if [ "$RC" -ne 42 ]; then
    echo "FAIL em4a expected rc=42 got rc=$RC"; exit 1
fi
# Asm-shape sanity: forward jmp, JUMP_F (jz) and JUMP_S (jnz) all present.
grep -q "jmp     .Lpc"   "$TMP/em4a.s" || { echo "FAIL no SM_JUMP jmp in em4a asm"; exit 1; }
grep -q "SM_JUMP_F"      "$TMP/em4a.s" || { echo "FAIL no SM_JUMP_F marker"; exit 1; }
grep -q "SM_JUMP_S"      "$TMP/em4a.s" || { echo "FAIL no SM_JUMP_S marker"; exit 1; }
grep -q "scrip_rt_last_ok@PLT" "$TMP/em4a.s" || { echo "FAIL no last_ok call"; exit 1; }
grep -qE 'jz +\.Lpc'     "$TMP/em4a.s" || { echo "FAIL no jz target in em4a"; exit 1; }
grep -qE 'jnz +\.Lpc'    "$TMP/em4a.s" || { echo "FAIL no jnz target in em4a"; exit 1; }
echo "  PASS EM-4a control flow (forward JUMP + JUMP_F not-taken + JUMP_S taken; rc=42)"

# -- Test 6b: EM-4 conditional backward loop ------------------------------
# 6-op SM body: counter=3, decrement, JUMP_F backward.  The driver below
# overrides scrip_rt_last_ok to return 0 twice (loop iterates 2x: 3->2, 2->1)
# then 1 (exit; counter -=1 -> 0; HALT rc=0).  Proves the JUMP_F backward
# branch lands on the .Lpc<top> label correctly.
cat > "$TMP/em4b_driver.c" <<'CEOF'
/* EM-4b loop driver: scrip_rt_last_ok override drives the backward loop.
 * Defined here so the linker resolves it before falling back to libscrip_rt.so. */
static int call_count = 0;
int scrip_rt_last_ok(void) {
    call_count++;
    return (call_count >= 3) ? 1 : 0;
}
CEOF
gcc -no-pie "$TMP/em4b.s" "$TMP/em4b_driver.c" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em4b_prog" 2> "$TMP/em4b_link.err" || {
    echo "FAIL em4b link"; cat "$TMP/em4b_link.err"; exit 1; }
set +e
"$TMP/em4b_prog"
RC=$?
set -e
if [ "$RC" -ne 0 ]; then
    echo "FAIL em4b expected rc=0 got rc=$RC"; exit 1
fi
# Asm-shape sanity: the backward jz must target the loop-top label (.Lpc1).
grep -qE 'jz +\.Lpc1\b' "$TMP/em4b.s" || { echo "FAIL no backward jz to .Lpc1"; exit 1; }
echo "  PASS EM-4b backward loop (JUMP_F backward x2, fallthrough; rc=0)"

# -- Test 7a: EM-5 chunk call/return -- two chunks calling each other -------
# Outer chunk_A calls inner chunk_B (returns 7), adds 6, returns 13.
# main calls chunk_A and HALTs with the returned value.  Proves the
# baked-direct call/ret discipline composes across nested chunks.
gcc -no-pie "$TMP/em5.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em5_prog" 2> "$TMP/em5_link.err" || {
    echo "FAIL em5 link"; cat "$TMP/em5_link.err"; exit 1; }
set +e
"$TMP/em5_prog"
RC=$?
set -e
if [ "$RC" -ne 13 ]; then
    echo "FAIL em5 expected rc=13 got rc=$RC"; exit 1
fi
# Asm-shape sanity: SM_RETURN bakes direct ret; SM_CALL_CHUNK bakes
# direct call to a .LpcN target -- no PLT call for either opcode.
grep -q "SM_RETURN"          "$TMP/em5.s" || { echo "FAIL no SM_RETURN marker in em5"; exit 1; }
grep -q "SM_CALL_CHUNK"      "$TMP/em5.s" || { echo "FAIL no SM_CALL_CHUNK marker in em5"; exit 1; }
grep -qE 'call +\.Lpc[0-9]+' "$TMP/em5.s" || { echo "FAIL no baked-direct call .LpcN in em5"; exit 1; }
grep -qE '^[[:space:]]+ret\b' "$TMP/em5.s" || { echo "FAIL no native ret in em5"; exit 1; }
echo "  PASS EM-5a chunk call/return  (chunk_A -> chunk_B; nested rc=13)"

# -- Test 7b: EM-5 SM_PUSH_CHUNK descriptor round-trip ----------------------
# PUSH_CHUNK (entry_pc=99, arity=2) then POP it; then PUSH_LIT_I 21 + HALT.
# Proves scrip_rt_push_chunk_descr@PLT round-trips without corrupting
# the SM stack.
gcc -no-pie "$TMP/em5b.s" \
    -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
    -o "$TMP/em5b_prog" 2> "$TMP/em5b_link.err" || {
    echo "FAIL em5b link"; cat "$TMP/em5b_link.err"; exit 1; }
set +e
"$TMP/em5b_prog"
RC=$?
set -e
if [ "$RC" -ne 21 ]; then
    echo "FAIL em5b expected rc=21 got rc=$RC"; exit 1
fi
grep -q "SM_PUSH_CHUNK"                "$TMP/em5b.s" || { echo "FAIL no SM_PUSH_CHUNK marker"; exit 1; }
grep -q "scrip_rt_push_chunk_descr@PLT" "$TMP/em5b.s" || { echo "FAIL no descriptor-push PLT call"; exit 1; }
echo "  PASS EM-5b push-chunk descr   (PUSH_CHUNK + POP round-trip; rc=21)"

# ── Test 10: RETIRED in EM-7-revert (session #72) ─────────────────────────────
# This test exercised the brokered Phase-3 path (scrip_rt_pat_*@PLT building
# a runtime descriptor tree, then scrip_rt_exec_stmt → exec_stmt → bb_broker)
# which Lon's correction confirmed was the WRONG architecture for mode-4.
# See GOAL-MODE4-EMIT.md "Design Discoveries" section for the corrected
# five-phase model.  EM-7c will reintroduce a pattern-matcher gate using
# the corrected emit shape (bb_flat in EMIT_TEXT mode for invariant
# sub-trees + bb_emit BINARY mode for variant nodes); the test slot is
# reserved for that work.

echo
echo "PASS=9 FAIL=0  (EM-1 wiring + EM-2 HALT/PUSH_LIT_I + EM-3 stack ops + arithmetic + EM-4 control flow + EM-5 chunks; EM-6 retired, awaiting EM-7c)"
