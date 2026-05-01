/*
 * test_snocone_parse_e.c — GOAL-SNOCONE-LANG-SPACE LS-4.e acceptance test
 *
 * The LS-4.e gate: all remaining SPITBOL unary operators parse and lower to
 * the correct EKind nodes.  Operators added this rung (beyond LS-4.a's
 * T_1PLUS / T_1MINUS):
 *
 *   *expr   → E_DEFER        (deferred evaluation / indirect pattern ref)
 *   .expr   → E_NAME         (name reference — returns name descriptor)
 *   $expr   → E_INDIRECT     (variable indirection)
 *   @expr   → E_CAPT_CURSOR  (cursor position capture)
 *   ~expr   → E_NOT          (negate success/failure)
 *   ?expr   → E_INTERROGATE  (null if succeeds, fail if fails)
 *   &expr   → E_OPSYN sval="&"  (bare ampersand — OPSYN slot pri 2)
 *   %expr   → E_OPSYN sval="%"  (OPSYN slot pri 10)
 *   /expr   → E_OPSYN sval="/"  (OPSYN slot)
 *   #expr   → E_OPSYN sval="#"  (OPSYN slot)
 *   |expr   → E_OPSYN sval="|"  (OPSYN slot)
 *   =expr   → E_OPSYN sval="="  (OPSYN slot)
 *
 * All unaries apply to expr17 (atoms) — highest priority, binding tighter
 * than any binary operator.  Chains like `~.x` (tilde then dot) nest right-
 * to-left: E_NOT(E_NAME(x)).
 *
 * Build:
 *   cc -Wall -o test_snocone_parse_e test_snocone_parse_e.c \
 *       ../../src/frontend/snocone/snocone_parse.tab.c \
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



/* ---- test harness ---- */
static int g_pass = 0, g_fail = 0;

static EXPR_t *parse_first_stmt(const char *src) {
    /* bare expressions go into head->subject (see snocone_parse.y:886) */
    Program *prog = snocone_parse_program(src, "<test>");
    if (!prog || !prog->head) return NULL;
    return prog->head->subject;
}

#define ASSERT(cond, fmt, ...) do { \
    if (cond) { g_pass++; } \
    else { fprintf(stderr, "FAIL %s:%d: " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); g_fail++; } \
} while(0)

/* ---- individual tests ---- */

static void test_defer_star(void) {
    /* *x  → E_DEFER(E_VAR("x")) */
    EXPR_t *e = parse_first_stmt("*x ;");
    ASSERT(e, "parsed *x");
    ASSERT(e->kind == E_DEFER, "kind E_DEFER, got %s", ekind_name[e->kind]);
    ASSERT(e->nchildren == 1, "one child");
    ASSERT(e->children[0]->kind == E_VAR, "child is E_VAR");
    ASSERT(strcmp(e->children[0]->sval, "x") == 0, "child sval=x");
}

static void test_name_dot(void) {
    /* .v  → E_NAME(E_VAR("v")) */
    EXPR_t *e = parse_first_stmt(".v ;");
    ASSERT(e, "parsed .v");
    ASSERT(e->kind == E_NAME, "kind E_NAME, got %s", ekind_name[e->kind]);
    ASSERT(e->nchildren == 1, "one child");
    ASSERT(e->children[0]->kind == E_VAR, "child is E_VAR");
}

static void test_indirect_dollar(void) {
    /* $x  → E_INDIRECT(E_VAR("x")) */
    EXPR_t *e = parse_first_stmt("$x ;");
    ASSERT(e, "parsed $x");
    ASSERT(e->kind == E_INDIRECT, "kind E_INDIRECT, got %s", ekind_name[e->kind]);
    ASSERT(e->nchildren == 1, "one child");
}

static void test_cursor_at(void) {
    /* @pos  → E_CAPT_CURSOR(E_VAR("pos")) */
    EXPR_t *e = parse_first_stmt("@pos ;");
    ASSERT(e, "parsed @pos");
    ASSERT(e->kind == E_CAPT_CURSOR, "kind E_CAPT_CURSOR, got %s", ekind_name[e->kind]);
    ASSERT(e->nchildren == 1, "one child");
    ASSERT(e->children[0]->kind == E_VAR, "child is E_VAR");
}

