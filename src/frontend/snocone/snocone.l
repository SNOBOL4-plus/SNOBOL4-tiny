%{
/*
 * snocone.l — Snocone Flex lexer (GOAL-SNOCONE-LANG-SPACE LS-3)
 *
 * DESIGN:
 *   Space envelops the operator.  Pattern {W}OP{W} matches the operator
 *   together with its surrounding whitespace.  Whitespace alone (between
 *   two atoms with no operator between them) lexes as T_CONCAT.  Unary
 *   versions of dual-role operators arrive bare (no leading space) and
 *   are returned as T_UN_*.  This matches the snobol4.l lexer pattern
 *   exactly (lines 235-315 of snobol4.l).
 *
 *   The CONCAT decision is therefore not a previous-token state machine
 *   — it falls out of the rule patterns themselves.  This is the
 *   elegant SNOBOL4-Flex approach: longest-match selects the right rule,
 *   and the token stream is unambiguous by construction.
 *
 * NAMING: token names match snobol4.tab.h conventions (T_IDENT, T_INT,
 *   T_REAL, T_STR, T_FUNCTION, T_KEYWORD, T_CONCAT, T_ADDITION, etc.)
 *   to keep one4all "same names everywhere for any concept equivalence"
 *   (RULES.md / Lon's session-#9 directive).
 *
 * Andrew's .sc operator set, minus &&, ||, %.  Plus C-style control
 * flow keywords (if/else/while/do/until/for/switch/case/default/break/
 * continue/goto/function/return/freturn/nreturn/struct).
 *
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snocone_lex2.h"

static int   sc_lineno = 1;
static char  sc_strbuf[65536];
static int   sc_strpos;

static void emit_simple(LexerState *st, int kind, const char *text);
static void emit_str(LexerState *st, const char *s);
static int  classify_keyword(const char *s);
static void count_newlines(const char *s);

/* push token with default text=yytext */
#define EMIT(kind) emit_simple(yyextra, (kind), yytext)
#define EMIT_NL(kind) do { count_newlines(yytext); emit_simple(yyextra, (kind), yytext); } while(0)

%}

%option noyywrap nounput noinput
%option reentrant
%option extra-type="LexerState *"
%option prefix="sc_"

%x BODY_START BODY STR1 STR2 BLOCK_COMMENT

DIGIT   [0-9]
ALPHA   [A-Za-z_]
IDCONT  [A-Za-z0-9_]
EXP     [eEdD][+-]?{DIGIT}+
WS      [ \t\r\f]
CONT    \n[+.][ \t\r\f]*
W       ({WS}|{CONT}|\n)+
G       ({WS}|{CONT}|\n)*

%%

  /* INITIAL: jump straight into BODY (no card-format dispatch in Snocone —
   * Snocone uses C-style blocks, semicolons, and free-form layout). */
.|\n            { yyless(0); BEGIN(BODY); }

  /* ========================================================== */
  /* BODY: statement body                                       */
  /* ========================================================== */

  /* Comments are pure whitespace (transparent for CONCAT). */
<BODY>"//"[^\n]*           { /* line comment — fall through to next ws */ }
<BODY>"/*"                  { BEGIN(BLOCK_COMMENT); }

<BLOCK_COMMENT>"*/"         { BEGIN(BODY); }
<BLOCK_COMMENT>\n           { sc_lineno++; }
<BLOCK_COMMENT>.            { /* absorbed */ }

  /* String literals — STR1 single-quote, STR2 double-quote.
   * SNOBOL4 strings: no backslash escapes; '' inside '...' is one '.   */
<BODY>\'                    { sc_strpos = 0; BEGIN(STR1); }
<BODY>\"                    { sc_strpos = 0; BEGIN(STR2); }

<STR1>\'\'                  { sc_strbuf[sc_strpos++] = '\''; }
<STR1>\'                    { sc_strbuf[sc_strpos] = '\0';
                              BEGIN(BODY);
                              emit_str(yyextra, sc_strbuf); }
