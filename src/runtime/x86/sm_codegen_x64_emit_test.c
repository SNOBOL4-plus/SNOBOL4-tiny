/*
 * sm_codegen_x64_emit_test.c -- synthetic-program harness for EM-2 + EM-3 gate
 *
 * Authors: Lon Jones Cherryholmes, Claude Sonnet
 * Date: 2026-05-06
 *
 * Builds tiny SM_Programs in memory and runs them through
 * sm_codegen_x64_emit(). The emitted asm is written to file paths
 * provided by argv. The companion shell script assembles, links against
 * libscrip_rt.so, runs, and checks the rc.
 *
 * EM-2 program (argv[1]):
 *   pc=0  SM_PUSH_LIT_I  42
 *   pc=1  SM_HALT
 *   Expected exit code: 42.
 *
 * EM-3 program (argv[2]):  (2 + 3) * 4 = 20
 *   pc=0  SM_PUSH_LIT_I  2
 *   pc=1  SM_PUSH_LIT_I  3
 *   pc=2  SM_ADD
 *   pc=3  SM_PUSH_LIT_I  4
 *   pc=4  SM_MUL
 *   pc=5  SM_HALT
 *   Expected exit code: 20.
 *
 * Usage: sm_codegen_x64_emit_test <em2.s> [<em3.s>]
 *   (argv[2] optional -- backward-compatible with EM-2 only usage)
 */

#include "sm_prog.h"
#include "sm_codegen_x64_emit.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: %s <em2.s> [<em3.s>]\n", argv[0]);
        return 2;
    }

    /* EM-2 program: PUSH_LIT_I 42 + HALT */
    {
        SM_Program *p = sm_prog_new();
        if (!p) { fprintf(stderr, "sm_prog_new failed\n"); return 1; }

        sm_emit_i(p, SM_PUSH_LIT_I, 42);
        sm_emit(p, SM_HALT);

        FILE *out = fopen(argv[1], "w");
        if (!out) { perror(argv[1]); sm_prog_free(p); return 1; }

        int rc = sm_codegen_x64_emit(p, out);
        fclose(out);
        sm_prog_free(p);

        if (rc != 0) {
            fprintf(stderr, "sm_codegen_x64_emit failed for EM-2 program (rc=%d)\n", rc);
            return 1;
        }
    }

    if (argc < 3) return 0;   /* backward-compatible: only EM-2 requested */

    /* EM-3 program: (2 + 3) * 4 = 20 */
    {
        SM_Program *p = sm_prog_new();
        if (!p) { fprintf(stderr, "sm_prog_new failed\n"); return 1; }

        sm_emit_i(p, SM_PUSH_LIT_I, 2);
        sm_emit_i(p, SM_PUSH_LIT_I, 3);
        sm_emit(p,  SM_ADD);
        sm_emit_i(p, SM_PUSH_LIT_I, 4);
        sm_emit(p,  SM_MUL);
        sm_emit(p,  SM_HALT);

        FILE *out = fopen(argv[2], "w");
        if (!out) { perror(argv[2]); sm_prog_free(p); return 1; }

        int rc = sm_codegen_x64_emit(p, out);
        fclose(out);
        sm_prog_free(p);

        if (rc != 0) {
            fprintf(stderr, "sm_codegen_x64_emit failed for EM-3 program (rc=%d)\n", rc);
            return 1;
        }
    }

    return 0;
}
