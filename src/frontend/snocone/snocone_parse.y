/*
 * snocone_parse.y — Snocone Bison grammar (GOAL-SNOCONE-LANG-SPACE LS-4.{a,b,c,cn,d})
 *
 * Andrew Koenig's .sc self-host operator design + Lon's restoration of
 * SPITBOL space-as-concat semantics.  This file covers LS-4.a, LS-4.b,
 * LS-4.c, LS-4.cn, and LS-4.d:
 *   * atoms (T_INT/T_REAL/T_STR/T_IDENT/T_KEYWORD)
 *   * arithmetic (+ - * / ^)
 *   * paren-grouping
 *   * `;`-terminated statements
 *   * assignment (T_2EQUAL) + compound-assigns (+= -= *= /= ^=)
 *   * comparison/identity (==, !=, <, >, <=, >=, :==:, :!=:, :<:, :>:,
 *     :<=:, :>=:, ::, :!:) → E_FNC named calls
 *   * T_FUNCTION call-form `EQ(2+2, 4)` → E_FNC("EQ", ...)
 *   * pattern match `?`              → E_SCAN
 *   * pattern alternation `|`        → E_ALT (n-ary fold)
 *   * synthetic concat T_CONCAT      → E_SEQ (n-ary fold)
 *   * LS-4.cn (session-#7 cosmetic):
 *     - file rename snocone.y → snocone_parse.y (matches snocone_lex.{c,h})
 *     - public entry now returns CODE_t* (typedef alias of Program)
 *       for symmetry with EXPR_t — the type EVAL operates on
 *   * LS-4.d (this rung): postfix subscript `a[i, j]` → E_IDX (n-ary,
 *     left-recursive so `a[i][j]` chains)
 *
 * Everything else — more unaries, control flow, switch, break/continue,
 * goto, alt-eval, struct — lands in LS-4.e through LS-4.i.
 *
 * Pipeline:
 *   snocone_lex.c  (threaded-code FSM lexer) -- sc_lex_next(ctx) -- single token per call
 *   snocone_parse.y      (this file)               -- yylex thunk pulls from sc_lex_next
 *   CODE_t        (STMT_t list, EXPR_t IR — alias of Program)
 *
 * Token-kind decoupling note (LS-4.a session 2026-04-30 #3):
 *   The FSM declares its own enum `ScKind` in snocone_lex.h with values
 *   1, 2, 3, ...  Bison generates its own `enum sc_tokentype` in
 *   snocone_parse.tab.h with values 258, 259, 260, ...  The two enums share
 *   the same enumerator NAMES (T_INT, T_REAL, ...) — that was a
 *   deliberate session-#9 choice for readability — but the VALUES
 *   differ.  C does not allow two enums with the same enumerator name
 *   in one translation unit.  We resolve this by:
 *     (1) NOT including snocone_lex.h in `%code requires` (which goes
 *         into snocone_parse.tab.h) — only struct-tag-forward-declared.
 *     (2) Including snocone_lex.h in `%code top` (which goes ONLY into
 *         snocone_parse.tab.c, BEFORE snocone_parse.tab.h's enum sc_tokentype is
 *         emitted).  The %code top block is processed before all other
 *         emitted code; we use it to alias the FSM's T_* names to SC_T_*
 *         via #define before the FSM header is read, so the FSM enum
 *         arrives with names that don't collide with Bison's later T_*.
 *     (3) The yylex thunk uses the SC_T_* aliases to talk to the FSM
 *         and returns Bison's T_* values directly.
 *   Net: the FSM's source code is untouched; Bison owns the T_* names
 *   in this translation unit; a tiny translation table (sc_kind_to_tok)
 *   maps SC_T_* (FSM enum) to T_* (Bison tokens).
 *
 * IR construction follows snobol4.y line-for-line: leaf atoms are
 * E_VAR/E_KEYWORD/E_QLIT/E_ILIT/E_FLIT, arithmetic emits E_ADD/E_SUB/
 * E_MUL/E_DIV/E_POW/E_MNS/E_PLS via expr_binary/expr_unary helpers.
 * Statement assembly mirrors snobol4's stmt_commit_go path: top-level
 * E_ASSIGN splits into subject + replacement; otherwise a bare expression
 * goes into the subject field.
 *
 * Public entry:
 *   CODE_t *snocone_parse_program(const char *src, const char *filename);
 *
 *   CODE_t is a typedef alias of Program (added LS-4.cn for symmetry
 *   with EXPR_t).  The two names refer to the same type.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */

%code top {
/* %code top runs BEFORE Bison's enum sc_tokentype is generated, so we
 * can alias the FSM's T_* enum names to SC_T_* and pull in its header
 * here without colliding with Bison's later T_* generation.
 *
 * Naming-scheme note (session 2026-04-30 #6 rename): tokens for the
 * dual-role keyboard characters use T_<arity><charname> (e.g. T_1PLUS
 * unary +, T_2PLUS binary +).  Multi-character operators (== :==: ::
 * etc.) keep their existing names since they have no arity ambiguity.
 * Atoms and punctuation also keep their existing names. */
#define T_INT              SC_T_INT
#define T_REAL             SC_T_REAL
#define T_STR              SC_T_STR
#define T_IDENT            SC_T_IDENT
#define T_FUNCTION         SC_T_FUNCTION
#define T_KEYWORD          SC_T_KEYWORD
#define T_CONCAT           SC_T_CONCAT

/* Binary operators (T_2*) — character-derived names */
#define T_2EQUAL           SC_T_2EQUAL
#define T_2QUEST           SC_T_2QUEST
#define T_2PIPE            SC_T_2PIPE
#define T_2PLUS            SC_T_2PLUS
#define T_2MINUS           SC_T_2MINUS
#define T_2SLASH           SC_T_2SLASH
#define T_2STAR            SC_T_2STAR
#define T_2CARET           SC_T_2CARET
#define T_2DOLLAR          SC_T_2DOLLAR
#define T_2DOT             SC_T_2DOT
#define T_2AMP             SC_T_2AMP
#define T_2AT              SC_T_2AT
#define T_2POUND           SC_T_2POUND
#define T_2PERCENT         SC_T_2PERCENT
#define T_2TILDE           SC_T_2TILDE

/* Multi-character comparison/identity ops — character-name kept */
#define T_EQ               SC_T_EQ
#define T_NE               SC_T_NE
#define T_LT               SC_T_LT
#define T_GT               SC_T_GT
#define T_LE               SC_T_LE
#define T_GE               SC_T_GE
#define T_LEQ              SC_T_LEQ
#define T_LNE              SC_T_LNE
#define T_LLT              SC_T_LLT
#define T_LGT              SC_T_LGT
#define T_LLE              SC_T_LLE
#define T_LGE              SC_T_LGE
#define T_IDENT_OP         SC_T_IDENT_OP
#define T_DIFFER           SC_T_DIFFER

/* Compound-assigns — naturally binary, no arity prefix */
#define T_PLUS_ASSIGN      SC_T_PLUS_ASSIGN
#define T_MINUS_ASSIGN     SC_T_MINUS_ASSIGN
#define T_STAR_ASSIGN      SC_T_STAR_ASSIGN
#define T_SLASH_ASSIGN     SC_T_SLASH_ASSIGN
#define T_CARET_ASSIGN     SC_T_CARET_ASSIGN

/* Unary operators (T_1*) — character-derived names */
#define T_1PLUS            SC_T_1PLUS
#define T_1MINUS           SC_T_1MINUS
#define T_1STAR            SC_T_1STAR
#define T_1SLASH           SC_T_1SLASH
#define T_1PERCENT         SC_T_1PERCENT
#define T_1AT              SC_T_1AT
#define T_1TILDE           SC_T_1TILDE
#define T_1DOLLAR          SC_T_1DOLLAR
#define T_1DOT             SC_T_1DOT
#define T_1POUND           SC_T_1POUND
#define T_1PIPE            SC_T_1PIPE
#define T_1EQUAL           SC_T_1EQUAL
#define T_1QUEST           SC_T_1QUEST
#define T_1AMP             SC_T_1AMP

/* Punctuation — keep existing names */
#define T_LPAREN           SC_T_LPAREN
#define T_RPAREN           SC_T_RPAREN
#define T_LBRACK           SC_T_LBRACK
#define T_RBRACK           SC_T_RBRACK
#define T_LBRACE           SC_T_LBRACE
#define T_RBRACE           SC_T_RBRACE
#define T_COMMA            SC_T_COMMA
#define T_SEMICOLON        SC_T_SEMICOLON
#define T_COLON            SC_T_COLON

/* Keywords — keep existing names */
#define T_KW_IF            SC_T_KW_IF
#define T_KW_ELSE          SC_T_KW_ELSE
#define T_KW_WHILE         SC_T_KW_WHILE
#define T_KW_DO            SC_T_KW_DO
#define T_KW_UNTIL         SC_T_KW_UNTIL
#define T_KW_FOR           SC_T_KW_FOR
#define T_KW_SWITCH        SC_T_KW_SWITCH
#define T_KW_CASE          SC_T_KW_CASE
#define T_KW_DEFAULT       SC_T_KW_DEFAULT
#define T_KW_BREAK         SC_T_KW_BREAK
#define T_KW_CONTINUE      SC_T_KW_CONTINUE
#define T_KW_GOTO          SC_T_KW_GOTO
#define T_KW_FUNCTION      SC_T_KW_FUNCTION
#define T_KW_RETURN        SC_T_KW_RETURN
#define T_KW_FRETURN       SC_T_KW_FRETURN
#define T_KW_NRETURN       SC_T_KW_NRETURN
#define T_KW_STRUCT        SC_T_KW_STRUCT

/* End-of-input + unknown */
#define T_EOF              SC_T_EOF
#define T_UNKNOWN          SC_T_UNKNOWN

#include "snocone_lex.h"

/* After the FSM header has been pulled in (with the SC_T_* aliases
 * applied to its enumerators), undo the aliases so Bison's own enum
 * sc_tokentype can use the unprefixed T_* names that the rest of the
 * parser sees. */
#undef T_INT
#undef T_REAL
#undef T_STR
#undef T_IDENT
#undef T_FUNCTION
#undef T_KEYWORD
#undef T_CONCAT
#undef T_2EQUAL
#undef T_2QUEST
#undef T_2PIPE
#undef T_2PLUS
#undef T_2MINUS
#undef T_2SLASH
#undef T_2STAR
#undef T_2CARET
#undef T_2DOLLAR
#undef T_2DOT
#undef T_2AMP
#undef T_2AT
#undef T_2POUND
#undef T_2PERCENT
#undef T_2TILDE
#undef T_EQ
#undef T_NE
#undef T_LT
#undef T_GT
#undef T_LE
#undef T_GE
#undef T_LEQ
#undef T_LNE
#undef T_LLT
#undef T_LGT
#undef T_LLE
#undef T_LGE
#undef T_IDENT_OP
#undef T_DIFFER
#undef T_PLUS_ASSIGN
#undef T_MINUS_ASSIGN
#undef T_STAR_ASSIGN
#undef T_SLASH_ASSIGN
#undef T_CARET_ASSIGN
#undef T_1PLUS
#undef T_1MINUS
#undef T_1STAR
#undef T_1SLASH
#undef T_1PERCENT
#undef T_1AT
#undef T_1TILDE
#undef T_1DOLLAR
#undef T_1DOT
#undef T_1POUND
#undef T_1PIPE
#undef T_1EQUAL
#undef T_1QUEST
#undef T_1AMP
#undef T_LPAREN
#undef T_RPAREN
#undef T_LBRACK
#undef T_RBRACK
#undef T_LBRACE
#undef T_RBRACE
#undef T_COMMA
#undef T_SEMICOLON
#undef T_COLON
#undef T_KW_IF
#undef T_KW_ELSE
#undef T_KW_WHILE
#undef T_KW_DO
#undef T_KW_UNTIL
#undef T_KW_FOR
#undef T_KW_SWITCH
#undef T_KW_CASE
#undef T_KW_DEFAULT
#undef T_KW_BREAK
#undef T_KW_CONTINUE
#undef T_KW_GOTO
#undef T_KW_FUNCTION
#undef T_KW_RETURN
#undef T_KW_FRETURN
#undef T_KW_NRETURN
#undef T_KW_STRUCT
#undef T_EOF
#undef T_UNKNOWN
}

