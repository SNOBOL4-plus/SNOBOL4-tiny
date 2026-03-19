/*
 * sc_lex_test.c -- M-SNOC-LEX quick-check + full test suite
 *
 * Mirrors TestSnoconeLexer.cs from snobol4dotnet exactly.
 * Sprint SC0 trigger (M-SNOC-LEX):
 *   `OUTPUT = 'hello'` -> IDENT ASSIGN STRING NEWLINE EOF  (3 content tokens)
 *
 * Build:
 *   gcc -o /tmp/sc_lex_test test/frontend/snocone/sc_lex_test.c \
 *       src/frontend/snocone/sc_lex.c
 *   /tmp/sc_lex_test
 */

#include "sc_lex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* --------------------------------------------------------------------------
 * Minimal test harness
 * ------------------------------------------------------------------------ */
static int g_pass = 0, g_fail = 0;

#define CHECK_KIND(label, got, want) do { \
    if ((got) != (want)) { \
        printf("FAIL %-50s got %-14s want %s\n", \
               (label), sc_kind_name(got), sc_kind_name(want)); \
        g_fail++; \
    } else { g_pass++; } \
} while(0)

#define CHECK_TEXT(label, got, want) do { \
    if (strcmp((got), (want)) != 0) { \
        printf("FAIL %-50s text got '%s' want '%s'\n", (label), (got), (want)); \
        g_fail++; \
    } else { g_pass++; } \
} while(0)

#define CHECK_INT(label, got, want) do { \
    if ((got) != (want)) { \
        printf("FAIL %-50s got %d want %d\n", (label), (got), (want)); \
        g_fail++; \
    } else { g_pass++; } \
} while(0)

/* Helper: collect non-EOF non-NEWLINE kinds from source */
static int kinds(const char *src, ScKind *out, int maxn)
{
    ScTokenArray arr = sc_lex(src);
    int n = 0;
    for (int i = 0; i < arr.count && n < maxn; i++) {
        ScKind k = arr.tokens[i].kind;
        if (k != SC_EOF && k != SC_NEWLINE)
            out[n++] = k;
    }
    sc_tokens_free(&arr);
    return n;
}

/* Helper: count tokens of a specific kind */
static int count_kind(const char *src, ScKind k)
{
    ScTokenArray arr = sc_lex(src);
    int n = 0;
    for (int i = 0; i < arr.count; i++)
        if (arr.tokens[i].kind == k) n++;
    sc_tokens_free(&arr);
    return n;
}

/* Helper: get kind at position i (all tokens) */
static ScKind tok_kind(ScTokenArray *arr, int i) { return arr->tokens[i].kind; }
static const char *tok_text(ScTokenArray *arr, int i) { return arr->tokens[i].text; }
static int tok_line(ScTokenArray *arr, int i) { return arr->tokens[i].line; }

/* --------------------------------------------------------------------------
 * 0. M-SNOC-LEX trigger  (OUTPUT = 'hello' -> 3 content tokens)
 * ------------------------------------------------------------------------ */
static void test_snoc_lex_trigger(void)
{
    ScTokenArray arr = sc_lex("OUTPUT = 'hello'\n");
    /* expect: IDENT ASSIGN STRING NEWLINE EOF */
    CHECK_KIND("trigger[0] IDENT",   tok_kind(&arr,0), SC_IDENT);
    CHECK_TEXT("trigger[0] OUTPUT",  tok_text(&arr,0), "OUTPUT");
    CHECK_KIND("trigger[1] ASSIGN",  tok_kind(&arr,1), SC_ASSIGN);
    CHECK_TEXT("trigger[1] =",       tok_text(&arr,1), "=");
    CHECK_KIND("trigger[2] STRING",  tok_kind(&arr,2), SC_STRING);
    CHECK_TEXT("trigger[2] 'hello'", tok_text(&arr,2), "'hello'");
    CHECK_KIND("trigger[3] NEWLINE", tok_kind(&arr,3), SC_NEWLINE);
    CHECK_KIND("trigger[4] EOF",     tok_kind(&arr,4), SC_EOF);
    sc_tokens_free(&arr);
}

/* --------------------------------------------------------------------------
 * 1. Helpers
 * ------------------------------------------------------------------------ */