<STR1>\n                    { sc_strbuf[sc_strpos++] = '\n'; sc_lineno++; }
<STR1>.                     { sc_strbuf[sc_strpos++] = yytext[0]; }

<STR2>\"\"                  { sc_strbuf[sc_strpos++] = '"'; }
<STR2>\"                    { sc_strbuf[sc_strpos] = '\0';
                              BEGIN(BODY);
                              emit_str(yyextra, sc_strbuf); }
<STR2>\n                    { sc_strbuf[sc_strpos++] = '\n'; sc_lineno++; }
<STR2>.                     { sc_strbuf[sc_strpos++] = yytext[0]; }

  /* &IDENT as one token (no space between & and ident) — like SNOBOL4 */
<BODY>"&"{ALPHA}{IDCONT}*   { emit_simple(yyextra, T_KEYWORD, yytext + 1); }

  /* T_CONCAT: pure whitespace between two atoms — emit if previous token
   * was atom-class.  We inhibit emission immediately after openers / before
   * closers / at end-of-statement using the prev-token-class check inside
   * emit_simple.  In the {W}OP{W} pattern below, the operator's surrounding
   * space is consumed *with* the operator, so plain {W} only matches when
   * no operator follows — that's where CONCAT belongs. */
<BODY>{W}                   { count_newlines(yytext);
                              emit_simple(yyextra, T_CONCAT, ""); }

  /* ========================================================== */
  /* Binary operators: {W} OP {W}                              */
  /* The mandatory whitespace on at least one side distinguishes */
  /* binary use from unary.  Where unary form is the ONLY        */
  /* meaningful interpretation we drop the W-envelope.           */
  /* ========================================================== */

  /* SPITBOL/Snocone priority-0 assign — = with required surrounding space */
<BODY>{W}"="{W}             { EMIT_NL(T_ASSIGNMENT); }
  /* = at end-of-line / no trailing space — still T_ASSIGNMENT (DYN-63 mirror) */
<BODY>{W}"="                 { EMIT_NL(T_ASSIGNMENT); }

  /* Pattern match — same shape */
<BODY>{W}"?"{W}             { EMIT_NL(T_MATCH); }

  /* Alternation */
<BODY>{W}"|"{W}             { EMIT_NL(T_ALTERNATION); }

  /* String comparison — multi-character, must come BEFORE the */
  /* single-char rules to win the longest-match contest.       */
<BODY>{W}":==:"{W}          { EMIT_NL(T_LEQ); }
<BODY>{W}":!=:"{W}          { EMIT_NL(T_LNE); }
<BODY>{W}":<=:"{W}          { EMIT_NL(T_LLE); }
<BODY>{W}":>=:"{W}          { EMIT_NL(T_LGE); }
<BODY>{W}":<:"{W}           { EMIT_NL(T_LLT); }
<BODY>{W}":>:"{W}           { EMIT_NL(T_LGT); }
<BODY>{W}":!:"{W}           { EMIT_NL(T_DIFFER); }
<BODY>{W}"::"{W}            { EMIT_NL(T_IDENT_OP); }

  /* Numeric comparison (Andrew's .sc 6-op set) */
<BODY>{W}"=="{W}            { EMIT_NL(T_EQ); }
<BODY>{W}"!="{W}            { EMIT_NL(T_NE); }
<BODY>{W}"<="{W}            { EMIT_NL(T_LE); }
<BODY>{W}">="{W}            { EMIT_NL(T_GE); }
<BODY>{W}"<"{W}             { EMIT_NL(T_LT); }
<BODY>{W}">"{W}             { EMIT_NL(T_GT); }

  /* Compound-assignments — adopted in today's lexer; LS-2 will decide
   * whether the grammar accepts these.  The lexer keeps them. */
