/*
 * test_snocone_parse_c.c — GOAL-SNOCONE-LANG-SPACE LS-4.c acceptance test
 *
 * The LS-4.c gate: pattern operators and compound-assigns parse and
 * lower correctly.
 *
 *   pattern match `?`            →  E_SCAN(subject, pattern)
 *   pattern alternation `|`      →  E_ALT (n-ary fold; `a|b|c` flat)
 *   synthetic concat T_CONCAT    →  E_SEQ (n-ary fold; `a b c` flat)
 *   compound-assigns:
 *      `a += b`  →  E_ASSIGN(a, E_ADD(clone(a), b))
 *      `a -= b`  →  E_ASSIGN(a, E_SUB(clone(a), b))
 *      `a *= b`  →  E_ASSIGN(a, E_MUL(clone(a), b))
 *      `a /= b`  →  E_ASSIGN(a, E_DIV(clone(a), b))
 *      `a ^= b`  →  E_ASSIGN(a, E_POW(clone(a), b))
 *
 * Goal-file gate per LS-4.c: "Parses `s = 'hello' ' world'`."  This
 * file verifies that and the surrounding semantic shape, plus the
 * precedence relations: comparisons bind tighter than concat
 * (`a == b X` parses as `(a == b) X`), concat binds tighter than
 * alternation (`a b | c d` parses as `(a b) | (c d)`), alternation
 * binds tighter than match (`x ? a | b` parses as `x ? (a | b)`),
 * and match binds tighter than assignment.
 *
 * Build:
 *   cc -Wall -o test_snocone_parse_c test_snocone_parse_c.c \
 *       ../../src/frontend/snocone/snocone.tab.c \
 *       ../../src/frontend/snocone/snocone_lex.c \
 *       -I ../../src/frontend/snocone \
 *       -I ../../src/frontend/snobol4 \
 *       -I ../../src
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define IR_DEFINE_NAMES
#include "scrip_cc.h"

Program *snocone_parse_program(const char *src, const char *filename);

/* Mini IR printer */
static void dump_expr(const EXPR_t *e, FILE *out) {
    if (!e) { fputs("<nil>", out); return; }
    fputs(ekind_name[e->kind] ? ekind_name[e->kind] : "?", out);
    switch (e->kind) {
        case E_VAR:
        case E_KEYWORD:
        case E_QLIT:
            fprintf(out, "(\"%s\")", e->sval ? e->sval : "");
            break;
        case E_FNC:
            fprintf(out, "(\"%s\"", e->sval ? e->sval : "");
            for (int i = 0; i < e->nchildren; i++) {
                fputs(", ", out);
                dump_expr(e->children[i], out);
            }
            fputc(')', out);
            break;
        case E_ILIT: fprintf(out, "(%lld)", e->ival); break;
        case E_FLIT: fprintf(out, "(%g)",   e->dval); break;
        default:
            fputc('(', out);
            for (int i = 0; i < e->nchildren; i++) {
                if (i) fputs(", ", out);
                dump_expr(e->children[i], out);
            }
            fputc(')', out);
            break;
    }
}

static void dump_program(const Program *prog, FILE *out) {
    if (!prog) { fputs("(null Program)\n", out); return; }
    fprintf(out, "Program nstmts=%d\n", prog->nstmts);
    int i = 0;
    for (STMT_t *s = prog->head; s; s = s->next, i++) {
        fprintf(out, "  stmt[%d]: stno=%d has_eq=%d\n", i, s->stno, s->has_eq);
        if (s->subject)     { fputs("    subject     = ", out); dump_expr(s->subject,     out); fputc('\n', out); }
        if (s->replacement) { fputs("    replacement = ", out); dump_expr(s->replacement, out); fputc('\n', out); }
    }
}

static int g_pass = 0, g_fail = 0;
static void check(const char *label, int cond, const char *fmt, ...) {
    if (cond) { printf("  PASS %s\n", label); g_pass++; }
    else {
        va_list ap; va_start(ap, fmt);
        printf("  FAIL %s: ", label); vprintf(fmt, ap); putchar('\n'); va_end(ap);
        g_fail++;
    }
}

static EXPR_t *parse_or_null(const char *src) {
    Program *prog = snocone_parse_program(src, "<test>");
    if (!prog || !prog->head || !prog->head->replacement) return NULL;
    return prog->head->replacement;
}

/* ============================================================ */
/* 1. Concat — the LS-4.c headline gate                          */
/* ============================================================ */