static void test_helpers(void)
{
    /* comment stripping -- we test indirectly via lex output */

    /* no comment: x = 1 untouched */
    { ScKind k[8]; int n = kinds("x = 1\n", k, 8);
      CHECK_INT("strip_no_comment kinds", n, 3); }

    /* trailing hash stripped: x = 1 # comment */
    { ScKind k[8]; int n = kinds("x = 1 # comment\n", k, 8);
      CHECK_INT("strip_trailing_hash kinds", n, 3); }

    /* hash inside single quote NOT stripped */
    { ScTokenArray arr = sc_lex("x = 'a#b'\n");
      CHECK_KIND("hash_in_single_quote tok2", tok_kind(&arr,2), SC_STRING);
      CHECK_TEXT("hash_in_single_quote text", tok_text(&arr,2), "'a#b'");
      sc_tokens_free(&arr); }

    /* hash inside double quote NOT stripped */
    { ScTokenArray arr = sc_lex("x = \"a#b\"\n");
      CHECK_KIND("hash_in_double_quote tok2", tok_kind(&arr,2), SC_STRING);
      CHECK_TEXT("hash_in_double_quote text", tok_text(&arr,2), "\"a#b\"");
      sc_tokens_free(&arr); }

    /* leading hash -- full comment, no content tokens */
    { ScKind k[8]; int n = kinds("# full comment line\n", k, 8);
      CHECK_INT("leading_hash no tokens", n, 0); }

    /* continuation: ends with + -> one NEWLINE for two physical lines */
    CHECK_INT("continuation_plus newlines", count_kind("x = 1 +\n2\n", SC_NEWLINE), 1);

    /* continuation: ends with colon */
    CHECK_INT("continuation_colon newlines", count_kind("x :\ny\n", SC_NEWLINE), 1);

    /* continuation: ends with comma */
    CHECK_INT("continuation_comma newlines", count_kind("f(a,\nb)\n", SC_NEWLINE), 1);

    /* NO continuation: ends with identifier */
    CHECK_INT("no_cont_ident newlines", count_kind("x = y\n", SC_NEWLINE), 1);

    /* continuation: whitespace after cont char still counts */
    CHECK_INT("cont_whitespace_after newlines", count_kind("x +  \n2\n", SC_NEWLINE), 1);

    /* SplitSemicolon: no semicolon -> 1 statement */
    CHECK_INT("split_no_semi", count_kind("x = 1\n", SC_NEWLINE), 1);

    /* SplitSemicolon: two statements */
    CHECK_INT("split_two_stmts", count_kind("x = 1; y = 2\n", SC_NEWLINE), 2);

    /* SplitSemicolon: semicolon inside string not split */
    CHECK_INT("split_semi_in_string", count_kind("x = 'a;b'\n", SC_NEWLINE), 1);
}

/* --------------------------------------------------------------------------
 * 2. Literals
 * ------------------------------------------------------------------------ */
static void test_literals(void)
{
    { ScTokenArray a = sc_lex("42\n");
      CHECK_KIND("integer kind", tok_kind(&a,0), SC_INTEGER);
      CHECK_TEXT("integer text", tok_text(&a,0), "42");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("3.14\n");
      CHECK_KIND("real_decimal kind", tok_kind(&a,0), SC_REAL);
      CHECK_TEXT("real_decimal text", tok_text(&a,0), "3.14");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("1e10\n");
      CHECK_KIND("real_exp_lower kind", tok_kind(&a,0), SC_REAL);
      CHECK_TEXT("real_exp_lower text", tok_text(&a,0), "1e10");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("2.5E-3\n");
      CHECK_KIND("real_exp_upper kind", tok_kind(&a,0), SC_REAL);
      CHECK_TEXT("real_exp_upper text", tok_text(&a,0), "2.5E-3");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("1D2\n");
      CHECK_KIND("real_exp_D kind", tok_kind(&a,0), SC_REAL);
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex(".5\n");
      CHECK_KIND("real_leading_dot kind", tok_kind(&a,0), SC_REAL);
      CHECK_TEXT("real_leading_dot text", tok_text(&a,0), ".5");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("'hello'\n");
      CHECK_KIND("string_single kind", tok_kind(&a,0), SC_STRING);
      CHECK_TEXT("string_single text", tok_text(&a,0), "'hello'");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("\"world\"\n");
      CHECK_KIND("string_double kind", tok_kind(&a,0), SC_STRING);
      CHECK_TEXT("string_double text", tok_text(&a,0), "\"world\"");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("'a;b'\n");
      CHECK_KIND("string_semi kind",   tok_kind(&a,0), SC_STRING);
      CHECK_TEXT("string_semi text",   tok_text(&a,0), "'a;b'");
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("'a#b'\n");
      CHECK_KIND("string_hash kind",   tok_kind(&a,0), SC_STRING);
      CHECK_TEXT("string_hash text",   tok_text(&a,0), "'a#b'");
      sc_tokens_free(&a); }
}