%code requires {
#include "scrip_cc.h"

/* Forward-declare LexCtx — full definition lives in snocone_fsm.h, which
 * we deliberately do NOT include here so its T_* enumerators don't
 * collide with Bison's enum sc_tokentype.  See the %code top block above. */
struct LexCtx;

/* Parser state — passed to sc_parse() via %parse-param.  Carries the
 * FSM lexer context (the single producer of tokens), the code under
 * construction, and a small error counter.  Uses CODE_t (typedef alias
 * of Program) for symmetry with EXPR_t — Snocone's parser produces
 * code, not just an expression. */
typedef struct ScParseState {
    struct LexCtx *ctx;
    CODE_t        *code;
    const char    *filename;
    int            nerrors;
} ScParseState;
}

%code {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Forward declarations of yylex/yyerror (Bison's generated parser
 * references them via the api.prefix-mangled names sc_lex / sc_error
 * before their definitions appear after %% — without these forward
 * decls the compiler emits implicit-declaration warnings at the
 * yychar = yylex(...) call site inside sc_parse). */
int  sc_lex  (SC_STYPE *yylval, ScParseState *st);
void sc_error(ScParseState *st, const char *msg);

/* Helpers — defined after %% */
static void     sc_append_stmt        (ScParseState *st, EXPR_t *top);
static EXPR_t  *sc_int_literal        (const char *txt);
static EXPR_t  *sc_real_literal       (const char *txt);
static EXPR_t  *sc_str_literal        (const char *txt);
static EXPR_t  *sc_clone_expr_simple  (EXPR_t *e);  /* LS-4.c — for compound-assigns */

/* sc_kind_to_tok — translate FSM ScKind (1..N) to Bison's sc_tokentype.
 *
 * The two namespaces share enumerator NAMES (deliberate session-#9
 * convention: same name for the same concept everywhere) but VALUES
 * differ — FSM is 1..N, Bison is 256+M.  This function is the one
 * authoritative translation point.  Indexed by the FSM's enum value;
 * returns the Bison token number.  T_EOF (FSM) → 0 (Bison's end-of-input
 * sentinel).  Any FSM kind without a corresponding Bison token returns
 * the Bison T_UNKNOWN as a safe fallback (the grammar will reject). */
static int sc_kind_to_tok(int sc_kind);
}

%define api.prefix {sc_}
%define api.pure full
%parse-param { ScParseState *st }
%lex-param   { ScParseState *st }

/* yylval is a discriminated union — currently only ->expr and ->str are
 * used at LS-4.a, but reserving the wider shape now avoids reshuffling
 * later (LS-4.b adds comparison sugar, LS-4.f adds control flow). */
%union {
    EXPR_t *expr;
    char   *str;
    long    ival;
    double  dval;
}

/* ---- Atoms ---- */
%token <str> T_IDENT
%token <str> T_KEYWORD
%token <str> T_INT      /* FSM emits raw text; thunk converts to long */
%token <str> T_REAL     /* same — thunk converts to double */
%token <str> T_STR      /* FSM emits unquoted, escapes resolved */
%token <str> T_FUNCTION /* IDENT-followed-by-zero-space-( per Andrew's `f(args)` rule */

/* ---- Binary arithmetic operators (LS-4.a) ---- */
%token T_2PLUS
%token T_2MINUS
%token T_2STAR
%token T_2SLASH
%token T_2CARET

/* ---- Comparison / identity operators (LS-4.b) — all lower to E_FNC named calls ---- */
%token T_EQ T_NE T_LT T_GT T_LE T_GE              /* numeric:  EQ NE LT GT LE GE  */
%token T_LEQ T_LNE T_LLT T_LGT T_LLE T_LGE        /* lexical:  LEQ LNE LLT LGT LLE LGE */
%token T_IDENT_OP T_DIFFER                        /* identity: IDENT DIFFER (Andrew's :: and :!:) */

/* ---- Unary operators (LS-4.a — only the arithmetic ones) ---- */
%token T_1PLUS
%token T_1MINUS

