/*
 * test_snocone_fsm.c -- Snocone LS-1.b 31-test acceptance harness
 *
 * Drives sc_lex_next(ctx) directly.  Each TestCase carries an
 * expected sequence of token kinds (T_EOF-terminated).  The
 * runner pulls one token at a time from the FSM and compares.
 * No buffering -- the harness is the consumer, the FSM is the
 * producer, and the loop runs token-by-token.
 *
 * Build:
 *   cc -Wall -o test_snocone_fsm test_snocone_fsm.c \
 *       ../../src/frontend/snocone/snocone_fsm.c \
 *       -I ../../src/frontend/snocone
 *
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snocone_fsm.h"
typedef struct {
    const char *name;
    const char *source;
    int         expected[64];                      /* T_EOF-terminated */
} TestCase;
static int passed = 0, failed = 0;
static void run_test(const TestCase *tc) {
    LexCtx ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.p    = tc->source;
    ctx.line = 1;
    int ok = 1;
    int i  = 0;
    int got_kinds[128];
    int got_count = 0;
    /* Pull tokens one at a time and compare against expected[i].
     * Keep going until we've seen T_EOF so we can print full diagnostics on failure. */
    for (;;) {
        int k = sc_lex_next(&ctx);
        if (got_count < 128) got_kinds[got_count++] = k;
        if (tc->expected[i] == T_EOF) {
            if (k != T_EOF) ok = 0;
        } else if (k != tc->expected[i]) {
            ok = 0;
        }
        if (k == T_EOF) break;
        i++;
    }
    if (ok) {
        printf("  PASS  %s\n", tc->name);
        passed++;
    } else {
        printf("  FAIL  %s\n", tc->name);
        printf("        source: %s\n", tc->source);
        printf("        expected: ");
        for (int j = 0; tc->expected[j] != T_EOF; j++) printf("%s ", sc2_kind_name(tc->expected[j]));
        printf("EOF\n");
        printf("        got:      ");
        for (int j = 0; j < got_count; j++) printf("%s ", sc2_kind_name(got_kinds[j]));
        printf("\n");
        failed++;
    }
}
int main(void) {
    printf("=== Snocone LS-3 lexer acceptance tests ===\n\n");

    /* ------------------------------------------------------------------
     * Test 1 — basic concat: two values separated by space
     * ----------------------------------------------------------------- */
    run_test(&(TestCase){
        "T01 basic concat x y",
        "x y",
        { T_IDENT, T_CONCAT, T_IDENT, T_EOF }
    });

    /* Test 2a — call: no space */
    run_test(&(TestCase){
        "T02a call f(x)",
        "f(x)",
        { T_FUNCTION, T_LPAREN, T_IDENT, T_RPAREN, T_EOF }
    });

    /* Test 2b — concat with paren: space before ( */
    run_test(&(TestCase){
        "T02b concat-with-paren f (x)",
        "f (x)",
        { T_IDENT, T_CONCAT, T_LPAREN, T_IDENT, T_RPAREN, T_EOF }
    });

    /* Test 3a — keyword name */
    run_test(&(TestCase){
        "T03a keyword name &FULLSCAN = 1;",
        "&FULLSCAN = 1;",
        { T_KEYWORD, T_ASSIGNMENT, T_INT, T_SEMICOLON, T_EOF }
    });

    /* Test 3b — unary & with space */
    run_test(&(TestCase){
        "T03b unary amp & FULLSCAN",
        "& FULLSCAN",
        { T_UN_AMPERSAND, T_IDENT, T_EOF }
    });

    /* Test 4 — bare expression with concat after call rparen */
    run_test(&(TestCase){
        "T04 EQ(x,0) x = 1;",
        "EQ(x, 0) x = 1;",
        { T_FUNCTION, T_LPAREN, T_IDENT, T_COMMA, T_INT, T_RPAREN,
          T_CONCAT, T_IDENT, T_ASSIGNMENT, T_INT, T_SEMICOLON, T_EOF }
    });

    /* Test 5 — alt-eval (commas inside parens — replaces ||) */
    run_test(&(TestCase){
        "T05 alt-eval A=(LT(I,J) I, GT(I,J) J, 'Same');",
        "A = (LT(I,J) I, GT(I,J) J, 'Same');",
        { T_IDENT, T_ASSIGNMENT,
          T_LPAREN,
            T_FUNCTION, T_LPAREN, T_IDENT, T_COMMA, T_IDENT, T_RPAREN, T_CONCAT, T_IDENT,
          T_COMMA,
            T_FUNCTION, T_LPAREN, T_IDENT, T_COMMA, T_IDENT, T_RPAREN, T_CONCAT, T_IDENT,
          T_COMMA,
            T_STR,
          T_RPAREN, T_SEMICOLON, T_EOF }
    });

    /* Test 6 — backtracking-expression condition in if */
    run_test(&(TestCase){
        "T06 if (x ? 'foo' = 'bar') doit();",
        "if (x ? 'foo' = 'bar') doit();",
        { T_KW_IF, T_LPAREN, T_IDENT, T_MATCH, T_STR, T_ASSIGNMENT,
          T_STR, T_RPAREN,
          T_CONCAT,
          T_FUNCTION, T_LPAREN, T_RPAREN, T_SEMICOLON, T_EOF }
    });

    /* Test 7 — string with embedded && (must NOT lex as separate tokens) */
    run_test(&(TestCase){
        "T07 string with &&",
        "s = '&&';",
        { T_IDENT, T_ASSIGNMENT, T_STR, T_SEMICOLON, T_EOF }
    });

    /* Test 8 — string with embedded || */
    run_test(&(TestCase){
        "T08 string with ||",
        "s = '||';",
        { T_IDENT, T_ASSIGNMENT, T_STR, T_SEMICOLON, T_EOF }
    });

    /* Test 9 — bare && is now two AMPERSAND tokens */
    run_test(&(TestCase){
        "T09 bare && -> AMPERSAND AMPERSAND",
        "x && y",
        { T_IDENT, T_CONCAT, T_UN_AMPERSAND, T_UN_AMPERSAND, T_IDENT, T_EOF }
    });

    /* Test 10 — bare || is now two PIPE tokens */
    run_test(&(TestCase){
        "T10 bare || -> PIPE PIPE",
        "x || y",
        { T_IDENT, T_CONCAT, T_UN_VERTICAL_BAR, T_UN_VERTICAL_BAR, T_IDENT, T_EOF }
    });

    /* Test 11 — identity comparison :: */
    run_test(&(TestCase){
        "T11 identity :: ",
        "x :: y",
        { T_IDENT, T_IDENT_OP, T_IDENT, T_EOF }
    });

    /* Test 12 — DIFFER :!: */
    run_test(&(TestCase){
        "T12 differ :!:",
        "x :!: y",
        { T_IDENT, T_DIFFER, T_IDENT, T_EOF }
    });

    /* Test 13a — caret exponent */
    run_test(&(TestCase){
        "T13a caret a ^ b",
        "a ^ b",
        { T_IDENT, T_EXPONENTIATION, T_IDENT, T_EOF }
    });

    /* Test 13b — ** exponent synonym */
    run_test(&(TestCase){
        "T13b ** exponent a ** b",
        "a ** b",
        { T_IDENT, T_EXPONENTIATION, T_IDENT, T_EOF }
    });

    /* Test 13c — ! exponent synonym */
    run_test(&(TestCase){
        "T13c ! exponent a ! b",
        "a ! b",
        { T_IDENT, T_EXPONENTIATION, T_IDENT, T_EOF }
    });

    /* Test 14 — % is OPSYN slot, not modulo */
    run_test(&(TestCase){
        "T14 percent OPSYN slot",
        "x % y",
        { T_IDENT, T_PERCENT, T_IDENT, T_EOF }
    });

    /* Test 16 — block comment between values triggers CONCAT */
    run_test(&(TestCase){
        "T16 block comment -> CONCAT",
        "x /* hi */ y",
        { T_IDENT, T_CONCAT, T_IDENT, T_EOF }
    });

    /* Test 17 — line comment */
    run_test(&(TestCase){
        "T17 line comment",
        "x = y; // a comment\nz = w;",
        { T_IDENT, T_ASSIGNMENT, T_IDENT, T_SEMICOLON,
          T_IDENT, T_ASSIGNMENT, T_IDENT, T_SEMICOLON, T_EOF }
    });

    /* Test 18b — if with parens: CONCAT fires between RPAREN and body */
    run_test(&(TestCase){
        "T18b if (a) b",
        "if (a) b",
        { T_KW_IF, T_LPAREN, T_IDENT, T_RPAREN, T_CONCAT, T_IDENT, T_EOF }
    });

    /* Test 19 — labels */
    run_test(&(TestCase){
        "T19 label top: x = 1;",
        "top: x = 1;",
        { T_IDENT, T_COLON, T_IDENT, T_ASSIGNMENT, T_INT, T_SEMICOLON, T_EOF }
    });

    /* Test 20 — goto */
    run_test(&(TestCase){
        "T20 goto top;",
        "goto top;",
        { T_KW_GOTO, T_IDENT, T_SEMICOLON, T_EOF }
    });

    /* Test 21a — break with label */
    run_test(&(TestCase){
        "T21a break loop_done;",
        "break loop_done;",
        { T_KW_BREAK, T_IDENT, T_SEMICOLON, T_EOF }
    });

    /* Test 21b — plain break */
    run_test(&(TestCase){
        "T21b break;",
        "break;",
        { T_KW_BREAK, T_SEMICOLON, T_EOF }
    });

    /* Test 22 — switch statement */
    run_test(&(TestCase){
        "T22 switch",
        "switch (x) { case 1: a = 1; default: a = 3; }",
        { T_KW_SWITCH, T_LPAREN, T_IDENT, T_RPAREN, T_LBRACE,
          T_KW_CASE, T_INT, T_COLON, T_IDENT, T_ASSIGNMENT, T_INT, T_SEMICOLON,
          T_KW_DEFAULT, T_COLON, T_IDENT, T_ASSIGNMENT, T_INT, T_SEMICOLON,
          T_RBRACE, T_EOF }
    });

    /* Test 23 — do/until */
    run_test(&(TestCase){
        "T23 do/until",
        "do { x = x + 1; } until (GT(x, 10));",
        { T_KW_DO, T_LBRACE,
          T_IDENT, T_ASSIGNMENT, T_IDENT, T_ADDITION, T_INT, T_SEMICOLON,
          T_RBRACE,
          T_KW_UNTIL, T_LPAREN,
          T_FUNCTION, T_LPAREN, T_IDENT, T_COMMA, T_INT, T_RPAREN,
          T_RPAREN, T_SEMICOLON, T_EOF }
    });

    /* Test — string operators */
    run_test(&(TestCase){
        "T_strops :==: :!=: :<=: :>=:",
        "a :==: b :!=: c :<=: d :>=: e",
        { T_IDENT, T_LEQ, T_IDENT, T_LNE, T_IDENT,
          T_LLE, T_IDENT, T_LGE, T_IDENT, T_EOF }
    });

    /* Test — function keyword (renamed from procedure) */
    run_test(&(TestCase){
        "T_function keyword",
        "function foo(x) { return x; }",
        { T_KW_FUNCTION, T_FUNCTION, T_LPAREN, T_IDENT, T_RPAREN,
          T_LBRACE,
          T_KW_RETURN, T_IDENT, T_SEMICOLON,
          T_RBRACE, T_EOF }
    });

    /* Test — real numbers */
    run_test(&(TestCase){
        "T_reals 3.14 .5 1e10 2.5e-3",
        "3.14 .5 1e10 2.5e-3",
        { T_REAL, T_CONCAT, T_REAL, T_CONCAT, T_REAL, T_CONCAT, T_REAL, T_EOF }
    });

    /* Test — compound-assign operators */
    run_test(&(TestCase){
        "T_compound_assign x += 1; y ^= 2;",
        "x += 1; y ^= 2;",
        { T_IDENT, T_PLUS_ASSIGN, T_INT, T_SEMICOLON,
          T_IDENT, T_CARET_ASSIGN, T_INT, T_SEMICOLON, T_EOF }
    });

    printf("\n=== Results: %d passed, %d failed ===\n", passed, failed);
    return failed ? 1 : 0;
}
