/*
 * scrip_rt.h — public ABI for libscrip_rt.so (M-JITEM-X64 / EM-1+EM-2)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * libscrip_rt.so is the runtime support library that mode-4-emitted
 * binaries link against. It carries language-level semantics — pattern
 * matcher, NV table, builtins, GC, generator BB pump (post-Step 19),
 * Prolog backtracking machinery (post-Step 19) — so that emitted
 * executables contain only compiled SM chunks plus calls into a stable
 * C ABI defined here.
 *
 * EM-1 surface:
 *   scrip_rt_init      — process startup; receives argc/argv
 *   scrip_rt_finalize  — process shutdown; returns the program rc
 *
 * EM-2 surface (this commit):
 *   scrip_rt_push_int  — push a 64-bit int onto the SM value stack
 *   scrip_rt_pop_int   — pop the top of the SM value stack as int
 *   scrip_rt_halt      — record the program's exit code; falls through
 *                        to scrip_rt_finalize via the emitted epilogue
 *   scrip_rt_unhandled_op — runtime trap for opcodes the emitter
 *                           does not yet bake; aborts loudly
 *
 * Stability: every symbol exported here is part of the mode-4 ABI.
 * Once an emitted binary references a symbol, the signature must not
 * change. Additions are backward-compatible by definition.
 *
 * EM-2 deviation note: scrip_rt_pop_int was originally planned for
 * EM-3 (full SM stack-ops set: PUSH/POP/DUP/SWAP).  It is pulled
 * forward by one rung because SM_HALT's codegen needs it — without
 * it the EM-2 gate cannot distinguish a program that successfully
 * runs PUSH_LIT_I+HALT from one that no-ops.  See sm_codegen_x64_emit.c
 * emit_op_halt() for the full justification.
 */

#ifndef SCRIP_RT_H
#define SCRIP_RT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── EM-1 surface ────────────────────────────────────────────────────── */

/*
 * scrip_rt_init — runtime startup. Called from emitted main() before
 * any other scrip_rt_* call. Receives the program's argv for &INPUT /
 * &OUTPUT wiring (in later rungs). EM-1: no-op.
 */
void scrip_rt_init(int argc, char **argv);

/*
 * scrip_rt_finalize — runtime shutdown. Called from emitted main() as
 * the last step. Returns the int the emitted main() should return —
 * the program's exit code.
 *
 * EM-1: returns 0.
 * EM-2: returns whatever scrip_rt_halt last recorded; 0 if halt never
 * called.
 */
int scrip_rt_finalize(void);

/* ── EM-2 surface ────────────────────────────────────────────────────── */

/*
 * scrip_rt_push_int — push a signed 64-bit int onto the SM value stack.
 * The value stack lives entirely inside libscrip_rt.so — emitted code
 * sees it only through the push/pop ABI in this rung.  Future rungs
 * (EM-3+) may bake direct stack manipulation; until then every stack
 * touch is a function call.
 *
 * Capacity is fixed in EM-2 (small static buffer; see scrip_rt.c).
 * Overflow aborts via abort(3).  EM-3 may grow / heap-allocate.
 */
void scrip_rt_push_int(int64_t v);

/*
 * scrip_rt_pop_int — pop the SM value-stack top as a signed 64-bit int.
 * Underflow aborts via abort(3).
 */
int64_t scrip_rt_pop_int(void);

/*
 * scrip_rt_halt — record the program's exit rc.  The emitted epilogue
 * always falls through to scrip_rt_finalize, which returns the
 * recorded rc.  Calling scrip_rt_halt(rc) does NOT exit immediately;
 * it only sets state.  The corresponding SM-mode 2 semantics is
 * "stop running"; the codegen achieves that by emitting halt as the
 * last block before the epilogue.
 *
 * Note for future rungs: when control flow lands (EM-4), SM_HALT may
 * need to short-circuit out of arbitrary nesting.  At that point this
 * function will likely be replaced with a longjmp-based escape to the
 * epilogue, or the emitter will route halt through an unconditional
 * jump to .Lend.  For EM-2 the synthetic test programs are linear so
 * a simple state record suffices.
 */
void scrip_rt_halt(int rc);

/*
 * scrip_rt_unhandled_op — runtime trap for any SM opcode the emitter
 * has not yet baked.  Prints a diagnostic and aborts via abort(3).
 * `op` is the integer opcode value (sm_opcode_t cast to int) for
 * cross-reference against sm_opcode_name() in scrip dumps.
 */
void scrip_rt_unhandled_op(int op);

#ifdef __cplusplus
}
#endif

#endif /* SCRIP_RT_H */