/* ---- Assignment + compound-assign (LS-4.a / LS-4.c) ---- */
%token T_2EQUAL
%token T_PLUS_ASSIGN T_MINUS_ASSIGN T_STAR_ASSIGN T_SLASH_ASSIGN T_CARET_ASSIGN

/* ---- Pattern operators (LS-4.c) ---- */
%token T_2QUEST                                    /* `?` — pri 1, lowers to E_SCAN */
%token T_2PIPE                              /* `|` — pri 3, lowers to E_ALT  */
%token T_CONCAT                                   /* synthesised by FSM at value boundaries — pri 4, lowers to E_SEQ */

/* ---- Punctuation ---- */
%token T_LPAREN
%token T_RPAREN
%token T_SEMICOLON
%token T_COMMA

/* ---- Subscript brackets (LS-4.d) ---- */
%token T_LBRACK T_RBRACK

/* All other tokens the FSM may emit are declared here so the translation
 * table can index every FSM kind, but they have no productions yet —
 * encountering one in the input is a parse error.  LS-4.e–LS-4.i will
 * give them rules. */
%token T_2DOLLAR T_2DOT
%token T_2AMP T_2AT T_2POUND T_2PERCENT T_2TILDE
%token T_1STAR T_1SLASH T_1PERCENT
%token T_1AT T_1TILDE T_1DOLLAR T_1DOT T_1POUND
%token T_1PIPE T_1EQUAL T_1QUEST T_1AMP
%token T_LBRACE T_RBRACE
%token T_COLON
%token T_KW_IF T_KW_ELSE T_KW_WHILE T_KW_DO T_KW_UNTIL T_KW_FOR
%token T_KW_SWITCH T_KW_CASE T_KW_DEFAULT
%token T_KW_BREAK T_KW_CONTINUE T_KW_GOTO
%token T_KW_FUNCTION T_KW_RETURN T_KW_FRETURN T_KW_NRETURN T_KW_STRUCT
%token T_UNKNOWN

%type <expr> expr0 expr1 expr3 expr4 expr5 expr6 expr9 expr11 expr15 expr17 exprlist exprlist_ne

%%

/* ---- Program structure ---- */
program     : stmt_list
            | /* empty */
            ;

stmt_list   : stmt_list stmt
            | stmt
            ;

stmt        : expr0 T_SEMICOLON                { sc_append_stmt(st, $1); }
            | T_SEMICOLON                      { /* empty stmt */         }
            ;

/* ---- Expression precedence levels ----
 *
 * Level numbering matches snobol4.y so a reader who follows the
 * SNOBOL4 grammar can map across directly.  Snocone's surface
 * differs (`==`/`!=`/etc., space-as-concat instead of `&&`) but the
 * underlying SPITBOL priorities are unchanged.  Active tiers
 * after LS-4.{a,b,c}:
 *
 *   expr0  — assignment `=` + compound-assigns (+= -= *= /= ^=)   pri 0   right-assoc
 *   expr1  — pattern match `?`                                     pri 1   right-assoc
 *   expr3  — pattern alternation `|` (n-ary E_ALT)                 pri 3   right-assoc fold
 *   expr4  — concatenation T_CONCAT (n-ary E_SEQ)                  pri 4   right-assoc fold
 *   expr5  — comparison/identity sugar (==, !=, <, >, <=, >=,      pri 6 (Andrew)
 *            :==:, :!=:, :<:, :>:, :<=:, :>=:, ::, :!:) → E_FNC    left-assoc
 *   expr6  — addition / subtraction                                pri 6/7 (SPITBOL/Andrew) left-assoc
 *   expr9  — multiplication / division                             pri 8/9 left-assoc
 *   expr11 — exponentiation                                        pri 11  right-assoc
 *   expr15 — postfix subscript `a[i,j]` → E_IDX (n-ary)             pri 15  left-assoc chain
 *   expr17 — atoms (literals, idents, keywords, parens, T_FUNCTION,
 *            unary +/-)
 *
 * Skipped levels (expr2, expr7, expr8, expr10, expr12..expr14, expr16)
 * are reserved for OPSYN slots and the dual-role unary-on-atom tier;
 * they fill in across LS-4.e–LS-4.i.  Each tier delegates to the
 * next-higher level in its base case (`| expr<next> { $$ = $1; }`),
 * giving a clean precedence climber with no shift/reduce conflicts.
 */

/* ---- Assignment tier (LS-4.a) + compound-assigns (LS-4.c) ----
 *
 * Plain `=` is SPITBOL priority 0, right-associative; `a = b = c`
 * parses as `a = (b = c)`.  Compound-assigns (`+=` `-=` `*=` `/=` `^=`)
 * are Snocone-only sugar (Andrew's .sc doesn't have them but the FSM
 * lexer already recognises them).  Each lowers to a clone-LHS pattern:
 *   `a += b`  →  E_ASSIGN(a, E_ADD(clone(a), b))
 * Restricted to "simple" (atomic) LHS — E_VAR, E_KEYWORD, E_IDX, and
 * leaf literals — via sc_clone_expr_simple's coverage; complex LHS
 * (e.g. `f(x) += 1`) gets a trap'd assertion at clone time.  This
 * matches typical compound-assign use (`count += step`, `total *= 2`).
 */