<BODY>{G}"+="{G}            { EMIT_NL(T_PLUS_ASSIGN); }
<BODY>{G}"-="{G}            { EMIT_NL(T_MINUS_ASSIGN); }
<BODY>{G}"*="{G}            { EMIT_NL(T_STAR_ASSIGN); }
<BODY>{G}"/="{G}            { EMIT_NL(T_SLASH_ASSIGN); }
<BODY>{G}"^="{G}            { EMIT_NL(T_CARET_ASSIGN); }

  /* Exponentiation synonyms: ^, **, !  (pri 11 right-assoc) */
<BODY>{W}"**"{W}            { EMIT_NL(T_EXPONENTIATION); }
<BODY>{W}"^"{W}             { EMIT_NL(T_EXPONENTIATION); }
<BODY>{W}"!"{W}             { EMIT_NL(T_EXPONENTIATION); }

  /* Arithmetic */
<BODY>{W}"+"{W}             { EMIT_NL(T_ADDITION); }
<BODY>{W}"-"{W}             { EMIT_NL(T_SUBTRACTION); }
<BODY>{W}"*"{W}             { EMIT_NL(T_MULTIPLICATION); }
<BODY>{W}"/"{W}             { EMIT_NL(T_DIVISION); }

  /* Indirection (binary $ — immediate assign in pattern context) */
<BODY>{W}"$"{W}             { EMIT_NL(T_IMMEDIATE_ASSIGN); }
  /* Name-of (binary . — conditional assign in pattern context) */
<BODY>{W}"."{W}             { EMIT_NL(T_COND_ASSIGN); }

  /* OPSYN slot operators (binary forms — Andrew's & % @ # ~) */
<BODY>{W}"&"{W}             { EMIT_NL(T_AMPERSAND); }
<BODY>{W}"@"{W}             { EMIT_NL(T_AT_SIGN); }
<BODY>{W}"%"{W}             { EMIT_NL(T_PERCENT); }
<BODY>{W}"~"{W}             { EMIT_NL(T_TILDE); }
<BODY>{W}"#"{W}             { EMIT_NL(T_POUND); }

  /* ========================================================== */
  /* Structural punctuation                                    */
  /* ========================================================== */

  /* ( absorbs trailing gray (matches snobol4.l line 281).
   * IMPORTANT: ( with NO leading whitespace and a preceding identifier
   * is the call-form — handled by the T_FUNCTION rule below via trailing
   * context.  This rule fires for sub-expression parens.            */
<BODY>"("{G}                { EMIT_NL(T_LPAREN); }
<BODY>{G}")"                { EMIT_NL(T_RPAREN); }
<BODY>"["{G}                { EMIT_NL(T_LBRACK); }
<BODY>{G}"]"                { EMIT_NL(T_RBRACK); }
<BODY>{G}","{G}             { EMIT_NL(T_COMMA); }
<BODY>{G}";"                { EMIT_NL(T_SEMICOLON); }

  /* Block braces (Snocone-only) — gray on outside */
<BODY>{G}"{"{G}             { EMIT_NL(T_LBRACE); }
<BODY>{G}"}"                { EMIT_NL(T_RBRACE); }

  /* Colon: label suffix.  Andrew's .sc uses `name : clause` (label) and
   * `case value :` (switch case).  This is a bare colon — the multi-char
   * comparison rules above (`:==:` etc.) win longest-match. */
<BODY>{G}":"                { EMIT_NL(T_COLON); }

  /* ========================================================== */
  /* Atoms — arrive bare (preceding T_CONCAT or W{OP} consumed   */
  /* the leading whitespace already)                            */
  /* ========================================================== */

  /* T_FUNCTION: identifier immediately followed by '(' (no space).
   * Same trick as snobol4.l line 290.  The '(' stays in the input
   * stream — the BODY '(' rule consumes it next as T_LPAREN.  Grammar
   * sees: T_FUNCTION T_LPAREN args T_RPAREN. */
<BODY>{ALPHA}{IDCONT}*/"("  { emit_simple(yyextra, T_FUNCTION, yytext); }

  /* T_IDENT or keyword */
<BODY>{ALPHA}{IDCONT}*      {
                              int kind = classify_keyword(yytext);
                              emit_simple(yyextra, kind, yytext);
                            }

  /* Numbers — REAL forms before INT (longest-match) */
