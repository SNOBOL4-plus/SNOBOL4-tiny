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
 *   * T_CALL call-form `EQ(2+2, 4)` → E_FNC("EQ", ...)
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
#define T_CALL         SC_T_CALL
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

/* Keywords — Snocone reserved words.  T_FUNCTION is the `function`
 * keyword; T_CALL is the IDENT-followed-by-zero-space-`(` call-form
 * token from the FSM (carries the identifier name, consumes the `(`). */
#define T_IF            SC_T_IF
#define T_ELSE          SC_T_ELSE
#define T_WHILE         SC_T_WHILE
#define T_DO            SC_T_DO
#define T_UNTIL         SC_T_UNTIL
#define T_FOR           SC_T_FOR
#define T_SWITCH        SC_T_SWITCH
#define T_CASE          SC_T_CASE
#define T_DEFAULT       SC_T_DEFAULT
#define T_BREAK         SC_T_BREAK
#define T_CONTINUE      SC_T_CONTINUE
#define T_GOTO          SC_T_GOTO
#define T_FUNCTION      SC_T_FUNCTION
#define T_RETURN        SC_T_RETURN
#define T_FRETURN       SC_T_FRETURN
#define T_NRETURN       SC_T_NRETURN
#define T_STRUCT        SC_T_STRUCT

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
#undef T_CALL
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
#undef T_IF
#undef T_ELSE
#undef T_WHILE
#undef T_DO
#undef T_UNTIL
#undef T_FOR
#undef T_SWITCH
#undef T_CASE
#undef T_DEFAULT
#undef T_BREAK
#undef T_CONTINUE
#undef T_GOTO
#undef T_FUNCTION
#undef T_RETURN
#undef T_FRETURN
#undef T_NRETURN
#undef T_STRUCT
#undef T_EOF
#undef T_UNKNOWN
}

%code requires {
#include "scrip_cc.h"

/* Forward-declare LexCtx — full definition lives in snocone_fsm.h, which
 * we deliberately do NOT include here so its T_* enumerators don't
 * collide with Bison's enum sc_tokentype.  See the %code top block above. */
struct LexCtx;

/* LS-4.f — control-flow handoff structs.  Built by if_head / while_head
 * non-terminals; consumed by sc_finalize_* in the parent rule's final
 * action.  Forward-declared here so the %union can reference them; the
 * full layout is defined in the epilogue alongside the helpers. */
struct IfHead;
struct WhileHead;
struct FuncHead;

/* LS-4.i.2 — LoopFrame: tracks one enclosing loop (or switch, in LS-4.i.3)
 * for the purpose of resolving break/continue.  Pushed onto a stack rooted
 * at ScParseState.loop_top when a loop's head fires (sc_while_head_new,
 * sc_do_head_new, sc_for_head_new); popped when the matching finalize_*
 * runs.  Each frame carries the synthetic labels the corresponding
 * finalize_* will use, so break/continue stmts emitted DURING body parsing
 * can target them by name.
 *
 * user_labels[] holds any user labels that immediately preceded the loop
 * (i.e. were in pending_user_labels when the head fired).  Stacked labels
 * (`a: b: while(...) {...}`) all attach — break a; and break b; both work,
 * naming the same loop.  Java-style.
 *
 * is_loop = 1 for while/do/for; LS-4.i.3 will add 0 for switch (continue
 * skips switch frames when looking for its target). */
typedef struct LoopFrame {
    char    *cont_label;          /* continue target (loop top or "Lcont" for for/do) */
    char    *end_label;           /* break target (loop end / switch end) */
    char   **user_labels;         /* names that the user attached (own'd, strdup'd) */
    int      user_labels_count;
    int      is_loop;             /* 1 = loop (continue legal); 0 = switch (LS-4.i.3) */
    /* LS-4.i.2 — usage flags: tracks whether the body actually emitted any
     * break/continue stmts targeting this frame.  Used by finalize_* to
     * decide whether to emit the Lcont pad (for/do/while) — keeping the
     * emit lazy preserves the LS-4.f/g lowering shapes for code that
     * doesn't use continue, and avoids dead label pads in the IR.  break
     * targets don't need a flag — the Lend pad is always emitted (it's
     * the loop's exit target regardless). */
    int      cont_used;
    struct LoopFrame *outer;      /* link toward outer scope; NULL at outermost */
} LoopFrame;

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
    int            label_seq;     /* LS-4.f: synthetic label counter */
    char          *cur_func_name; /* LS-4.h: enclosing function name (NULL at top level) */
    /* LS-4.i.2 — break/continue support.
     *
     * pending_user_labels accumulates label names emitted by recent
     * label_decl reductions that have not yet been "consumed" by either
     * a non-control stmt commit (which clears them) or a loop/switch
     * head (which captures them onto a LoopFrame).  This lets
     * `outer: while(...) { ... break outer; ... }` resolve the break to
     * the labeled loop frame.
     *
     * stash_for_pending_labels — a one-slot temporary that the for_lead
     * non-terminal moves pending into BEFORE the for-loop's init expr
     * gets emitted (which would otherwise clear pending via sc_append_stmt
     * before sc_for_head_new runs).  for_head's action consumes the stash. */
    LoopFrame    *loop_top;
    char        **pending_user_labels;
    int           pending_user_labels_count;
    int           pending_user_labels_cap;
    char        **stash_for_pending_labels;
    int           stash_for_pending_labels_count;
    /* LS-4.i.3 — innermost-switch pointer.  Used by case_or_default_label
     * actions to find the SwitchHead that owns them.  Saved/restored on
     * SwitchHead.prev_switch for nested switches. */
    struct SwitchHead *cur_switch;
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
static void     sc_split_subject_pattern(EXPR_t **subj_io, EXPR_t **pat_io); /* LS-4.l */
static EXPR_t  *sc_int_literal        (const char *txt);
static EXPR_t  *sc_real_literal       (const char *txt);
static EXPR_t  *sc_str_literal        (const char *txt);
static EXPR_t  *sc_clone_expr_simple  (EXPR_t *e);  /* LS-4.c — for compound-assigns */

/* LS-4.f — control-flow lowering helpers.
 * Architecture: control-flow head non-terminals (if_head, while_head)
 * snapshot the linked-list tail BEFORE the body is parsed and return
 * the snapshot via a heap-allocated handoff struct.  The body's stmts
 * append normally to st->code in their order.  The parent rule's
 * final action calls sc_finalize_* with the head struct (and, for
 * if-else, also the else_keyword snapshot of where the else-body
 * begins) to splice the cond-stmt and label-stmts into the correct
 * positions.  This emit-and-splice avoids needing MRAs in the middle
 * of the rule, which is what causes reduce/reduce conflicts in
 * Bison's balanced if-else grammars.
 */
struct IfHead {
    EXPR_t *cond;          /* condition expression                              */
    STMT_t *before_body;   /* st->code->tail snapshot at end of if_head reduction
                              (NULL if list was empty); body starts at
                              before_body->next or st->code->head if NULL.     */
    int     lineno;        /* lineno of if_head for the cond-stmt's lineno     */
};
struct WhileHead {
    EXPR_t *cond;
    STMT_t *before_body;
    int     lineno;
    /* LS-4.i.2 — synthetic labels owned by the head, allocated eagerly so
     * break/continue stmts emitted DURING body parsing target them by
     * name.  finalize_while uses these same names instead of allocating
     * fresh ones. */
    char   *cont_label;     /* loop top — continue target */
    char   *end_label;      /* loop end — break target */
};
/* LS-4.g — do/while: snapshot before body (at KW_DO), cond
 * provided when the trailing while clause is parsed. */
struct DoHead {
    STMT_t *before_body;   /* tail snapshot at KW_DO */
    int     lineno;
    /* LS-4.i.2 — eager labels.  cont_label is a fresh "_Lcont_NNNN" pad
     * that finalize_do_while splices in just before the trailing
     * cond stmt — `continue;` in a do/while re-evaluates the cond. */
    char   *cont_label;
    char   *end_label;
};
/* LS-4.g — for (init; cond; step): snapshot after init emits, before
 * the loop-top label and cond-stmt are spliced. */
struct ForHead {
    STMT_t *before_loop;   /* tail snapshot after init stmt appended */
    EXPR_t *cond;          /* loop condition */
    EXPR_t *step;          /* step expression (emitted at loop bottom) */
    int     lineno;
    /* LS-4.i.2 — eager labels.  cont_label is "_Lcont_NNNN" — `continue;`
     * in a for-loop must execute the step expression before re-testing
     * cond, so its target is just before the step stmt, not Ltop. */
    char   *cont_label;
    char   *end_label;
};
/* LS-4.h — function definition handoff.
 *
 * `function NAME(args) { body }` lowers to:
 *
 *      <existing stmts up to before_func>
 *      DEFINE('NAME(args)')              :(NAME_end)        <- emitted by sc_func_head_new
 *      NAME    <body-stmts>                                  <- spliced by sc_finalize_function
 *      NAME_end                                              <- appended by sc_finalize_function
 *
 * This matches the SPITBOL idiom: the DEFINE installs the function
 * descriptor at load time, the unconditional goto jumps over the body
 * so it does not execute as straight-line code, the entry-point label
 * `NAME` is where calls land, and `NAME_end` is the skip-target.
 *
 * Inside the body, return/freturn/nreturn use the cur_func_name field
 * (saved by sc_func_head_new and restored by sc_finalize_function) to
 * emit the SPITBOL `:(RETURN)`, `:(FRETURN)`, `:(NRETURN)` branches.
 *
 * Nested function definitions are not supported (Andrew's `.sc`
 * doesn't have them either) — the implementation assumes
 * cur_func_name is NULL when sc_func_head_new fires.
 */
struct FuncHead {
    char   *name;          /* function name — used as entry-point label,
                              and for the prefix of the end-skip label */
    char   *end_label;     /* "NAME_end" — the skip target */
    char   *prev_func;     /* saved cur_func_name (for nested-function safety) */
    STMT_t *after_goto;    /* tail snapshot AFTER the DEFINE+goto stmts,
                              before any body stmt is appended.  The
                              entry-point label `NAME` is spliced just
                              after this anchor. */
    int     lineno;
};

/* LS-4.i.3 — switch / case / default lowering handoff.
 *
 *   switch (e) { case v1: A; case v2: B; default: D; }
 *
 * lowers to:
 *
 *      tmp = e                                 <- emitted by sc_switch_head_new
 *      IDENT(tmp, v1)        :S(_Lcase_0001)   <-+
 *      IDENT(tmp, v2)        :S(_Lcase_0002)   <-+ dispatch chain — built
 *                            :(_Ldefault_0003) <-+ in head->dispatch_*, spliced
 *                                                   in finalize after after_tmp_assign
 *      _Lcase_0001  A      :(_Lend_0004)       <- A's body, then implicit break
 *      _Lcase_0002  B      :(_Lend_0004)       <- B's body, then implicit break
 *      _Ldefault_0003 D                          <- D's body — NO trailing goto
 *                                                   (last case naturally falls
 *                                                   through to _Lend below)
 *      _Lend_0004                                <- appended at finalize
 *
 * Each case's body falls through to _Lend implicitly at the start of the next
 * case label (or at finalize for the last one) — modern no-fall-through
 * semantics per Q6.  An empty body between two case labels (`case 1: case 2:
 * stmts;`) suppresses the implicit break: detected via last_case_label_tail
 * being the same as code->tail when the next case head fires (no body stmts
 * appended in between).  Both labels point at the same body via SNOBOL4
 * label-chaining — the grammar emits two label pads back-to-back; the second
 * is the resolved jump target for `IDENT(tmp, 1)`, the first is the resolved
 * target for `IDENT(tmp, 2)`, both label pads chain forward to the actual body.
 *
 * `default:` is a special case-head that allocates `_Ldefault_NNNN` instead
 * of `_Lcase_NNNN`, doesn't emit a dispatch IDENT() entry (the dispatch chain
 * just appends `:(_Ldefault_NNNN)` as the catch-all), and sets has_default=1.
 * Multiple `default:` clauses in one switch is a parse error.  If no default
 * appears, the dispatch's catch-all goto targets _Lend directly.
 *
 * `break;` inside a switch body resolves to the switch's end_label via the
 * existing LoopFrame stack — sc_switch_head_new pushes a frame with is_loop=0
 * (forward-compat groundwork from LS-4.i.2), so sc_append_break finds it but
 * sc_append_continue skips it (continue is a parse error in switch).
 */
struct CaseEntry {
    char   *case_label;    /* "_Lcase_NNNN" or "_Ldefault_NNNN" — owned */
    EXPR_t *value;         /* case value expression (still owned); NULL = default */
};
struct SwitchHead {
    EXPR_t *disc;                  /* discriminant expression (consumed by tmp=disc) */
    char   *tmp_name;              /* "_Lswitch_t_NNNN" — synthetic var name */
    char   *end_label;             /* "_Lend_NNNN" — break target */
    char   *default_label;         /* "_Ldefault_NNNN" or strdup(end_label) if no default */
    int     has_default;
    STMT_t *after_tmp_assign;      /* splice anchor for dispatch list */
    struct CaseEntry *cases;       /* dynamic array */
    int     cases_count;
    int     cases_cap;
    /* For implicit-break suppression on stacked case labels (`case 1: case 2:`). */
    STMT_t *last_case_label_tail;
    /* Saved outer-switch pointer for nested switches. */
    struct SwitchHead *prev_switch;
    int     lineno;
};

static char    *sc_label_new          (ScParseState *st, const char *prefix);
static struct IfHead    *sc_if_head_new    (ScParseState *st, EXPR_t *cond);
static struct WhileHead *sc_while_head_new (ScParseState *st, EXPR_t *cond);
static struct DoHead    *sc_do_head_new    (ScParseState *st);
static struct ForHead   *sc_for_head_new   (ScParseState *st, EXPR_t *cond, EXPR_t *step);
/* LS-4.h */
static void     sc_append_return      (ScParseState *st, EXPR_t *retval);
static void     sc_append_freturn     (ScParseState *st);
static void     sc_append_nreturn     (ScParseState *st);
/* LS-4.h — forward decls for stmt-builder helpers used inside sc_func_head_new
 * (the helpers themselves are defined later in the epilogue). */
static STMT_t  *sc_make_label_stmt    (ScParseState *st, char *label);
static STMT_t  *sc_make_goto_uncond_stmt(ScParseState *st, char *target);
static void     sc_splice_after       (ScParseState *st, STMT_t *anchor, STMT_t *chain_head, STMT_t *chain_tail);
static void     sc_append_chain       (ScParseState *st, STMT_t *chain_head, STMT_t *chain_tail);
static void     sc_finalize_if_no_else(ScParseState *st, struct IfHead *h);
static void     sc_finalize_if_else   (ScParseState *st, struct IfHead *h, STMT_t *before_else);
static void     sc_finalize_while     (ScParseState *st, struct WhileHead *h);
static void     sc_finalize_do_while  (ScParseState *st, struct DoHead *h, EXPR_t *cond);
static void     sc_finalize_for       (ScParseState *st, struct ForHead *h);
static struct FuncHead *sc_func_head_new(ScParseState *st, char *name, char *argstr);
static void     sc_finalize_function  (ScParseState *st, struct FuncHead *h);
/* LS-4.i.1 — goto / label */
static void     sc_emit_label_pad     (ScParseState *st, char *label);
static void     sc_append_goto_label  (ScParseState *st, char *target);
/* LS-4.i.2 — break / continue */
static void     sc_pending_label_add   (ScParseState *st, const char *name);
static void     sc_pending_label_clear (ScParseState *st);
static void     sc_pending_to_stash    (ScParseState *st);     /* for_lead */
static void     sc_loop_push           (ScParseState *st, char *cont_label, char *end_label, int is_loop, int from_stash);
static void     sc_loop_pop            (ScParseState *st);
static LoopFrame *sc_loop_find_by_user_label(ScParseState *st, const char *name, int want_loop);
static void     sc_append_break        (ScParseState *st, char *user_label /* NULL = innermost */);
static void     sc_append_continue     (ScParseState *st, char *user_label /* NULL = innermost loop */);
/* LS-4.i.3 — switch / case / default */
static struct SwitchHead *sc_switch_head_new(ScParseState *st, EXPR_t *disc);
static void     sc_switch_case_label   (ScParseState *st, EXPR_t *value);
static void     sc_switch_default_label(ScParseState *st);
static void     sc_finalize_switch     (ScParseState *st, struct SwitchHead *h);
/* LS-4.i.5 — struct NAME { f1, f2, ... } — Andrew's .sc line 162 record-decl.
 * Lowers to a single SPITBOL DATA('NAME(f1,f2,...)') bare-expression statement.
 * SPITBOL's DATA primitive defines the constructor + per-field accessors in
 * one call; struct is sugar for that.  Empty `struct NAME { }` lowers to
 * DATA('NAME()') — legal SPITBOL (a zero-field record). */
static void     sc_emit_struct         (ScParseState *st, char *name, char *fields);

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
    /* LS-4.f — control-flow handoff types.  IfHead/WhileHead are
     * returned by the head-prefix non-terminals (if_head, while_head)
     * and consumed by the finalize-* helpers.  stmt_ptr is returned
     * by `else_keyword` to snapshot the linked-list cursor before
     * the else-branch begins, enabling splice-in-the-middle. */
    struct IfHead    *ifhead;
    struct WhileHead *whilehead;
    struct DoHead    *dohead;
    struct ForHead   *forhead;
    /* LS-4.h — function definition handoff type. */
    struct FuncHead  *funchead;
    /* LS-4.i.3 — switch/case/default handoff type. */
    struct SwitchHead *switchhead;
    STMT_t           *stmt_ptr;
}