/* --------------------------------------------------------------------------
 * 3. Keywords
 * ------------------------------------------------------------------------ */
static void test_keywords(void)
{
    ScKind k[4];
#define KW(src, want) do { kinds(src"\n", k, 4); CHECK_KIND("kw_"src, k[0], want); } while(0)
    KW("if",        SC_KW_IF);
    KW("else",      SC_KW_ELSE);
    KW("while",     SC_KW_WHILE);
    KW("do",        SC_KW_DO);
    KW("for",       SC_KW_FOR);
    KW("return",    SC_KW_RETURN);
    KW("freturn",   SC_KW_FRETURN);
    KW("nreturn",   SC_KW_NRETURN);
    KW("go",        SC_KW_GO);
    KW("to",        SC_KW_TO);
    KW("procedure", SC_KW_PROCEDURE);
    KW("struct",    SC_KW_STRUCT);
#undef KW

    /* "iffy" must be IDENT, not IF + fy */
    kinds("iffy\n", k, 4);
    CHECK_KIND("kw_not_prefix_iffy", k[0], SC_IDENT);

    /* Case-sensitive: "IF" is an identifier */
    kinds("IF\n", k, 4);
    CHECK_KIND("kw_case_IF", k[0], SC_IDENT);
}

/* --------------------------------------------------------------------------
 * 4. Operators -- every bconv entry
 * ------------------------------------------------------------------------ */
static void test_operators(void)
{
    ScKind k[4];
#define OP(src, want) do { kinds(src"\n", k, 4); CHECK_KIND("op_"src, k[0], want); } while(0)
    OP("=",    SC_ASSIGN);
    OP("?",    SC_QUESTION);
    OP("|",    SC_PIPE);
    OP("||",   SC_OR);
    OP("&&",   SC_CONCAT);
    OP("==",   SC_EQ);
    OP("!=",   SC_NE);
    OP("<",    SC_LT);
    OP(">",    SC_GT);
    OP("<=",   SC_LE);
    OP(">=",   SC_GE);
    OP("::",   SC_STR_IDENT);
    OP(":!:",  SC_STR_DIFFER);
    OP(":<:",  SC_STR_LT);
    OP(":>:",  SC_STR_GT);
    OP(":<=:", SC_STR_LE);
    OP(":>=:", SC_STR_GE);
    OP(":==:", SC_STR_EQ);
    OP(":!=:", SC_STR_NE);
    OP("+",    SC_PLUS);
    OP("-",    SC_MINUS);
    OP("/",    SC_SLASH);
    OP("*",    SC_STAR);
    OP("%",    SC_PERCENT);
    OP("^",    SC_CARET);
    OP(".",    SC_PERIOD);
    OP("$",    SC_DOLLAR);
    OP("~",    SC_TILDE);
    OP("@",    SC_AT);
    OP("&",    SC_AMPERSAND);
    OP("**",   SC_CARET);    /* ** same as ^ */
#undef OP

    /* longest-match: :!=: is one 4-char token */
    { int n = kinds(":!=:\n", k, 4);
      CHECK_INT("longest_match_strne count", n, 1);
      CHECK_KIND("longest_match_strne kind", k[0], SC_STR_NE); }

    /* || is one token */
    { int n = kinds("||\n", k, 4);
      CHECK_INT("longest_match_or count", n, 1);
      CHECK_KIND("longest_match_or kind", k[0], SC_OR); }

    /* && is one token */
    { int n = kinds("&&\n", k, 4);
      CHECK_INT("longest_match_concat count", n, 1);
      CHECK_KIND("longest_match_concat kind", k[0], SC_CONCAT); }
}

/* --------------------------------------------------------------------------
 * 5. Punctuation
 * ------------------------------------------------------------------------ */
static void test_punctuation(void)
{
    ScKind k[4];
#define PUNCT(src, want) do { kinds(src"\n", k, 4); CHECK_KIND("punct_"src, k[0], want); } while(0)
    PUNCT("(", SC_LPAREN);
    PUNCT(")", SC_RPAREN);
    PUNCT("{", SC_LBRACE);
    PUNCT("}", SC_RBRACE);
    PUNCT("[", SC_LBRACKET);
    PUNCT("]", SC_RBRACKET);
    PUNCT(",", SC_COMMA);
    PUNCT(":", SC_COLON);
#undef PUNCT
}