<BODY>{DIGIT}+\.{DIGIT}*({EXP})?  { EMIT(T_REAL); }
<BODY>{DIGIT}+{EXP}                { EMIT(T_REAL); }
<BODY>\.{DIGIT}+({EXP})?           { EMIT(T_REAL); }
<BODY>{DIGIT}+                     { EMIT(T_INT); }

  /* ========================================================== */
  /* Unary operators — bare (no leading W).  The grammar has    */
  /* T_UN_* productions for each.                               */
  /* ========================================================== */

<BODY>"**"   { EMIT(T_EXPONENTIATION); }
<BODY>"^"    { EMIT(T_EXPONENTIATION); }
<BODY>"!"    { EMIT(T_EXPONENTIATION); }
<BODY>"+"    { EMIT(T_UN_PLUS); }
<BODY>"-"    { EMIT(T_UN_MINUS); }
<BODY>"*"    { EMIT(T_UN_ASTERISK); }
<BODY>"/"    { EMIT(T_UN_SLASH); }
<BODY>"%"    { EMIT(T_UN_PERCENT); }
<BODY>"@"    { EMIT(T_UN_AT_SIGN); }
<BODY>"~"    { EMIT(T_UN_TILDE); }
<BODY>"$"    { EMIT(T_UN_DOLLAR_SIGN); }
<BODY>"."    { EMIT(T_UN_PERIOD); }
<BODY>"#"    { EMIT(T_UN_POUND); }
<BODY>"|"    { EMIT(T_UN_VERTICAL_BAR); }
<BODY>"="    { EMIT(T_UN_EQUAL); }
<BODY>"?"    { EMIT(T_UN_QUESTION_MARK); }
<BODY>"&"    { EMIT(T_UN_AMPERSAND); }

<BODY>.      {
    /* Unrecognised character */
    emit_simple(yyextra, T_UNKNOWN, yytext);
}

<<EOF>>      {
    emit_simple(yyextra, T_EOF, "");
    yyterminate();
}

%%

/* ========================================================== */
/* Keyword table — case-sensitive (RULES.md)                  */
/* ========================================================== */

typedef struct { const char *word; int kind; } KwEntry;
static const KwEntry KW_TABLE[] = {
    /* Control flow */
    { "if",       T_KW_IF       },
    { "else",     T_KW_ELSE     },
    { "while",    T_KW_WHILE    },
    { "do",       T_KW_DO       },
    { "until",    T_KW_UNTIL    },
    { "for",      T_KW_FOR      },
    { "switch",   T_KW_SWITCH   },
    { "case",     T_KW_CASE     },
    { "default",  T_KW_DEFAULT  },
    { "break",    T_KW_BREAK    },
    { "continue", T_KW_CONTINUE },
    { "goto",     T_KW_GOTO     },
    /* Function / return */
    { "function", T_KW_FUNCTION },   /* RENAMED from procedure (session #7) */
    { "return",   T_KW_RETURN   },
    { "freturn",  T_KW_FRETURN  },
    { "nreturn",  T_KW_NRETURN  },
    /* Data */
    { "struct",   T_KW_STRUCT   },
    { NULL,       T_IDENT       }
};

static int classify_keyword(const char *s) {
    for (const KwEntry *e = KW_TABLE; e->word; e++)
        if (strcmp(s, e->word) == 0) return e->kind;
    return T_IDENT;
}

/* ========================================================== */
/* Token-buffer accumulation                                  */
/* ========================================================== */

