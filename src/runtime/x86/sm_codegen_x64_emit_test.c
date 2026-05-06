/*
 * sm_codegen_x64_emit_test.c -- synthetic-program harness for EM-2..EM-4 gate
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
 * EM-4a program (argv[3]):  forward jump + conditional shapes
 *   Demonstrates SM_JUMP, SM_JUMP_F (not taken when last_ok=1),
 *   SM_JUMP_S (taken when last_ok=1).  At process start last_ok=1
 *   so we can verify all three opcode shapes fire correctly without
 *   needing a runtime hook to flip last_ok.
 *   Expected exit code: 42.
 *
 * EM-4b program (argv[4]):  conditional backward loop body
 *   The asm produced is a small countdown loop:
 *     pc=0  PUSH_LIT_I 3                ; counter
 *     pc=1  LABEL                       ; loop top
 *     pc=2  PUSH_LIT_I 1
 *     pc=3  SUB                         ; counter -= 1
 *     pc=4  JUMP_F  1                   ; backward jump if !last_ok
 *     pc=5  HALT                        ; rc = remaining counter (0)
 *   Companion C driver flips last_ok to 0 before entering the SM body
 *   on iterations 1..3, then to 1 on the final iteration so the loop
 *   exits.  This proves the SM_JUMP_F backward-jump shape executes
 *   correctly.  See scripts/test_smoke_jit_emit_x64.sh test 6b.
 *   Expected exit code: 0.
 *
 * Usage: sm_codegen_x64_emit_test <em2.s> [<em3.s>] [<em4a.s>] [<em4b.s>]
 *   (each successive arg unlocks the next program; missing args = skip)
 */

#include "sm_prog.h"
#include "sm_codegen_x64_emit.h"

#include <stdio.h>
#include <stdlib.h>

