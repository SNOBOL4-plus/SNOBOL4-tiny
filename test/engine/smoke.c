/*
 * smoke.c — smoke test for engine.c / engine.h
 *
 * Builds the BEAD pattern in C and runs it against several subjects.
 * No Python.  No IR.  No emit_c.py.  Direct API.
 *
 * BEAD = POS(0)  (B|R)  (E|EA)  (D|DS)  RPOS(0)
 *
 * Build:
 *   cc -o smoke smoke.c ../../src/runtime/engine.c -I ../../src/runtime
 *   ./smoke
 */
#include <stdio.h>
#include <string.h>
#include "../../src/runtime/engine.h"

/* Helper: build a literal Pattern node */
static Pattern *lit(PatternList *pl, const char *s) {
    Pattern *p  = pattern_alloc(pl);
    p->type     = T_LITERAL;
    p->s        = s;
    p->s_len    = (int)strlen(s);
    return p;
}

/* Helper: build an alternation (Π) of two children */
static Pattern *alt2(PatternList *pl, Pattern *a, Pattern *b) {
    Pattern *p  = pattern_alloc(pl);
    p->type     = T_PI;
    p->n        = 2;
    p->children[0] = a;
    p->children[1] = b;
    return p;
}

/* Helper: build a sequence (Σ) of n children */
static Pattern *seq(PatternList *pl, int n, Pattern **kids) {
    Pattern *p  = pattern_alloc(pl);
    p->type     = T_SIGMA;
    p->n        = n;
    for (int i = 0; i < n; i++) p->children[i] = kids[i];
    return p;
}

/* Helper: POS(n) */
static Pattern *pos(PatternList *pl, int n) {
    Pattern *p  = pattern_alloc(pl);
    p->type     = T_POS;
    p->n        = n;
    return p;
}

/* Helper: RPOS(n) */
static Pattern *rpos(PatternList *pl, int n) {
    Pattern *p  = pattern_alloc(pl);
    p->type     = T_RPOS;
    p->n        = n;
    return p;
}

int main(void) {
    int passed = 0, failed = 0;

    /* Cases: { subject, expect_match, expect_end } */
    struct { const char *subj; int match; int end; } cases[] = {
        { "BEAD",   1, 4 },
        { "BEADS",  1, 5 },
        { "READS",  1, 5 },
        { "READ",   1, 4 },
        { "RED",    1, 3 },
        { "BED",    1, 3 },
        { "BEDS",   1, 4 },
        { "XYZ",    0, 0 },
        { "",       0, 0 },
        { "B",      0, 0 },
    };
    int ncases = (int)(sizeof(cases)/sizeof(cases[0]));

    printf("BEAD smoke test\n");
    printf("%-12s  %-6s  %-10s  %-10s  %s\n",
           "subject", "match", "end", "expected", "");

    for (int i = 0; i < ncases; i++) {
        PatternList pl = {NULL, 0};

        /* Build BEAD = POS(0) (B|R) (E|EA) (D|DS) RPOS(0) */
        Pattern *kids[5] = {
            pos(&pl, 0),
            alt2(&pl, lit(&pl, "B"),  lit(&pl, "R")),
            alt2(&pl, lit(&pl, "E"),  lit(&pl, "EA")),
            alt2(&pl, lit(&pl, "D"),  lit(&pl, "DS")),
            rpos(&pl, 0),
        };
        Pattern *bead = seq(&pl, 5, kids);

        const char *subj = cases[i].subj;
        MatchResult r = engine_match(bead, subj, (int)strlen(subj));
        pattern_free_all(&pl);

        int ok = (r.matched == cases[i].match) &&
                 (!r.matched || r.end == cases[i].end);
        printf("%-12s  %-6s  %-10d  match=%-3d end=%-3d  %s\n",
               subj[0] ? subj : "(empty)",
               r.matched ? "yes" : "no",
               r.end,
               cases[i].match, cases[i].end,
               ok ? "PASS" : "FAIL");
        if (ok) passed++; else failed++;
    }

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