static void emit_simple(LexerState *st, int kind, const char *text) {
    /* CONCAT suppression: never emit two CONCATs in a row, never emit
     * CONCAT at the start of the token stream, never emit CONCAT
     * immediately after an opener or operator (its purpose is to
     * separate two value-yielding atoms only). */
    if (kind == T_CONCAT) {
        if (st->buf->count == 0) return;
        int last = st->buf->tokens[st->buf->count - 1].kind;
        if (last == T_CONCAT) return;
        /* If the previous token is non-value (operator, opener, separator),
         * the whitespace is just trivia.  Only emit CONCAT after value-
         * yielding tokens. */
        if (!sc_kind_is_value(last)) return;
    }

    /* If we were about to emit a value-yielding token but the previous
     * token was also a value-yielding token AND no CONCAT has been
     * emitted between them, the source had no whitespace between the
     * two atoms.  In SNOBOL4 lexing the {W} envelope already inserts
     * the CONCAT (because at least one space is required between two
     * atoms with no operator between them).  In Snocone we inherit
     * the same property: this branch is dead in correct programs. */

    if (st->buf->count >= st->buf->capacity) {
        st->buf->capacity = st->buf->capacity ? st->buf->capacity * 2 : 64;
        st->buf->tokens   = realloc(st->buf->tokens,
                                    st->buf->capacity * sizeof(ScToken2));
    }
    ScToken2 *t = &st->buf->tokens[st->buf->count++];
    t->kind = kind;
    t->text = strdup(text);
    t->line = sc_lineno;
}

static void emit_str(LexerState *st, const char *s) {
    emit_simple(st, T_STR, s);
}

static void count_newlines(const char *s) {
    for (const char *p = s; *p; p++)
        if (*p == '\n') sc_lineno++;
}

/* sc_kind_is_value: does this token kind end an expression / could a
 * CONCAT validly follow it? */
int sc_kind_is_value(int kind) {
    switch (kind) {
    case T_IDENT:
    case T_FUNCTION:
    case T_INT:
    case T_REAL:
    case T_STR:
    case T_KEYWORD:
    case T_RPAREN:
    case T_RBRACK:
        return 1;
    default:
        return 0;
    }
}

/* ========================================================== */
/* Public API                                                 */
/* ========================================================== */

ScTokenBuf snocone_lex2(const char *source) {
    LexerState st;
    memset(&st, 0, sizeof(st));

    ScTokenBuf buf;
    memset(&buf, 0, sizeof(buf));
    st.buf = &buf;

    sc_lineno = 1;

    yyscan_t scanner;
    sc_lex_init_extra(&st, &scanner);
    YY_BUFFER_STATE ybuf = sc__scan_string(source, scanner);
    sc_lex(scanner);
    sc__delete_buffer(ybuf, scanner);
    sc_lex_destroy(scanner);

    return buf;
}

void sc_token_buf_free(ScTokenBuf *buf) {
    for (int i = 0; i < buf->count; i++) free(buf->tokens[i].text);
    free(buf->tokens);
    buf->tokens = NULL;
    buf->count = 0;
    buf->capacity = 0;
}

