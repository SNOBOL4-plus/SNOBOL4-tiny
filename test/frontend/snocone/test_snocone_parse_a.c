/*
 * test_snocone_parse_a.c — GOAL-SNOCONE-LANG-SPACE LS-4.a acceptance test
 *
 * The LS-4.a gate: parse `OUTPUT = 2 + 3;` and verify the resulting
 * Program* has the correct shape — one statement whose subject is
 * the variable OUTPUT and whose replacement is the IR tree
 * E_ADD(E_ILIT(2), E_ILIT(3)).
 *
 * No interpreter linking; pure parser-side verification.  When LS-4.j
 * lands, the same source will go through scrip --ir-run and produce
 * the literal output "5" (matching the existing snocone smoke gate's
 * "arith" case).  For LS-4.a we verify the IR shape only.
 *
 * Build:
 *   cc -Wall -o test_snocone_parse_a test_snocone_parse_a.c \
 *       ../../src/frontend/snocone/snocone.tab.c \
 *       ../../src/frontend/snocone/snocone_lex.c \
 *       -I ../../src/frontend/snocone \
 *       -I ../../src/frontend/snobol4 \
 *       -I ../../src
 *
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Pull in the shared IR (EXPR_t, Program, EKind, expr_*, stmt_new, etc.).
 * IR_DEFINE_NAMES requests the optional ekind_name[] table so we can
 * print kind labels in the IR dump without linking ir_print.c. */
#define IR_DEFINE_NAMES
#include "scrip_cc.h"

/* Public entry from snocone.tab.c */
Program *snocone_parse_program(const char *src, const char *filename);

/* Mini IR printer — compact one-line tree dump for verification.
 * This avoids linking the runtime's heavyweight ir_print / ir_dump_program. */
static void dump_expr(const EXPR_t *e, FILE *out) {
    if (!e) { fputs("<nil>", out); return; }
    fputs(ekind_name[e->kind] ? ekind_name[e->kind] : "?", out);
    switch (e->kind) {
        case E_VAR:
        case E_KEYWORD:
        case E_QLIT:
            fprintf(out, "(\"%s\")", e->sval ? e->sval : "");
            break;
        case E_ILIT:
            fprintf(out, "(%lld)", e->ival);
            break;
        case E_FLIT:
            fprintf(out, "(%g)", e->dval);
            break;
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
        if (s->subject) {
            fputs("    subject     = ", out);
            dump_expr(s->subject, out);
            fputc('\n', out);
        }
        if (s->pattern) {
            fputs("    pattern     = ", out);
            dump_expr(s->pattern, out);
            fputc('\n', out);
        }
        if (s->replacement) {
            fputs("    replacement = ", out);
            dump_expr(s->replacement, out);
            fputc('\n', out);
        }
    }
}

/* ---- one tiny test runner ---- */
static int g_pass = 0, g_fail = 0;

static void check(const char *label, int cond, const char *fmt, ...) {
    if (cond) {
        printf("  PASS %s\n", label);
        g_pass++;
    } else {
        va_list ap; va_start(ap, fmt);
        printf("  FAIL %s: ", label);
        vprintf(fmt, ap);
        putchar('\n');
        va_end(ap);
        g_fail++;
    }
}

/* ---- the LS-4.a checks ---- */
static void test_assign_int_plus_int(void) {
    const char *src = "OUTPUT = 2 + 3;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");

    check("parse succeeds", prog != NULL, "snocone_parse_program returned NULL");
    if (!prog) return;

    dump_program(prog, stdout);

    check("one statement", prog->nstmts == 1,
          "expected nstmts=1, got %d", prog->nstmts);
    check("head non-null", prog->head != NULL, "no head");
    if (!prog->head) return;

    STMT_t *s = prog->head;
    check("has_eq",   s->has_eq == 1,        "expected has_eq=1, got %d", s->has_eq);
    check("subject is E_VAR", s->subject && s->subject->kind == E_VAR,
          "expected subject kind=E_VAR");
    check("subject sval = OUTPUT",
          s->subject && s->subject->sval &&
              strcmp(s->subject->sval, "OUTPUT") == 0,
          "expected OUTPUT, got %s", s->subject ? s->subject->sval : "(nil)");

    check("replacement is E_ADD",
          s->replacement && s->replacement->kind == E_ADD,
          "expected replacement kind=E_ADD");
    if (s->replacement && s->replacement->kind == E_ADD &&
        s->replacement->nchildren == 2) {
        EXPR_t *l = s->replacement->children[0];
        EXPR_t *r = s->replacement->children[1];
        check("E_ADD left = E_ILIT(2)",
              l && l->kind == E_ILIT && l->ival == 2,
              "got kind=%d ival=%lld",
              l ? (int)l->kind : -1, l ? l->ival : -1LL);
        check("E_ADD right = E_ILIT(3)",
              r && r->kind == E_ILIT && r->ival == 3,
              "got kind=%d ival=%lld",
              r ? (int)r->kind : -1, r ? r->ival : -1LL);
    }
}