static void test_not_tilde(void) {
    /* ~cond  → E_NOT(E_VAR("cond")) */
    EXPR_t *e = parse_first_stmt("~cond ;");
    ASSERT(e, "parsed ~cond");
    ASSERT(e->kind == E_NOT, "kind E_NOT, got %s", ekind_name[e->kind]);
    ASSERT(e->nchildren == 1, "one child");
}

static void test_interrogate_quest(void) {
    /* ?x  → E_INTERROGATE(E_VAR("x")) */
    EXPR_t *e = parse_first_stmt("?x ;");
    ASSERT(e, "parsed ?x");
    ASSERT(e->kind == E_INTERROGATE, "kind E_INTERROGATE, got %s", ekind_name[e->kind]);
    ASSERT(e->nchildren == 1, "one child");
}

static void test_opsyn_amp(void) {
    /* &x  → E_OPSYN("&", E_VAR("x")) */
    EXPR_t *e = parse_first_stmt("& x ;");   /* note space — no-space is T_KEYWORD */
    ASSERT(e, "parsed & x");
    ASSERT(e->kind == E_OPSYN, "kind E_OPSYN, got %s", ekind_name[e->kind]);
    ASSERT(e->sval && strcmp(e->sval, "&") == 0, "sval=&");
    ASSERT(e->nchildren == 1, "one child");
}

static void test_opsyn_percent(void) {
    /* %x  → E_OPSYN("%", E_VAR("x")) */
    EXPR_t *e = parse_first_stmt("%x ;");
    ASSERT(e, "parsed %%x");
    ASSERT(e->kind == E_OPSYN, "kind E_OPSYN, got %s", ekind_name[e->kind]);
    ASSERT(e->sval && strcmp(e->sval, "%") == 0, "sval=%%");
}

static void test_opsyn_slash(void) {
    /* /x  → E_OPSYN("/", ...) */
    EXPR_t *e = parse_first_stmt("/x ;");
    ASSERT(e, "parsed /x");
    ASSERT(e->kind == E_OPSYN, "kind E_OPSYN");
    ASSERT(e->sval && strcmp(e->sval, "/") == 0, "sval=/");
}

static void test_opsyn_pound(void) {
    EXPR_t *e = parse_first_stmt("#x ;");
    ASSERT(e, "parsed #x");
    ASSERT(e->kind == E_OPSYN, "kind E_OPSYN");
    ASSERT(e->sval && strcmp(e->sval, "#") == 0, "sval=#");
}

static void test_opsyn_pipe(void) {
    EXPR_t *e = parse_first_stmt("|x ;");
    ASSERT(e, "parsed |x");
    ASSERT(e->kind == E_OPSYN, "kind E_OPSYN");
    ASSERT(e->sval && strcmp(e->sval, "|") == 0, "sval=|");
}

static void test_opsyn_equal(void) {
    EXPR_t *e = parse_first_stmt("=x ;");
    ASSERT(e, "parsed =x");
    ASSERT(e->kind == E_OPSYN, "kind E_OPSYN");
    ASSERT(e->sval && strcmp(e->sval, "=") == 0, "sval==");
}

static void test_chain_not_dot(void) {
    /* ~.x  → E_NOT(E_NAME(E_VAR("x"))) — right-to-left nesting */
    EXPR_t *e = parse_first_stmt("~.x ;");
    ASSERT(e, "parsed ~.x");
    ASSERT(e->kind == E_NOT, "outer E_NOT");
    ASSERT(e->nchildren == 1, "one child");
    ASSERT(e->children[0]->kind == E_NAME, "inner E_NAME");
    ASSERT(e->children[0]->nchildren == 1, "E_NAME has child");
    ASSERT(e->children[0]->children[0]->kind == E_VAR, "innermost E_VAR");
}