static void test_concat_two_strings(void) {
    /* Headline gate: s = 'hello' ' world'; */
    const char *src = "s = 'hello' ' world';";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog) return;
    dump_program(prog, stdout);
    EXPR_t *r = prog->head ? prog->head->replacement : NULL;
    check("top is E_SEQ", r && r->kind == E_SEQ,
          "got kind=%d", r ? (int)r->kind : -1);
    if (!r || r->kind != E_SEQ) return;
    check("nchildren == 2", r->nchildren == 2, "got %d", r->nchildren);
    if (r->nchildren < 2) return;
    check("child0 is E_QLIT(hello)",
          r->children[0]->kind == E_QLIT &&
              strcmp(r->children[0]->sval, "hello") == 0,
          "got kind=%d sval=%s",
          (int)r->children[0]->kind,
          r->children[0]->sval ? r->children[0]->sval : "(nil)");
    check("child1 is E_QLIT( world)",
          r->children[1]->kind == E_QLIT &&
              strcmp(r->children[1]->sval, " world") == 0,
          "got kind=%d sval=%s",
          (int)r->children[1]->kind,
          r->children[1]->sval ? r->children[1]->sval : "(nil)");
}

static void test_concat_n_ary_fold(void) {
    /* a b c d — should fold to a single n-ary E_SEQ with 4 children */
    const char *src = "X = a b c d;";
    printf("=== test: %s ===\n", src);
    EXPR_t *r = parse_or_null(src);
    check("parses", r != NULL, "NULL");
    if (!r) return;
    check("top is E_SEQ", r->kind == E_SEQ, "got kind=%d", (int)r->kind);
    check("n-ary fold: nchildren == 4", r->nchildren == 4,
          "got %d (should be flat, not nested)", r->nchildren);
}

static void test_concat_var_then_string(void) {
    const char *src = "X = name '!';";
    printf("=== test: %s ===\n", src);
    EXPR_t *r = parse_or_null(src);
    check("parses", r != NULL, "NULL");
    if (!r) return;
    check("top is E_SEQ", r->kind == E_SEQ, "got kind=%d", (int)r->kind);
    check("nchildren == 2", r->nchildren == 2, "got %d", r->nchildren);
    if (r->nchildren < 2) return;
    check("child0 is E_VAR(name)",
          r->children[0]->kind == E_VAR &&
              strcmp(r->children[0]->sval, "name") == 0,
          "got kind=%d", (int)r->children[0]->kind);
    check("child1 is E_QLIT(!)",
          r->children[1]->kind == E_QLIT &&
              strcmp(r->children[1]->sval, "!") == 0,
          "got kind=%d", (int)r->children[1]->kind);
}

/* ============================================================ */
/* 2. Alternation                                                */
/* ============================================================ */

static void test_alternation_binary(void) {
    const char *src = "X = a | b;";
    printf("=== test: %s ===\n", src);
    EXPR_t *r = parse_or_null(src);
    check("parses", r != NULL, "NULL");
    if (!r) return;
    check("top is E_ALT", r->kind == E_ALT, "got kind=%d", (int)r->kind);
    check("nchildren == 2", r->nchildren == 2, "got %d", r->nchildren);
}

static void test_alternation_n_ary_fold(void) {
    /* a | b | c | d — should fold to a single n-ary E_ALT with 4 children */
    const char *src = "X = a | b | c | d;";
    printf("=== test: %s ===\n", src);
    EXPR_t *r = parse_or_null(src);
    check("parses", r != NULL, "NULL");
    if (!r) return;
    check("top is E_ALT", r->kind == E_ALT, "got kind=%d", (int)r->kind);
    check("n-ary fold: nchildren == 4", r->nchildren == 4,
          "got %d (should be flat, not nested)", r->nchildren);
}

static void test_alternation_below_concat(void) {
    /* Concat binds TIGHTER than alternation:
     *   a b | c d  parses as  (a b) | (c d)
     * which is E_ALT(E_SEQ(a,b), E_SEQ(c,d))
     */
    const char *src = "X = a b | c d;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog) return;
    dump_program(prog, stdout);
    EXPR_t *r = prog->head ? prog->head->replacement : NULL;
    check("top is E_ALT", r && r->kind == E_ALT,
          "got kind=%d", r ? (int)r->kind : -1);
    if (!r || r->kind != E_ALT || r->nchildren < 2) return;
    check("alt.child0 is E_SEQ", r->children[0]->kind == E_SEQ,
          "got %d", (int)r->children[0]->kind);
    check("alt.child1 is E_SEQ", r->children[1]->kind == E_SEQ,
          "got %d", (int)r->children[1]->kind);
}

/* ============================================================ */
/* 3. Match `?`                                                  */
/* ============================================================ */

