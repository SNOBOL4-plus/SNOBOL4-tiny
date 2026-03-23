/*
 * icon_lex_test.c — Unit tests for the Tiny-ICON lexer (icon_lex.c)
 *
 * Acceptance criteria for M-ICON-LEX:
 *   - All keywords recognized
 *   - All operators tokenized correctly
 *   - Integer, real, string, cset literals parsed
 *   - Identifiers distinguished from keywords
 *   - Every token in the 6 Rung 1 corpus programs tokenized correctly
 *   - No auto-semicolon insertion (explicit ';' only)
 *   - Error on unexpected characters
 *
 * Build:
 *   cc -Wall -Wextra -std=c11 -I. icon_lex.c icon_lex_test.c -o icon_lex_test
 * Run:
 *   ./icon_lex_test
 */

#include "icon_lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* =========================================================================
 * Test harness
 * ========================================================================= */
static int g_pass = 0, g_fail = 0;

static void test_ok(const char *name) {
    printf("  PASS  %s\n", name);
    g_pass++;
}
static void test_fail(const char *name, const char *detail) {
    printf("  FAIL  %s — %s\n", name, detail);
    g_fail++;
}

/* Lex all tokens from src, return heap array; *out_n = count (including EOF). */
static IcnToken *lex_all(const char *src, int *out_n) {
    IcnLexer lx;
    icn_lex_init(&lx, src);
    int cap = 16, n = 0;
    IcnToken *toks = malloc((size_t)cap * sizeof(IcnToken));
    for (;;) {
        if (n >= cap) { cap *= 2; toks = realloc(toks, (size_t)cap * sizeof(IcnToken)); }
        toks[n] = icn_lex_next(&lx);
        n++;
        if (toks[n-1].kind == TK_EOF || toks[n-1].kind == TK_ERROR) break;
    }
    *out_n = n;
    return toks;
}

/* Assert that lexing 'src' produces exactly the sequence of token kinds in 'kinds[]' (length n) */
static void check_seq(const char *name, const char *src,
                      const IcnTkKind *kinds, int n) {
    int got_n = 0;
    IcnToken *toks = lex_all(src, &got_n);

    if (got_n != n) {
        char buf[256];
        snprintf(buf, sizeof(buf), "expected %d tokens, got %d (last=%s)",
                 n, got_n, icn_tk_name(toks[got_n-1].kind));
        test_fail(name, buf);
        free(toks);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (toks[i].kind != kinds[i]) {
            char buf[256];
            snprintf(buf, sizeof(buf), "token[%d]: expected %s got %s",
                     i, icn_tk_name(kinds[i]), icn_tk_name(toks[i].kind));
            test_fail(name, buf);
            free(toks);
            return;
        }
    }
    test_ok(name);
    free(toks);
}

/* Single-token helper */
static void check1(const char *name, const char *src, IcnTkKind want) {
    IcnTkKind seq[2] = {want, TK_EOF};
    check_seq(name, src, seq, 2);
}

/* =========================================================================
 * Test groups
 * ========================================================================= */

static void test_keywords(void) {
    printf("\n--- Keywords ---\n");
    check1("kw_to",        "to",        TK_TO);
    check1("kw_by",        "by",        TK_BY);
    check1("kw_every",     "every",     TK_EVERY);
    check1("kw_do",        "do",        TK_DO);
    check1("kw_if",        "if",        TK_IF);
    check1("kw_then",      "then",      TK_THEN);
    check1("kw_else",      "else",      TK_ELSE);
    check1("kw_while",     "while",     TK_WHILE);
    check1("kw_until",     "until",     TK_UNTIL);
    check1("kw_repeat",    "repeat",    TK_REPEAT);
    check1("kw_return",    "return",    TK_RETURN);
    check1("kw_suspend",   "suspend",   TK_SUSPEND);
    check1("kw_fail",      "fail",      TK_FAIL);
    check1("kw_break",     "break",     TK_BREAK);
    check1("kw_next",      "next",      TK_NEXT);
    check1("kw_not",       "not",       TK_NOT);
    check1("kw_procedure", "procedure", TK_PROCEDURE);
    check1("kw_end",       "end",       TK_END);
    check1("kw_global",    "global",    TK_GLOBAL);
    check1("kw_local",     "local",     TK_LOCAL);
    check1("kw_static",    "static",    TK_STATIC);
    check1("kw_record",    "record",    TK_RECORD);
    check1("kw_case",      "case",      TK_CASE);
    check1("kw_of",        "of",        TK_OF);
    check1("kw_default",   "default",   TK_DEFAULT);
    check1("kw_create",    "create",    TK_CREATE);
    check1("kw_initial",   "initial",   TK_INITIAL);
    /* ident that starts like a keyword */
    check1("ident_total",  "total",     TK_IDENT);
    check1("ident_endian", "endian",    TK_IDENT);
}