const char *sc2_kind_name(int kind) {
    switch (kind) {
    case T_INT:                  return "T_INT";
    case T_REAL:                 return "T_REAL";
    case T_STR:                  return "T_STR";
    case T_IDENT:                return "T_IDENT";
    case T_FUNCTION:             return "T_FUNCTION";
    case T_KEYWORD:              return "T_KEYWORD";
    case T_CONCAT:               return "T_CONCAT";
    case T_ASSIGNMENT:           return "T_ASSIGNMENT";
    case T_MATCH:                return "T_MATCH";
    case T_ALTERNATION:          return "T_ALTERNATION";
    case T_ADDITION:             return "T_ADDITION";
    case T_SUBTRACTION:          return "T_SUBTRACTION";
    case T_MULTIPLICATION:       return "T_MULTIPLICATION";
    case T_DIVISION:             return "T_DIVISION";
    case T_EXPONENTIATION:       return "T_EXPONENTIATION";
    case T_IMMEDIATE_ASSIGN:     return "T_IMMEDIATE_ASSIGN";
    case T_COND_ASSIGN:          return "T_COND_ASSIGN";
    case T_AMPERSAND:            return "T_AMPERSAND";
    case T_AT_SIGN:              return "T_AT_SIGN";
    case T_POUND:                return "T_POUND";
    case T_PERCENT:              return "T_PERCENT";
    case T_TILDE:                return "T_TILDE";
    case T_EQ:                   return "T_EQ";
    case T_NE:                   return "T_NE";
    case T_LT:                   return "T_LT";
    case T_GT:                   return "T_GT";
    case T_LE:                   return "T_LE";
    case T_GE:                   return "T_GE";
    case T_LEQ:                  return "T_LEQ";
    case T_LNE:                  return "T_LNE";
    case T_LLT:                  return "T_LLT";
    case T_LGT:                  return "T_LGT";
    case T_LLE:                  return "T_LLE";
    case T_LGE:                  return "T_LGE";
    case T_IDENT_OP:             return "T_IDENT_OP";
    case T_DIFFER:               return "T_DIFFER";
    case T_PLUS_ASSIGN:          return "T_PLUS_ASSIGN";
    case T_MINUS_ASSIGN:         return "T_MINUS_ASSIGN";
    case T_STAR_ASSIGN:          return "T_STAR_ASSIGN";
    case T_SLASH_ASSIGN:         return "T_SLASH_ASSIGN";
    case T_CARET_ASSIGN:         return "T_CARET_ASSIGN";
    case T_UN_PLUS:              return "T_UN_PLUS";
    case T_UN_MINUS:             return "T_UN_MINUS";
    case T_UN_ASTERISK:          return "T_UN_ASTERISK";
    case T_UN_SLASH:             return "T_UN_SLASH";
    case T_UN_PERCENT:           return "T_UN_PERCENT";
    case T_UN_AT_SIGN:           return "T_UN_AT_SIGN";
    case T_UN_TILDE:             return "T_UN_TILDE";
    case T_UN_DOLLAR_SIGN:       return "T_UN_DOLLAR_SIGN";
    case T_UN_PERIOD:            return "T_UN_PERIOD";
    case T_UN_POUND:             return "T_UN_POUND";
    case T_UN_VERTICAL_BAR:      return "T_UN_VERTICAL_BAR";
    case T_UN_EQUAL:             return "T_UN_EQUAL";
    case T_UN_QUESTION_MARK:     return "T_UN_QUESTION_MARK";
    case T_UN_AMPERSAND:         return "T_UN_AMPERSAND";
    case T_LPAREN:               return "T_LPAREN";
    case T_RPAREN:               return "T_RPAREN";
    case T_LBRACE:               return "T_LBRACE";
    case T_RBRACE:               return "T_RBRACE";
    case T_LBRACK:               return "T_LBRACK";
    case T_RBRACK:               return "T_RBRACK";
    case T_COMMA:                return "T_COMMA";
    case T_SEMICOLON:            return "T_SEMICOLON";
    case T_COLON:                return "T_COLON";
    case T_KW_IF:                return "T_KW_IF";
    case T_KW_ELSE:              return "T_KW_ELSE";
    case T_KW_WHILE:             return "T_KW_WHILE";
    case T_KW_DO:                return "T_KW_DO";
    case T_KW_UNTIL:             return "T_KW_UNTIL";
    case T_KW_FOR:               return "T_KW_FOR";
    case T_KW_SWITCH:            return "T_KW_SWITCH";
    case T_KW_CASE:              return "T_KW_CASE";
    case T_KW_DEFAULT:           return "T_KW_DEFAULT";
    case T_KW_BREAK:             return "T_KW_BREAK";
    case T_KW_CONTINUE:          return "T_KW_CONTINUE";
    case T_KW_GOTO:              return "T_KW_GOTO";
    case T_KW_FUNCTION:          return "T_KW_FUNCTION";
    case T_KW_RETURN:            return "T_KW_RETURN";
    case T_KW_FRETURN:           return "T_KW_FRETURN";
    case T_KW_NRETURN:           return "T_KW_NRETURN";
    case T_KW_STRUCT:            return "T_KW_STRUCT";
    case T_EOF:                  return "T_EOF";
    case T_UNKNOWN:              return "T_UNKNOWN";
    default:                     return "T_???";
    }
}