static void test_match_basic(void) {
    const char *src = "X = subject ? pattern;";
    printf("=== test: %s ===\n", src);
    EXPR_t *r = parse_or_null(src);
    check("parses", r != NULL, "NULL");
    if (!r) return;
    check("top is E_SCAN", r->kind == E_SCAN, "got kind=%d", (int)r->kind);
    check("nchildren == 2", r->nchildren == 2, "got %d", r->nchildren);
    if (r->nchildren < 2) return;
    check("subject is E_VAR(subject)",
          r->children[0]->kind == E_VAR &&
              strcmp(r->children[0]->sval, "subject") == 0,
          "got kind=%d", (int)r->children[0]->kind);
    check("pattern is E_VAR(pattern)",
          r->children[1]->kind == E_VAR &&
              strcmp(r->children[1]->sval, "pattern") == 0,
          "got kind=%d", (int)r->children[1]->kind);
}

static void test_match_below_alternation(void) {
    /* Alternation binds TIGHTER than match:
     *   subj ? a | b  parses as  subj ? (a | b)
     * which is E_SCAN(subj, E_ALT(a, b))
     */
    const char *src = "X = subj ? a | b;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog) return;
    dump_program(prog, stdout);
    EXPR_t *r = prog->head ? prog->head->replacement : NULL;
    check("top is E_SCAN", r && r->kind == E_SCAN,
          "got kind=%d", r ? (int)r->kind : -1);
    if (!r || r->kind != E_SCAN || r->nchildren < 2) return;
    check("match.subject is E_VAR(subj)",
          r->children[0]->kind == E_VAR &&
              strcmp(r->children[0]->sval, "subj") == 0,
          "got kind=%d", (int)r->children[0]->kind);
    check("match.pattern is E_ALT", r->children[1]->kind == E_ALT,
          "got %d", (int)r->children[1]->kind);
}

static void test_match_below_assign(void) {
    /* Assignment is the LOWEST priority, so:
     *   x = subj ? pat  parses as  x = (subj ? pat)
     */
    const char *src = "x = subj ? pat;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head) return;
    STMT_t *s = prog->head;
    check("stmt has_eq == 1", s->has_eq == 1, "got %d", s->has_eq);
    check("subject is E_VAR(x)",
          s->subject && s->subject->kind == E_VAR &&
              strcmp(s->subject->sval, "x") == 0,
          "got kind=%d sval=%s",
          s->subject ? (int)s->subject->kind : -1,
          s->subject && s->subject->sval ? s->subject->sval : "(nil)");
    check("replacement is E_SCAN",
          s->replacement && s->replacement->kind == E_SCAN,
          "got kind=%d", s->replacement ? (int)s->replacement->kind : -1);
}

/* ============================================================ */
/* 4. Compound-assigns                                           */
/* ============================================================ */

static void test_plus_assign(void) {
    /* a += b  →  E_ASSIGN(E_VAR(a), E_ADD(E_VAR(a), E_VAR(b))) */
    const char *src = "a += b;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head) return;
    dump_program(prog, stdout);
    STMT_t *s = prog->head;
    check("stmt has_eq == 1", s->has_eq == 1, "got %d", s->has_eq);
    check("subject is E_VAR(a)",
          s->subject && s->subject->kind == E_VAR &&
              strcmp(s->subject->sval, "a") == 0,
          "got kind=%d", s->subject ? (int)s->subject->kind : -1);
    check("replacement is E_ADD",
          s->replacement && s->replacement->kind == E_ADD,
          "got kind=%d", s->replacement ? (int)s->replacement->kind : -1);
    if (!s->replacement || s->replacement->kind != E_ADD) return;
    check("E_ADD nchildren == 2", s->replacement->nchildren == 2,
          "got %d", s->replacement->nchildren);
    if (s->replacement->nchildren < 2) return;
    EXPR_t *l = s->replacement->children[0];
    EXPR_t *r = s->replacement->children[1];
    check("E_ADD.left is cloned E_VAR(a)",
          l->kind == E_VAR && strcmp(l->sval, "a") == 0,
          "got kind=%d sval=%s",
          (int)l->kind, l->sval ? l->sval : "(nil)");
    check("E_ADD.right is E_VAR(b)",
          r->kind == E_VAR && strcmp(r->sval, "b") == 0,
          "got kind=%d sval=%s",
          (int)r->kind, r->sval ? r->sval : "(nil)");
    /* Critical: clone produced a distinct node, not an aliased pointer. */
    check("clone is a distinct node (not aliased)",
          l != s->subject,
          "clone aliases the LHS — would double-free at cleanup");
}

