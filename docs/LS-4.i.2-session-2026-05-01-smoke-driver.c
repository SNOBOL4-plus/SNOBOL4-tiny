/*
 * docs/LS-4.i.2-session-2026-05-01-smoke-driver.c
 *
 * GOAL-SNOCONE-LANG-SPACE LS-4.i.2 — break/continue end-to-end smoke driver.
 *
 * Saved at session #2 (2026-05-01) handoff per RULES.md "Diagnostic patches
 * stay in docs" convention.  This is a side-channel reproducer for the
 * LS-4.i.2 implementation; it parses a snippet exercising bare break,
 * bare continue, labeled break, and labeled continue across nested while
 * loops, and verifies the lowered IR's goto targets land where expected.
 *
 * Build:
 *   cc -Wall -o /tmp/ls4i2_smoke docs/LS-4.i.2-session-2026-05-01-smoke-driver.c \
 *       src/frontend/snocone/snocone_parse.tab.c \
 *       src/frontend/snocone/snocone_lex.c \
 *       -I src/frontend/snocone -I src/frontend/snobol4 -I src
 *
 * Run:
 *   /tmp/ls4i2_smoke
 *
 * Expected output:
 *   parsed OK, N stmts
 *   break-to-outer-end gotos: 1
 *   goto-to-outer-top: 2  (1 from `continue outer` + 1 from natural back-edge)
 *
 * Session #3 task: fold these into test/frontend/snocone/test_snocone_parse_i.c
 * with proper ASSERT-based test functions, then close LS-4.i.2.
 */
#include <stdio.h>
#include <string.h>
#define IR_DEFINE_NAMES
#include "scrip_cc.h"
extern Program *snocone_parse_program(const char *src, const char *filename);

int main(void) {
    /* Test 1 — single-loop bare break + bare continue */
    {
        const char *src =
            "i = 0;\n"
            "while (LT(i, 10)) {\n"
            "  i = i + 1;\n"
            "  if (EQ(i, 5)) break;\n"
            "  if (EQ(i, 3)) continue;\n"
            "}\n";
        Program *p = snocone_parse_program(src, "test1");
        if (!p) { printf("FAIL parse 1\n"); return 1; }
        int n = 0; for (STMT_t *s = p->head; s; s = s->next) n++;
        printf("test 1 parsed OK, %d stmts\n", n);
        for (STMT_t *s = p->head; s; s = s->next) {
            const char *lab = s->label ? s->label : "-";
            const char *unc = (s->go && s->go->uncond) ? s->go->uncond : "-";
            const char *onf = (s->go && s->go->onfailure) ? s->go->onfailure : "-";
            const char *ons = (s->go && s->go->onsuccess) ? s->go->onsuccess : "-";
            printf("  lab=%-12s unc=%-12s onF=%-12s onS=%-12s subj=%s\n",
                   lab, unc, onf, ons, s->subject ? "Y" : "-");
        }
    }

    /* Test 2 — labeled break/continue across nested loops */
    {
        const char *src =
            "outer: while (LT(i, 10)) {\n"
            "  while (LT(j, 5)) {\n"
            "    if (EQ(i, 7)) break outer;\n"
            "    if (EQ(j, 3)) continue outer;\n"
            "    j = j + 1;\n"
            "  }\n"
            "  i = i + 1;\n"
            "}\n";
        Program *p = snocone_parse_program(src, "test2");
        if (!p) { printf("FAIL parse 2\n"); return 1; }
        int found_break_to_outer = 0;
        int found_continue_to_outer = 0;
        for (STMT_t *s = p->head; s; s = s->next) {
            if (s->go && s->go->uncond) {
                if (strcmp(s->go->uncond, "_Lend_0002") == 0) found_break_to_outer++;
                if (strcmp(s->go->uncond, "_Ltop_0001") == 0) found_continue_to_outer++;
            }
        }
        printf("test 2: break-to-outer-end gotos: %d (expect >=1)\n", found_break_to_outer);
        printf("test 2: goto-to-outer-top:        %d (expect 2: continue outer + back-edge)\n",
               found_continue_to_outer);
        if (found_break_to_outer < 1 || found_continue_to_outer < 2) return 1;
    }

    /* Test 3 — error: break outside loop */
    {
        const char *src = "break;\n";
        fflush(stdout);
        Program *p = snocone_parse_program(src, "test3");
        printf("test 3: break-outside-loop -> %s (expect parse failed)\n",
               p == NULL ? "parse failed (correct)" : "PARSED (wrong!)");
        if (p) return 1;
    }

    /* Test 4 — for-loop continue triggers Lcont pad emission */
    {
        const char *src =
            "for (i = 0; LT(i, 10); i = i + 1) {\n"
            "  if (EQ(i, 5)) continue;\n"
            "  x = x + i;\n"
            "}\n";
        Program *p = snocone_parse_program(src, "test4");
        if (!p) { printf("FAIL parse 4\n"); return 1; }
        int found_lcont = 0;
        for (STMT_t *s = p->head; s; s = s->next) {
            if (s->label && strncmp(s->label, "_Lcont_", 7) == 0) found_lcont++;
        }
        printf("test 4: _Lcont_* labels in for+continue: %d (expect 1)\n", found_lcont);
        if (found_lcont != 1) return 1;
    }

    /* Test 5 — for-loop without continue: Lcont pad NOT emitted (lazy-emit) */
    {
        const char *src =
            "for (i = 0; LT(i, 10); i = i + 1) {\n"
            "  x = x + i;\n"
            "}\n";
        Program *p = snocone_parse_program(src, "test5");
        if (!p) { printf("FAIL parse 5\n"); return 1; }
        int found_lcont = 0;
        for (STMT_t *s = p->head; s; s = s->next) {
            if (s->label && strncmp(s->label, "_Lcont_", 7) == 0) found_lcont++;
        }
        printf("test 5: _Lcont_* labels in for-without-continue: %d (expect 0)\n", found_lcont);
        if (found_lcont != 0) return 1;
    }

    printf("\nALL SMOKE CHECKS PASSED\n");
    return 0;
}
