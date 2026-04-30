/*
 * test_snocone_lex2.c — LS-3.c acceptance test
 *
 * Runs the 30-test token-stream corpus from GOAL-SNOCONE-LANG-SPACE §5.
 * Each test tokenises a source snippet and checks the token stream
 * against the expected sequence.  A mismatch prints a diff-style report.
 *
 * Build:
 *   cc -Wall -o test_snocone_lex2 test_snocone_lex2.c snocone.lex.c -lfl
 *
 * Commit identity: LCherryholmes / lcherryh@yahoo.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snocone_lex2.h"

/* -----------------------------------------------------------------------
 * A test case: input source + expected token kinds (terminated by SC2_EOF)
 * ----------------------------------------------------------------------- */
typedef struct {
    const char *name;
    const char *source;
    int         expected[64];  /* SC2_EOF-terminated */
} TestCase;

static int passed = 0, failed = 0;

static void run_test(const TestCase *tc) {
    ScTokenBuf buf = snocone_lex2(tc->source);

    int ok = 1;
    int i = 0;
    for (i = 0; tc->expected[i] != SC2_EOF; i++) {
        if (i >= buf.count || buf.tokens[i].kind != tc->expected[i]) {
            ok = 0;
            break;
        }
    }
    /* Also check that we consumed everything (next should be SC2_EOF) */
    if (ok && (i >= buf.count || buf.tokens[i].kind != SC2_EOF))
        ok = 0;

    if (ok) {
        printf("  PASS  %s\n", tc->name);
        passed++;
    } else {
        printf("  FAIL  %s\n", tc->name);
        printf("        source: %s\n", tc->source);
        printf("        expected: ");
        for (int j = 0; tc->expected[j] != SC2_EOF; j++)
            printf("%s ", sc2_kind_name(tc->expected[j]));
        printf("EOF\n");
        printf("        got:      ");
        for (int j = 0; j < buf.count; j++)
            printf("%s(%s) ", sc2_kind_name(buf.tokens[j].kind),
                   buf.tokens[j].text[0] ? buf.tokens[j].text : "");
        printf("\n");
        failed++;
    }

    sc_token_buf_free(&buf);
}