/* --------------------------------------------------------------------------
 * 6. Statement boundaries
 * ------------------------------------------------------------------------ */
static void test_boundaries(void)
{
    /* newline after statement */
    CHECK_INT("newline_after_stmt", count_kind("x = 1\n", SC_NEWLINE), 1);

    /* two statements, two newlines */
    CHECK_INT("two_stmts_two_newlines", count_kind("x = 1\ny = 2\n", SC_NEWLINE), 2);

    /* continuation: x = 1 + \n 2 -> one newline, token stream x = 1 + 2 */
    { ScTokenArray a = sc_lex("x = 1 +\n2\n");
      int nl = 0;
      for (int i = 0; i < a.count; i++)
          if (a.tokens[i].kind == SC_NEWLINE) nl++;
      CHECK_INT("cont_plus one_newline", nl, 1);
      /* collect non-newline, non-EOF kinds */
      ScKind k[10]; int n = 0;
      for (int i = 0; i < a.count; i++) {
          ScKind kk = a.tokens[i].kind;
          if (kk != SC_NEWLINE && kk != SC_EOF) k[n++] = kk;
      }
      CHECK_INT("cont_plus kind_count", n, 5);
      CHECK_KIND("cont_plus k[0]", k[0], SC_IDENT);
      CHECK_KIND("cont_plus k[1]", k[1], SC_ASSIGN);
      CHECK_KIND("cont_plus k[2]", k[2], SC_INTEGER);
      CHECK_KIND("cont_plus k[3]", k[3], SC_PLUS);
      CHECK_KIND("cont_plus k[4]", k[4], SC_INTEGER);
      sc_tokens_free(&a); }

    /* continuation on comma */
    CHECK_INT("cont_comma", count_kind("f(a,\nb)\n", SC_NEWLINE), 1);

    /* comment stripped before continuation check */
    CHECK_INT("cont_comment_strip", count_kind("x +  # note\n2\n", SC_NEWLINE), 1);

    /* blank line skipped */
    CHECK_INT("blank_line_skip", count_kind("x = 1\n\ny = 2\n", SC_NEWLINE), 2);

    /* comment-only line skipped */
    CHECK_INT("comment_only_skip", count_kind("x = 1\n# comment\ny = 2\n", SC_NEWLINE), 2);
}

/* --------------------------------------------------------------------------
 * 7. Semicolons
 * ------------------------------------------------------------------------ */
static void test_semicolons(void)
{
    CHECK_INT("semi_two_stmts",  count_kind("x = 1; y = 2\n", SC_NEWLINE), 2);
    CHECK_INT("semi_in_string",  count_kind("x = 'a;b'\n",    SC_NEWLINE), 1);
}

/* --------------------------------------------------------------------------
 * 8. Line numbers
 * ------------------------------------------------------------------------ */
static void test_line_numbers(void)
{
    { ScTokenArray a = sc_lex("x = 1\n");
      CHECK_INT("line_first_tok", tok_line(&a,0), 1);
      sc_tokens_free(&a); }

    { ScTokenArray a = sc_lex("x = 1\ny = 2\n");
      /* find 'y' token */
      for (int i = 0; i < a.count; i++) {
          if (a.tokens[i].kind == SC_IDENT &&
              strcmp(a.tokens[i].text, "y") == 0) {
              CHECK_INT("line_second_stmt", a.tokens[i].line, 2);
              break;
          }
      }
      sc_tokens_free(&a); }

    { /* continued stmt: all tokens use line 1 */
      ScTokenArray a = sc_lex("x = 1 +\n2\n");
      int all_line1 = 1;
      for (int i = 0; i < a.count; i++) {
          ScKind k = a.tokens[i].kind;
          if (k != SC_NEWLINE && k != SC_EOF && a.tokens[i].line != 1)
              all_line1 = 0;
      }
      CHECK_INT("line_cont_all_line1", all_line1, 1);
      sc_tokens_free(&a); }
}

/* --------------------------------------------------------------------------
 * 9. End-to-end snippets (from TestSnoconeLexer.cs E2E tests)
 * ------------------------------------------------------------------------ */
