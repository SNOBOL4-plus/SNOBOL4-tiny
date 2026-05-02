/*
 * snocone_lex.h -- Snocone threaded-code FSM lexer public API
 *
 * One function: sc_lex_next(ctx) returns the next token kind.
 * Token text payload (for value tokens) lands in ctx->text.
 *
 * Token kinds (T_*) are owned by Bison and live in snocone_parse.tab.h.
 * The lexer's IMPLEMENTATION (snocone_lex.c) includes that header to
 * resolve the T_* names; this header (the lexer's API) deliberately
 * does NOT include it, so callers don't drag the parser's enum around.
 * Token-kind values are returned as plain `int`.  There is exactly one
 * set of names for tokens across the lexer/parser boundary, no aliases,
 * no translation table.  (Session 2026-05-01 cleanup.)
 *
 * No buffer types.  Callers either pull tokens one at a time
 * (Bison's yylex pattern) or accumulate locally if needed.
 */
#ifndef SNOCONE_LEX_H
#define SNOCONE_LEX_H
#include <stddef.h>

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