/* ---- Atoms ---- */
%token <str> T_IDENT
%token <str> T_KEYWORD
%token <str> T_INT      /* FSM emits raw text; thunk converts to long */
%token <str> T_REAL     /* same — thunk converts to double */
%token <str> T_STR      /* FSM emits unquoted, escapes resolved */
%token <str> T_CALL /* IDENT-followed-by-zero-space-( per Andrew's `f(args)` rule */

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
%token T_COLON
%token T_DO T_FOR
%token T_SWITCH T_CASE T_DEFAULT
%token T_BREAK T_CONTINUE T_GOTO
%token T_FUNCTION T_RETURN T_FRETURN T_NRETURN T_STRUCT
%token T_UNKNOWN

/* ---- Block delimiters and control-flow keywords (LS-4.f) ---- */
%token T_LBRACE T_RBRACE
%token T_IF T_ELSE T_WHILE

%type <expr> expr0 expr1 expr3 expr4 expr5 expr6 expr9 expr11 expr12 expr15 expr17 exprlist exprlist_ne

/* LS-4.f — control-flow non-terminal types */
%type <ifhead>    if_head
%type <whilehead> while_head
%type <dohead>    do_head
%type <stmt_ptr>  else_keyword
/* LS-4.g — for head carries cond+step captured after init parses */
%type <forhead>   for_head
/* LS-4.h — function definition head */
%type <funchead>  func_head
%type <str>       func_arglist func_arglist_ne
/* LS-4.i.3 — switch/case/default head */
%type <switchhead> switch_head
/* LS-4.i.5 — struct field list — comma-separated IDENT list, accumulated
 * left-to-right into a single malloc'd "f1,f2,f3" string handed to
 * sc_emit_struct.  Mirrors func_arglist_ne shape verbatim. */
%type <str>       struct_field_list

%%

/* ---- Program structure ---- */
program     : stmt_list
            | /* empty */
            ;

stmt_list   : stmt_list stmt
            | stmt
            ;

/* ---- LS-4.f: matched / unmatched split for control flow ----
 *
 * Pascal/Algol balanced grammar for the dangling-else.  An `if`
 * paired with a matching `else` is matched_stmt; an `if` without
 * (yet) a paired `else`, or whose else-branch contains an unmatched
 * if, is unmatched_stmt.  Inside matched_stmt's then- and else-
 * branches only matched_stmt is permitted, so an `else` always
 * binds to the nearest unmatched `if`.  Zero shift/reduce / zero
 * reduce/reduce conflicts.
 *
 * Lowering — emit-and-splice: control-flow heads (if_head,
 * while_head) snapshot st->code->tail at the moment of reduction
 * but emit nothing.  The body parses and appends statements
 * normally.  The parent rule's final action calls sc_finalize_*
 * to splice the cond-stmt and label-stmts into their correct
 * positions in the linked list.  This pattern avoids MRAs in the
 * middle of the rule — which would cause reduce/reduce conflicts
 * when multiple matched/unmatched alternatives share a prefix.
 *
 * Goal-file note (LS-4.d forward note): "use balanced matched_stmt /
 * unmatched_stmt clause-phrase split (Pascal/Algol style, zero
 * conflicts) rather than C's default-shift acceptance".  Honored.
 */
stmt        : matched_stmt
            | unmatched_stmt
            ;

matched_stmt
            : simple_stmt
            | block_stmt
            | if_head matched_stmt else_keyword matched_stmt
                                        { sc_finalize_if_else(st, $1, $3); }
            | while_head matched_stmt
                                        { sc_finalize_while(st, $1); }
            /* LS-4.g — do/while (always matched — no dangling-else risk).
             * Uses do_body (always a brace block) to avoid the shift/reduce tension
             * that arises when T_WHILE follows a matched_stmt on the stack:
             * the parser would want to start a new while_head rather than close
             * the do-loop.  Requiring { } makes the body unambiguous. */
            | do_head do_body T_WHILE T_LPAREN expr0 T_RPAREN T_SEMICOLON
                                        { sc_finalize_do_while(st, $1, $5); }
            /* LS-4.g — for (init; cond; step) body */
            | for_head matched_stmt
                                        { sc_finalize_for(st, $1); }
            /* LS-4.h — function name(args) { body } */
            | func_head T_LBRACE stmt_list T_RBRACE
                                        { sc_finalize_function(st, $1); }
            | func_head T_LBRACE T_RBRACE
                                        { sc_finalize_function(st, $1); }
            /* LS-4.i.3 — switch (e) { case v1: ... case v2: ... default: ... }
             * Always matched (no dangling-else risk — switch is a self-contained
             * brace block).  Empty switch body `{ }` is permitted; non-empty
             * body must start with a case_clause (case or default label —
             * raw stmts before the first case label is a parse error). */
            | switch_head T_LBRACE switch_body T_RBRACE
                                        { sc_finalize_switch(st, $1); }
            | switch_head T_LBRACE T_RBRACE
                                        { sc_finalize_switch(st, $1); }
            /* LS-4.i.5 — struct NAME { f1, f2, ... } record declaration.
             * Lowers to a single bare-expression stmt: DATA('NAME(f1,f2,...)').
             * Empty `struct NAME { }` lowers to DATA('NAME()').  No trailing
             * semicolon — matches Andrew's .sc surface and the corpus
             * (include-sc/stack.sc, tree.sc, counter.sc all written as
             * `struct NAME { fields }` with no `;`). */
            | T_STRUCT T_IDENT T_LBRACE struct_field_list T_RBRACE
                                        { sc_emit_struct(st, $2, $4); free($2); free($4); }
            | T_STRUCT T_IDENT T_LBRACE T_RBRACE
                                        { sc_emit_struct(st, $2, strdup("")); free($2); }
            /* LS-4.i.1 — labeled stmt: `name:` followed by a stmt */
            | label_decl matched_stmt
            ;