static void test_arith_precedence(void) {
    /* a * b + c — should be E_ADD(E_MUL(a,b), c)  (LS-4.a precedence check) */
    const char *src = "X = A * B + C;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head || !prog->head->replacement) return;
    EXPR_t *r = prog->head->replacement;
    check("top is E_ADD", r->kind == E_ADD, "top kind=%d", (int)r->kind);
    if (r->kind == E_ADD && r->nchildren == 2) {
        check("left is E_MUL", r->children[0]->kind == E_MUL,
              "got %d", (int)r->children[0]->kind);
        check("right is E_VAR(C)",
              r->children[1]->kind == E_VAR &&
                  strcmp(r->children[1]->sval, "C") == 0,
              "got kind=%d sval=%s",
              (int)r->children[1]->kind,
              r->children[1]->sval ? r->children[1]->sval : "(nil)");
    }
}

static void test_paren_overrides(void) {
    /* a * (b + c) — should be E_MUL(a, E_ADD(b,c)) */
    const char *src = "X = A * (B + C);";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head || !prog->head->replacement) return;
    EXPR_t *r = prog->head->replacement;
    check("top is E_MUL", r->kind == E_MUL, "top kind=%d", (int)r->kind);
    if (r->kind == E_MUL && r->nchildren == 2) {
        check("right is E_ADD", r->children[1]->kind == E_ADD,
              "got %d", (int)r->children[1]->kind);
    }
}

static void test_exponent_right_assoc(void) {
    /* 2 ^ 3 ^ 2 — right assoc → E_POW(2, E_POW(3, 2)) */
    const char *src = "X = 2 ^ 3 ^ 2;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head || !prog->head->replacement) return;
    EXPR_t *r = prog->head->replacement;
    check("top is E_POW", r->kind == E_POW, "top kind=%d", (int)r->kind);
    if (r->kind == E_POW && r->nchildren == 2) {
        check("left is E_ILIT(2)",
              r->children[0]->kind == E_ILIT && r->children[0]->ival == 2,
              "got kind=%d ival=%lld",
              (int)r->children[0]->kind, r->children[0]->ival);
        check("right is E_POW (right-assoc)",
              r->children[1]->kind == E_POW,
              "got kind=%d", (int)r->children[1]->kind);
    }
}

static void test_unary_minus(void) {
    const char *src = "X = -5;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head || !prog->head->replacement) return;
    EXPR_t *r = prog->head->replacement;
    check("top is E_MNS", r->kind == E_MNS, "top kind=%d", (int)r->kind);
    if (r->kind == E_MNS && r->nchildren == 1) {
        check("child is E_ILIT(5)",
              r->children[0]->kind == E_ILIT && r->children[0]->ival == 5,
              "got kind=%d ival=%lld",
              (int)r->children[0]->kind, r->children[0]->ival);
    }
}

static void test_bare_expr_stmt(void) {
    /* Bare expression — no `=`, lands in subject only (per LS-4.a rules). */
    const char *src = "1 + 1;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head) return;
    check("has_eq=0", prog->head->has_eq == 0,
          "expected 0 got %d", prog->head->has_eq);
    check("subject is E_ADD",
          prog->head->subject && prog->head->subject->kind == E_ADD,
          "got kind=%d",
          prog->head->subject ? (int)prog->head->subject->kind : -1);
    check("replacement is NULL", prog->head->replacement == NULL,
          "non-null");
}

static void test_multiple_statements(void) {
    const char *src = "A = 1; B = 2; C = 3;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog) return;
    check("nstmts=3", prog->nstmts == 3, "got %d", prog->nstmts);
}

static void test_string_literal(void) {
    const char *src = "OUTPUT = \"hello\";";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head || !prog->head->replacement) return;
    EXPR_t *r = prog->head->replacement;
    check("replacement is E_QLIT",
          r->kind == E_QLIT, "got kind=%d", (int)r->kind);
    check("sval = hello",
          r->sval && strcmp(r->sval, "hello") == 0,
          "got '%s'", r->sval ? r->sval : "(nil)");
}

static void test_real_literal(void) {
    const char *src = "X = 3.14;";
    printf("=== test: %s ===\n", src);
    Program *prog = snocone_parse_program(src, "<test>");
    check("parses", prog != NULL, "NULL");
    if (!prog || !prog->head || !prog->head->replacement) return;
    EXPR_t *r = prog->head->replacement;
    check("replacement is E_FLIT",
          r->kind == E_FLIT, "got kind=%d", (int)r->kind);
    check("dval ≈ 3.14",
          r->dval > 3.13 && r->dval < 3.15,
          "got %g", r->dval);
}

int main(void) {
    test_assign_int_plus_int();
    test_arith_precedence();
    test_paren_overrides();
    test_exponent_right_assoc();
    test_unary_minus();
    test_bare_expr_stmt();
    test_multiple_statements();
    test_string_literal();
    test_real_literal();

    printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
