/*
 * snocone_lex.h -- Snocone threaded-code FSM lexer public API
 *
 * One function: sc_lex_next(ctx) returns the next token kind.
 * Token text payload (for value tokens) lands in ctx->text.
 * Token kinds (T_*) match snobol4.tab.h naming -- same name
 * everywhere for any concept equivalence (RULES.md / Lon's
 * session-#9 directive).
 *
 * No buffer types.  Callers either pull tokens one at a time
 * (Bison's yylex pattern) or accumulate locally if needed.
 *
 * Renamed from snocone_fsm.h at LS-4.a (session 2026-04-30 #3) — this
 * IS the Snocone lexer now; the old hand-written one moved to
 * one4all/archive/snocone_lex.c when this threaded-code FSM took
 * its place as the standard frontend lexer.  "FSM" remains in the
 * description (it accurately names the implementation technique).
 */
#ifndef SNOCONE_LEX_H
#define SNOCONE_LEX_H
#include <stddef.h>
typedef enum {
    T_INT = 1, T_REAL, T_STR, T_IDENT,
    T_FUNCTION,                                    /* ident immediately followed by '(' */
    T_KEYWORD,                                     /* &IDENT */
    T_CONCAT,                                      /* whitespace between two atoms */
    T_ASSIGNMENT, T_MATCH, T_ALTERNATION,
    T_EQ, T_NE, T_LT, T_GT, T_LE, T_GE,            /* numeric compare pri 6 */
    T_LEQ, T_LNE, T_LLT, T_LGT, T_LLE, T_LGE,      /* string compare :==: etc. */
    T_IDENT_OP,                                    /* ::   pri 6 IDENT() */
    T_DIFFER,                                      /* :!:  pri 6 DIFFER() */
    T_ADDITION, T_SUBTRACTION, T_DIVISION, T_MULTIPLICATION,
    T_EXPONENTIATION,                              /* ^ ** ! pri 11 right */
    T_IMMEDIATE_ASSIGN,                            /* $    pri 12 binary */
    T_COND_ASSIGN,                                 /* .    pri 12 binary */
    T_AMPERSAND, T_AT_SIGN, T_POUND, T_PERCENT, T_TILDE,
    T_PLUS_ASSIGN, T_MINUS_ASSIGN, T_STAR_ASSIGN, T_SLASH_ASSIGN, T_CARET_ASSIGN,
    T_UN_PLUS, T_UN_MINUS, T_UN_ASTERISK, T_UN_SLASH, T_UN_PERCENT,
    T_UN_AT_SIGN, T_UN_TILDE, T_UN_DOLLAR_SIGN, T_UN_PERIOD, T_UN_POUND,
    T_UN_VERTICAL_BAR, T_UN_EQUAL, T_UN_QUESTION_MARK, T_UN_AMPERSAND,
    T_LPAREN, T_RPAREN, T_LBRACK, T_RBRACK, T_LBRACE, T_RBRACE,
    T_COMMA, T_SEMICOLON, T_COLON,
    T_KW_IF, T_KW_ELSE, T_KW_WHILE, T_KW_DO, T_KW_UNTIL, T_KW_FOR,
    T_KW_SWITCH, T_KW_CASE, T_KW_DEFAULT,
    T_KW_BREAK, T_KW_CONTINUE, T_KW_GOTO,
    T_KW_FUNCTION, T_KW_RETURN, T_KW_FRETURN, T_KW_NRETURN,
    T_KW_STRUCT,
    T_EOF, T_UNKNOWN
} ScKind;
typedef struct LexCtx {
    const char *p;                                 /* cursor into source (advances per call) */
    int         line;                              /* 1-based line number */
    int         last_kind;                         /* kind of last emitted token, 0 at start */
    char        text[65536];                       /* payload for the most recent value token */
    char        strbuf[65536];                     /* string-literal accumulation buffer */
    int         strpos;
} LexCtx;
int         sc_lex_next     (LexCtx *ctx);
int         sc_kind_is_value(int kind);
const char *sc2_kind_name   (int kind);
#endif