unmatched_stmt
            : if_head stmt
                                        { sc_finalize_if_no_else(st, $1); }
            | if_head matched_stmt else_keyword unmatched_stmt
                                        { sc_finalize_if_else(st, $1, $3); }
            | while_head unmatched_stmt
                                        { sc_finalize_while(st, $1); }
            /* LS-4.g — for with unmatched body */
            | for_head unmatched_stmt
                                        { sc_finalize_for(st, $1); }
            /* LS-4.i.1 — labeled stmt: `name:` followed by an unmatched stmt */
            | label_decl unmatched_stmt
            ;

if_head     : T_IF T_LPAREN expr0 T_RPAREN opt_head_sep
                                        { $$ = sc_if_head_new(st, $3); }
            ;

while_head  : T_WHILE T_LPAREN expr0 T_RPAREN opt_head_sep
                                        { $$ = sc_while_head_new(st, $3); }
            ;

/* LS-4.g — do_head fires at the `do` keyword before the body block.
 * The trailing while clause (with the condition) is parsed by the
 * parent rule; do_head just snapshots the linked-list tail. */
do_head     : T_DO                  { $$ = sc_do_head_new(st); }
            ;

/* do_body — always a brace block.  Requiring { } here is not a semantic
 * restriction (C-style do {} while always uses braces in practice) and
 * is a necessary grammar disambiguation: if do_body were `stmt` the
 * parser would face a shift/reduce conflict at T_WHILE — it could
 * not decide whether WHILE starts a new while_head (another matched_stmt)
 * or closes the do-loop.  Brace-delimited bodies have a clear endpoint. */
do_body     : T_LBRACE stmt_list T_RBRACE
            | T_LBRACE T_RBRACE
            ;

/* LS-4.g — for_head: `for ( init ; cond ; step )` opt_head_sep.
 * init is a full expr0 that is immediately emitted as a statement.
 * cond and step are captured in the ForHead struct for use in finalize.
 * The snapshot of st->code->tail happens AFTER init is emitted —
 * that is the before_loop anchor used to splice Ltop + cond-stmt.
 *
 * LS-4.i.2 — for_lead is a tiny non-terminal that fires on T_FOR alone,
 * BEFORE init's expr0 starts parsing.  Its action moves
 * pending_user_labels into a one-slot stash on ScParseState.  This is
 * needed because init's emission via sc_append_stmt would otherwise
 * clear pending_user_labels before sc_for_head_new can capture them
 * onto the loop frame.  (For while/do, the head action runs before any
 * stmt commit — pending is still alive there.) */
for_lead    : T_FOR                  { sc_pending_to_stash(st); }
            ;

for_head    : for_lead T_LPAREN expr0 T_SEMICOLON expr0 T_SEMICOLON expr0 T_RPAREN opt_head_sep
                                        { sc_append_stmt(st, $3);
                                          $$ = sc_for_head_new(st, $5, $7); }
            ;

/* LS-4.i.3 — switch_head fires at `switch (expr0)` close-paren reduction,
 * BEFORE the brace body is parsed.  Allocates the synthetic tmp variable,
 * emits `tmp = expr0;` as a real stmt, snapshots code->tail as the splice
 * anchor for the dispatch chain (built in sc_switch_case_label / finalize),
 * and pushes a LoopFrame with is_loop=0 — break; targets the switch's
 * end_label, continue; rejects (skips switch frames per LS-4.i.2's
 * sc_loop_find_innermost(want_loop=1)).
 *
 * Note: no opt_head_sep here.  T_LBRACE follows directly; the FSM does
 * NOT synthesize T_CONCAT between `)` and `{` because `{` is not a
 * value-starter (it's a block delimiter that opens a new scope).  Same
 * reason `func_head ... T_LBRACE` doesn't need opt_head_sep. */
switch_head : T_SWITCH T_LPAREN expr0 T_RPAREN
                                        { $$ = sc_switch_head_new(st, $3); }
            ;

/* switch_body must start with at least one case_clause — raw stmts before
 * the first case label are a parse error (matches C semantics).  Empty
 * switch body `{ }` handled by the parent rule's separate T_LBRACE T_RBRACE
 * alternative (no switch_body involved). */
switch_body : case_clause
            | switch_body case_clause
            ;

/* A case_clause is one case-or-default label followed by zero or more body
 * stmts.  Multiple case-or-default labels in succession (`case 1: case 2:`)
 * each become their own clause — the LR(1) lookahead at T_CASE / T_DEFAULT
 * unambiguously starts a new clause.  Empty body between adjacent labels
 * triggers the implicit-break suppression in sc_switch_case_label /
 * sc_switch_default_label (when last_case_label_tail == code->tail, no
 * body has been appended since, so no implicit `:(_Lend)` is emitted). */
case_clause : case_or_default_label
            | case_clause stmt
            ;

case_or_default_label
            : T_CASE expr0 T_COLON      { sc_switch_case_label(st, $2); }
            | T_DEFAULT T_COLON         { sc_switch_default_label(st); }
            ;

/* opt_head_sep — absorbs the spurious T_CONCAT the LS-3 lexer emits
 * between an if/while head's `)` and a value-starting body token (e.g.
 * `if (c) y = 1;` — between `)` and `y` the W{OP}W envelope sees a
 * value-then-value space gap and synthesizes T_CONCAT).  Inside an
 * expression that CONCAT is meaningful concatenation; after a control-
 * flow head it must be discarded.  Accepting it here, separate from
 * the expr4 concat tier, keeps the grammar unambiguous. */
opt_head_sep
            : /* empty */
            | T_CONCAT
            ;

/* LS-4.h — func_head: `function NAME ( arglist )` opt_head_sep.
 *
 * The function name is carried by T_CALL (IDENT immediately followed
 * by zero-whitespace `(` — the call-form token from the FSM).  The arg
 * list is zero or more comma-separated IDENT names, returned as a single
 * malloc'd string of the form "arg1,arg2,arg3".  An empty arg list
 * returns "".
 *
 * func_head emits:
 *   1. A DEFINE('name(args)') call statement — appended immediately.
 *   2. An unconditional goto :(name_end) — skips the body at load time.
 * It snapshots st->code->tail AFTER those two stmts, so the body stmts
 * are appended after the snapshot; sc_finalize_function then appends
 * the name label (entry point) BEFORE the body by splicing after the
 * goto, and appends name_end AFTER the body.
 *
 * cur_func_name is set so return/freturn/nreturn inside the body can
 * reference the function name.
 */
func_head   : T_FUNCTION T_CALL T_LPAREN func_arglist opt_head_sep
                                        { $$ = sc_func_head_new(st, $2, $4); free($2); free($4); }
            ;

/* func_arglist — the argument portion of `function name(args)`.
 * T_CALL already consumed the name and the `(`; we now consume
 * the comma-separated IDENT list and the closing `)`.
 * The value is a malloc'd string "arg1,arg2" (empty string if no args).
 */
func_arglist
            : T_RPAREN                 { $$ = strdup(""); }
            | T_IDENT T_RPAREN         { $$ = strdup($1); free($1); }
            | func_arglist_ne T_RPAREN { $$ = $1; }
            ;

/* Helper — builds the comma-separated arg string, left-to-right. */
func_arglist_ne
            : T_IDENT T_COMMA T_IDENT
                { int len = strlen($1) + 1 + strlen($3) + 1;
                  char *s = malloc(len); snprintf(s, len, "%s,%s", $1, $3);
                  free($1); free($3); $$ = s; }
            | func_arglist_ne T_COMMA T_IDENT
                { int len = strlen($1) + 1 + strlen($3) + 1;
                  char *s = malloc(len); snprintf(s, len, "%s,%s", $1, $3);
                  free($1); free($3); $$ = s; }
            ;

/* LS-4.i.5 — struct_field_list: comma-separated IDENT list inside the
 * `{ ... }` of a `struct NAME { ... }` declaration.  Yields a single
 * malloc'd string of the form "f1,f2,f3" (no leading/trailing comma,
 * no spaces) handed to sc_emit_struct, which embeds it inside the
 * DATA('NAME(...)') argument string.  Mirrors func_arglist_ne's
 * left-to-right accumulation pattern verbatim. */
struct_field_list
            : T_IDENT
                { $$ = strdup($1); free($1); }
            | struct_field_list T_COMMA T_IDENT
                { int len = strlen($1) + 1 + strlen($3) + 1;
                  char *s = malloc(len); snprintf(s, len, "%s,%s", $1, $3);
                  free($1); free($3); $$ = s; }
            ;

/* else_keyword reduces on T_ELSE, snapshotting the linked-list tail
 * at the moment the else is recognised.  The if-else finalizer uses
 * this snapshot to delimit then-body's last stmt vs else-body's first.
 * One non-terminal shared between the matched and unmatched if-else
 * rules — the same anonymous reduction would have been a conflict. */
else_keyword
            : T_ELSE                 { $$ = st->code->tail; }
            ;

/* LS-4.i.1 — label_decl: `name:` prefix that emits a label-pad stmt
 * eagerly and lets the following stmt's stmts append after it.  In
 * SNOBOL4, a label on a label-only pad chains to the next non-empty
 * stmt — that is exactly the semantics we want for `top: stmt`.
 *
 * The semantic action runs when Bison reduces `T_IDENT T_COLON`,
 * before the trailing stmt is parsed.  Therefore the label-pad sits
 * in st->code immediately before whatever stmts the body contributes.
 *
 * Disambiguation with expression-statement: at stmt-start, on seeing
 * T_IDENT, Bison's lookahead is T_COLON.  COLON does not appear in
 * any expression rule's FOLLOW set, so the LR(1) item set has only
 * one viable parse — shift T_COLON into label_decl rather than reduce
 * T_IDENT to expr17.  Bison resolves cleanly with no conflicts. */
label_decl
            : T_IDENT T_COLON        { sc_emit_label_pad(st, $1); free($1); }
            ;

simple_stmt : expr0 T_SEMICOLON                { sc_append_stmt(st, $1); }
            | T_SEMICOLON                      { /* empty stmt */         }
            /* LS-4.h — return/freturn/nreturn inside a function body */
            | T_RETURN expr0 T_SEMICOLON    { sc_append_return(st, $2); }
            | T_RETURN T_SEMICOLON          { sc_append_return(st, NULL); }
            | T_FRETURN T_SEMICOLON         { sc_append_freturn(st); }
            | T_NRETURN T_SEMICOLON         { sc_append_nreturn(st); }
            /* LS-4.i.1 — goto LABEL; */
            | T_GOTO T_IDENT T_SEMICOLON    { sc_append_goto_label(st, $2); free($2); }
            /* LS-4.i.2 — break / continue (with optional user label per Q13 Option A) */
            | T_BREAK T_SEMICOLON           { sc_append_break(st, NULL); }
            | T_BREAK T_IDENT T_SEMICOLON   { sc_append_break(st, $2); free($2); }
            | T_CONTINUE T_SEMICOLON        { sc_append_continue(st, NULL); }
            | T_CONTINUE T_IDENT T_SEMICOLON { sc_append_continue(st, $2); free($2); }
            ;