static void test_operators(void) {
    printf("\n--- Operators ---\n");
    /* Arithmetic */
    check1("op_plus",    "+",   TK_PLUS);
    check1("op_minus",   "-",   TK_MINUS);
    check1("op_star",    "*",   TK_STAR);
    check1("op_slash",   "/",   TK_SLASH);
    check1("op_mod",     "%",   TK_MOD);
    check1("op_caret",   "^",   TK_CARET);
    /* Numeric relational */
    check1("op_lt",      "<",   TK_LT);
    check1("op_le",      "<=",  TK_LE);
    check1("op_gt",      ">",   TK_GT);
    check1("op_ge",      ">=",  TK_GE);
    check1("op_eq",      "=",   TK_EQ);
    check1("op_neq",     "~=",  TK_NEQ);
    /* String relational */
    check1("op_slt",     "<<",  TK_SLT);
    check1("op_sle",     "<<=", TK_SLE);
    check1("op_sgt",     ">>",  TK_SGT);
    check1("op_sge",     ">>=", TK_SGE);
    check1("op_seq",     "==",  TK_SEQ);
    check1("op_sne",     "~==", TK_SNE);
    /* Concat */
    check1("op_concat",  "||",  TK_CONCAT);
    check1("op_lconcat", "|||", TK_LCONCAT);
    /* Assignment */
    check1("op_assign",  ":=",  TK_ASSIGN);
    check1("op_swap",    ":=:", TK_SWAP);
    check1("op_revassign","<-", TK_REVASSIGN);
    /* Augmented */
    check1("op_augplus",  "+:=", TK_AUGPLUS);
    check1("op_augminus", "-:=", TK_AUGMINUS);
    check1("op_augstar",  "*:=", TK_AUGSTAR);
    check1("op_augslash", "/:=", TK_AUGSLASH);
    check1("op_augmod",   "%:=", TK_AUGMOD);
    check1("op_augconcat","||:=",TK_AUGCONCAT);
    /* Misc */
    check1("op_and",     "&",   TK_AND);
    check1("op_bar",     "|",   TK_BAR);
    check1("op_backslash","\\", TK_BACKSLASH);
    check1("op_bang",    "!",   TK_BANG);
    check1("op_qmark",   "?",   TK_QMARK);
    check1("op_at",      "@",   TK_AT);
    check1("op_tilde",   "~",   TK_TILDE);
    check1("op_dot",     ".",   TK_DOT);
}

static void test_punctuation(void) {
    printf("\n--- Punctuation ---\n");
    check1("punc_lparen",  "(",  TK_LPAREN);
    check1("punc_rparen",  ")",  TK_RPAREN);
    check1("punc_lbrace",  "{",  TK_LBRACE);
    check1("punc_rbrace",  "}",  TK_RBRACE);
    check1("punc_lbrack",  "[",  TK_LBRACK);
    check1("punc_rbrack",  "]",  TK_RBRACK);
    check1("punc_comma",   ",",  TK_COMMA);
    check1("punc_semicol", ";",  TK_SEMICOL);
    check1("punc_colon",   ":",  TK_COLON);
}

static void test_integer_literals(void) {
    printf("\n--- Integer literals ---\n");
    IcnLexer lx;
    IcnToken t;

    /* Decimal */
    icn_lex_init(&lx, "42");
    t = icn_lex_next(&lx);
    if (t.kind == TK_INT && t.val.ival == 42) test_ok("int_decimal_42");
    else test_fail("int_decimal_42", "wrong kind or value");

    /* Zero */
    icn_lex_init(&lx, "0");
    t = icn_lex_next(&lx);
    if (t.kind == TK_INT && t.val.ival == 0) test_ok("int_zero");
    else test_fail("int_zero", "wrong kind or value");

    /* Large */
    icn_lex_init(&lx, "1234567890");
    t = icn_lex_next(&lx);
    if (t.kind == TK_INT && t.val.ival == 1234567890L) test_ok("int_large");
    else test_fail("int_large", "wrong value");

    /* Radix: 16r1F = 31 */
    icn_lex_init(&lx, "16r1F");
    t = icn_lex_next(&lx);
    if (t.kind == TK_INT && t.val.ival == 31) test_ok("int_radix16");
    else { char b[64]; snprintf(b,64,"got kind=%s val=%ld",icn_tk_name(t.kind),t.val.ival); test_fail("int_radix16", b); }

    /* Radix: 8r17 = 15 */
    icn_lex_init(&lx, "8r17");
    t = icn_lex_next(&lx);
    if (t.kind == TK_INT && t.val.ival == 15) test_ok("int_radix8");
    else test_fail("int_radix8", "wrong value");

    /* Radix: 2r1010 = 10 */
    icn_lex_init(&lx, "2r1010");
    t = icn_lex_next(&lx);
    if (t.kind == TK_INT && t.val.ival == 10) test_ok("int_radix2");
    else test_fail("int_radix2", "wrong value");
}

