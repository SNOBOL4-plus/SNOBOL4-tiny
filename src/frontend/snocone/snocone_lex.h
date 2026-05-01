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
    T_CALL,                                    /* ident immediately followed by '(' */
    T_KEYWORD,                                     /* &IDENT */
    T_CONCAT,                                      /* whitespace between two atoms */
    T_2EQUAL, T_2QUEST, T_2PIPE,
    T_EQ, T_NE, T_LT, T_GT, T_LE, T_GE,            /* numeric compare pri 6 */
    T_LEQ, T_LNE, T_LLT, T_LGT, T_LLE, T_LGE,      /* string compare :==: etc. */
    T_IDENT_OP,                                    /* ::   pri 6 IDENT() */
    T_DIFFER,                                      /* :!:  pri 6 DIFFER() */
    T_2PLUS, T_2MINUS, T_2SLASH, T_2STAR,
    T_2CARET,                              /* ^ ** ! pri 11 right */
    T_2DOLLAR,                            /* $    pri 12 binary */
    T_2DOT,                                 /* .    pri 12 binary */
    T_2AMP, T_2AT, T_2POUND, T_2PERCENT, T_2TILDE,
    T_PLUS_ASSIGN, T_MINUS_ASSIGN, T_STAR_ASSIGN, T_SLASH_ASSIGN, T_CARET_ASSIGN,
    T_1PLUS, T_1MINUS, T_1STAR, T_1SLASH, T_1PERCENT,
    T_1AT, T_1TILDE, T_1DOLLAR, T_1DOT, T_1POUND,
    T_1PIPE, T_1EQUAL, T_1QUEST, T_1AMP,
    T_LPAREN, T_RPAREN, T_LBRACK, T_RBRACK, T_LBRACE, T_RBRACE,
    T_COMMA, T_SEMICOLON, T_COLON,
    T_IF, T_ELSE, T_WHILE, T_DO, T_UNTIL, T_FOR,
    T_SWITCH, T_CASE, T_DEFAULT,
    T_BREAK, T_CONTINUE, T_GOTO,
    T_DEFINE, T_RETURN, T_FRETURN, T_NRETURN,
    T_STRUCT,
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
int         sc_kind_has_payload(int kind);
const char *sc2_kind_name   (int kind);
#endif
