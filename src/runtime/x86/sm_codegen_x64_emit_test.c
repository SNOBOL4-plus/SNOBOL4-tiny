/*
 * sm_codegen_x64_emit_test.c — synthetic-program harness for EM-2 gate
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * Builds a tiny SM_Program in memory and runs it through
 * sm_codegen_x64_emit().  The emitted asm is written to argv[1].
 * The companion shell script (test_smoke_jit_emit_x64_em2.sh) then
 * assembles, links against libscrip_rt.so, runs, and checks the rc.
 *
 * Why a standalone harness instead of a .sno fixture: the EM-2 rung
 * covers exactly two opcodes (SM_HALT, SM_PUSH_LIT_I).  No realistic
 * SNOBOL4 frontend lowering produces a SM_Program limited to those
 * two — even a minimal `END` lowers to substantially more.  A
 * synthetic program is the only way to exercise EM-2 in isolation.
 *
 * The synthetic program selected (per the rung's "exits with the
 * expected rc" gate language):
 *
 *     pc=0  SM_PUSH_LIT_I  42
 *     pc=1  SM_HALT
 *
 * Expected exit code: 42.
 *
 * Usage: sm_codegen_x64_emit_test <out.s>
 */

#include "sm_prog.h"
#include "sm_codegen_x64_emit.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <out.s>\n", argv[0]);
        return 2;
    }

    SM_Program *p = sm_prog_new();
    if (!p) { fprintf(stderr, "sm_prog_new failed\n"); return 1; }

    /* pc=0: SM_PUSH_LIT_I 42 */
    sm_emit_i(p, SM_PUSH_LIT_I, 42);
    /* pc=1: SM_HALT (rc <- TOS) */
    sm_emit(p, SM_HALT);

    FILE *out = fopen(argv[1], "w");
    if (!out) { perror(argv[1]); sm_prog_free(p); return 1; }

    int rc = sm_codegen_x64_emit(p, out);
    fclose(out);
    sm_prog_free(p);

    if (rc != 0) {
        fprintf(stderr, "sm_codegen_x64_emit failed (rc=%d)\n", rc);
        return 1;
    }
    return 0;
}