static void test_chain_defer_indirect(void) {
    /* *$x  → E_DEFER(E_INDIRECT(E_VAR("x"))) */
    EXPR_t *e = parse_first_stmt("*$x ;");
    ASSERT(e, "parsed *$x");
    ASSERT(e->kind == E_DEFER, "outer E_DEFER");
    ASSERT(e->children[0]->kind == E_INDIRECT, "inner E_INDIRECT");
}

static void test_unary_on_literal(void) {
    /* .'hello'  → E_NAME(E_QLIT("hello")) */
    EXPR_t *e = parse_first_stmt(".'hello' ;");
    ASSERT(e, "parsed .'hello'");
    ASSERT(e->kind == E_NAME, "kind E_NAME");
    ASSERT(e->children[0]->kind == E_QLIT, "child E_QLIT");
    ASSERT(strcmp(e->children[0]->sval, "hello") == 0, "sval=hello");
}

static void test_unary_on_call(void) {
    /* *f(x)  — E_DEFER applied to an E_FNC node                    */
    /* Lexer sees: T_1STAR T_CALL LPAREN T_IDENT RPAREN SEMI    */
    EXPR_t *e = parse_first_stmt("*f(x) ;");
    ASSERT(e, "parsed *f(x)");
    ASSERT(e->kind == E_DEFER, "outer E_DEFER");
    ASSERT(e->children[0]->kind == E_FNC, "child is E_FNC");
}

static void test_unary_in_binary_context(void) {
    /* a + *b  — unary * binds tighter than binary +               */
    EXPR_t *e = parse_first_stmt("a + *b ;");
    /* Should parse as a + (*b), i.e. E_ADD(a, E_DEFER(b)) */
    ASSERT(e, "parsed a + *b");
    ASSERT(e->kind == E_ADD, "top E_ADD");
    ASSERT(e->nchildren == 2, "two children");
    ASSERT(e->children[0]->kind == E_VAR, "left E_VAR(a)");
    ASSERT(e->children[1]->kind == E_DEFER, "right E_DEFER");
}

static void test_not_in_if_cond(void) {
    /* ~HOST(2) is a valid unary — tests unary on a function call    */
    EXPR_t *e = parse_first_stmt("~HOST(2) ;");
    ASSERT(e, "parsed ~HOST(2)");
    ASSERT(e->kind == E_NOT, "kind E_NOT");
    ASSERT(e->children[0]->kind == E_FNC, "child E_FNC");
    ASSERT(strcmp(e->children[0]->sval, "HOST") == 0, "sval=HOST");
}

static void test_interrogate_on_call(void) {
    /* ?EQ(x, y) → E_INTERROGATE(E_FNC("EQ", x, y))               */
    EXPR_t *e = parse_first_stmt("?EQ(x, y) ;");
    ASSERT(e, "parsed ?EQ(x, y)");
    ASSERT(e->kind == E_INTERROGATE, "kind E_INTERROGATE");
    ASSERT(e->children[0]->kind == E_FNC, "child E_FNC");
    ASSERT(strcmp(e->children[0]->sval, "EQ") == 0, "sval=EQ");
    ASSERT(e->children[0]->nchildren == 2, "EQ has 2 children");
}

int main(void) {
    test_defer_star();
    test_name_dot();
    test_indirect_dollar();
    test_cursor_at();
    test_not_tilde();
    test_interrogate_quest();
    test_opsyn_amp();
    test_opsyn_percent();
    test_opsyn_slash();
    test_opsyn_pound();
    test_opsyn_pipe();
    test_opsyn_equal();
    test_chain_not_dot();
    test_chain_defer_indirect();
    test_unary_on_literal();
    test_unary_on_call();
    test_unary_in_binary_context();
    test_not_in_if_cond();
    test_interrogate_on_call();

    int total = g_pass + g_fail;
    printf("PASS=%d FAIL=%d TOTAL=%d\n", g_pass, g_fail, total);
    return g_fail ? 1 : 0;
}