block_stmt  : T_LBRACE stmt_list T_RBRACE      { /* statements already appended */ }
            | T_LBRACE T_RBRACE                { /* empty block */                  }
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
 *   expr17 — atoms (literals, idents, keywords, parens, T_CALL,
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

/* Right-associative exponentiation: expr12 ^ expr11.
 * (Was expr15 before LS-4.l fence/semantic fix; now goes through expr12
 * so binary `.` and `$` pattern-binding ops sit between exponent and
 * subscript — matching snobol4.y:156-162.) */
expr11      : expr12 T_2CARET expr11
                                { $$ = expr_binary(E_POW, $1, $3); }
            | expr12
                                { $$ = $1; }
            ;

/* ---- Pattern-bind tier (LS-4.l fence/semantic fix) ----------------------
 *
 * Binary `.` and `$` — SPITBOL pattern-binding operators, priority 12,
 * left-associative.  Mirrors snobol4.y:159-161 byte-for-byte (modulo
 * tier name — snobol4.y's expr13 is our expr15).
 *
 *   pat . var   →  E_CAPT_COND_ASGN(pat, var)   conditional-on-success
 *   pat $ var   →  E_CAPT_IMMED_ASGN(pat, var)  immediate (during match)
 *
 * Examples (all SPITBOL Manual Ch.15 idioms):
 *   'ab' ? LEN(1) . X        → bind X='a' on overall match success
 *   epsilon . *Counter()     → call Counter() each time pattern reaches
 *                               this point (immediate-side-effect idiom)
 *   subj ? (LEN(1) . X | FENCE)
 *                              → tried first, FENCE seals on backtrack
 *
 * Andrew's `.sc` self-host has these at lp/rp 10/10 (his top scale);
 * we slot them at the SPITBOL Manual priority-12 position which is
 * tighter than `^`/exponent (pri 11) and looser than subscript (pri 15).
 * Same relative ordering as Andrew, same as SPITBOL Manual table.
 *
 * Left-associative chain — `pat . X $ Y` parses as `(pat . X) $ Y`.
 *
 * Note: the unary forms `.X` and `$X` (E_NAME and E_INDIRECT) live at
 * expr17 — they are different tokens (T_1DOT/T_1DOLLAR, no leading
 * whitespace) emitted by the FSM lexer's W{OP}W envelope rule.  The
 * binary forms here use T_2DOT/T_2DOLLAR (whitespace-enveloped, two-
 * operand context).
 */
expr12      : expr12 T_2DOLLAR expr15
                                { $$ = expr_binary(E_CAPT_IMMED_ASGN, $1, $3); }
            | expr12 T_2DOT    expr15
                                { $$ = expr_binary(E_CAPT_COND_ASGN,  $1, $3); }
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
 * T_CALL rule in expr17 — drain children into the E_IDX node, then
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
 * is just an E_NUL with no children.  Caller (T_CALL rule)
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
 * LS-4.b adds T_CALL call-form: `EQ(2+2, 4)` → E_FNC("EQ", ...).
 * The lexer has already classified the IDENT-followed-by-zero-space-(
 * as T_CALL (the "f(args) vs f (args)" disambiguation rule from
 * the goal); the matching ( is the next token in the stream. */
expr17      : T_CALL T_LPAREN exprlist T_RPAREN
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
            /* ---- LS-4.i.4 alt-eval ----
             * SPITBOL extension (Manual Ch.15 footnote):
             *   A = (LT(I,J) I , GT(I,J) J , "Same")
             * A parenthesised, comma-separated list of expressions is
             * evaluated left-to-right; the value of the first to succeed
             * is the value of the whole; if all fail, the whole fails.
             * Goal file calls this "alt-eval" — Andrew Koenig's `||`
             * lowered to this form, so dropping `||` and exposing the
             * primitive directly is the canonical replacement.
             *
             * Lower to E_VLIST n-ary node — IR kind already exists in
             * src/ir/ir.h:83 with this exact semantics.  Mirrors
             * snobol4.y:195 byte-for-byte.  Distinct from E_ALT
             * (pattern alternation, lazy at match time).
             *
             * LR(1) lookahead disambiguates the two paren-rules cleanly:
             *   T_LPAREN expr0  ·  { T_RPAREN | T_COMMA }
             * — T_RPAREN reduces to plain grouping; T_COMMA shifts into
             * the VLIST production.  Zero shift/reduce conflicts.    */
            | T_LPAREN expr0 T_COMMA exprlist_ne T_RPAREN
                                { EXPR_t *a = expr_new(E_VLIST);
                                  expr_add_child(a, $2);
                                  for (int i = 0; i < $4->nchildren; i++)
                                      expr_add_child(a, $4->children[i]);
                                  free($4->children); free($4);
                                  $$ = a; }
            /* Empty parens `()` — mirror snobol4.y:196.  Yields E_NUL,
             * the canonical null/empty expression.  Same semantics as
             * SPITBOL `()` evaluating to the null string.             */
            | T_LPAREN T_RPAREN
                                { $$ = expr_new(E_NUL); }
            | T_1PLUS  expr17
                                { $$ = expr_unary(E_PLS, $2); }
            | T_1MINUS expr17
                                { $$ = expr_unary(E_MNS, $2); }
            /* ---- LS-4.e: remaining SPITBOL unary operators ---- */
            /* *expr  — deferred evaluation / indirect pattern ref */
            | T_1STAR   expr17  { $$ = expr_unary(E_DEFER,       $2); }
            /* .expr  — name reference (returns name descriptor)   */
            | T_1DOT    expr17  { $$ = expr_unary(E_NAME,        $2); }
            /* $expr  — indirect (variable indirection)            */
            | T_1DOLLAR expr17  { $$ = expr_unary(E_INDIRECT,    $2); }
            /* @expr  — cursor position capture                    */
            | T_1AT     expr17  { $$ = expr_unary(E_CAPT_CURSOR, $2); }
            /* ~expr  — negate success/failure (NOT)               */
            | T_1TILDE  expr17  { $$ = expr_unary(E_NOT,         $2); }
            /* ?expr  — interrogation (null if succeeds, fail if fails) */
            | T_1QUEST  expr17  { $$ = expr_unary(E_INTERROGATE, $2); }
            /* &expr  — bare ampersand unary (OPSYN slot pri 2)    */
            | T_1AMP    expr17  { EXPR_t *_e = expr_unary(E_OPSYN, $2);
                                  _e->sval = strdup("&"); $$ = _e; }
            /* OPSYN-slot unaries — user-definable via OPSYN       */
            | T_1PERCENT expr17 { EXPR_t *_e = expr_unary(E_OPSYN, $2);
                                  _e->sval = strdup("%"); $$ = _e; }
            | T_1SLASH   expr17 { EXPR_t *_e = expr_unary(E_OPSYN, $2);
                                  _e->sval = strdup("/"); $$ = _e; }
            | T_1POUND   expr17 { EXPR_t *_e = expr_unary(E_OPSYN, $2);
                                  _e->sval = strdup("#"); $$ = _e; }
            | T_1PIPE    expr17 { EXPR_t *_e = expr_unary(E_OPSYN, $2);
                                  _e->sval = strdup("|"); $$ = _e; }
            | T_1EQUAL   expr17 { EXPR_t *_e = expr_unary(E_OPSYN, $2);
                                  _e->sval = strdup("="); $$ = _e; }
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
        case SC_T_CALL:         return T_CALL;
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
        case SC_T_IF:            return T_IF;
        case SC_T_ELSE:          return T_ELSE;
        case SC_T_WHILE:         return T_WHILE;
        case SC_T_DO:            return T_DO;
        case SC_T_FOR:           return T_FOR;
        case SC_T_SWITCH:        return T_SWITCH;
        case SC_T_CASE:          return T_CASE;
        case SC_T_DEFAULT:       return T_DEFAULT;
        case SC_T_BREAK:         return T_BREAK;
        case SC_T_CONTINUE:      return T_CONTINUE;
        case SC_T_GOTO:          return T_GOTO;
        case SC_T_FUNCTION:      return T_FUNCTION;
        case SC_T_RETURN:        return T_RETURN;
        case SC_T_FRETURN:       return T_FRETURN;
        case SC_T_NRETURN:       return T_NRETURN;
        case SC_T_STRUCT:        return T_STRUCT;
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
 *
 * LS-4.l fence/match/semantic/trace fix: also split E_SCAN(subj, pat)
 * out of `s->subject` into separate `s->subject = subj` /
 * `s->pattern = pat` slots so the runtime's pattern-match engine
 * fires.  Without this split, the runtime would evaluate E_SCAN as a
 * value (always succeeding) instead of doing the SNOBOL4 pattern
 * match.  Mirrors snobol4.y:248-270 byte-for-byte.
 *
 * Two split forms (mirror snobol4.y):
 *
 *   1. E_SCAN(subj, pat)
 *        -> s->subject = subj, s->pattern = pat
 *      Comes from the binary `?` operator: `subj ? pat` and the
 *      replace form `subj ? pat = repl` (where E_ASSIGN's lhs is
 *      the E_SCAN).
 *
 *   2. E_SEQ(name, rest...) where first child is a name-yielding
 *      atom (E_VAR / E_KEYWORD / E_QLIT / E_INDIRECT)
 *        -> s->subject = name, s->pattern = rest
 *      Comes from bare juxtaposition: in Snocone with space-as-
 *      concat, `s pat;` lexes as `IDENT(s) T_CONCAT IDENT(pat) ;`
 *      which lowers to E_SEQ(s, pat).  This is the SNOBOL4
 *      stmt-level "subject pattern" idiom and the runtime expects
 *      the same split.
 *
 * The split applies ONLY to the subject slot.  Replacement is left
 * unchanged: `result = subj ? pat` is E_ASSIGN(result, E_SCAN(subj,
 * pat)) and the replacement E_SCAN evaluates as a value (the
 * matched substring) — that path is correct as is, no split needed.
 */
static void sc_split_subject_pattern(EXPR_t **subj_io, EXPR_t **pat_io) {
    EXPR_t *subj = *subj_io;
    if (*pat_io || !subj) return;

    /* Form 1: E_SCAN(subj, pat) */
    if (subj->kind == E_SCAN && subj->nchildren == 2) {
        EXPR_t *new_subj = subj->children[0];
        EXPR_t *new_pat  = subj->children[1];
        free(subj->children);
        free(subj);
        *subj_io = new_subj;
        *pat_io  = new_pat;
        return;
    }

    /* Form 2: E_SEQ(name, rest...) where first child is name-like */
    if (subj->kind == E_SEQ && subj->nchildren >= 2) {
        EXPR_t *first = subj->children[0];
        if (first->kind == E_VAR || first->kind == E_KEYWORD ||
            first->kind == E_QLIT || first->kind == E_INDIRECT) {
            int nc = subj->nchildren - 1;
            EXPR_t *rest;
            if (nc == 1) {
                rest = subj->children[1];
            } else {
                rest = expr_new(E_SEQ);
                for (int i = 1; i < subj->nchildren; i++)
                    expr_add_child(rest, subj->children[i]);
            }
            /* Detach the children we kept; free the now-empty E_SEQ shell. */
            free(subj->children);
            free(subj);
            *subj_io = first;
            *pat_io  = rest;
            return;
        }
    }
}

static void sc_append_stmt(ScParseState *st, EXPR_t *top) {
    if (!top) return;
    /* LS-4.i.2 — a "real" stmt commit consumes any pending user labels:
     * label_decl already emitted a label-pad which chains forward by
     * SNOBOL4 semantics, so the labels semantically attach to this
     * stmt; subsequent loops should not re-capture them. */
    sc_pending_label_clear(st);
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
    /* LS-4.l: split E_SCAN/E_SEQ out of subject into subject+pattern.
     * Applies after E_ASSIGN-split so `subj ? pat = repl` (which
     * lowers to E_ASSIGN(E_SCAN(subj,pat), repl)) gets correctly
     * split into s->subject=subj, s->pattern=pat, s->replacement=repl. */
    sc_split_subject_pattern(&s->subject, &s->pattern);
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
 *  LS-4.f — control-flow lowering helpers
 *
 *  Architecture: emit-and-splice.  Control-flow head non-terminals
 *  (if_head, while_head) reduce on T_RPAREN of the head, snapshot
 *  st->code->tail at that moment, and emit no statements.  The body
 *  parses normally and appends its statements to st->code.  At the
 *  parent rule's final action, sc_finalize_* is called with the head
 *  struct (and, for if-else, the else_keyword snapshot).  These
 *  finalizers splice the cond-stmt and label-stmts into the correct
 *  positions in the linked list.
 *
 *  Why splice?  Because emitting in mid-rule MRAs causes Bison
 *  reduce/reduce conflicts when multiple matched/unmatched alternatives
 *  share a prefix (the four if-using rules with their per-rule MRAs at
 *  the same parser state).  Snapshot + final-action splice keeps
 *  emissions out of the contended region.
 *
 *  Naming: synthetic labels are "_Ltop_NNNN", "_Lelse_NNNN",
 *  "_Lend_NNNN" with NNNN a zero-padded 4-digit per-parse counter
 *  (st->label_seq).  The leading underscore guarantees no collision
 *  with user labels (which start with a letter).
 *
 *  Lowering shapes (using L1, L2 for clarity):
 *
 *    if (C) S
 *        →   subj=C  go.onfailure=L1
 *            <stmts of S>
 *            label=L1     (empty landing pad)
 *
 *    if (C) S1 else S2
 *        →   subj=C  go.onfailure=L1
 *            <stmts of S1>
 *            label=NULL  go.uncond=L2
 *            label=L1     (empty landing pad)
 *            <stmts of S2>
 *            label=L2     (empty landing pad)
 *
 *    while (C) S
 *        →   label=L1     (loop top — empty pad)
 *            subj=C  go.onfailure=L2
 *            <stmts of S>
 *            label=NULL  go.uncond=L1
 *            label=L2     (loop end — empty pad)
 *
 *  All landing-pad statements are STMT_t with subject=NULL, has_eq=0,
 *  replacement=NULL.  The downstream IR-walk treats them as no-ops
 *  with a label, exactly as SPITBOL `L1  ` would.
 * ========================================================================= */

static char *sc_label_new(ScParseState *st, const char *prefix) {
    char buf[64];
    snprintf(buf, sizeof buf, "%s_%04d", prefix, ++st->label_seq);
    return strdup(buf);
}

/* Snapshot helper: build an IfHead capturing cond and the current tail. */
static struct IfHead *sc_if_head_new(ScParseState *st, EXPR_t *cond) {
    struct IfHead *h = calloc(1, sizeof *h);
    h->cond        = cond;
    h->before_body = st->code->tail;       /* may be NULL */
    h->lineno      = st->ctx ? st->ctx->line : 0;
    return h;
}

static struct WhileHead *sc_while_head_new(ScParseState *st, EXPR_t *cond) {
    struct WhileHead *h = calloc(1, sizeof *h);
    h->cond        = cond;
    h->before_body = st->code->tail;
    h->lineno      = st->ctx ? st->ctx->line : 0;
    /* LS-4.i.2 — eager labels.  finalize_while will reuse these instead of
     * allocating fresh ones, so the body's break/continue stmts target
     * the same names finalize_while emits. */
    h->cont_label  = sc_label_new(st, "_Ltop");
    h->end_label   = sc_label_new(st, "_Lend");
    /* Push a loop frame; consumes pending_user_labels. */
    sc_loop_push(st, strdup(h->cont_label), strdup(h->end_label), 1, 0);
    return h;
}

/* LS-4.g — do_head: snapshot before the do-body is parsed. */
static struct DoHead *sc_do_head_new(ScParseState *st) {
    struct DoHead *h = calloc(1, sizeof *h);
    h->before_body = st->code->tail;
    h->lineno      = st->ctx ? st->ctx->line : 0;
    /* LS-4.i.2 — eager labels.  do/while's `continue` re-evaluates the
     * cond, so cont_label sits before the cond stmt (a fresh "_Lcont_NNNN"
     * that finalize_do_while will splice into the right spot). */
    h->cont_label  = sc_label_new(st, "_Lcont");
    h->end_label   = sc_label_new(st, "_Lend");
    sc_loop_push(st, strdup(h->cont_label), strdup(h->end_label), 1, 0);
    return h;
}

/* LS-4.g — for_head: called AFTER the init expr is emitted, so
 * before_loop snaps the tail that now includes the init stmt.
 *
 * LS-4.i.2 — pending user labels were already moved to st->stash_*
 * by for_lead's action (which fired on T_FOR before init parsed).
 * We pass `from_stash=1` to sc_loop_push so it picks them up there
 * instead of from the (now-cleared) pending list. */
static struct ForHead *sc_for_head_new(ScParseState *st, EXPR_t *cond, EXPR_t *step) {
    struct ForHead *h = calloc(1, sizeof *h);
    h->before_loop = st->code->tail;
    h->cond        = cond;
    h->step        = step;
    h->lineno      = st->ctx ? st->ctx->line : 0;
    /* LS-4.i.2 — eager labels.  for-loop's `continue` runs the step,
     * so cont_label sits just before the step stmt. */
    h->cont_label  = sc_label_new(st, "_Lcont");
    h->end_label   = sc_label_new(st, "_Lend");
    sc_loop_push(st, strdup(h->cont_label), strdup(h->end_label), 1, 1);
    return h;
}

/* LS-4.h — function-definition head.  Called when the parser reduces
 * `function NAME ( arglist )`.  Emits the DEFINE call statement and
 * the unconditional skip-the-body goto, snapshots the tail, and saves
 * cur_func_name on the head struct so sc_finalize_function can restore.
 *
 * Emits two STMT_t's:
 *   1. subject = E_FNC("DEFINE", E_QLIT("NAME(args)")), no goto
 *   2. subject = NULL, go.uncond = "NAME_end"  (a bare goto stmt)
 *
 * The body's stmts will then be appended via sc_append_stmt() in the
 * usual way.  sc_finalize_function patches in:
 *   - The entry-point label "NAME" — spliced as a label-only pad
 *     immediately AFTER the goto stmt (i.e. at the body's first
 *     position).  Because labels in SNOBOL4 attach to the next
 *     non-empty stmt, this is equivalent to labelling the body's
 *     first stmt with "NAME".
 *   - The end label "NAME_end" — appended at the end as a label pad.
 */
static struct FuncHead *sc_func_head_new(ScParseState *st, char *name, char *argstr) {
    struct FuncHead *h = calloc(1, sizeof *h);
    h->name      = strdup(name);
    /* end_label: "NAME_end" */
    int elen = strlen(name) + 5;
    h->end_label = malloc(elen);
    snprintf(h->end_label, elen, "%s_end", name);
    h->prev_func = st->cur_func_name;     /* save (handles "no nested fn" defensively) */
    h->lineno    = st->ctx ? st->ctx->line : 0;

    /* ---- 1. emit DEFINE('NAME(args)') ---- */
    int slen = strlen(name) + 1 + strlen(argstr) + 2;     /* NAME(args) + NUL */
    char *defarg = malloc(slen);
    snprintf(defarg, slen, "%s(%s)", name, argstr);
    EXPR_t *qarg = expr_new(E_QLIT);
    qarg->sval   = defarg;
    EXPR_t *def_call = expr_new(E_FNC);
    def_call->sval   = strdup("DEFINE");
    expr_add_child(def_call, qarg);
    sc_append_stmt(st, def_call);   /* appends as bare-expr stmt */

    /* ---- 2. emit :(NAME_end) skip-the-body goto ---- */
    STMT_t *skip = sc_make_goto_uncond_stmt(st, strdup(h->end_label));
    sc_append_chain(st, skip, skip);

    /* Snapshot tail AFTER the goto, so the body splices cleanly. */
    h->after_goto = st->code->tail;

    /* Mark the function context for return/freturn/nreturn */
    st->cur_func_name = h->name;
    return h;
}

/* LS-4.h — sc_finalize_function: splice entry-point label and append end label.
 *
 * Body has already been parsed and appended.  st->code looks like:
 *
 *   ... pre-func stmts ... | DEFINE(...) | goto :(NAME_end) | body0 | body1 | ... | tail
 *                                          ^ h->after_goto                                  ^
 *
 * We need:
 *   1. A label-pad "NAME" between h->after_goto and body0 (so body0 is
 *      the entry point).  If the body is empty (h->after_goto == tail),
 *      append the label pad at the end (it still serves as a label).
 *   2. A label-pad "NAME_end" appended at the very end.
 *
 * Restore cur_func_name to the saved prev_func.
 */
static void sc_finalize_function(ScParseState *st, struct FuncHead *h) {
    /* Entry-point label */
    STMT_t *entry = sc_make_label_stmt(st, strdup(h->name));
    sc_splice_after(st, h->after_goto, entry, entry);

    /* End label (skip target) */
    STMT_t *endpad = sc_make_label_stmt(st, strdup(h->end_label));
    sc_append_chain(st, endpad, endpad);

    /* Restore enclosing function context */
    st->cur_func_name = h->prev_func;

    free(h->name);
    free(h->end_label);
    free(h);
}

/* LS-4.h — sc_append_return: emit `cur_func_name = retval :(RETURN)`
 * (or, with no retval, just `:(RETURN)` as a bare goto-only stmt).
 *
 * SPITBOL/SNOBOL4 functions return their value by assigning to a
 * variable named after the function and then branching to the
 * pseudo-label RETURN.  Snocone's `return E;` lowers to that idiom;
 * `return;` lowers to a bare `:(RETURN)` (the function's slot retains
 * whatever previous value it was set to in the body, or null if
 * never assigned).
 *
 * Outside a function (cur_func_name == NULL), `return` is still
 * legal — it just lowers to `:(RETURN)` and behaves as in SPITBOL
 * (a top-level `:(RETURN)` is a runtime error or fallthrough,
 * depending on dialect).  We do not enforce structural validity here.
 */
static void sc_append_return(ScParseState *st, EXPR_t *retval) {
    sc_pending_label_clear(st);
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    if (retval && st->cur_func_name) {
        /* fname = retval :(RETURN) */
        EXPR_t *lhs = expr_new(E_VAR);
        lhs->sval   = strdup(st->cur_func_name);
        s->subject     = lhs;
        s->replacement = retval;
        s->has_eq      = 1;
    } else if (retval) {
        /* `return E;` outside a function — keep E as bare-expr subject;
         * still emit the RETURN goto so it's syntactically a return.   */
        s->subject = retval;
    }
    s->go = sgoto_new();
    s->go->uncond = strdup("RETURN");
    if (!st->code->head) st->code->head = st->code->tail = s;
    else { st->code->tail->next = s; st->code->tail = s; }
}

static void sc_append_freturn(ScParseState *st) {
    sc_pending_label_clear(st);
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    s->go     = sgoto_new();
    s->go->uncond = strdup("FRETURN");
    if (!st->code->head) st->code->head = st->code->tail = s;
    else { st->code->tail->next = s; st->code->tail = s; }
}

static void sc_append_nreturn(ScParseState *st) {
    sc_pending_label_clear(st);
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    s->go     = sgoto_new();
    s->go->uncond = strdup("NRETURN");
    if (!st->code->head) st->code->head = st->code->tail = s;
    else { st->code->tail->next = s; st->code->tail = s; }
}

/* Build a label-only landing-pad STMT (subject=NULL).  Takes ownership
 * of `label`.  Does NOT link it into st->code — caller does that. */
static STMT_t *sc_make_label_stmt(ScParseState *st, char *label) {
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    s->label  = label;
    return s;
}

/* Build a STMT whose subject is `cond` and whose go.onfailure points at
 * `fail_target`.  Takes ownership of both.
 *
 * LS-4.l fix — applies the same E_SCAN/E_SEQ split used by
 * sc_append_stmt so `if (subj ? pat) {…}`, `while (subj ? pat) {…}`,
 * etc. drive the runtime's pattern-match engine instead of evaluating
 * E_SCAN as a value (which always succeeds and breaks the if/while
 * failure-driven branch). */
static STMT_t *sc_make_cond_fail_stmt(ScParseState *st, EXPR_t *cond, char *fail_target, int lineno) {
    STMT_t *s = stmt_new();
    s->lineno = lineno;
    s->stno   = ++st->code->nstmts;
    s->subject = cond;
    sc_split_subject_pattern(&s->subject, &s->pattern);
    s->go      = sgoto_new();
    s->go->onfailure = fail_target;
    return s;
}

/* Build a STMT carrying only an unconditional goto.  Takes ownership. */
static STMT_t *sc_make_goto_uncond_stmt(ScParseState *st, char *target) {
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    s->go     = sgoto_new();
    s->go->uncond = target;
    return s;
}

/* LS-4.i.1 — sc_emit_label_pad: append a label-only no-op stmt.
 *
 * Used by the `label_decl : T_IDENT T_COLON` rule.  Produces a STMT_t
 * with `label = name`, no subject, no replacement, no goto.  In
 * SNOBOL4, such a stmt is a valid branch target and chains the
 * label to the next non-empty stmt — exactly matching what the user
 * wrote: `top: x = 1;` should make `top` reach `x = 1` semantically.
 *
 * LS-4.i.2 — also tracks `name` in pending_user_labels so that if the
 * next thing parsed is a loop head, the loop frame can record this
 * label as a `break/continue LABEL;` target.  Stacked labels accumulate
 * (a: b: while(...) — both a and b name the same loop). */
static void sc_emit_label_pad(ScParseState *st, char *label) {
    STMT_t *pad = sc_make_label_stmt(st, strdup(label));
    sc_append_chain(st, pad, pad);
    sc_pending_label_add(st, label);
}

/* LS-4.i.1 — sc_append_goto_label: emit `:(target)` unconditional goto.
 *
 * Used by the `T_GOTO T_IDENT T_SEMICOLON` rule in simple_stmt.  The
 * target is a user-written label name; we duplicate it (since the
 * caller still owns the original IDENT string for free()).
 *
 * LS-4.i.2 — clears pending user labels (a stmt commit, like sc_append_stmt). */
static void sc_append_goto_label(ScParseState *st, char *target) {
    sc_pending_label_clear(st);
    STMT_t *g = sc_make_goto_uncond_stmt(st, strdup(target));
    sc_append_chain(st, g, g);
}

/* Splice helpers — operate on st->code's linked list.
 *
 * sc_splice_after(st, anchor, chain_head, chain_tail)
 *   Inserts the chain [chain_head..chain_tail] (linked via next pointers)
 *   right after `anchor`.  If `anchor` is NULL, prepends to st->code->head.
 *   Updates st->code->tail if appending at end.
 */
static void sc_splice_after(ScParseState *st, STMT_t *anchor,
                            STMT_t *chain_head, STMT_t *chain_tail) {
    if (!chain_head) return;
    if (!chain_tail) chain_tail = chain_head;
    if (anchor) {
        chain_tail->next = anchor->next;
        anchor->next     = chain_head;
        if (st->code->tail == anchor) st->code->tail = chain_tail;
    } else {
        chain_tail->next = st->code->head;
        st->code->head   = chain_head;
        if (!st->code->tail) st->code->tail = chain_tail;
    }
}

/* Append a chain to the end of st->code. */
static void sc_append_chain(ScParseState *st, STMT_t *chain_head, STMT_t *chain_tail) {
    if (!chain_head) return;
    if (!chain_tail) chain_tail = chain_head;
    if (!st->code->head) st->code->head = chain_head;
    else                 st->code->tail->next = chain_head;
    st->code->tail = chain_tail;
}

/* Finalize `if (cond) body` (no else).
 *
 *   <existing stmts up to before_body>
 *   cond  :F(Lend)            <- spliced after before_body
 *   <body stmts>
 *   Lend                      <- appended at end
 */
static void sc_finalize_if_no_else(ScParseState *st, struct IfHead *h) {
    char   *Lend       = sc_label_new(st, "_Lend");
    STMT_t *cond_stmt  = sc_make_cond_fail_stmt(st, h->cond, strdup(Lend), h->lineno);
    STMT_t *end_label  = sc_make_label_stmt(st, Lend);
    sc_splice_after(st, h->before_body, cond_stmt, cond_stmt);
    sc_append_chain(st, end_label, end_label);
    free(h);
}

/* Finalize `if (cond) S1 else S2`.
 *
 *   <existing stmts up to before_body>
 *   cond  :F(Lelse)           <- spliced after before_body
 *   <S1 stmts up to before_else>
 *   :(Lend)                   <- spliced after before_else, before S2 begins
 *   Lelse                     <- spliced after the goto
 *   <S2 stmts>
 *   Lend                      <- appended at end
 *
 * before_else snapshots st->code->tail at the moment T_ELSE is
 * recognised (after S1 was fully parsed).  The else-body is whatever
 * was appended after that point.  If S1 was empty (e.g. `if (c) ; else ...`)
 * before_else == h->before_body (or NULL); the splice math still works.
 */
static void sc_finalize_if_else(ScParseState *st, struct IfHead *h, STMT_t *before_else) {
    char   *Lelse     = sc_label_new(st, "_Lelse");
    char   *Lend      = sc_label_new(st, "_Lend");
    STMT_t *cond_stmt = sc_make_cond_fail_stmt(st, h->cond, strdup(Lelse), h->lineno);
    STMT_t *goto_end  = sc_make_goto_uncond_stmt(st, strdup(Lend));
    STMT_t *else_pad  = sc_make_label_stmt(st, Lelse);
    STMT_t *end_pad   = sc_make_label_stmt(st, Lend);
    /* (1) Splice cond_stmt right after before_body (i.e. before S1). */
    sc_splice_after(st, h->before_body, cond_stmt, cond_stmt);
    /* (2) Build chain [goto_end -> else_pad].  Splice anchor: the last
     *     stmt of S1.  When S1 was empty, before_else == h->before_body,
     *     so after step (1) the correct anchor is cond_stmt itself
     *     (otherwise we'd insert before cond, not after).  Otherwise
     *     before_else is the last S1 stmt and remains valid (splicing
     *     cond before S1 only edited h->before_body's next link). */
    STMT_t *anchor = (before_else == h->before_body) ? cond_stmt : before_else;
    goto_end->next = else_pad;
    sc_splice_after(st, anchor, goto_end, else_pad);
    /* (3) Append end_pad at the very end. */
    sc_append_chain(st, end_pad, end_pad);
    free(h);
}

/* Finalize `while (cond) body`.
 *
 *   <existing stmts up to before_body>
 *   Ltop                      <- spliced after before_body
 *   cond  :F(Lend)            <- next in chain
 *   <body stmts>
 *   :(Ltop)                   <- appended at end
 *   Lend                      <- appended at end
 */
static void sc_finalize_while(ScParseState *st, struct WhileHead *h) {
    /* LS-4.i.2 — reuse the eager labels allocated by sc_while_head_new
     * so any `break`/`continue` emitted inside the body targets the
     * same names we lay down here. */
    char   *Ltop      = h->cont_label;       /* take ownership */
    char   *Lend      = h->end_label;
    STMT_t *top_pad   = sc_make_label_stmt(st, Ltop);
    STMT_t *cond_stmt = sc_make_cond_fail_stmt(st, h->cond, strdup(Lend), h->lineno);
    STMT_t *goto_top  = sc_make_goto_uncond_stmt(st, strdup(Ltop));
    STMT_t *end_pad   = sc_make_label_stmt(st, Lend);
    /* (1) Splice [top_pad -> cond_stmt] after before_body. */
    top_pad->next = cond_stmt;
    sc_splice_after(st, h->before_body, top_pad, cond_stmt);
    /* (2) Append [goto_top -> end_pad] at the end. */
    goto_top->next = end_pad;
    sc_append_chain(st, goto_top, end_pad);
    sc_loop_pop(st);
    free(h);
}

/* =========================================================================
 *  LS-4.g — do/while and for lowering helpers
 *
 *  do { S } while (C);
 *      →   Ltop                      <- spliced after before_body
 *          <S stmts>
 *          C  :S(Ltop)               <- appended (success loops back)
 *          Lend                      <- appended
 *
 *  for (init; C; step) S
 *      →   <init stmt>               <- already in list (emitted by for_head rule)
 *          Ltop                      <- spliced after before_loop (= after init)
 *          C  :F(Lend)               <- next in chain
 *          <S stmts>
 *          <step stmt>               <- appended
 *          :(Ltop)                   <- appended
 *          Lend                      <- appended
 *
 *  Note: do/until removed — Snocone follows C's loop forms exactly
 *  (while and do/while only).  Lon directive session 2026-04-30 #12.
 * ========================================================================= */

/* Build a STMT whose subject is `cond` and whose go.onsuccess points at
 * `succ_target`.  Takes ownership of both.
 *
 * LS-4.l fix — same E_SCAN/E_SEQ split as sc_make_cond_fail_stmt so
 * `do { ... } while (subj ? pat);` drives the pattern-match engine
 * (without the split, the cond evaluates E_SCAN as a value, always
 * succeeding, turning do/while into an infinite loop). */
static STMT_t *sc_make_cond_succ_stmt(ScParseState *st, EXPR_t *cond, char *succ_target, int lineno) {
    STMT_t *s = stmt_new();
    s->lineno  = lineno;
    s->stno    = ++st->code->nstmts;
    s->subject = cond;
    sc_split_subject_pattern(&s->subject, &s->pattern);
    s->go      = sgoto_new();
    s->go->onsuccess = succ_target;
    return s;
}

static void sc_finalize_do_while(ScParseState *st, struct DoHead *h, EXPR_t *cond) {
    /* LS-4.i.2 — eager labels.  Ltop is a fresh "_Ltop_NNNN"; cont_label
     * was allocated eagerly by sc_do_head_new for use by `continue;` stmts
     * inside the body.  For do/while, `continue` must re-evaluate cond,
     * so the cont_label sits BEFORE the cond stmt.  Lazy emit: only
     * splice the cont pad if the body actually referenced it (frame's
     * cont_used flag set by sc_append_continue) — keeps the IR shape
     * unchanged for do/while bodies that don't use continue. */
    char   *Ltop      = sc_label_new(st, "_Ltop");
    char   *Lcont     = h->cont_label;       /* take ownership */
    char   *Lend      = h->end_label;
    int     cont_used = st->loop_top ? st->loop_top->cont_used : 0;
    STMT_t *top_pad   = sc_make_label_stmt(st, Ltop);
    STMT_t *cond_stmt = sc_make_cond_succ_stmt(st, cond, strdup(Ltop), h->lineno);
    STMT_t *end_pad   = sc_make_label_stmt(st, Lend);
    /* Splice Ltop-pad right before the do-body. */
    sc_splice_after(st, h->before_body, top_pad, top_pad);
    if (cont_used) {
        STMT_t *cont_pad = sc_make_label_stmt(st, Lcont);
        cont_pad->next  = cond_stmt;
        cond_stmt->next = end_pad;
        sc_append_chain(st, cont_pad, end_pad);
    } else {
        free(Lcont);   /* no continue used; pad and label both unused */
        cond_stmt->next = end_pad;
        sc_append_chain(st, cond_stmt, end_pad);
    }
    sc_loop_pop(st);
    free(h);
}

static void sc_finalize_for(ScParseState *st, struct ForHead *h) {
    /* LS-4.i.2 — eager labels.  Ltop is fresh; cont_label was allocated
     * eagerly by sc_for_head_new for use by `continue;` stmts inside the
     * body.  For for-loop, `continue` must run the step before re-testing
     * cond, so the cont_label sits just before the step stmt.  Lazy emit:
     * only splice the cont pad if the body actually referenced it
     * (frame's cont_used flag set by sc_append_continue) — keeps the IR
     * shape unchanged for bodies that don't use continue. */
    char   *Ltop      = sc_label_new(st, "_Ltop");
    char   *Lcont     = h->cont_label;       /* take ownership */
    char   *Lend      = h->end_label;
    int     cont_used = st->loop_top ? st->loop_top->cont_used : 0;
    STMT_t *top_pad   = sc_make_label_stmt(st, Ltop);
    STMT_t *cond_stmt = sc_make_cond_fail_stmt(st, h->cond, strdup(Lend), h->lineno);
    STMT_t *step_stmt = stmt_new();
    step_stmt->lineno  = h->lineno;
    step_stmt->stno    = ++st->code->nstmts;
    step_stmt->subject = h->step;
    STMT_t *goto_top  = sc_make_goto_uncond_stmt(st, strdup(Ltop));
    STMT_t *end_pad   = sc_make_label_stmt(st, Lend);
    /* Splice [top_pad -> cond_stmt] right after before_loop (after init). */
    top_pad->next = cond_stmt;
    sc_splice_after(st, h->before_loop, top_pad, cond_stmt);
    if (cont_used) {
        STMT_t *cont_pad = sc_make_label_stmt(st, Lcont);
        /* Append cont_pad, step, goto Ltop, Lend. */
        cont_pad->next  = step_stmt;
        step_stmt->next = goto_top;
        goto_top->next  = end_pad;
        sc_append_chain(st, cont_pad, end_pad);
    } else {
        free(Lcont);   /* no continue used; label allocated but unused */
        /* Append step, goto Ltop, Lend (original LS-4.g shape). */
        step_stmt->next = goto_top;
        goto_top->next  = end_pad;
        sc_append_chain(st, step_stmt, end_pad);
    }
    sc_loop_pop(st);
    free(h);
}

/* =========================================================================
 *  LS-4.i.2 — break / continue support
 *
 *  Lowering shapes:
 *
 *    break ;            ->   :(end_label_of_innermost_frame)
 *    break LABEL ;      ->   :(end_label_of_frame_with_user_label_LABEL)
 *    continue ;         ->   :(cont_label_of_innermost_loop_frame)
 *    continue LABEL ;   ->   :(cont_label_of_loop_frame_with_user_label_LABEL)
 *
 *  The cont/end labels are allocated eagerly by sc_*_head_new (so that
 *  break/continue stmts emitted DURING body parsing target them by name)
 *  and reused in sc_finalize_*.  user_labels[] on each frame holds any
 *  labels the user attached just before the loop (`a: while(...)`); both
 *  break a; and break LABEL; (named) and break; (innermost) work.
 *
 *  Out-of-context break/continue and unresolved-label cases call sc_error.
 * ========================================================================= */

static void sc_pending_label_add(ScParseState *st, const char *name) {
    if (st->pending_user_labels_count >= st->pending_user_labels_cap) {
        int newcap = st->pending_user_labels_cap ? st->pending_user_labels_cap * 2 : 4;
        st->pending_user_labels = realloc(st->pending_user_labels, newcap * sizeof(char *));
        st->pending_user_labels_cap = newcap;
    }
    st->pending_user_labels[st->pending_user_labels_count++] = strdup(name);
}

static void sc_pending_label_clear(ScParseState *st) {
    for (int i = 0; i < st->pending_user_labels_count; i++) free(st->pending_user_labels[i]);
    st->pending_user_labels_count = 0;
}

/* Move pending list to the one-slot stash (used by for_lead, since for's
 * init expr emission would otherwise clear pending before sc_for_head_new
 * fires).  Releases any prior stash contents (defensive — should be empty
 * unless someone wrote a top-level `for (for(...; ...; ...);  ...; ...)`
 * which is grammatically valid but unusual). */
static void sc_pending_to_stash(ScParseState *st) {
    /* Free any leftover stash from a prior for_lead that didn't get
     * consumed (shouldn't happen in well-formed input — defensive). */
    for (int i = 0; i < st->stash_for_pending_labels_count; i++) free(st->stash_for_pending_labels[i]);
    free(st->stash_for_pending_labels);
    st->stash_for_pending_labels       = st->pending_user_labels;
    st->stash_for_pending_labels_count = st->pending_user_labels_count;
    st->pending_user_labels       = NULL;
    st->pending_user_labels_count = 0;
    st->pending_user_labels_cap   = 0;
}

/* Push a new LoopFrame onto the loop stack.  Takes ownership of cont_label
 * and end_label.  user_labels are sourced from either pending_user_labels
 * (from_stash=0) or stash_for_pending_labels (from_stash=1, used by for-loop
 * since for_lead moved them to the stash before init parsed). */
static void sc_loop_push(ScParseState *st, char *cont_label, char *end_label, int is_loop, int from_stash) {
    LoopFrame *f = calloc(1, sizeof *f);
    f->cont_label = cont_label;
    f->end_label  = end_label;
    f->is_loop    = is_loop;
    f->outer      = st->loop_top;
    /* Capture user labels: take them by ownership from pending or stash. */
    if (from_stash) {
        f->user_labels       = st->stash_for_pending_labels;
        f->user_labels_count = st->stash_for_pending_labels_count;
        st->stash_for_pending_labels       = NULL;
        st->stash_for_pending_labels_count = 0;
    } else {
        f->user_labels       = st->pending_user_labels;
        f->user_labels_count = st->pending_user_labels_count;
        st->pending_user_labels       = NULL;
        st->pending_user_labels_count = 0;
        st->pending_user_labels_cap   = 0;
    }
    st->loop_top = f;
}

static void sc_loop_pop(ScParseState *st) {
    LoopFrame *f = st->loop_top;
    if (!f) return;
    st->loop_top = f->outer;
    free(f->cont_label);
    free(f->end_label);
    for (int i = 0; i < f->user_labels_count; i++) free(f->user_labels[i]);
    free(f->user_labels);
    free(f);
}

/* Search the loop stack innermost-first for a frame whose user_labels
 * contains `name`.  If want_loop is set, switch frames are skipped (LS-4.i.3
 * will make is_loop=0 frames possible; for LS-4.i.2 every frame is a loop). */
static LoopFrame *sc_loop_find_by_user_label(ScParseState *st, const char *name, int want_loop) {
    for (LoopFrame *f = st->loop_top; f; f = f->outer) {
        if (want_loop && !f->is_loop) continue;
        for (int i = 0; i < f->user_labels_count; i++) {
            if (strcmp(f->user_labels[i], name) == 0) return f;
        }
    }
    return NULL;
}

/* Find the innermost frame matching want_loop (1 = loop only, 0 = any). */
static LoopFrame *sc_loop_find_innermost(ScParseState *st, int want_loop) {
    for (LoopFrame *f = st->loop_top; f; f = f->outer) {
        if (want_loop && !f->is_loop) continue;
        return f;
    }
    return NULL;
}

/* `break;` (NULL label) jumps to the innermost loop or switch frame's end.
 * `break LABEL;` jumps to the named frame's end.
 * Out-of-context or unresolved labels emit a parse error and emit no stmt. */
static void sc_append_break(ScParseState *st, char *user_label) {
    LoopFrame *f = user_label
        ? sc_loop_find_by_user_label(st, user_label, 0)   /* break can target switch too */
        : sc_loop_find_innermost(st, 0);
    if (!f) {
        if (user_label) {
            char buf[256];
            snprintf(buf, sizeof buf, "break: no enclosing loop or switch labeled '%s'", user_label);
            sc_error(st, buf);
        } else {
            sc_error(st, "break outside of loop or switch");
        }
        sc_pending_label_clear(st);
        return;
    }
    sc_pending_label_clear(st);
    STMT_t *g = sc_make_goto_uncond_stmt(st, strdup(f->end_label));
    sc_append_chain(st, g, g);
}

/* `continue;` (NULL label) jumps to the innermost LOOP frame's cont target.
 * `continue LABEL;` jumps to the named loop frame's cont target.
 * Switch frames are not valid `continue` targets. */
static void sc_append_continue(ScParseState *st, char *user_label) {
    LoopFrame *f = user_label
        ? sc_loop_find_by_user_label(st, user_label, 1)   /* continue requires loop */
        : sc_loop_find_innermost(st, 1);
    if (!f) {
        if (user_label) {
            char buf[256];
            snprintf(buf, sizeof buf, "continue: no enclosing loop labeled '%s'", user_label);
            sc_error(st, buf);
        } else {
            sc_error(st, "continue outside of loop");
        }
        sc_pending_label_clear(st);
        return;
    }
    f->cont_used = 1;          /* tells finalize_* to emit the Lcont pad (do/for) */
    sc_pending_label_clear(st);
    STMT_t *g = sc_make_goto_uncond_stmt(st, strdup(f->cont_label));
    sc_append_chain(st, g, g);
}

/* =========================================================================
 *  LS-4.i.3 — switch / case / default support
 *
 *  Lowering shape (per Q6 — modern no-fall-through):
 *
 *    switch (e) { case v1: A; case v2: B; default: D; }
 *
 *      tmp = e
 *      IDENT(tmp, v1)        :S(_Lcase_0001)
 *      IDENT(tmp, v2)        :S(_Lcase_0002)
 *                            :(_Ldefault_0003)        <- fallthrough = default jump
 *      _Lcase_0001  A        :(_Lend_0004)
 *      _Lcase_0002  B        :(_Lend_0004)
 *      _Ldefault_0003 D                                <- last clause: no goto
 *      _Lend_0004
 *
 *  If no `default:` clause is present, the dispatch's catch-all goto
 *  targets `_Lend` directly.  Empty `case 1: case 2: stmts;` (stacked
 *  labels with no body between) emits two label pads but no implicit
 *  break between them — both labels chain forward to the body via
 *  SNOBOL4 label-pad chaining semantics.  Detection: when a new
 *  case-or-default label fires, if `code->tail == last_case_label_tail`
 *  no body has been appended since the previous label, suppress the
 *  implicit `:(_Lend)` goto.
 *
 *  `break;` inside the body finds the switch's LoopFrame (is_loop=0)
 *  via sc_loop_find_innermost(want_loop=0) — already-implemented in
 *  LS-4.i.2.  `continue;` skips switch frames (want_loop=1) — also
 *  already-implemented in LS-4.i.2.  All forward-compat groundwork
 *  is now consumed.
 * ========================================================================= */

/* Architecture note re: head/finalize symmetry with while/for/dowhile:
 *
 *   sc_switch_head_new   — eager: emits `tmp = e;` immediately, allocates
 *                           all labels, snapshots after_tmp_assign, pushes
 *                           the frame.  Returns SwitchHead for the parent
 *                           rule to thread through to finalize.
 *
 *   sc_switch_case_label — fires DURING body parsing on each `case v:` head.
 *                           Emits implicit-break-to-end if previous case
 *                           had body, then emits the case-label pad, records
 *                           the (label, value) pair on the SwitchHead's
 *                           cases[] array (consumed by finalize for dispatch
 *                           construction), and snapshots the new last-case-
 *                           label-tail for the next case's break-suppression
 *                           check.
 *
 *   sc_switch_default_label — same as case but with no value (NULL marker)
 *                              and the _Ldefault_NNNN spelling.  Sets
 *                              has_default=1 — multiple defaults are a
 *                              parse error.
 *
 *   sc_finalize_switch   — builds the dispatch chain from cases[] (n
 *                           IDENT(tmp, vN) :S(case_label) stmts plus a
 *                           trailing :(default-or-end) goto), splices it
 *                           after after_tmp_assign, appends the _Lend
 *                           pad at the very end, pops the loop frame,
 *                           frees the SwitchHead and its cases[] array.
 */

static void sc_switch_cases_grow(struct SwitchHead *h) {
    if (h->cases_count >= h->cases_cap) {
        int newcap = h->cases_cap ? h->cases_cap * 2 : 4;
        h->cases = realloc(h->cases, newcap * sizeof *h->cases);
        h->cases_cap = newcap;
    }
}

static struct SwitchHead *sc_switch_head_new(ScParseState *st, EXPR_t *disc) {
    struct SwitchHead *h = calloc(1, sizeof *h);
    h->disc          = disc;
    h->lineno        = st->ctx ? st->ctx->line : 0;
    h->prev_switch   = st->cur_switch;
    /* Allocate synthetic tmp variable name + the three switch-control labels.
     * Order matters for predictable label numbering: tmp-var, end, default
     * are all allocated up front; per-case labels come later as bodies parse. */
    char buf[64];
    snprintf(buf, sizeof buf, "_Lswitch_t_%04d", ++st->label_seq);
    h->tmp_name      = strdup(buf);
    h->end_label     = sc_label_new(st, "_Lend");
    h->default_label = sc_label_new(st, "_Ldefault");
    h->has_default   = 0;
    /* Emit `tmp = disc;` as a regular assignment statement — sc_append_stmt
     * handles the E_ASSIGN-to-subject-and-replacement split. */
    EXPR_t *lhs = expr_new(E_VAR);
    lhs->sval   = strdup(h->tmp_name);
    EXPR_t *assign = expr_new(E_ASSIGN);
    expr_add_child(assign, lhs);
    expr_add_child(assign, disc);   /* takes ownership of disc */
    sc_append_stmt(st, assign);
    /* Snapshot tail AFTER the tmp-assign — this is the splice anchor for
     * the dispatch chain that finalize will build. */
    h->after_tmp_assign     = st->code->tail;
    h->last_case_label_tail = NULL;   /* no case label emitted yet */
    /* Push loop frame with is_loop=0 — break finds it, continue skips. */
    sc_loop_push(st, strdup(h->end_label) /* unused for switch */,
                 strdup(h->end_label), 0 /* is_loop */, 0 /* from_stash */);
    /* Activate this switch as innermost. */
    st->cur_switch = h;
    return h;
}

/* Find the current switch's SwitchHead.  We don't carry it on the LoopFrame
 * (that would couple the LS-4.i.2 frame layout to switch internals), so the
 * grammar instead routes each case_or_default_label action through st via
 * a small per-state pointer.  Set in sc_switch_head_new, cleared in finalize.
 *
 * For nested switches (allowed: `switch (a) { case 1: switch (b) { ... } }`)
 * the inner switch's head saves the outer pointer onto its struct and
 * restores at finalize.  Both switches' cases[] arrays remain independent.
 */
/* Implemented via st->cur_switch field added below. */

/* Emit the implicit-break goto (if needed) right before a case-or-default
 * label is laid down.  "Needed" means: we are not the very first case
 * (last_case_label_tail != NULL) AND a body stmt has been appended since
 * the previous case label (code->tail != last_case_label_tail). */
static void sc_switch_emit_implicit_break(ScParseState *st, struct SwitchHead *h) {
    if (!h->last_case_label_tail) return;            /* first case in the switch */
    if (st->code->tail == h->last_case_label_tail) return;  /* empty body */
    STMT_t *g = sc_make_goto_uncond_stmt(st, strdup(h->end_label));
    sc_append_chain(st, g, g);
}

static void sc_switch_case_label(ScParseState *st, EXPR_t *value) {
    struct SwitchHead *h = st->cur_switch;
    if (!h) {
        sc_error(st, "case label outside of switch");
        /* `value` leaks on this error path — matches codebase convention
         * (no general expr_free helper exists; cleanup is end-of-parse). */
        (void)value;
        return;
    }
    sc_switch_emit_implicit_break(st, h);
    /* Allocate fresh case label and emit the label pad. */
    char *case_label = sc_label_new(st, "_Lcase");
    STMT_t *pad      = sc_make_label_stmt(st, strdup(case_label));
    sc_append_chain(st, pad, pad);
    /* Record dispatch entry (consumed by finalize). */
    sc_switch_cases_grow(h);
    h->cases[h->cases_count].case_label = case_label;
    h->cases[h->cases_count].value      = value;
    h->cases_count++;
    h->last_case_label_tail = st->code->tail;
    /* A case label is a real "stmt commit" — clear pending user labels so
     * stacked `a: case 1:` (if anyone writes that) doesn't mis-attach. */
    sc_pending_label_clear(st);
}

static void sc_switch_default_label(ScParseState *st) {
    struct SwitchHead *h = st->cur_switch;
    if (!h) {
        sc_error(st, "default label outside of switch");
        return;
    }
    if (h->has_default) {
        sc_error(st, "duplicate default label in switch");
        return;
    }
    h->has_default = 1;
    sc_switch_emit_implicit_break(st, h);
    /* Use the pre-allocated _Ldefault label from sc_switch_head_new. */
    STMT_t *pad = sc_make_label_stmt(st, strdup(h->default_label));
    sc_append_chain(st, pad, pad);
    /* Record default as a case-entry with NULL value.  Dispatch chain
     * skips NULL-value entries (they carry no IDENT() probe — default is
     * the trailing catch-all goto). */
    sc_switch_cases_grow(h);
    h->cases[h->cases_count].case_label = strdup(h->default_label);
    h->cases[h->cases_count].value      = NULL;
    h->cases_count++;
    h->last_case_label_tail = st->code->tail;
    sc_pending_label_clear(st);
}

static void sc_finalize_switch(ScParseState *st, struct SwitchHead *h) {
    /* Build the dispatch chain.  For each case entry with a non-NULL value:
     *   IDENT(tmp, value)   :S(case_label)
     * Followed by a single uncond goto to default_label (if has_default)
     * or end_label (if no default). */
    STMT_t *chain_head = NULL;
    STMT_t *chain_tail = NULL;
    for (int i = 0; i < h->cases_count; i++) {
        if (!h->cases[i].value) continue;   /* default entry — no probe */
        EXPR_t *probe = expr_new(E_FNC);
        probe->sval   = strdup("IDENT");
        EXPR_t *tmp_ref = expr_new(E_VAR);
        tmp_ref->sval   = strdup(h->tmp_name);
        expr_add_child(probe, tmp_ref);
        expr_add_child(probe, h->cases[i].value);   /* takes ownership */
        h->cases[i].value = NULL;                   /* prevent double-free */
        STMT_t *s = sc_make_cond_succ_stmt(st, probe,
                                           strdup(h->cases[i].case_label),
                                           h->lineno);
        if (!chain_head) chain_head = chain_tail = s;
        else { chain_tail->next = s; chain_tail = s; }
    }
    /* Catch-all goto: targets default_label if a default existed, else end_label. */
    char *catchall = h->has_default ? strdup(h->default_label) : strdup(h->end_label);
    STMT_t *catchgo = sc_make_goto_uncond_stmt(st, catchall);
    if (!chain_head) chain_head = chain_tail = catchgo;
    else { chain_tail->next = catchgo; chain_tail = catchgo; }
    /* Splice dispatch chain after the tmp-assign anchor. */
    sc_splice_after(st, h->after_tmp_assign, chain_head, chain_tail);
    /* Append _Lend pad at the very end. */
    STMT_t *end_pad = sc_make_label_stmt(st, strdup(h->end_label));
    sc_append_chain(st, end_pad, end_pad);
    /* Clean up: pop loop frame, free case-label strings + tmp_name + labels.
     * Note: case[].value is NULL here (transferred to probe nodes above);
     * any unconsumed case-label strings (the case_label allocated by
     * sc_switch_case_label) are owned by the cases[] array and freed here. */
    sc_loop_pop(st);
    /* Restore cur_switch — supports nested switches.  prev_switch is saved
     * in the SwitchHead struct itself by the parent grammar rule (see the
     * { sc_switch_head_new } action, which saves/restores via a temp). */
    st->cur_switch = h->prev_switch;
    for (int i = 0; i < h->cases_count; i++) {
        free(h->cases[i].case_label);
        /* h->cases[i].value should be NULL here (transferred to probe nodes
         * above).  Any non-NULL is a defensive defect — leaked, matches
         * codebase convention (no expr_free helper exists). */
        (void)h->cases[i].value;
    }
    free(h->cases);
    free(h->tmp_name);
    free(h->end_label);
    free(h->default_label);
    free(h);
}

/* =========================================================================
 *  LS-4.i.5 — sc_emit_struct: emit the SPITBOL DATA() call for a struct decl.
 *
 *  Snocone:        struct NAME { f1, f2, f3 }
 *  Lowers to:      DATA('NAME(f1,f2,f3)')         (one bare-expr stmt)
 *
 *  Andrew's `.sc` line 162 introduces `struct` as a record-declaration
 *  keyword; the SNOBOL4/SPITBOL underlying primitive is DATA('NAME(...)'),
 *  which simultaneously installs:
 *    • a constructor function NAME(arg1, arg2, ..., argN), creating a
 *      record value with N fields
 *    • per-field accessor functions f1(), f2(), ..., each acting as both
 *      a getter (`fk(x)` returns the k-th field) and an L-value
 *      (`fk(x) = newval` updates it)
 *
 *  This rung is the bare minimum for Andrew's stack.sc / tree.sc /
 *  counter.sc shapes — `struct link { next, value }` etc. lower to a
 *  single `DATA('link(next,value)')` statement that sits exactly where
 *  the source `struct` did.
 *
 *  IR shape (mirrors sc_func_head_new's DEFINE-emission idiom):
 *
 *    EXPR_t  qarg     = E_QLIT  sval = "NAME(f1,f2,f3)"
 *    EXPR_t  data_call= E_FNC   sval = "DATA"   children = [ qarg ]
 *    STMT_t  bare-expr stmt with subject = data_call,  no goto, no label.
 *
 *  Empty-fields case: `struct NAME { }` lowers to `DATA('NAME()')`, which
 *  SPITBOL accepts as a zero-field record (rare but legal).  Caller
 *  passes `fields = strdup("")`.
 *
 *  ⛔ This emits in the same place sc_func_head_new emits its DEFINE
 *  call — at top level (or within whatever block the struct decl sits
 *  in).  It does NOT push a new context, has no body to splice, and
 *  needs no finalize companion — struct is a single statement.
 * ========================================================================= */
static void sc_emit_struct(ScParseState *st, char *name, char *fields) {
    /* Build "NAME(fields)" — the QLIT argument to DATA(). */
    int slen = strlen(name) + 1 + strlen(fields) + 2;     /* NAME(fields) + NUL */
    char *spec = malloc(slen);
    snprintf(spec, slen, "%s(%s)", name, fields);

    EXPR_t *qarg = expr_new(E_QLIT);
    qarg->sval   = spec;                                   /* takes ownership */

    EXPR_t *data_call = expr_new(E_FNC);
    data_call->sval   = strdup("DATA");
    expr_add_child(data_call, qarg);

    sc_append_stmt(st, data_call);                         /* bare-expr stmt */
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

    /* LS-4.i.2 — clean up any pending label state.  Should be empty at
     * end of a well-formed parse, but errors can leave residue. */
    sc_pending_label_clear(&state);
    free(state.pending_user_labels);
    for (int i = 0; i < state.stash_for_pending_labels_count; i++)
        free(state.stash_for_pending_labels[i]);
    free(state.stash_for_pending_labels);
    while (state.loop_top) sc_loop_pop(&state);

    if (rc != 0 || state.nerrors > 0) {
        free(state.code);
        return NULL;
    }
    return state.code;
}