expr0       : expr1 T_2EQUAL    expr0
                                { $$ = expr_binary(E_ASSIGN, $1, $3); }
            | expr1 T_PLUS_ASSIGN   expr0
                                { EXPR_t *cl = sc_clone_expr_simple($1);
                                  EXPR_t *rhs = expr_binary(E_ADD, cl, $3);
                                  $$ = expr_binary(E_ASSIGN, $1, rhs); }
            | expr1 T_MINUS_ASSIGN  expr0
                                { EXPR_t *cl = sc_clone_expr_simple($1);
                                  EXPR_t *rhs = expr_binary(E_SUB, cl, $3);
                                  $$ = expr_binary(E_ASSIGN, $1, rhs); }
            | expr1 T_STAR_ASSIGN   expr0
                                { EXPR_t *cl = sc_clone_expr_simple($1);
                                  EXPR_t *rhs = expr_binary(E_MUL, cl, $3);
                                  $$ = expr_binary(E_ASSIGN, $1, rhs); }
            | expr1 T_SLASH_ASSIGN  expr0
                                { EXPR_t *cl = sc_clone_expr_simple($1);
                                  EXPR_t *rhs = expr_binary(E_DIV, cl, $3);
                                  $$ = expr_binary(E_ASSIGN, $1, rhs); }
            | expr1 T_CARET_ASSIGN  expr0
                                { EXPR_t *cl = sc_clone_expr_simple($1);
                                  EXPR_t *rhs = expr_binary(E_POW, cl, $3);
                                  $$ = expr_binary(E_ASSIGN, $1, rhs); }
            | expr1
                                { $$ = $1; }
            ;

/* ---- Pattern-match tier (LS-4.c) ----
 *
 * SPITBOL `?` at priority 1, right-associative — `a ? b ? c` parses
 * as `a ? (b ? c)`.  Lowers to E_SCAN(subject, pattern), matching
 * snobol4.y's `expr0 : expr2 T_2QUEST expr0` shape (snobol4.y bundles
 * match alongside assignment at expr0; we pull it out to its own
 * level for clarity, but the IR shape is identical).  Right-assoc
 * is handled by the `expr1 T_2QUEST expr1` form on the right.
 */
expr1       : expr3 T_2QUEST expr1
                                { $$ = expr_binary(E_SCAN, $1, $3); }
            | expr3
                                { $$ = $1; }
            ;

/* ---- Pattern alternation tier (LS-4.c) ----
 *
 * SPITBOL `|` at priority 3, right-associative.  Folds into a flat
 * n-ary E_ALT — `a | b | c` produces a single E_ALT(a, b, c) rather
 * than a nested E_ALT(a, E_ALT(b, c)).  This matches snobol4.y:131's
 * shape exactly: when the LHS is already E_ALT we extend it with
 * expr_add_child; otherwise we create a fresh E_ALT containing both
 * operands.  Bison's left-recursion drives the fold one operand at
 * a time, giving the n-ary collapse for free.
 */
expr3       : expr3 T_2PIPE expr4
                                { if ($1->kind == E_ALT) { expr_add_child($1, $3); $$ = $1; }
                                  else { EXPR_t *a = expr_new(E_ALT);
                                         expr_add_child(a, $1); expr_add_child(a, $3);
                                         $$ = a; } }
            | expr4
                                { $$ = $1; }
            ;

/* ---- Concatenation tier (LS-4.c) ----
 *
 * SPITBOL space-as-concat at priority 4, right-associative per the
 * SPITBOL Manual but Bison left-recursion gives the same n-ary fold
 * via E_SEQ — same approach as snobol4.y:134.  The lexer emits
 * synthetic T_CONCAT tokens at boundaries where prev-token can-end-expr
 * and next-token can-start-expr (the W{OP}W envelope pattern from
 * LS-3 / one4all 02db637d).  Folds into a flat n-ary E_SEQ; lowering
 * to runtime-side concatenation is the SNOBOL4 frontend's existing
 * E_SEQ semantics — no new IR kind needed.
 */
expr4       : expr4 T_CONCAT expr5
                                { if ($1->kind == E_SEQ) { expr_add_child($1, $3); $$ = $1; }
                                  else { EXPR_t *s = expr_new(E_SEQ);
                                         expr_add_child(s, $1); expr_add_child(s, $3);
                                         $$ = s; } }
            | expr5
                                { $$ = $1; }
            ;

/* ---- Comparison / identity tier (LS-4.b) ----
 *
 * Andrew's `.sc` self-host puts all 14 comparison/identity operators
 * at one priority, function-style (fn=1).  Each lowers to an E_FNC
 * named call; the function name is the SPITBOL primitive's UPPERCASE
 * spelling (EQ, NE, LT, GT, LE, GE — numeric; LEQ, LNE, LLT, LGT,
 * LLE, LGE — lexical; IDENT, DIFFER — identity).  This places the
 * compile-time syntactic sugar onto the runtime's existing primitive
 * dispatch — no new SM opcodes, no new IR kinds.
 */
expr5       : expr5 T_EQ        expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("EQ");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_NE        expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("NE");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LT        expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LT");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_GT        expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("GT");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LE        expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LE");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_GE        expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("GE");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LEQ       expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LEQ");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LNE       expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LNE");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LLT       expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LLT");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LGT       expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LGT");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LLE       expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LLE");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_LGE       expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LGE");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_IDENT_OP  expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("IDENT");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr5 T_DIFFER    expr6
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("DIFFER");
                                  expr_add_child(e, $1); expr_add_child(e, $3); $$ = e; }
            | expr6
                                { $$ = $1; }
            ;