static void test_real_literals(void) {
    printf("\n--- Real literals ---\n");
    IcnLexer lx;
    IcnToken t;

    icn_lex_init(&lx, "3.14");
    t = icn_lex_next(&lx);
    if (t.kind == TK_REAL && fabs(t.val.fval - 3.14) < 1e-9) test_ok("real_pi");
    else test_fail("real_pi", "wrong value");

    icn_lex_init(&lx, "1.0e10");
    t = icn_lex_next(&lx);
    if (t.kind == TK_REAL && fabs(t.val.fval - 1e10) < 1.0) test_ok("real_exp");
    else test_fail("real_exp", "wrong value");

    icn_lex_init(&lx, "0.5");
    t = icn_lex_next(&lx);
    if (t.kind == TK_REAL && fabs(t.val.fval - 0.5) < 1e-12) test_ok("real_half");
    else test_fail("real_half", "wrong value");
}

static void test_string_literals(void) {
    printf("\n--- String literals ---\n");
    IcnLexer lx;
    IcnToken t;

    icn_lex_init(&lx, "\"hello\"");
    t = icn_lex_next(&lx);
    if (t.kind == TK_STRING && strcmp(t.val.sval.data, "hello") == 0) test_ok("str_hello");
    else test_fail("str_hello", "wrong content");
    free(t.val.sval.data);

    icn_lex_init(&lx, "\"\"");
    t = icn_lex_next(&lx);
    if (t.kind == TK_STRING && t.val.sval.len == 0) test_ok("str_empty");
    else test_fail("str_empty", "not empty");
    free(t.val.sval.data);

    icn_lex_init(&lx, "\"done\"");
    t = icn_lex_next(&lx);
    if (t.kind == TK_STRING && strcmp(t.val.sval.data, "done") == 0) test_ok("str_done");
    else test_fail("str_done", "wrong content");
    free(t.val.sval.data);

    /* Escape sequences */
    icn_lex_init(&lx, "\"a\\nb\"");
    t = icn_lex_next(&lx);
    if (t.kind == TK_STRING && t.val.sval.data[1] == '\n') test_ok("str_escape_newline");
    else test_fail("str_escape_newline", "wrong escape");
    free(t.val.sval.data);
}

static void test_cset_literals(void) {
    printf("\n--- Cset literals ---\n");
    IcnLexer lx;
    IcnToken t;

    icn_lex_init(&lx, "'abc'");
    t = icn_lex_next(&lx);
    if (t.kind == TK_CSET && strcmp(t.val.sval.data, "abc") == 0) test_ok("cset_abc");
    else test_fail("cset_abc", "wrong content");
    free(t.val.sval.data);

    icn_lex_init(&lx, "''");
    t = icn_lex_next(&lx);
    if (t.kind == TK_CSET && t.val.sval.len == 0) test_ok("cset_empty");
    else test_fail("cset_empty", "not empty");
    free(t.val.sval.data);
}

static void test_identifiers(void) {
    printf("\n--- Identifiers ---\n");
    IcnLexer lx;
    IcnToken t;

    icn_lex_init(&lx, "write");
    t = icn_lex_next(&lx);
    if (t.kind == TK_IDENT && strcmp(t.val.sval.data, "write") == 0) test_ok("ident_write");
    else test_fail("ident_write", "wrong");
    free(t.val.sval.data);

    icn_lex_init(&lx, "main");
    t = icn_lex_next(&lx);
    if (t.kind == TK_IDENT && strcmp(t.val.sval.data, "main") == 0) test_ok("ident_main");
    else test_fail("ident_main", "wrong");
    free(t.val.sval.data);

    icn_lex_init(&lx, "_tmp123");
    t = icn_lex_next(&lx);
    if (t.kind == TK_IDENT && strcmp(t.val.sval.data, "_tmp123") == 0) test_ok("ident_underscore");
    else test_fail("ident_underscore", "wrong");
    free(t.val.sval.data);
}

static void test_comments(void) {
    printf("\n--- Comments ---\n");
    /* # comment should be skipped */
    IcnTkKind seq[] = {TK_INT, TK_EOF};
    check_seq("comment_hash", "# this is a comment\n42", seq, 2);
    check_seq("comment_inline", "42 # inline\n", seq, 2);
}

static void test_no_auto_semicolon(void) {
    printf("\n--- No auto-semicolon ---\n");
    /* A newline between tokens should NOT produce a semicolon */
    IcnTkKind seq[] = {TK_INT, TK_PLUS, TK_INT, TK_EOF};
    check_seq("no_auto_semi", "1\n+\n2", seq, 4);
    /* Explicit semicolons only */
    IcnTkKind seq2[] = {TK_INT, TK_SEMICOL, TK_INT, TK_EOF};
    check_seq("explicit_semi", "1;2", seq2, 4);
}

