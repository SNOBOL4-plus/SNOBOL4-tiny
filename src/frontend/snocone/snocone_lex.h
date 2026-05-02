/*
 * snocone_lex.h -- Snocone threaded-code FSM lexer public API
 *
 * One function: sc_lex(yylval, st) — Bison's yylex contract directly.
 * For value tokens, payload is strdup'd into yylval->str by emit_value.
 * Token kinds (T_*) are owned by Bison and live in snocone_parse.tab.h.
 *
 * No thunks.  The FSM IS yylex.
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

int         sc_kind_is_value   (int kind);
int         sc_kind_has_payload(int kind);
const char *sc2_kind_name      (int kind);
#endif