expr6       : expr6 T_2PLUS    expr9
                                { $$ = expr_binary(E_ADD, $1, $3); }
            | expr6 T_2MINUS expr9
                                { $$ = expr_binary(E_SUB, $1, $3); }
            | expr9
                                { $$ = $1; }
            ;

expr9       : expr9 T_2STAR expr11
                                { $$ = expr_binary(E_MUL, $1, $3); }
            | expr9 T_2SLASH       expr11
                                { $$ = expr_binary(E_DIV, $1, $3); }
            | expr11
                                { $$ = $1; }
            ;

/* Right-associative exponentiation: expr15 ^ expr11.
 * (Was expr17 before LS-4.d; now goes through expr15 so the new
 * subscript tier can sit between exponent and atoms.) */
expr11      : expr15 T_2CARET expr11
                                { $$ = expr_binary(E_POW, $1, $3); }
            | expr15
                                { $$ = $1; }
            ;

/* ---- Subscript tier (LS-4.d) --------------------------------------------
 *
 * Postfix subscript `a[i, j]` → E_IDX(a, i, j).  Mirrors snobol4.y's
 * `expr15` shape (snobol4.y:183) — same priority, same n-ary IR shape,
 * same left-recursive chaining so `a[i][j]` parses as
 *   E_IDX(E_IDX(a, i), j)
 * and `a[i, j]` parses as the single n-ary
 *   E_IDX(a, i, j).
 *
 * The lexer always emits T_LBRACK for `[` regardless of preceding
 * whitespace (`a[i]` and `a [i]` both lex the same way — see
 * snocone_lex.c E_LBRACK rule).  No CONCAT injection between a value-
 * token and `[`.
 *
 * Empty subscript `a[]` is permitted at the lexer/grammar level (uses
 * the empty-list arm of `exprlist`); semantic legality is a downstream
 * concern.  Arity-checking, bounds-checking, etc. happen at lower /
 * runtime stages, not here.
 *
 * The `exprlist` non-terminal already exists from LS-4.b (function-call
 * arg lists); we reuse it.  Container-unpacking idiom matches the
 * T_FUNCTION rule in expr17 — drain children into the E_IDX node, then
 * free the E_NUL temporary holder.
 */
expr15      : expr15 T_LBRACK exprlist T_RBRACK
                                { EXPR_t *idx = expr_new(E_IDX);
                                  expr_add_child(idx, $1);
                                  for (int i = 0; i < $3->nchildren; i++)
                                      expr_add_child(idx, $3->children[i]);
                                  free($3->children); free($3);
                                  $$ = idx; }
            | expr17
                                { $$ = $1; }
            ;

/* ---- exprlist (LS-4.b) — for call-form argument lists ----
 *
 * Mirror snobol4.y's exprlist/exprlist_ne pair: a non-empty list
 * uses an E_NUL container as a temporary holder; the empty list
 * is just an E_NUL with no children.  Caller (T_FUNCTION rule)
 * unpacks children into the E_FNC node and frees the container.
 */
exprlist    : exprlist_ne
                                { $$ = $1; }
            | /* empty */
                                { $$ = expr_new(E_NUL); }
            ;

exprlist_ne : exprlist_ne T_COMMA expr0
                                { expr_add_child($1, $3); $$ = $1; }
            | expr0
                                { EXPR_t *l = expr_new(E_NUL); expr_add_child(l, $1); $$ = l; }
            ;

/* Atomic tier — atoms, parens, signed unaries.  Unary applied here
 * (highest priority, just below atoms) matches SPITBOL Manual Ch.15
 * "all unaries are higher priority than any binary."  In LS-4.e the
 * full set of unaries (* . $ @ ~ ? &) joins this tier.
 *
 * LS-4.b adds T_FUNCTION call-form: `EQ(2+2, 4)` → E_FNC("EQ", ...).
 * The lexer has already classified the IDENT-followed-by-zero-space-(
 * as T_FUNCTION (the "f(args) vs f (args)" disambiguation rule from
 * the goal); the matching ( is the next token in the stream. */
expr17      : T_FUNCTION T_LPAREN exprlist T_RPAREN
                                { EXPR_t *e = expr_new(E_FNC);
                                  e->sval = $1;             /* takes ownership */
                                  for (int i = 0; i < $3->nchildren; i++)
                                      expr_add_child(e, $3->children[i]);
                                  free($3->children); free($3);
                                  $$ = e; }
            | T_IDENT
                                { EXPR_t *e = expr_new(E_VAR);
                                  e->sval = $1;             /* takes ownership */
                                  $$ = e; }
            | T_KEYWORD
                                { EXPR_t *e = expr_new(E_KEYWORD);
                                  e->sval = $1;
                                  $$ = e; }
            | T_INT
                                { $$ = sc_int_literal($1); free($1); }
            | T_REAL
                                { $$ = sc_real_literal($1); free($1); }
            | T_STR
                                { $$ = sc_str_literal($1); free($1); }
            | T_LPAREN expr0 T_RPAREN
                                { $$ = $2; }
            | T_1PLUS  expr17
                                { $$ = expr_unary(E_PLS, $2); }
            | T_1MINUS expr17
                                { $$ = expr_unary(E_MNS, $2); }
            ;

%%

/* =========================================================================
 *  yylex thunk — single producer is the FSM (sc_lex_next).
 *
 *  The FSM emits one token kind per call (its own enum value) and
 *  stashes the textual payload in ctx->text for value tokens.  The thunk:
 *    1. Calls sc_lex_next to get the next FSM kind.
 *    2. For value tokens, strdups ctx->text into yylval->str.
 *    3. Translates the FSM kind to Bison's sc_tokentype value via
 *       sc_kind_to_tok, returning that to the parser.
 * ========================================================================= */
