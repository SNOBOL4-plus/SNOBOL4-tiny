/*
 * scrip_rt.h — public ABI for libscrip_rt.so (M-JITEM-X64 / EM-1+)
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
 * EM-1 surface (this commit):
 *   scrip_rt_init      — process startup; receives argc/argv
 *   scrip_rt_finalize  — process shutdown; returns the program rc
 *
 * Both are no-ops in this rung. Subsequent rungs (EM-2..EM-9) add
 * push/pop/peek, call_chunk, push_int, halt, NV table entries, the
 * pattern matcher, builtins, etc.
 *
 * Stability: every symbol exported here is part of the mode-4 ABI.
 * Once an emitted binary references a symbol, the signature must not
 * change. Additions are backward-compatible by definition.
 */

#ifndef SCRIP_RT_H
#define SCRIP_RT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * scrip_rt_init — runtime startup. Called from emitted main() before
 * any other scrip_rt_* call. Receives the program's argv for &INPUT /
 * &OUTPUT wiring (in later rungs). EM-1: no-op.
 */
void scrip_rt_init(int argc, char **argv);

/*
 * scrip_rt_finalize — runtime shutdown. Called from emitted main() as
 * the last step. Returns the int the emitted main() should return —
 * the program's exit code. EM-1: returns 0.
 */
int scrip_rt_finalize(void);

#ifdef __cplusplus
}
#endif

#endif /* SCRIP_RT_H */