static int emit_to(const char *path, SM_Program *p)
{
    FILE *out = fopen(path, "w");
    if (!out) { perror(path); return 1; }
    /* Synthetic programs don't have a source file; pass NULL so the
     * emitter falls back to structural banners only. */
    int rc = sm_codegen_x64_emit(p, out, NULL);
    fclose(out);
    return rc != 0 ? 1 : 0;
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 5) {
        fprintf(stderr,
            "usage: %s <em2.s> [<em3.s>] [<em4a.s>] [<em4b.s>]\n", argv[0]);
        return 2;
    }

    /* EM-2 program: PUSH_LIT_I 42 + HALT */
    {
        SM_Program *p = sm_prog_new();
        if (!p) { fprintf(stderr, "sm_prog_new failed\n"); return 1; }

        sm_emit_i(p, SM_PUSH_LIT_I, 42);
        sm_emit(p, SM_HALT);

        int rc = emit_to(argv[1], p);
        sm_prog_free(p);
        if (rc != 0) {
            fprintf(stderr, "sm_codegen_x64_emit failed for EM-2 program\n");
            return 1;
        }
    }

    if (argc < 3) return 0;

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

        int rc = emit_to(argv[2], p);
        sm_prog_free(p);
        if (rc != 0) {
            fprintf(stderr, "sm_codegen_x64_emit failed for EM-3 program\n");
            return 1;
        }
    }

    if (argc < 4) return 0;

    /* EM-4a program: forward jump + conditional jump shapes (last_ok=1).
     *
     * pc=0   PUSH_LIT_I  100      ; seed value on TOS
     * pc=1   JUMP    L_skip       ; forward jump skips dead code
     * pc=2   PUSH_LIT_I  99       ; DEAD
     * pc=3   HALT                 ; DEAD (would exit 99)
     * pc=4   LABEL  (L_skip)      ; landing pad
     * pc=5   PUSH_LIT_I  -58      ; -58
     * pc=6   ADD                  ; 100 + -58 = 42
     * pc=7   JUMP_F  L_dead       ; last_ok=1, NOT taken
     * pc=8   JUMP_S  L_final      ; last_ok=1, IS taken
     * pc=9   PUSH_LIT_I  77       ; DEAD-by-jump_s
     * pc=10  HALT                 ; DEAD-by-jump_s (would exit 77)
     * pc=11  LABEL  (L_dead)      ; would be JUMP_F target; unreached
     * pc=12  HALT                 ; unreached
     * pc=13  LABEL  (L_final)     ; JUMP_S landing
     * pc=14  HALT                 ; rc=42
     */
    {
        SM_Program *p = sm_prog_new();
        if (!p) { fprintf(stderr, "sm_prog_new failed\n"); return 1; }

        sm_emit_i(p, SM_PUSH_LIT_I, 100);    /* pc=0 */
        int j_skip = sm_emit_i(p, SM_JUMP, 0); /* pc=1 patch later */
        sm_emit_i(p, SM_PUSH_LIT_I, 99);     /* pc=2 dead */
        sm_emit(p,   SM_HALT);               /* pc=3 dead */
        int L_skip = sm_label(p);            /* pc=4 */
        sm_emit_i(p, SM_PUSH_LIT_I, -58);    /* pc=5 */
        sm_emit(p,   SM_ADD);                /* pc=6 -> TOS=42 */
        int j_dead = sm_emit_i(p, SM_JUMP_F, 0); /* pc=7 patch later */
        int j_final= sm_emit_i(p, SM_JUMP_S, 0); /* pc=8 patch later */
        sm_emit_i(p, SM_PUSH_LIT_I, 77);     /* pc=9 dead-by-jump_s */
        sm_emit(p,   SM_HALT);               /* pc=10 dead */
        int L_dead = sm_label(p);            /* pc=11 unreached */
        sm_emit(p,   SM_HALT);               /* pc=12 unreached */
        int L_final= sm_label(p);            /* pc=13 */
        sm_emit(p,   SM_HALT);               /* pc=14 rc=42 */

        sm_patch_jump(p, j_skip,  L_skip);
        sm_patch_jump(p, j_dead,  L_dead);
        sm_patch_jump(p, j_final, L_final);

        int rc = emit_to(argv[3], p);
        sm_prog_free(p);
        if (rc != 0) {
            fprintf(stderr, "sm_codegen_x64_emit failed for EM-4a program\n");
            return 1;
        }
    }

    if (argc < 5) return 0;

    /* EM-4b program: conditional backward loop body.
     *
     * pc=0   PUSH_LIT_I 3              ; counter on TOS
     * pc=1   LABEL                     ; loop top
     * pc=2   PUSH_LIT_I 1
     * pc=3   SUB                       ; counter -= 1
     * pc=4   JUMP_F  1                 ; if !last_ok, backward to loop top
     * pc=5   HALT                      ; rc = final counter
     *
     * The driver in test_smoke_jit_emit_x64.sh calls scrip_rt_set_last_ok(0)
     * before entering this body to drive the loop, then sets it to 1 to exit.
     * This proves SM_JUMP_F backward-jump shape executes correctly.
     */
    {
        SM_Program *p = sm_prog_new();
        if (!p) { fprintf(stderr, "sm_prog_new failed\n"); return 1; }

        sm_emit_i(p, SM_PUSH_LIT_I, 3);      /* pc=0 */
        int L_top = sm_label(p);             /* pc=1 */
        sm_emit_i(p, SM_PUSH_LIT_I, 1);      /* pc=2 */
        sm_emit(p,   SM_SUB);                /* pc=3 -> counter-=1 */
        int j_back = sm_emit_i(p, SM_JUMP_F, 0); /* pc=4 */
        sm_emit(p,   SM_HALT);               /* pc=5 */

        sm_patch_jump(p, j_back, L_top);

        int rc = emit_to(argv[4], p);
        sm_prog_free(p);
        if (rc != 0) {
            fprintf(stderr, "sm_codegen_x64_emit failed for EM-4b program\n");
            return 1;
        }
    }

    return 0;
}