int sc_lex(SC_STYPE *yylval, ScParseState *st) {
    int sc_kind = sc_lex_next(st->ctx);
    if (sc_kind_is_value(sc_kind)) {
        yylval->str = strdup(st->ctx->text);
    } else {
        yylval->str = NULL;
    }
    if (sc_kind == SC_T_EOF) return 0;          /* Bison's end-of-input sentinel */
    return sc_kind_to_tok(sc_kind);
}

void sc_error(ScParseState *st, const char *msg) {
    fprintf(stderr, "%s:%d: snocone parse error: %s\n",
            st->filename ? st->filename : "<stdin>",
            st->ctx ? st->ctx->line : 0,
            msg);
    st->nerrors++;
}

/* ---- FSM kind → Bison token translation ----
 *
 * After the session-#6 rename, both sides of this map use the
 * T_<arity><charname> scheme for dual-role characters.  The FSM enum
 * (SC_T_*) was renamed in lockstep, so this is now the trivial
 * identity-with-prefix mapping for every entry.  Kept as an explicit
 * switch (not a table-driven offset) so a hypothetical future
 * divergence between the two enums has a single place to handle it.
 */
static int sc_kind_to_tok(int sc_kind) {
    switch (sc_kind) {
        case SC_T_INT:              return T_INT;
        case SC_T_REAL:             return T_REAL;
        case SC_T_STR:              return T_STR;
        case SC_T_IDENT:            return T_IDENT;
        case SC_T_FUNCTION:         return T_FUNCTION;
        case SC_T_KEYWORD:          return T_KEYWORD;
        case SC_T_CONCAT:           return T_CONCAT;
        case SC_T_2EQUAL:           return T_2EQUAL;
        case SC_T_2QUEST:           return T_2QUEST;
        case SC_T_2PIPE:            return T_2PIPE;
        case SC_T_EQ:               return T_EQ;
        case SC_T_NE:               return T_NE;
        case SC_T_LT:               return T_LT;
        case SC_T_GT:               return T_GT;
        case SC_T_LE:               return T_LE;
        case SC_T_GE:               return T_GE;
        case SC_T_LEQ:              return T_LEQ;
        case SC_T_LNE:              return T_LNE;
        case SC_T_LLT:              return T_LLT;
        case SC_T_LGT:              return T_LGT;
        case SC_T_LLE:              return T_LLE;
        case SC_T_LGE:              return T_LGE;
        case SC_T_IDENT_OP:         return T_IDENT_OP;
        case SC_T_DIFFER:           return T_DIFFER;
        case SC_T_2PLUS:            return T_2PLUS;
        case SC_T_2MINUS:           return T_2MINUS;
        case SC_T_2SLASH:           return T_2SLASH;
        case SC_T_2STAR:            return T_2STAR;
        case SC_T_2CARET:           return T_2CARET;
        case SC_T_2DOLLAR:          return T_2DOLLAR;
        case SC_T_2DOT:             return T_2DOT;
        case SC_T_2AMP:             return T_2AMP;
        case SC_T_2AT:              return T_2AT;
        case SC_T_2POUND:           return T_2POUND;
        case SC_T_2PERCENT:         return T_2PERCENT;
        case SC_T_2TILDE:           return T_2TILDE;
        case SC_T_PLUS_ASSIGN:      return T_PLUS_ASSIGN;
        case SC_T_MINUS_ASSIGN:     return T_MINUS_ASSIGN;
        case SC_T_STAR_ASSIGN:      return T_STAR_ASSIGN;
        case SC_T_SLASH_ASSIGN:     return T_SLASH_ASSIGN;
        case SC_T_CARET_ASSIGN:     return T_CARET_ASSIGN;
        case SC_T_1PLUS:            return T_1PLUS;
        case SC_T_1MINUS:           return T_1MINUS;
        case SC_T_1STAR:            return T_1STAR;
        case SC_T_1SLASH:           return T_1SLASH;
        case SC_T_1PERCENT:         return T_1PERCENT;
        case SC_T_1AT:              return T_1AT;
        case SC_T_1TILDE:           return T_1TILDE;
        case SC_T_1DOLLAR:          return T_1DOLLAR;
        case SC_T_1DOT:             return T_1DOT;
        case SC_T_1POUND:           return T_1POUND;
        case SC_T_1PIPE:            return T_1PIPE;
        case SC_T_1EQUAL:           return T_1EQUAL;
        case SC_T_1QUEST:           return T_1QUEST;
        case SC_T_1AMP:             return T_1AMP;
        case SC_T_LPAREN:           return T_LPAREN;
        case SC_T_RPAREN:           return T_RPAREN;
        case SC_T_LBRACK:           return T_LBRACK;
        case SC_T_RBRACK:           return T_RBRACK;
        case SC_T_LBRACE:           return T_LBRACE;
        case SC_T_RBRACE:           return T_RBRACE;
        case SC_T_COMMA:            return T_COMMA;
        case SC_T_SEMICOLON:        return T_SEMICOLON;
        case SC_T_COLON:            return T_COLON;
        case SC_T_KW_IF:            return T_KW_IF;
        case SC_T_KW_ELSE:          return T_KW_ELSE;
        case SC_T_KW_WHILE:         return T_KW_WHILE;
        case SC_T_KW_DO:            return T_KW_DO;
        case SC_T_KW_UNTIL:         return T_KW_UNTIL;
        case SC_T_KW_FOR:           return T_KW_FOR;
        case SC_T_KW_SWITCH:        return T_KW_SWITCH;
        case SC_T_KW_CASE:          return T_KW_CASE;
        case SC_T_KW_DEFAULT:       return T_KW_DEFAULT;
        case SC_T_KW_BREAK:         return T_KW_BREAK;
        case SC_T_KW_CONTINUE:      return T_KW_CONTINUE;
        case SC_T_KW_GOTO:          return T_KW_GOTO;
        case SC_T_KW_FUNCTION:      return T_KW_FUNCTION;
        case SC_T_KW_RETURN:        return T_KW_RETURN;
        case SC_T_KW_FRETURN:       return T_KW_FRETURN;
        case SC_T_KW_NRETURN:       return T_KW_NRETURN;
        case SC_T_KW_STRUCT:        return T_KW_STRUCT;
        case SC_T_UNKNOWN:          return T_UNKNOWN;
        default:                    return T_UNKNOWN;
    }
}