int main(void) {
    printf("=== Snocone LS-3 lexer acceptance tests ===\n\n");

    /* ------------------------------------------------------------------
     * Test 1 — basic concat: two values separated by space
     * ----------------------------------------------------------------- */
    run_test(&(TestCase){
        "T01 basic concat x y",
        "x y",
        { SC2_IDENT, SC2_CONCAT, SC2_IDENT, SC2_EOF }
    });

    /* Test 2a — call: no space */
    run_test(&(TestCase){
        "T02a call f(x)",
        "f(x)",
        { SC2_IDENT_LPAREN_NOSP, SC2_IDENT, SC2_RPAREN, SC2_EOF }
    });

    /* Test 2b — concat with paren: space before ( */
    run_test(&(TestCase){
        "T02b concat-with-paren f (x)",
        "f (x)",
        { SC2_IDENT, SC2_CONCAT, SC2_LPAREN, SC2_IDENT, SC2_RPAREN, SC2_EOF }
    });

    /* Test 3a — keyword name */
    run_test(&(TestCase){
        "T03a keyword name &FULLSCAN = 1;",
        "&FULLSCAN = 1;",
        { SC2_KEYWORD_NAME, SC2_ASSIGN, SC2_INTEGER, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 3b — unary & with space */
    run_test(&(TestCase){
        "T03b unary amp & FULLSCAN",
        "& FULLSCAN",
        { SC2_AMPERSAND, SC2_IDENT, SC2_EOF }
    });

    /* Test 4 — bare expression with concat after call rparen */
    run_test(&(TestCase){
        "T04 EQ(x,0) x = 1;",
        "EQ(x, 0) x = 1;",
        { SC2_IDENT_LPAREN_NOSP, SC2_IDENT, SC2_COMMA, SC2_INTEGER, SC2_RPAREN,
          SC2_CONCAT, SC2_IDENT, SC2_ASSIGN, SC2_INTEGER, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 5 — alt-eval (commas inside parens — replaces ||) */
    run_test(&(TestCase){
        "T05 alt-eval A=(LT(I,J) I, GT(I,J) J, 'Same');",
        "A = (LT(I,J) I, GT(I,J) J, 'Same');",
        { SC2_IDENT, SC2_ASSIGN,
          SC2_LPAREN,
            SC2_IDENT_LPAREN_NOSP, SC2_IDENT, SC2_COMMA, SC2_IDENT, SC2_RPAREN, SC2_CONCAT, SC2_IDENT,
          SC2_COMMA,
            SC2_IDENT_LPAREN_NOSP, SC2_IDENT, SC2_COMMA, SC2_IDENT, SC2_RPAREN, SC2_CONCAT, SC2_IDENT,
          SC2_COMMA,
            SC2_STRING,
          SC2_RPAREN, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 6 — backtracking-expression condition in if */
    run_test(&(TestCase){
        "T06 if (x ? 'foo' = 'bar') doit();",
        "if (x ? 'foo' = 'bar') doit();",
        { SC2_KW_IF, SC2_LPAREN, SC2_IDENT, SC2_QUESTION, SC2_STRING, SC2_ASSIGN,
          SC2_STRING, SC2_RPAREN,
          SC2_CONCAT,
          SC2_IDENT_LPAREN_NOSP, SC2_RPAREN, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 7 — string with embedded && (must NOT lex as separate tokens) */
    run_test(&(TestCase){
        "T07 string with &&",
        "s = '&&';",
        { SC2_IDENT, SC2_ASSIGN, SC2_STRING, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 8 — string with embedded || */
    run_test(&(TestCase){
        "T08 string with ||",
        "s = '||';",
        { SC2_IDENT, SC2_ASSIGN, SC2_STRING, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 9 — bare && is now two AMPERSAND tokens */
    run_test(&(TestCase){
        "T09 bare && -> AMPERSAND AMPERSAND",
        "x && y",
        { SC2_IDENT, SC2_AMPERSAND, SC2_AMPERSAND, SC2_IDENT, SC2_EOF }
    });

    /* Test 10 — bare || is now two PIPE tokens */
    run_test(&(TestCase){
        "T10 bare || -> PIPE PIPE",
        "x || y",
        { SC2_IDENT, SC2_PIPE, SC2_PIPE, SC2_IDENT, SC2_EOF }
    });

    /* Test 11 — identity comparison :: */
    run_test(&(TestCase){
        "T11 identity :: ",
        "x :: y",
        { SC2_IDENT, SC2_STR_IDENT, SC2_IDENT, SC2_EOF }
    });

    /* Test 12 — DIFFER :!: */
    run_test(&(TestCase){
        "T12 differ :!:",
        "x :!: y",
        { SC2_IDENT, SC2_STR_DIFFER, SC2_IDENT, SC2_EOF }
    });

    /* Test 13a — caret exponent */
    run_test(&(TestCase){
        "T13a caret a ^ b",
        "a ^ b",
        { SC2_IDENT, SC2_CARET, SC2_IDENT, SC2_EOF }
    });

    /* Test 13b — ** exponent synonym */
    run_test(&(TestCase){
        "T13b ** exponent a ** b",
        "a ** b",
        { SC2_IDENT, SC2_CARET, SC2_IDENT, SC2_EOF }
    });

    /* Test 13c — ! exponent synonym */
    run_test(&(TestCase){
        "T13c ! exponent a ! b",
        "a ! b",
        { SC2_IDENT, SC2_BANG_EXPONENT, SC2_IDENT, SC2_EOF }
    });

    /* Test 14 — % is OPSYN slot, not modulo */
    run_test(&(TestCase){
        "T14 percent OPSYN slot",
        "x % y",
        { SC2_IDENT, SC2_PERCENT, SC2_IDENT, SC2_EOF }
    });

    /* Test 16 — block comment between values triggers CONCAT */
    run_test(&(TestCase){
        "T16 block comment -> CONCAT",
        "x /* hi */ y",
        { SC2_IDENT, SC2_CONCAT, SC2_IDENT, SC2_EOF }
    });

    /* Test 17 — line comment */
    run_test(&(TestCase){
        "T17 line comment",
        "x = y; // a comment\nz = w;",
        { SC2_IDENT, SC2_ASSIGN, SC2_IDENT, SC2_SEMICOLON,
          SC2_IDENT, SC2_ASSIGN, SC2_IDENT, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 18b — if with parens: CONCAT fires between RPAREN and body */
    run_test(&(TestCase){
        "T18b if (a) b",
        "if (a) b",
        { SC2_KW_IF, SC2_LPAREN, SC2_IDENT, SC2_RPAREN, SC2_CONCAT, SC2_IDENT, SC2_EOF }
    });

    /* Test 19 — labels */
    run_test(&(TestCase){
        "T19 label top: x = 1;",
        "top: x = 1;",
        { SC2_IDENT, SC2_COLON, SC2_IDENT, SC2_ASSIGN, SC2_INTEGER, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 20 — goto */
    run_test(&(TestCase){
        "T20 goto top;",
        "goto top;",
        { SC2_KW_GOTO, SC2_IDENT, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 21a — break with label */
    run_test(&(TestCase){
        "T21a break loop_done;",
        "break loop_done;",
        { SC2_KW_BREAK, SC2_IDENT, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 21b — plain break */
    run_test(&(TestCase){
        "T21b break;",
        "break;",
        { SC2_KW_BREAK, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test 22 — switch statement */
    run_test(&(TestCase){
        "T22 switch",
        "switch (x) { case 1: a = 1; default: a = 3; }",
        { SC2_KW_SWITCH, SC2_LPAREN, SC2_IDENT, SC2_RPAREN, SC2_LBRACE,
          SC2_KW_CASE, SC2_INTEGER, SC2_COLON, SC2_IDENT, SC2_ASSIGN, SC2_INTEGER, SC2_SEMICOLON,
          SC2_KW_DEFAULT, SC2_COLON, SC2_IDENT, SC2_ASSIGN, SC2_INTEGER, SC2_SEMICOLON,
          SC2_RBRACE, SC2_EOF }
    });

    /* Test 23 — do/until */
    run_test(&(TestCase){
        "T23 do/until",
        "do { x = x + 1; } until (GT(x, 10));",
        { SC2_KW_DO, SC2_LBRACE,
          SC2_IDENT, SC2_ASSIGN, SC2_IDENT, SC2_PLUS, SC2_INTEGER, SC2_SEMICOLON,
          SC2_RBRACE,
          SC2_KW_UNTIL, SC2_LPAREN,
          SC2_IDENT_LPAREN_NOSP, SC2_IDENT, SC2_COMMA, SC2_INTEGER, SC2_RPAREN,
          SC2_RPAREN, SC2_SEMICOLON, SC2_EOF }
    });

    /* Test — string operators */
    run_test(&(TestCase){
        "T_strops :==: :!=: :<=: :>=:",
        "a :==: b :!=: c :<=: d :>=: e",
        { SC2_IDENT, SC2_STR_EQ, SC2_IDENT, SC2_STR_NE, SC2_IDENT,
          SC2_STR_LE, SC2_IDENT, SC2_STR_GE, SC2_IDENT, SC2_EOF }
    });

    /* Test — function keyword (renamed from procedure) */
    run_test(&(TestCase){
        "T_function keyword",
        "function foo(x) { return x; }",
        { SC2_KW_FUNCTION, SC2_IDENT_LPAREN_NOSP, SC2_IDENT, SC2_RPAREN,
          SC2_LBRACE,
          SC2_KW_RETURN, SC2_IDENT, SC2_SEMICOLON,
          SC2_RBRACE, SC2_EOF }
    });

    /* Test — real numbers */
    run_test(&(TestCase){
        "T_reals 3.14 .5 1e10 2.5e-3",
        "3.14 .5 1e10 2.5e-3",
        { SC2_REAL, SC2_CONCAT, SC2_REAL, SC2_CONCAT, SC2_REAL, SC2_CONCAT, SC2_REAL, SC2_EOF }
    });

    /* Test — compound-assign operators */
    run_test(&(TestCase){
        "T_compound_assign x += 1; y ^= 2;",
        "x += 1; y ^= 2;",
        { SC2_IDENT, SC2_PLUS_ASSIGN, SC2_INTEGER, SC2_SEMICOLON,
          SC2_IDENT, SC2_CARET_ASSIGN, SC2_INTEGER, SC2_SEMICOLON, SC2_EOF }
    });

    printf("\n=== Results: %d passed, %d failed ===\n", passed, failed);
    return failed ? 1 : 0;
}