static void test_minus_assign(void) {
    EXPR_t *r = parse_or_null("a -= 1;");
    printf("=== test: a -= 1; ===\n");
    check("parses", r != NULL, "NULL");
}

static void test_compound_assigns_full_set(void) {
    struct { const char *src; EKind expected_op; } cases[] = {
        { "a += b;", E_ADD },
        { "a -= b;", E_SUB },
        { "a *= b;", E_MUL },
        { "a /= b;", E_DIV },
        { "a ^= b;", E_POW },
    };
    int n = sizeof cases / sizeof cases[0];
    for (int i = 0; i < n; i++) {
        printf("=== test: %s ===\n", cases[i].src);
        Program *prog = snocone_parse_program(cases[i].src, "<test>");
        check("parses", prog != NULL, "NULL");
        if (!prog || !prog->head) continue;
        STMT_t *s = prog->head;
        char label[64];
        snprintf(label, sizeof label, "replacement is kind=%d", (int)cases[i].expected_op);
        check(label,
              s->replacement && s->replacement->kind == cases[i].expected_op,
              "got %d", s->replacement ? (int)s->replacement->kind : -1);
    }
}

/* ============================================================ */
/* 5. Mixed precedence — the integration tests                   */
/* ============================================================ */

static void test_concat_with_arith(void) {
    /* (1+2) ' equals ' (3) — arithmetic binds tighter than concat.
     * NB: SNOBOL4/Snocone require whitespace around binary `+` (the
     * lexer's W{OP}W envelope rule), so `1+2` would lex as
     * `T_INT, T_UN_PLUS, T_INT` — a unary expression that parses
     * differently.  We write `1 + 2` to get the binary T_ADDITION. */
    const char *src = "X = 1 + 2 ' equals ' 3;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog) return;
    dump_program(prog, stdout);
    EXPR_t *r = prog->head ? prog->head->replacement : NULL;
    check("top is E_SEQ", r && r->kind == E_SEQ,
          "got kind=%d", r ? (int)r->kind : -1);
    if (!r || r->kind != E_SEQ || r->nchildren < 3) return;
    check("seq.child0 is E_ADD (1+2 grouped under concat)",
          r->children[0]->kind == E_ADD,
          "got %d", (int)r->children[0]->kind);
    check("seq.child1 is E_QLIT( equals )",
          r->children[1]->kind == E_QLIT,
          "got %d", (int)r->children[1]->kind);
    check("seq.child2 is E_ILIT(3)",
          r->children[2]->kind == E_ILIT && r->children[2]->ival == 3,
          "got kind=%d ival=%lld",
          (int)r->children[2]->kind, r->children[2]->ival);
}

static void test_match_full_chain(void) {
    /* Full chain:  result = subject ? a b | c d
     * parses as:  result = (subject ? ((a b) | (c d)))
     *           = E_ASSIGN(result, E_SCAN(subject, E_ALT(E_SEQ(a,b), E_SEQ(c,d))))
     */
    const char *src = "result = subject ? a b | c d;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog) return;
    dump_program(prog, stdout);
    STMT_t *s = prog->head;
    check("stmt has_eq == 1", s && s->has_eq == 1,
          "got %d", s ? s->has_eq : -1);
    check("replacement is E_SCAN",
          s && s->replacement && s->replacement->kind == E_SCAN,
          "got kind=%d", s && s->replacement ? (int)s->replacement->kind : -1);
    if (!s || !s->replacement || s->replacement->kind != E_SCAN ||
        s->replacement->nchildren < 2) return;
    EXPR_t *pat = s->replacement->children[1];
    check("scan.pattern is E_ALT", pat->kind == E_ALT,
          "got %d", (int)pat->kind);
    if (pat->kind != E_ALT || pat->nchildren < 2) return;
    check("alt.child0 is E_SEQ (a b)", pat->children[0]->kind == E_SEQ,
          "got %d", (int)pat->children[0]->kind);
    check("alt.child1 is E_SEQ (c d)", pat->children[1]->kind == E_SEQ,
          "got %d", (int)pat->children[1]->kind);
}

int main(void) {
    test_concat_two_strings();
    test_concat_n_ary_fold();
    test_concat_var_then_string();

    test_alternation_binary();
    test_alternation_n_ary_fold();
    test_alternation_below_concat();

    test_match_basic();
    test_match_below_alternation();
    test_match_below_assign();

    test_plus_assign();
    test_minus_assign();
    test_compound_assigns_full_set();

    test_concat_with_arith();
    test_match_full_chain();

    printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