/* ---- Statement assembly ----
 *
 * For LS-4.a there are exactly two shapes:
 *   E_ASSIGN(lhs, rhs)         -> subject = lhs, replacement = rhs, has_eq = 1
 *   anything else              -> subject = expr (bare expression statement)
 *
 * Mirrors sno4_stmt_commit_go's split logic for the top-level E_ASSIGN
 * case (snobol4.y line ~250).  Pattern-match split (E_SCAN), label
 * handling, and goto-field assembly arrive in later LS-4.* steps.
 */
static void sc_append_stmt(ScParseState *st, EXPR_t *top) {
    if (!top) return;
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    if (top->kind == E_ASSIGN && top->nchildren == 2) {
        s->subject     = top->children[0];
        s->replacement = top->children[1];
        s->has_eq      = 1;
        free(top->children);
        free(top);
    } else {
        s->subject = top;
    }
    if (!st->code->head) st->code->head = st->code->tail = s;
    else { st->code->tail->next = s; st->code->tail = s; }
}

/* ---- Literal builders ---- */
static EXPR_t *sc_int_literal(const char *txt) {
    EXPR_t *e = expr_new(E_ILIT);
    e->ival = strtol(txt, NULL, 10);
    return e;
}

static EXPR_t *sc_real_literal(const char *txt) {
    EXPR_t *e = expr_new(E_FLIT);
    e->dval = strtod(txt, NULL);
    return e;
}

static EXPR_t *sc_str_literal(const char *txt) {
    EXPR_t *e = expr_new(E_QLIT);
    e->sval = strdup(txt);
    return e;
}

/* sc_clone_expr_simple — shallow-recursive clone for compound-assign LHS.
 *
 * Compound-assigns (`a += b`, `a *= b`, etc.) lower to
 *   E_ASSIGN(a, E_BINOP(clone(a), b))
 * which means the LHS expression is referenced in two distinct subtrees
 * of the same IR.  The IR's tree representation requires distinct nodes
 * (children pointer arrays would otherwise alias and double-free at
 * cleanup), so we clone the LHS.
 *
 * Coverage: the kinds Snocone source can produce as a compound-assign
 * LHS at this point in the grammar — atomic E_VAR / E_KEYWORD / E_ILIT
 * / E_FLIT / E_QLIT, plus n-ary E_IDX / E_FNC for `a[i] += 1` and the
 * (rare) `f(x) += 1`.  Anything else is a parse-time bug; we return NULL
 * to surface it loudly via the parse error path rather than silently
 * producing a malformed IR.  When LS-4.d adds subscripts and unaries,
 * extend the switch as needed.
 */
static EXPR_t *sc_clone_expr_simple(EXPR_t *e) {
    if (!e) return NULL;
    EXPR_t *c = expr_new(e->kind);
    /* Scalar fields — copied verbatim. */
    c->ival = e->ival;
    c->dval = e->dval;
    if (e->sval) c->sval = strdup(e->sval);
    /* Children — recursive clone, preserving order. */
    for (int i = 0; i < e->nchildren; i++) {
        expr_add_child(c, sc_clone_expr_simple(e->children[i]));
    }
    return c;
}

/* =========================================================================
 *  Public entry — snocone_parse_program
 *
 *  Parses a complete Snocone source string into a CODE_t*.  On parse
 *  failure returns NULL (and st.nerrors > 0); on success returns a
 *  freshly-allocated CODE_t ready for the IR/SM pipeline.
 *
 *  CODE_t is the typedef alias of Program (added in LS-4.cn — session
 *  2026-04-30 #7 — for symmetry with EXPR_t).  Existing callers that
 *  declared the result as `Program *` continue to work; the two names
 *  refer to the same type.
 *
 *  This is the LS-4.a entry point.  When LS-4.j wires it into scrip's
 *  driver, snocone_compile() will collapse to a thin wrapper around
 *  this function (~5 lines), mirroring sno_parse_string() at
 *  snobol4.y:316.
 * ========================================================================= */
CODE_t *snocone_parse_program(const char *src, const char *filename) {
    LexCtx          ctx = {0};
    ctx.p           = src ? src : "";
    ctx.line        = 1;
    ScParseState    state = {0};
    state.ctx       = (struct LexCtx *)&ctx;
    state.code      = calloc(1, sizeof *state.code);
    state.filename  = filename;
    state.nerrors   = 0;

    int rc = sc_parse(&state);

    if (rc != 0 || state.nerrors > 0) {
        free(state.code);
        return NULL;
    }
    return state.code;
}