static void test_e2e(void)
{
    /* if (x == 0) { y = 1 } */
    { ScKind k[12]; int n = kinds("if (x == 0) { y = 1 }\n", k, 12);
      CHECK_INT   ("e2e_if kind_count",  n, 11);
      CHECK_KIND  ("e2e_if k[0]",  k[0],  SC_KW_IF);
      CHECK_KIND  ("e2e_if k[1]",  k[1],  SC_LPAREN);
      CHECK_KIND  ("e2e_if k[2]",  k[2],  SC_IDENT);
      CHECK_KIND  ("e2e_if k[3]",  k[3],  SC_EQ);
      CHECK_KIND  ("e2e_if k[4]",  k[4],  SC_INTEGER);
      CHECK_KIND  ("e2e_if k[5]",  k[5],  SC_RPAREN);
      CHECK_KIND  ("e2e_if k[6]",  k[6],  SC_LBRACE);
      CHECK_KIND  ("e2e_if k[7]",  k[7],  SC_IDENT);
      CHECK_KIND  ("e2e_if k[8]",  k[8],  SC_ASSIGN);
      CHECK_KIND  ("e2e_if k[9]",  k[9],  SC_INTEGER);
      CHECK_KIND  ("e2e_if k[10]", k[10], SC_RBRACE); }

    /* procedure foo(a, b) { */
    { ScKind k[10]; kinds("procedure foo(a, b) {\n", k, 10);
      CHECK_KIND("e2e_proc k[0]", k[0], SC_KW_PROCEDURE);
      CHECK_KIND("e2e_proc k[1]", k[1], SC_IDENT);
      CHECK_KIND("e2e_proc k[2]", k[2], SC_LPAREN);
      CHECK_KIND("e2e_proc k[3]", k[3], SC_IDENT);
      CHECK_KIND("e2e_proc k[4]", k[4], SC_COMMA);
      CHECK_KIND("e2e_proc k[5]", k[5], SC_IDENT);
      CHECK_KIND("e2e_proc k[6]", k[6], SC_RPAREN);
      CHECK_KIND("e2e_proc k[7]", k[7], SC_LBRACE); }

    /* while (i > 0) { */
    { ScKind k[8]; kinds("while (i > 0) {\n", k, 8);
      CHECK_KIND("e2e_while k[0]", k[0], SC_KW_WHILE);
      CHECK_KIND("e2e_while k[1]", k[1], SC_LPAREN);
      CHECK_KIND("e2e_while k[2]", k[2], SC_IDENT);
      CHECK_KIND("e2e_while k[3]", k[3], SC_GT);
      CHECK_KIND("e2e_while k[4]", k[4], SC_INTEGER);
      CHECK_KIND("e2e_while k[5]", k[5], SC_RPAREN);
      CHECK_KIND("e2e_while k[6]", k[6], SC_LBRACE); }

    /* for (i = 1, i <= n, i = i + 1) -- 2 commas */
    { ScKind k[20]; int n = kinds("for (i = 1, i <= n, i = i + 1)\n", k, 20);
      CHECK_KIND("e2e_for k[0]", k[0], SC_KW_FOR);
      int commas = 0;
      for (int i = 0; i < n; i++) if (k[i] == SC_COMMA) commas++;
      CHECK_INT("e2e_for commas", commas, 2); }

    /* string comparison ops */
    { ScKind k[4]; kinds("a :==: b\n", k, 4); CHECK_KIND("e2e_streq", k[1], SC_STR_EQ); }
    { ScKind k[4]; kinds("a :!=: b\n", k, 4); CHECK_KIND("e2e_strne", k[1], SC_STR_NE); }
    { ScKind k[4]; kinds("a :<: b\n",  k, 4); CHECK_KIND("e2e_strlt", k[1], SC_STR_LT); }
    { ScKind k[4]; kinds("a :>: b\n",  k, 4); CHECK_KIND("e2e_strgt", k[1], SC_STR_GT); }

    /* return x + 1 */
    { ScKind k[6]; kinds("return x + 1\n", k, 6);
      CHECK_KIND("e2e_return k[0]", k[0], SC_KW_RETURN);
      CHECK_KIND("e2e_return k[1]", k[1], SC_IDENT);
      CHECK_KIND("e2e_return k[2]", k[2], SC_PLUS);
      CHECK_KIND("e2e_return k[3]", k[3], SC_INTEGER); }

    /* go to END */
    { ScKind k[4]; kinds("go to END\n", k, 4);
      CHECK_KIND("e2e_goto k[0]", k[0], SC_KW_GO);
      CHECK_KIND("e2e_goto k[1]", k[1], SC_KW_TO);
      CHECK_KIND("e2e_goto k[2]", k[2], SC_IDENT); }

    /* struct point { x, y } */
    { ScKind k[8]; kinds("struct point { x, y }\n", k, 8);
      CHECK_KIND("e2e_struct k[0]", k[0], SC_KW_STRUCT);
      CHECK_KIND("e2e_struct k[1]", k[1], SC_IDENT);
      CHECK_KIND("e2e_struct k[2]", k[2], SC_LBRACE);
      CHECK_KIND("e2e_struct k[3]", k[3], SC_IDENT);
      CHECK_KIND("e2e_struct k[4]", k[4], SC_COMMA);
      CHECK_KIND("e2e_struct k[5]", k[5], SC_IDENT);
      CHECK_KIND("e2e_struct k[6]", k[6], SC_RBRACE); }

    /* arr[i] */
    { ScKind k[6]; kinds("arr[i]\n", k, 6);
      CHECK_KIND("e2e_array k[0]", k[0], SC_IDENT);
      CHECK_KIND("e2e_array k[1]", k[1], SC_LBRACKET);
      CHECK_KIND("e2e_array k[2]", k[2], SC_IDENT);
      CHECK_KIND("e2e_array k[3]", k[3], SC_RBRACKET); }

    /* x && y */
    { ScKind k[4]; kinds("x && y\n", k, 4);
      CHECK_KIND("e2e_concat k[0]", k[0], SC_IDENT);
      CHECK_KIND("e2e_concat k[1]", k[1], SC_CONCAT);
      CHECK_KIND("e2e_concat k[2]", k[2], SC_IDENT); }

    /* multiline continuation: x = a +\nb +\nc -> 1 newline, texts x = a + b + c */
    { ScTokenArray a = sc_lex("x = a +\nb +\nc\n");
      CHECK_INT("e2e_multiline newlines", count_kind("x = a +\nb +\nc\n", SC_NEWLINE), 1);
      char *texts[10]; int tn = 0;
      for (int i = 0; i < a.count; i++) {
          if (a.tokens[i].kind != SC_NEWLINE && a.tokens[i].kind != SC_EOF)
              texts[tn++] = a.tokens[i].text;
      }
      CHECK_INT   ("e2e_multiline tok_count", tn, 7);
      CHECK_TEXT  ("e2e_multiline t[0]", texts[0], "x");
      CHECK_TEXT  ("e2e_multiline t[1]", texts[1], "=");
      CHECK_TEXT  ("e2e_multiline t[2]", texts[2], "a");
      CHECK_TEXT  ("e2e_multiline t[3]", texts[3], "+");
      CHECK_TEXT  ("e2e_multiline t[4]", texts[4], "b");
      CHECK_TEXT  ("e2e_multiline t[5]", texts[5], "+");
      CHECK_TEXT  ("e2e_multiline t[6]", texts[6], "c");
      sc_tokens_free(&a); }

    /* EOF token present */
    { ScTokenArray a = sc_lex("x = 1\n");
      CHECK_KIND("e2e_eof", a.tokens[a.count-1].kind, SC_EOF);
      sc_tokens_free(&a); }

    /* empty source -> only EOF */
    { ScTokenArray a = sc_lex("");
      CHECK_INT ("e2e_empty count", a.count, 1);
      CHECK_KIND("e2e_empty eof",   a.tokens[0].kind, SC_EOF);
      sc_tokens_free(&a); }

    /* comment-only source -> only EOF */
    { ScTokenArray a = sc_lex("# nothing here\n# more nothing\n");
      CHECK_INT ("e2e_comment_only count", a.count, 1);
      CHECK_KIND("e2e_comment_only eof",   a.tokens[0].kind, SC_EOF);
      sc_tokens_free(&a); }
}

/* --------------------------------------------------------------------------
 * main
 * ------------------------------------------------------------------------ */
int main(void)
{
    printf("=== sc_lex_test ===\n");
    test_snoc_lex_trigger();
    test_helpers();
    test_literals();
    test_keywords();
    test_operators();
    test_punctuation();
    test_boundaries();
    test_semicolons();
    test_line_numbers();
    test_e2e();

    printf("==================\n");
    printf("Results: %d passed, %d failed\n", g_pass, g_fail);
    if (g_fail == 0) {
        printf("PASS\n");
        return 0;
    } else {
        printf("FAIL\n");
        return 1;
    }
}