/* =========================================================================
 * Rung 1 corpus program tokenization tests
 * ========================================================================= */

static void test_rung1_programs(void) {
    printf("\n--- Rung 1 corpus programs ---\n");

    /* t01: procedure main(); every write(1 to 5); end */
    {
        const char *src = "procedure main();\n  every write(1 to 5);\nend";
        IcnTkKind seq[] = {
            TK_PROCEDURE, TK_IDENT, TK_LPAREN, TK_RPAREN, TK_SEMICOL,
            TK_EVERY, TK_IDENT, TK_LPAREN, TK_INT, TK_TO, TK_INT, TK_RPAREN, TK_SEMICOL,
            TK_END, TK_EOF
        };
        check_seq("rung1_t01_to5", src, seq, 15);
    }

    /* t02: procedure main(); every write((1 to 3) * (1 to 2)); end */
    {
        const char *src = "procedure main();\n  every write((1 to 3) * (1 to 2));\nend";
        IcnTkKind seq[] = {
            TK_PROCEDURE, TK_IDENT, TK_LPAREN, TK_RPAREN, TK_SEMICOL,
            TK_EVERY, TK_IDENT, TK_LPAREN,
              TK_LPAREN, TK_INT, TK_TO, TK_INT, TK_RPAREN,
              TK_STAR,
              TK_LPAREN, TK_INT, TK_TO, TK_INT, TK_RPAREN,
            TK_RPAREN, TK_SEMICOL,
            TK_END, TK_EOF
        };
        check_seq("rung1_t02_mult", src, seq, 23);
    }

    /* t04: 2 < (1 to 4) — relational goal-directed */
    {
        const char *src = "procedure main();\n  every write(2 < (1 to 4));\nend";
        IcnTkKind seq[] = {
            TK_PROCEDURE, TK_IDENT, TK_LPAREN, TK_RPAREN, TK_SEMICOL,
            TK_EVERY, TK_IDENT, TK_LPAREN,
              TK_INT, TK_LT, TK_LPAREN, TK_INT, TK_TO, TK_INT, TK_RPAREN,
            TK_RPAREN, TK_SEMICOL,
            TK_END, TK_EOF
        };
        check_seq("rung1_t04_lt", src, seq, 19);
    }

    /* t06: every write(5 > ((1 to 2) * (3 to 4))); write("done"); */
    {
        const char *src =
            "procedure main();\n"
            "  every write(5 > ((1 to 2) * (3 to 4)));\n"
            "  write(\"done\");\n"
            "end";
        IcnTkKind seq[] = {
            TK_PROCEDURE, TK_IDENT, TK_LPAREN, TK_RPAREN, TK_SEMICOL,
            /* every write(5 > ((1 to 2) * (3 to 4))); */
            TK_EVERY, TK_IDENT, TK_LPAREN,
              TK_INT, TK_GT,
              TK_LPAREN,
                TK_LPAREN, TK_INT, TK_TO, TK_INT, TK_RPAREN,
                TK_STAR,
                TK_LPAREN, TK_INT, TK_TO, TK_INT, TK_RPAREN,
              TK_RPAREN,
            TK_RPAREN, TK_SEMICOL,
            /* write("done"); */
            TK_IDENT, TK_LPAREN, TK_STRING, TK_RPAREN, TK_SEMICOL,
            TK_END, TK_EOF
        };
        check_seq("rung1_t06_paper", src, seq, 32);
    }
}

static void test_peek(void) {
    printf("\n--- Peek (lookahead) ---\n");
    IcnLexer lx;
    icn_lex_init(&lx, "1 to 5");

    IcnToken p1 = icn_lex_peek(&lx);
    IcnToken n1 = icn_lex_next(&lx);
    if (p1.kind == n1.kind && p1.kind == TK_INT) test_ok("peek_preserves_state");
    else test_fail("peek_preserves_state", "mismatch");

    /* After consuming '1', peek should see 'to' */
    IcnToken p2 = icn_lex_peek(&lx);
    IcnToken n2 = icn_lex_next(&lx);
    if (p2.kind == TK_TO && n2.kind == TK_TO) test_ok("peek_to_keyword");
    else test_fail("peek_to_keyword", "wrong kind");
}

/* =========================================================================
 * main
 * ========================================================================= */
int main(void) {
    printf("=== icon_lex_test — M-ICON-LEX ===\n");

    test_keywords();
    test_operators();
    test_punctuation();
    test_integer_literals();
    test_real_literals();
    test_string_literals();
    test_cset_literals();
    test_identifiers();
    test_comments();
    test_no_auto_semicolon();
    test_rung1_programs();
    test_peek();

    printf("\n=== Results: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
