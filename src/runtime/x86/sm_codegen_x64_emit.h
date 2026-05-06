/*
 * sm_codegen_x64_emit.h — SM_Program → standalone x86-64 asm/binary
 *                        (M-JITEM-X64, GOAL-MODE4-EMIT EM-1+)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * This is the mode-4 emitter entry point. Distinct from sm_codegen.c's
 * --jit-run path, which builds an in-process RX slab and jumps in.
 * mode-4 emits standalone assembler text that, when assembled and linked
 * against libscrip_rt.so, runs the program as an independent ELF binary.
 *
 * EM-1 scope (this commit): wiring + literal-zero stub. The function
 * writes a minimal asm program whose `main` returns 0 (and ignores the
 * SM_Program entirely). Subsequent rungs (EM-2..EM-9) progressively
 * cover the SM opcode set.
 */

#ifndef SM_CODEGEN_X64_EMIT_H
#define SM_CODEGEN_X64_EMIT_H

#include <stdio.h>
#include "sm_prog.h"

/*
 * sm_codegen_x64_emit — emit asm for prog to `out`.
 *
 * EM-1: writes a literal-zero program (System V AMD64 main returning 0)
 * regardless of prog contents. Returns 0 on success, -1 on I/O error.
 *
 * Subsequent EM-N rungs extend coverage; the function signature is stable.
 */
int sm_codegen_x64_emit(SM_Program *prog, FILE *out);

#endif /* SM_CODEGEN_X64_EMIT_H */
