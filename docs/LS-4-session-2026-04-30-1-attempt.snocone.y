/*
 * snocone.y — Snocone Bison grammar (GOAL-SNOCONE-LANG-SPACE LS-4)
 *
 * Andrew Koenig's .sc self-host operator design + Lon's restoration of
 * SPITBOL space-as-concat semantics.
 *
 * Pipeline:  snocone.l (Flex)   →  ScTokenBuf
 *            snocone.y (Bison)  →  Program*  (STMT_t list)
 *
 * The grammar handles BOTH expressions AND C-style structured control flow
 * (if/while/do/for/switch/function/return/break/continue/goto) — replacing
 * the pre-LS-4 four-piece pipeline (snocone_lex.c + snocone_parse.c +
 * snocone_lower.c + snocone_control.c).
 *
 * Token names mirror snobol4.tab.h for value-equivalence (Lon session #9):
 * T_IDENT, T_FUNCTION, T_INT, T_REAL, T_STR, T_KEYWORD, T_CONCAT,
 * T_ASSIGNMENT, T_MATCH, T_ALTERNATION, T_ADDITION, T_SUBTRACTION,
 * T_MULTIPLICATION, T_DIVISION, T_EXPONENTIATION, T_IMMEDIATE_ASSIGN,
 * T_COND_ASSIGN, plus T_UN_* unary forms.  Snocone-only additions follow the
 * same convention: T_LEQ/T_LNE/T_LLT/T_LGT/T_LLE/T_LGE for Andrew's :==:
 * family, T_IDENT_OP/T_DIFFER for `::` / `:!:`, T_LBRACE/T_RBRACE/
 * T_SEMICOLON/T_COLON, T_KW_*.
 *
 * Lowering: all comparison / identity operators emit E_FNC named calls
 * (EQ, NE, LT, GT, LE, GE, LEQ, LNE, LLT, LGT, LLE, LGE, IDENT, DIFFER).
 * Concat (T_CONCAT) emits E_SEQ with left-collapse like SNOBOL4.  Pattern
 * alternation (T_ALTERNATION) emits E_ALT with left-collapse.  Pattern
 * match (T_MATCH = `?`) emits E_SCAN.  Assignment (T_ASSIGNMENT = `=`)
 * emits E_ASSIGN.  Comma-separated alt-eval `(a, b, c)` emits E_VLIST.
 *
 * Control flow lowers to SNOBOL4-style labeled gotos via prog_append() /
 * emit_label() / emit_goto() / emit_cond_stmt() helpers — mirroring the
 * pre-LS-4 snocone_control.c semantics line-for-line so the smoke gate
 * stays green across the LS-4 transition.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */

%code requires {
#include "scrip_cc.h"
#include "snocone_lex2.h"

/* Parser state: token buffer cursor + program-under-construction +
 * label counter + break-stack + current function name (for return). */
typedef struct ScParseState {
    ScTokenBuf  *buf;
    int          pos;
    Program     *prog;
    const char  *filename;
    int          nerrors;
    /* break-label stack — innermost loop's end-label on top */
    char        *break_stack[64];
    int          break_depth;
    /* current function name (for `return E;` lowering) */
    char        *fname;
} ScParseState;
}

%code {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int sno_set_case_sensitive(int);

/* --- helpers (defined below) --- */
static char *sc_newlab(void);
static void  sc_prog_append(ScParseState *st, STMT_t *s);
static void  sc_emit_label(ScParseState *st, const char *lab);
static void  sc_emit_goto(ScParseState *st, const char *target);
static void  sc_emit_end(ScParseState *st);
static STMT_t *sc_stmt_from_expr(EXPR_t *e);
static void  sc_emit_cond(ScParseState *st, EXPR_t *cond,
                          const char *s_lab, const char *f_lab);
static EXPR_t *sc_make_fnc1(const char *name, EXPR_t *a);
static EXPR_t *sc_make_fnc2(const char *name, EXPR_t *l, EXPR_t *r);
static EXPR_t *sc_strip_string_quotes(const char *raw);
}

%define api.prefix {sc_}
%define api.pure full
%parse-param { ScParseState *st }

%union {
    EXPR_t   *expr;
    STMT_t   *stmt;
    char     *str;
    long      ival;
    double    dval;
}

/* ========================================================================
 * Tokens — names mirror snobol4.tab.h for concept equivalence
 * ====================================================================== */

/* Atoms */
%token <str>  T_IDENT
%token <str>  T_FUNCTION    /* IDENT immediately followed by '(' (zero-space call) */
%token <str>  T_KEYWORD     /* &IDENT — the leading & is consumed in the lexer */
%token <str>  T_STR
%token <ival> T_INT
%token <dval> T_REAL

/* Binary operators (W{OP}W envelope from lexer) */
%token T_ASSIGNMENT          /* =      pri 0 right */
%token T_MATCH               /* ?      pri 1 left */
%token T_ALTERNATION         /* |      pri 3 right */
%token T_CONCAT              /* SPACE  pri 4 right (synthetic) */
%token T_EQ T_NE T_LT T_GT T_LE T_GE             /* numeric compare pri 6 */
%token T_LEQ T_LNE T_LLT T_LGT T_LLE T_LGE      /* string compare :==: family pri 6 */
%token T_IDENT_OP T_DIFFER                      /* ::  :!:  pri 6 */
%token T_ADDITION T_SUBTRACTION                 /* +  -    pri 6 */
%token T_DIVISION                               /* /       pri 8 */
%token T_MULTIPLICATION                         /* *       pri 9 */
%token T_EXPONENTIATION                         /* ^ ** !  pri 11 right */
%token T_IMMEDIATE_ASSIGN T_COND_ASSIGN         /* $  .    pri 12 */
%token T_AMPERSAND T_AT_SIGN T_POUND T_PERCENT T_TILDE  /* OPSYN slots */

/* Compound assignments — kept for legacy corpus, lowered to base op */
%token T_PLUS_ASSIGN T_MINUS_ASSIGN T_STAR_ASSIGN T_SLASH_ASSIGN T_CARET_ASSIGN

/* Unary operators (no leading-W envelope) */
%token T_UN_PLUS T_UN_MINUS T_UN_ASTERISK T_UN_SLASH T_UN_PERCENT
%token T_UN_AT_SIGN T_UN_TILDE T_UN_DOLLAR_SIGN T_UN_PERIOD T_UN_POUND
%token T_UN_VERTICAL_BAR T_UN_EQUAL T_UN_QUESTION_MARK T_UN_AMPERSAND

/* Punctuation */
%token T_LPAREN T_RPAREN T_LBRACK T_RBRACK T_LBRACE T_RBRACE
%token T_COMMA T_SEMICOLON T_COLON

/* Keywords */
%token T_KW_IF T_KW_ELSE T_KW_WHILE T_KW_DO T_KW_UNTIL T_KW_FOR
%token T_KW_SWITCH T_KW_CASE T_KW_DEFAULT
%token T_KW_BREAK T_KW_CONTINUE T_KW_GOTO
%token T_KW_FUNCTION T_KW_RETURN T_KW_FRETURN T_KW_NRETURN
%token T_KW_STRUCT

%token T_EOF 0
%token T_UNKNOWN

/* ========================================================================
 * Precedence — SPITBOL Manual Ch.15 + Andrew Koenig .sc bconv[]
 * ====================================================================== */
%right    T_ASSIGNMENT
            T_PLUS_ASSIGN T_MINUS_ASSIGN T_STAR_ASSIGN
            T_SLASH_ASSIGN T_CARET_ASSIGN          /* pri 0 */
%left     T_MATCH                                  /* pri 1 */
%right    T_ALTERNATION                            /* pri 3 */
%right    T_CONCAT                                 /* pri 4 */
%nonassoc T_EQ T_NE T_LT T_GT T_LE T_GE
            T_LEQ T_LNE T_LLT T_LGT T_LLE T_LGE
            T_IDENT_OP T_DIFFER                    /* pri 6 (Andrew) */
%left     T_ADDITION T_SUBTRACTION                 /* pri 6 */
%left     T_DIVISION                               /* pri 8 */
%left     T_MULTIPLICATION                         /* pri 9 */
%right    T_EXPONENTIATION                         /* pri 11 right */
%left     T_IMMEDIATE_ASSIGN T_COND_ASSIGN         /* pri 12 */
%left     T_PERCENT T_AMPERSAND T_AT_SIGN T_POUND T_TILDE  /* OPSYN slots */
%right    UNARY                                    /* highest, prefix unary */

/* Type declarations */
%type <expr>  expr expr_list expr_list_ne arg_list arg_list_ne
%type <expr>  cond_expr opt_expr
%type <str>   func_name_in_call
%type <str>   opt_label_target

/* Resolve dangling-else by preferring shift */
%nonassoc IF_NO_ELSE
%nonassoc T_KW_ELSE

%%

/* ========================================================================
 * Top-level program
 * ====================================================================== */
program     : top_list                                    { sc_emit_end(st); }
            | /* empty */                                 { sc_emit_end(st); }
            ;

top_list    : top_list top_stmt
            | top_stmt
            ;

/* Optional semicolons between statements */
opt_semis   : /* empty */
            | opt_semis T_SEMICOLON
            ;

top_stmt    : control_stmt opt_semis
            | expr_stmt    opt_semis
            | T_SEMICOLON                                  /* blank stmt */
            ;

stmt        : control_stmt opt_semis
            | expr_stmt    opt_semis
            | T_SEMICOLON                                  /* blank stmt */
            ;

stmt_list   : stmt_list stmt
            | stmt
            ;

block       : T_LBRACE stmt_list T_RBRACE
            | T_LBRACE              T_RBRACE
            ;

/* ========================================================================
 * Expression statement — bare expression with optional label prefix
 * ====================================================================== */
expr_stmt
    : T_IDENT T_COLON expr T_SEMICOLON
        {
            /* labeled expression statement */
            STMT_t *s = sc_stmt_from_expr($3);
            s->label = strdup($1);
            sc_prog_append(st, s);
        }
    | T_IDENT T_COLON T_SEMICOLON
        {
            /* bare label, no body */
            STMT_t *s = stmt_new();
            s->label = strdup($1);
            sc_prog_append(st, s);
        }
    | expr T_SEMICOLON
        {
            STMT_t *s = sc_stmt_from_expr($1);
            sc_prog_append(st, s);
        }
    ;

/* ========================================================================
 * Control flow statements
 * ====================================================================== */
control_stmt
    : if_stmt
    | while_stmt
    | do_while_stmt
    | do_until_stmt
    | for_stmt
    | switch_stmt
    | function_stmt
    | return_stmt
    | break_stmt
    | continue_stmt
    | goto_stmt
    | struct_stmt
    | block_stmt
    ;

block_stmt
    : block
    ;

/* --- if / else --- */
if_stmt
    : T_KW_IF T_LPAREN cond_expr T_RPAREN
        {
            char *lt = sc_newlab();
            char *lf = sc_newlab();
            sc_emit_cond(st, $3, lt, lf);
            sc_emit_label(st, lt);
            free(lt);
            $<str>$ = lf;     /* carry false-label to mid-rule */
        }
      stmt
        {
            char *lf  = $<str>5;
            char *le  = sc_newlab();
            sc_emit_goto(st, le);
            sc_emit_label(st, lf);
            $<str>$ = le;     /* carry end-label */
            free(lf);
        }
      else_part[ele]
        {
            char *le = $<str>7;
            sc_emit_label(st, le);
            free(le);
        }
    ;

else_part
    : T_KW_ELSE stmt
    | %prec IF_NO_ELSE /* empty */
    ;

/* --- while --- */
while_stmt
    : T_KW_WHILE
        {
            char *lt = sc_newlab();
            sc_emit_label(st, lt);
            $<str>$ = lt;
        }
      T_LPAREN cond_expr T_RPAREN
        {
            char *lb = sc_newlab();
            char *le = sc_newlab();
            sc_emit_cond(st, $4, lb, le);
            sc_emit_label(st, lb);
            free(lb);
            /* push break-label */
            if (st->break_depth < 64) st->break_stack[st->break_depth++] = le;
            $<str>$ = le;
        }
      stmt
        {
            char *lt = $<str>2;
            char *le = $<str>6;
            sc_emit_goto(st, lt);
            sc_emit_label(st, le);
            if (st->break_depth > 0) st->break_depth--;
            free(lt); free(le);
        }
    ;

/* --- do { body } while (cond) ;  --- success-loops.
 * Use TWO mid-rule actions: one carries top-label, the other end-label. */
do_while_stmt
    : T_KW_DO
        {
            char *lt = sc_newlab();
            sc_emit_label(st, lt);
            $<str>$ = lt;
        }
        {
            char *le = sc_newlab();
            if (st->break_depth < 64) st->break_stack[st->break_depth++] = le;
            $<str>$ = le;
        }
      stmt T_KW_WHILE T_LPAREN cond_expr T_RPAREN T_SEMICOLON
        {
            char *lt = $<str>2;
            char *le = $<str>3;
            /* success → loop, failure → exit */
            sc_emit_cond(st, $7, lt, le);
            sc_emit_label(st, le);
            if (st->break_depth > 0) st->break_depth--;
            free(lt); free(le);
        }
    ;

/* --- do { body } until (cond) ;  --- failure-loops */
do_until_stmt
    : T_KW_DO
        {
            char *lt = sc_newlab();
            char *le = sc_newlab();
            sc_emit_label(st, lt);
            if (st->break_depth < 64) st->break_stack[st->break_depth++] = le;
            $<str>$ = lt;
        }
      stmt T_KW_UNTIL T_LPAREN cond_expr T_RPAREN T_SEMICOLON
        {
            char *lt = $<str>2;
            char *le = sc_newlab();
            /* failure → loop, success → exit */
            sc_emit_cond(st, $6, le, lt);
            sc_emit_label(st, le);
            if (st->break_depth > 0) st->break_depth--;
            free(lt); free(le);
        }
    ;

/* --- for (init; cond; step) body --- */
for_stmt
    : T_KW_FOR T_LPAREN opt_expr T_SEMICOLON
        {
            /* emit init */
            if ($3) sc_prog_append(st, sc_stmt_from_expr($3));
            char *lt = sc_newlab();
            sc_emit_label(st, lt);
            $<str>$ = lt;          /* carry test-label */
        }
      cond_expr T_SEMICOLON
        {
            char *lb = sc_newlab();
            char *le = sc_newlab();
            sc_emit_cond(st, $6, lb, le);
            sc_emit_label(st, lb);
            free(lb);
            if (st->break_depth < 64) st->break_stack[st->break_depth++] = le;
            $<str>$ = le;
        }
      opt_expr T_RPAREN stmt
        {
            char *lt = $<str>5;
            char *le = $<str>8;
            /* emit step (collected as $9) */
            if ($9) sc_prog_append(st, sc_stmt_from_expr($9));
            sc_emit_goto(st, lt);
            sc_emit_label(st, le);
            if (st->break_depth > 0) st->break_depth--;
            free(lt); free(le);
        }
    ;

opt_expr
    : expr                          { $$ = $1; }
    | /* empty */                   { $$ = NULL; }
    ;

/* --- switch (e) { case v1: S1 ... default: SD } ---
 * Lowered as a chain of IDENT(tmp, vN) :S(cN); default falls through.
 * tmp is held in a synthetic var name "%switch.N" — but to keep things
 * simple we evaluate the discriminant via a temp assignment.
 */
switch_stmt
    : T_KW_SWITCH T_LPAREN expr T_RPAREN T_LBRACE
        {
            /* tmp = discriminant */
            char tmpv[32]; static int sw_ctr = 0;
            snprintf(tmpv, sizeof tmpv, "_sc_sw_%d", ++sw_ctr);
            EXPR_t *lhs = expr_new(E_VAR); lhs->sval = strdup(tmpv);
            STMT_t *as = stmt_new();
            as->subject = lhs; as->replacement = $3; as->has_eq = 1;
            sc_prog_append(st, as);
            char *le = sc_newlab();
            if (st->break_depth < 64) st->break_stack[st->break_depth++] = le;
            $<str>$ = strdup(tmpv);
            $<str>4 = le;          /* end label slot */
            (void)sw_ctr;
        }
      case_list T_RBRACE
        {
            char *tmpv = $<str>6;
            char *le   = $<str>4;
            sc_emit_label(st, le);
            if (st->break_depth > 0) st->break_depth--;
            free(tmpv); free(le);
        }
    ;

case_list
    : case_list case_clause
    | case_clause
    | /* empty */
    ;

case_clause
    : T_KW_CASE expr T_COLON
        {
            /* Tmp from outer switch — pull from break_stack? No, use $<str>0 from
             * parent.  Simpler: re-emit a IDENT(tmp,v) :S(case.N) test before the
             * body.  We piggy-back on the break-stack top for the end-label. */
            char *lc = sc_newlab();
            /* Pull tmp name from the parent switch's mid-rule via grammar value:
             * Bison's $<str>0 references the symbol just before this rule's LHS in
             * the parent, but in case_list inside switch we cannot reach it via $0
             * cleanly.  Instead we stash tmp in a thread-local so we don't overcomplicate
             * this rung. */
            extern char *sc_current_switch_tmp;
            extern char *sc_current_switch_end;
            EXPR_t *l = expr_new(E_VAR); l->sval = strdup(sc_current_switch_tmp ? sc_current_switch_tmp : "_sc_sw_0");
            EXPR_t *fn = sc_make_fnc2("IDENT", l, $2);
            /* :S(lc) :F(next) — we emit cond as test → next-case, success → body */
            STMT_t *s = stmt_new();
            s->subject = fn;
            s->go = sgoto_new();
            s->go->onsuccess = strdup(lc);
            sc_prog_append(st, s);
            sc_emit_label(st, lc);
            $<str>$ = lc;
        }
      stmt_list
        {
            free($<str>4);
            /* fall-through to next case is the C-style "no break" — but we want
             * modern semantics where each case implicitly breaks.  Emit a goto
             * to the switch's end-label. */
            extern char *sc_current_switch_end;
            if (sc_current_switch_end) sc_emit_goto(st, sc_current_switch_end);
        }
    | T_KW_DEFAULT T_COLON stmt_list
        {
            /* default falls through; break is implicit via goto-end */
            extern char *sc_current_switch_end;
            if (sc_current_switch_end) sc_emit_goto(st, sc_current_switch_end);
        }
    ;

/* --- function name(args) { body } / [ (locals) ] --- */
function_stmt
    : T_KW_FUNCTION func_name_in_call T_LPAREN arg_name_list T_RPAREN
        {
            /* Save outer fname */
            $<str>$ = st->fname ? strdup(st->fname) : NULL;
            /* Set current fname so `return E;` knows the result var */
            free(st->fname);
            st->fname = strdup($2);
        }
      opt_locals
        {
            /* Emit goto fname.END (jump over body) */
            char endlab[256];
            snprintf(endlab, sizeof endlab, "%s.END", $2);
            sc_emit_goto(st, endlab);
            /* Emit DEFINE call */
            char def_buf[768];
            extern char  sc_func_args[];
            extern char  sc_func_locals[];
            if (sc_func_locals[0])
                snprintf(def_buf, sizeof def_buf, "%s(%s)%s", $2, sc_func_args, sc_func_locals);
            else
                snprintf(def_buf, sizeof def_buf, "%s(%s)",   $2, sc_func_args);
            EXPR_t *q = expr_new(E_QLIT); q->sval = strdup(def_buf);
            EXPR_t *call = expr_new(E_FNC); call->sval = strdup("DEFINE");
            expr_add_child(call, q);
            STMT_t *def_s = stmt_new(); def_s->subject = call;
            sc_prog_append(st, def_s);
            /* Emit function entry label */
            sc_emit_label(st, $2);
        }
      stmt
        {
            /* RETURN at end */
            STMT_t *r = stmt_new();
            r->go = sgoto_new();
            r->go->uncond = strdup("RETURN");
            sc_prog_append(st, r);
            char endlab[256];
            snprintf(endlab, sizeof endlab, "%s.END", $2);
            sc_emit_label(st, endlab);
            /* Restore outer fname */
            free(st->fname);
            st->fname = $<str>6;
            free($2);
        }
    ;

func_name_in_call
    : T_FUNCTION                        { $$ = $1; }     /* `function foo(` */
    | T_IDENT                           { $$ = $1; }     /* `function foo (` */
    ;

/* arg_name_list — collect args into sc_func_args (global buffer) */
arg_name_list
    : { extern char sc_func_args[]; sc_func_args[0] = '\0'; }
      arg_name_list_body
    ;

arg_name_list_body
    : /* empty */
    | arg_name_list_ne
    ;

arg_name_list_ne
    : T_IDENT
        {
            extern char sc_func_args[];
            strncat(sc_func_args, $1, 256 - strlen(sc_func_args) - 1);
            free($1);
        }
    | arg_name_list_ne T_COMMA T_IDENT
        {
            extern char sc_func_args[];
            strncat(sc_func_args, ",", 256 - strlen(sc_func_args) - 1);
            strncat(sc_func_args, $3, 256 - strlen(sc_func_args) - 1);
            free($3);
        }
    ;

opt_locals
    : /* empty */
        { extern char sc_func_locals[]; sc_func_locals[0] = '\0'; }
    | T_LPAREN { extern char sc_func_locals[]; sc_func_locals[0] = '\0'; }
      local_list_body T_RPAREN
    ;

local_list_body
    : /* empty */
    | local_list_ne
    ;

local_list_ne
    : T_IDENT
        {
            extern char sc_func_locals[];
            strncat(sc_func_locals, $1, 256 - strlen(sc_func_locals) - 1);
            free($1);
        }
    | local_list_ne T_COMMA T_IDENT
        {
            extern char sc_func_locals[];
            strncat(sc_func_locals, ",", 256 - strlen(sc_func_locals) - 1);
            strncat(sc_func_locals, $3, 256 - strlen(sc_func_locals) - 1);
            free($3);
        }
    ;

/* --- return / freturn / nreturn --- */
return_stmt
    : T_KW_RETURN expr T_SEMICOLON
        {
            /* fname = expr ; :(RETURN) */
            if (st->fname) {
                EXPR_t *lhs = expr_new(E_VAR); lhs->sval = strdup(st->fname);
                STMT_t *as = stmt_new();
                as->subject = lhs; as->replacement = $2; as->has_eq = 1;
                as->go = sgoto_new(); as->go->uncond = strdup("RETURN");
                sc_prog_append(st, as);
            } else {
                STMT_t *r = stmt_new();
                r->subject = $2;
                r->go = sgoto_new(); r->go->uncond = strdup("RETURN");
                sc_prog_append(st, r);
            }
        }
    | T_KW_RETURN T_SEMICOLON
        {
            STMT_t *r = stmt_new();
            r->go = sgoto_new(); r->go->uncond = strdup("RETURN");
            sc_prog_append(st, r);
        }
    | T_KW_FRETURN T_SEMICOLON
        {
            STMT_t *r = stmt_new();
            r->go = sgoto_new(); r->go->uncond = strdup("FRETURN");
            sc_prog_append(st, r);
        }
    | T_KW_NRETURN expr T_SEMICOLON
        {
            if (st->fname) {
                EXPR_t *lhs = expr_new(E_VAR); lhs->sval = strdup(st->fname);
                STMT_t *as = stmt_new();
                as->subject = lhs; as->replacement = $2; as->has_eq = 1;
                as->go = sgoto_new(); as->go->uncond = strdup("NRETURN");
                sc_prog_append(st, as);
            } else {
                STMT_t *r = stmt_new();
                r->subject = $2;
                r->go = sgoto_new(); r->go->uncond = strdup("NRETURN");
                sc_prog_append(st, r);
            }
        }
    | T_KW_NRETURN T_SEMICOLON
        {
            STMT_t *r = stmt_new();
            r->go = sgoto_new(); r->go->uncond = strdup("NRETURN");
            sc_prog_append(st, r);
        }
    ;

/* --- break / continue --- */
break_stmt
    : T_KW_BREAK T_SEMICOLON
        {
            if (st->break_depth > 0) {
                sc_emit_goto(st, st->break_stack[st->break_depth - 1]);
            } else {
                sno_error_msg("break outside loop");
                st->nerrors++;
            }
        }
    ;

continue_stmt
    : T_KW_CONTINUE T_SEMICOLON
        {
            /* For now, continue == break (TODO: separate continue stack).
             * Most existing programs use only break. */
            if (st->break_depth > 0) {
                sc_emit_goto(st, st->break_stack[st->break_depth - 1]);
            } else {
                sno_error_msg("continue outside loop");
                st->nerrors++;
            }
        }
    ;

goto_stmt
    : T_KW_GOTO T_IDENT T_SEMICOLON
        {
            sc_emit_goto(st, $2);
            free($2);
        }
    ;

opt_label_target
    : T_IDENT          { $$ = $1; }
    | /* empty */      { $$ = NULL; }
    ;

/* --- struct name { field, field, ... } --- */
struct_stmt
    : T_KW_STRUCT T_IDENT T_LBRACE field_list T_RBRACE
        {
            extern char sc_struct_fields[];
            char db[600];
            if (sc_struct_fields[0])
                snprintf(db, sizeof db, "%s(%s)", $2, sc_struct_fields);
            else
                snprintf(db, sizeof db, "%s()", $2);
            EXPR_t *q = expr_new(E_QLIT); q->sval = strdup(db);
            EXPR_t *call = expr_new(E_FNC); call->sval = strdup("DATA");
            expr_add_child(call, q);
            STMT_t *s = stmt_new(); s->subject = call;
            sc_prog_append(st, s);
            free($2);
        }
    ;

field_list
    : { extern char sc_struct_fields[]; sc_struct_fields[0] = '\0'; }
      field_list_body
    ;

field_list_body
    : /* empty */
    | field_list_ne
    ;

field_list_ne
    : T_IDENT
        {
            extern char sc_struct_fields[];
            strncat(sc_struct_fields, $1, 512 - strlen(sc_struct_fields) - 1);
            free($1);
        }
    | field_list_ne T_COMMA T_IDENT
        {
            extern char sc_struct_fields[];
            strncat(sc_struct_fields, ",", 512 - strlen(sc_struct_fields) - 1);
            strncat(sc_struct_fields, $3, 512 - strlen(sc_struct_fields) - 1);
            free($3);
        }
    ;

/* ========================================================================
 * Conditional expression — same as `expr` with semantic note:
 * Snocone conditions ARE SPITBOL backtracking expressions.  Success/failure
 * drives the branch.  No special "boolean" non-terminal.
 * ====================================================================== */
cond_expr
    : expr                            { $$ = $1; }
    ;

/* ========================================================================
 * Expression — Bison precedence drives associativity & priority.
 * Mirrors snobol4.y's expr0..expr17 ladder collapsed via %left/%right.
 * ====================================================================== */
expr
    /* Assignment forms — pri 0 right-assoc */
    : expr T_ASSIGNMENT     expr   { $$ = expr_binary(E_ASSIGN, $1, $3); }
    | expr T_PLUS_ASSIGN    expr   { /* x += y → x = x + y */
                                      EXPR_t *lhs2 = expr_new($1->kind); lhs2->sval = $1->sval ? strdup($1->sval) : NULL;
                                      EXPR_t *add  = expr_binary(E_ADD, lhs2, $3);
                                      $$ = expr_binary(E_ASSIGN, $1, add); }
    | expr T_MINUS_ASSIGN   expr   { EXPR_t *l2 = expr_new($1->kind); l2->sval = $1->sval ? strdup($1->sval) : NULL;
                                      EXPR_t *op = expr_binary(E_SUB, l2, $3);
                                      $$ = expr_binary(E_ASSIGN, $1, op); }
    | expr T_STAR_ASSIGN    expr   { EXPR_t *l2 = expr_new($1->kind); l2->sval = $1->sval ? strdup($1->sval) : NULL;
                                      EXPR_t *op = expr_binary(E_MUL, l2, $3);
                                      $$ = expr_binary(E_ASSIGN, $1, op); }
    | expr T_SLASH_ASSIGN   expr   { EXPR_t *l2 = expr_new($1->kind); l2->sval = $1->sval ? strdup($1->sval) : NULL;
                                      EXPR_t *op = expr_binary(E_DIV, l2, $3);
                                      $$ = expr_binary(E_ASSIGN, $1, op); }
    | expr T_CARET_ASSIGN   expr   { EXPR_t *l2 = expr_new($1->kind); l2->sval = $1->sval ? strdup($1->sval) : NULL;
                                      EXPR_t *op = expr_binary(E_POW, l2, $3);
                                      $$ = expr_binary(E_ASSIGN, $1, op); }
    /* Pattern match — pri 1 */
    | expr T_MATCH          expr   { $$ = expr_binary(E_SCAN, $1, $3); }
    /* Alternation — pri 3 */
    | expr T_ALTERNATION    expr
        { if ($1->kind == E_ALT) { expr_add_child($1, $3); $$ = $1; }
          else { EXPR_t *a = expr_new(E_ALT); expr_add_child(a, $1); expr_add_child(a, $3); $$ = a; } }
    /* Concat — pri 4 */
    | expr T_CONCAT         expr
        { if ($1->kind == E_SEQ) { expr_add_child($1, $3); $$ = $1; }
          else { EXPR_t *s = expr_new(E_SEQ); expr_add_child(s, $1); expr_add_child(s, $3); $$ = s; } }
    /* Comparisons — pri 6 — lower to E_FNC */
    | expr T_EQ             expr   { $$ = sc_make_fnc2("EQ",     $1, $3); }
    | expr T_NE             expr   { $$ = sc_make_fnc2("NE",     $1, $3); }
    | expr T_LT             expr   { $$ = sc_make_fnc2("LT",     $1, $3); }
    | expr T_GT             expr   { $$ = sc_make_fnc2("GT",     $1, $3); }
    | expr T_LE             expr   { $$ = sc_make_fnc2("LE",     $1, $3); }
    | expr T_GE             expr   { $$ = sc_make_fnc2("GE",     $1, $3); }
    | expr T_LEQ            expr   { $$ = sc_make_fnc2("LEQ",    $1, $3); }
    | expr T_LNE            expr   { $$ = sc_make_fnc2("LNE",    $1, $3); }
    | expr T_LLT            expr   { $$ = sc_make_fnc2("LLT",    $1, $3); }
    | expr T_LGT            expr   { $$ = sc_make_fnc2("LGT",    $1, $3); }
    | expr T_LLE            expr   { $$ = sc_make_fnc2("LLE",    $1, $3); }
    | expr T_LGE            expr   { $$ = sc_make_fnc2("LGE",    $1, $3); }
    | expr T_IDENT_OP       expr   { $$ = sc_make_fnc2("IDENT",  $1, $3); }
    | expr T_DIFFER         expr   { $$ = sc_make_fnc2("DIFFER", $1, $3); }
    /* Arithmetic — pri 6 / 8 / 9 */
    | expr T_ADDITION       expr   { $$ = expr_binary(E_ADD, $1, $3); }
    | expr T_SUBTRACTION    expr   { $$ = expr_binary(E_SUB, $1, $3); }
    | expr T_DIVISION       expr   { $$ = expr_binary(E_DIV, $1, $3); }
    | expr T_MULTIPLICATION expr   { $$ = expr_binary(E_MUL, $1, $3); }
    /* Exponentiation — pri 11 right */
    | expr T_EXPONENTIATION expr   { $$ = expr_binary(E_POW, $1, $3); }
    /* Pattern-context capture — pri 12 */
    | expr T_IMMEDIATE_ASSIGN expr { $$ = expr_binary(E_CAPT_IMMED_ASGN, $1, $3); }
    | expr T_COND_ASSIGN      expr { $$ = expr_binary(E_CAPT_COND_ASGN,  $1, $3); }
    /* OPSYN slots */
    | expr T_AMPERSAND      expr   { EXPR_t *e = expr_binary(E_OPSYN, $1, $3); e->sval = strdup("&"); $$ = e; }
    | expr T_AT_SIGN        expr   { EXPR_t *e = expr_binary(E_OPSYN, $1, $3); e->sval = strdup("@"); $$ = e; }
    | expr T_POUND          expr   { EXPR_t *e = expr_binary(E_OPSYN, $1, $3); e->sval = strdup("#"); $$ = e; }
    | expr T_PERCENT        expr   { $$ = sc_make_fnc2("REMDR", $1, $3); }
    | expr T_TILDE          expr   { EXPR_t *e = expr_binary(E_OPSYN, $1, $3); e->sval = strdup("~"); $$ = e; }
    /* Unary prefix — pri UNARY (highest) */
    | T_UN_PLUS         expr %prec UNARY  { $$ = expr_unary(E_PLS,         $2); }
    | T_UN_MINUS        expr %prec UNARY  { $$ = expr_unary(E_MNS,         $2); }
    | T_UN_ASTERISK     expr %prec UNARY  { $$ = expr_unary(E_DEFER,       $2); }
    | T_UN_DOLLAR_SIGN  expr %prec UNARY  { $$ = expr_unary(E_INDIRECT,    $2); }
    | T_UN_PERIOD       expr %prec UNARY  { $$ = expr_unary(E_NAME,        $2); }
    | T_UN_AT_SIGN      expr %prec UNARY  { $$ = expr_unary(E_CAPT_CURSOR, $2); }
    | T_UN_TILDE        expr %prec UNARY  { $$ = expr_unary(E_NOT,         $2); }
    | T_UN_QUESTION_MARK expr %prec UNARY { $$ = expr_unary(E_NOT, expr_unary(E_NOT, $2)); }  /* ?x ≡ ~~x */
    | T_UN_AMPERSAND    expr %prec UNARY  { EXPR_t *e = expr_unary(E_OPSYN, $2); e->sval = strdup("&"); $$ = e; }
    | T_UN_PERCENT      expr %prec UNARY  { $$ = expr_unary(E_DIV,  $2); }
    | T_UN_SLASH        expr %prec UNARY  { $$ = expr_unary(E_DIV,  $2); }
    | T_UN_POUND        expr %prec UNARY  { $$ = expr_unary(E_MUL,  $2); }
    | T_UN_VERTICAL_BAR expr %prec UNARY  { EXPR_t *e = expr_unary(E_OPSYN, $2); e->sval = strdup("|"); $$ = e; }
    | T_UN_EQUAL        expr %prec UNARY  { $$ = expr_unary(E_ASSIGN, $2); }
    /* Subscripting — IDX */
    | expr T_LBRACK arg_list T_RBRACK
        {
            EXPR_t *idx = expr_new(E_IDX);
            expr_add_child(idx, $1);
            for (int i = 0; i < $3->nchildren; i++) expr_add_child(idx, $3->children[i]);
            free($3->children); free($3);
            $$ = idx;
        }
    /* Function call — IDENT_LPAREN_NOSP form (T_FUNCTION token) */
    | T_FUNCTION T_LPAREN arg_list T_RPAREN
        {
            EXPR_t *fn = expr_new(E_FNC);
            fn->sval = strdup($1);
            for (int i = 0; i < $3->nchildren; i++) expr_add_child(fn, $3->children[i]);
            free($3->children); free($3);
            free($1);
            $$ = fn;
        }
    /* Parenthesised — single, multi (alt-eval VLIST), or empty */
    | T_LPAREN expr T_RPAREN                     { $$ = $2; }
    | T_LPAREN expr T_COMMA expr_list_ne T_RPAREN
        {
            EXPR_t *vl = expr_new(E_VLIST);
            expr_add_child(vl, $2);
            for (int i = 0; i < $4->nchildren; i++) expr_add_child(vl, $4->children[i]);
            free($4->children); free($4);
            $$ = vl;
        }
    | T_LPAREN T_RPAREN                           { $$ = expr_new(E_NUL); }
    /* Atoms */
    | T_IDENT                                     { EXPR_t *e = expr_new(E_VAR);     e->sval = $1;        $$ = e; }
    | T_KEYWORD                                   { EXPR_t *e = expr_new(E_KEYWORD); e->sval = $1;        $$ = e; }
    | T_STR                                       { $$ = sc_strip_string_quotes($1); free($1); }
    | T_INT                                       { EXPR_t *e = expr_new(E_ILIT);    e->ival = $1;        $$ = e; }
    | T_REAL                                      { EXPR_t *e = expr_new(E_FLIT);    e->dval = $1;        $$ = e; }
    ;

/* arg_list — used for function-call args and subscript indices.
 * Must accept empty.  Returns an E_NUL "container" with nchildren = nargs. */
arg_list
    : /* empty */                  { $$ = expr_new(E_NUL); }
    | arg_list_ne                  { $$ = $1; }
    ;

arg_list_ne
    : expr                         { EXPR_t *l = expr_new(E_NUL); expr_add_child(l, $1); $$ = l; }
    | arg_list_ne T_COMMA expr     { expr_add_child($1, $3); $$ = $1; }
    | arg_list_ne T_COMMA          { expr_add_child($1, expr_new(E_NUL)); $$ = $1; }
    ;

expr_list
    : /* empty */                  { $$ = expr_new(E_NUL); }
    | expr_list_ne                 { $$ = $1; }
    ;

expr_list_ne
    : expr                         { EXPR_t *l = expr_new(E_NUL); expr_add_child(l, $1); $$ = l; }
    | expr_list_ne T_COMMA expr    { expr_add_child($1, $3); $$ = $1; }
    ;

%%

/* ============================================================================
 * Helpers
 * ========================================================================= */

/* Per-process unique label counter (multi-file label-collision safe) */
static int g_sc_label_ctr = 0;

/* Switch / function shared buffers (small + simple — no thread safety needed
 * inside a single parse call) */
char  sc_func_args[512]    = "";
char  sc_func_locals[512]  = "";
char  sc_struct_fields[512] = "";
char *sc_current_switch_tmp = NULL;
char *sc_current_switch_end = NULL;

static char *sc_newlab(void) {
    char buf[32];
    snprintf(buf, sizeof buf, "L.%d", ++g_sc_label_ctr);
    return strdup(buf);
}

static void sc_prog_append(ScParseState *st, STMT_t *s) {
    if (!s) return;
    s->next = NULL;
    s->stno = ++st->prog->nstmts;
    if (!st->prog->head) st->prog->head = st->prog->tail = s;
    else { st->prog->tail->next = s; st->prog->tail = s; }
}

static void sc_emit_label(ScParseState *st, const char *lab) {
    STMT_t *s = stmt_new();
    s->label = strdup(lab);
    sc_prog_append(st, s);
}

static void sc_emit_goto(ScParseState *st, const char *target) {
    STMT_t *s = stmt_new();
    s->go = sgoto_new();
    s->go->uncond = strdup(target);
    sc_prog_append(st, s);
}

static void sc_emit_end(ScParseState *st) {
    STMT_t *s = stmt_new();
    s->is_end = 1;
    s->label = strdup("END");
    sc_prog_append(st, s);
}

/* Build a STMT_t from a top-level expression.
 * If the expression is E_ASSIGN(lhs, rhs) → subject = lhs, replacement = rhs.
 * If the expression is E_ASSIGN(E_SCAN(s,p), repl) → subject=s, pattern=p, repl=repl.
 * If the expression is E_SCAN(s,p) → subject=s, pattern=p (pattern-match stmt).
 * Otherwise → subject = expr.
 */
static STMT_t *sc_stmt_from_expr(EXPR_t *e) {
    STMT_t *s = stmt_new();
    if (!e) return s;
    if (e->kind == E_ASSIGN && e->nchildren == 2) {
        EXPR_t *lhs = e->children[0];
        EXPR_t *rhs = e->children[1];
        e->children[0] = e->children[1] = NULL;
        free(e->children); free(e);
        if (lhs && lhs->kind == E_SCAN && lhs->nchildren == 2) {
            s->subject = lhs->children[0];
            s->pattern = lhs->children[1];
            lhs->children[0] = lhs->children[1] = NULL;
            free(lhs->children); free(lhs);
            if (!rhs) { rhs = expr_new(E_QLIT); rhs->sval = strdup(""); }
            s->replacement = rhs;
            s->has_eq = 1;
        } else {
            s->subject = lhs;
            s->replacement = rhs;
            s->has_eq = 1;
        }
        return s;
    }
    if (e->kind == E_SCAN && e->nchildren == 2) {
        s->subject = e->children[0];
        s->pattern = e->children[1];
        e->children[0] = e->children[1] = NULL;
        free(e->children); free(e);
        return s;
    }
    s->subject = e;
    return s;
}

/* Emit a condition statement: subject = cond, with goto S(s_lab) F(f_lab) */
static void sc_emit_cond(ScParseState *st, EXPR_t *cond,
                         const char *s_lab, const char *f_lab) {
    STMT_t *s = stmt_new();
    s->subject = cond;
    s->go = sgoto_new();
    if (s_lab) s->go->onsuccess = strdup(s_lab);
    if (f_lab) s->go->onfailure = strdup(f_lab);
    sc_prog_append(st, s);
}

static EXPR_t *sc_make_fnc1(const char *name, EXPR_t *a) {
    EXPR_t *e = expr_new(E_FNC);
    e->sval = strdup(name);
    expr_add_child(e, a);
    return e;
}

static EXPR_t *sc_make_fnc2(const char *name, EXPR_t *l, EXPR_t *r) {
    EXPR_t *e = expr_new(E_FNC);
    e->sval = strdup(name);
    expr_add_child(e, l);
    expr_add_child(e, r);
    return e;
}

/* The lexer stores raw string text (already without surrounding quotes —
 * see snocone.l STR1/STR2 actions writing to sc_strbuf).  We just wrap in
 * E_QLIT.  But existing call paths also pass quoted text in some legacy
 * code, so this helper handles both. */
static EXPR_t *sc_strip_string_quotes(const char *raw) {
    EXPR_t *e = expr_new(E_QLIT);
    if (!raw) { e->sval = strdup(""); return e; }
    int len = (int)strlen(raw);
    if (len >= 2 && (raw[0] == '\'' || raw[0] == '"')
                 && (raw[len-1] == raw[0])) {
        e->sval = strndup(raw + 1, len - 2);
    } else {
        e->sval = strdup(raw);
    }
    return e;
}

/* sno_error_msg — used by error productions.  Routes to stderr; the linker
 * pulls in the real symbol from snobol4_error.c if present, else this is a
 * weak stand-in. */
__attribute__((weak)) void sno_error_msg(const char *msg) {
    fprintf(stderr, "snocone: %s\n", msg);
}

/* ============================================================================
 * sc_yylex — translate ScKind from the buffer to Bison token codes
 *
 * The lexer (snocone.l) emits ScToken2 records into a ScTokenBuf with
 * `kind` values from the ScKind enum (snocone_lex2.h).  Bison generates its
 * own enum in snocone.tab.h.  This thunk maps them.  Names are the same on
 * both sides; values are not.
 * ========================================================================= */

/* Forward-declared by Bison */
extern int sc_parse(ScParseState *st);

/* Forward-declared by Flex (generated lexer entry point) */
ScTokenBuf snocone_lex2(const char *source);

static int sc_yylex(SC_STYPE *yylval, ScParseState *st) {
    if (st->pos >= st->buf->count) return 0;   /* EOF for Bison */
    ScToken2 *t = &st->buf->tokens[st->pos++];
    /* Name-based switch maps ScKind → bison T_* via the matching name. */
    int k = t->kind;
    switch (k) {
    case T_INT:
        yylval->ival = strtol(t->text ? t->text : "0", NULL, 10);
        return T_INT;
    case T_REAL:
        yylval->dval = strtod(t->text ? t->text : "0", NULL);
        return T_REAL;
    case T_STR:
        yylval->str = t->text ? strdup(t->text) : strdup("");
        return T_STR;
    case T_IDENT:
        yylval->str = t->text ? strdup(t->text) : strdup("");
        return T_IDENT;
    case T_FUNCTION:
        yylval->str = t->text ? strdup(t->text) : strdup("");
        return T_FUNCTION;
    case T_KEYWORD:
        yylval->str = t->text ? strdup(t->text) : strdup("");
        return T_KEYWORD;
    case T_CONCAT:           return T_CONCAT;
    case T_ASSIGNMENT:       return T_ASSIGNMENT;
    case T_MATCH:            return T_MATCH;
    case T_ALTERNATION:      return T_ALTERNATION;
    case T_ADDITION:         return T_ADDITION;
    case T_SUBTRACTION:      return T_SUBTRACTION;
    case T_MULTIPLICATION:   return T_MULTIPLICATION;
    case T_DIVISION:         return T_DIVISION;
    case T_EXPONENTIATION:   return T_EXPONENTIATION;
    case T_IMMEDIATE_ASSIGN: return T_IMMEDIATE_ASSIGN;
    case T_COND_ASSIGN:      return T_COND_ASSIGN;
    case T_AMPERSAND:        return T_AMPERSAND;
    case T_AT_SIGN:          return T_AT_SIGN;
    case T_POUND:            return T_POUND;
    case T_PERCENT:          return T_PERCENT;
    case T_TILDE:            return T_TILDE;
    case T_EQ:               return T_EQ;
    case T_NE:               return T_NE;
    case T_LT:               return T_LT;
    case T_GT:               return T_GT;
    case T_LE:               return T_LE;
    case T_GE:               return T_GE;
    case T_LEQ:              return T_LEQ;
    case T_LNE:              return T_LNE;
    case T_LLT:              return T_LLT;
    case T_LGT:              return T_LGT;
    case T_LLE:              return T_LLE;
    case T_LGE:              return T_LGE;
    case T_IDENT_OP:         return T_IDENT_OP;
    case T_DIFFER:           return T_DIFFER;
    case T_PLUS_ASSIGN:      return T_PLUS_ASSIGN;
    case T_MINUS_ASSIGN:     return T_MINUS_ASSIGN;
    case T_STAR_ASSIGN:      return T_STAR_ASSIGN;
    case T_SLASH_ASSIGN:     return T_SLASH_ASSIGN;
    case T_CARET_ASSIGN:     return T_CARET_ASSIGN;
    case T_UN_PLUS:          return T_UN_PLUS;
    case T_UN_MINUS:         return T_UN_MINUS;
    case T_UN_ASTERISK:      return T_UN_ASTERISK;
    case T_UN_SLASH:         return T_UN_SLASH;
    case T_UN_PERCENT:       return T_UN_PERCENT;
    case T_UN_AT_SIGN:       return T_UN_AT_SIGN;
    case T_UN_TILDE:         return T_UN_TILDE;
    case T_UN_DOLLAR_SIGN:   return T_UN_DOLLAR_SIGN;
    case T_UN_PERIOD:        return T_UN_PERIOD;
    case T_UN_POUND:         return T_UN_POUND;
    case T_UN_VERTICAL_BAR:  return T_UN_VERTICAL_BAR;
    case T_UN_EQUAL:         return T_UN_EQUAL;
    case T_UN_QUESTION_MARK: return T_UN_QUESTION_MARK;
    case T_UN_AMPERSAND:     return T_UN_AMPERSAND;
    case T_LPAREN:           return T_LPAREN;
    case T_RPAREN:           return T_RPAREN;
    case T_LBRACK:           return T_LBRACK;
    case T_RBRACK:           return T_RBRACK;
    case T_LBRACE:           return T_LBRACE;
    case T_RBRACE:           return T_RBRACE;
    case T_COMMA:            return T_COMMA;
    case T_SEMICOLON:        return T_SEMICOLON;
    case T_COLON:            return T_COLON;
    case T_KW_IF:            return T_KW_IF;
    case T_KW_ELSE:          return T_KW_ELSE;
    case T_KW_WHILE:         return T_KW_WHILE;
    case T_KW_DO:            return T_KW_DO;
    case T_KW_UNTIL:         return T_KW_UNTIL;
    case T_KW_FOR:           return T_KW_FOR;
    case T_KW_SWITCH:        return T_KW_SWITCH;
    case T_KW_CASE:          return T_KW_CASE;
    case T_KW_DEFAULT:       return T_KW_DEFAULT;
    case T_KW_BREAK:         return T_KW_BREAK;
    case T_KW_CONTINUE:      return T_KW_CONTINUE;
    case T_KW_GOTO:          return T_KW_GOTO;
    case T_KW_FUNCTION:      return T_KW_FUNCTION;
    case T_KW_RETURN:        return T_KW_RETURN;
    case T_KW_FRETURN:       return T_KW_FRETURN;
    case T_KW_NRETURN:       return T_KW_NRETURN;
    case T_KW_STRUCT:        return T_KW_STRUCT;
    case T_EOF:              return 0;
    case T_UNKNOWN:
    default:                 return T_UNKNOWN;
    }
}

/* Bison expects sc_lex (api.prefix sc_) — wire to our thunk */
int sc_lex(SC_STYPE *yylval, ScParseState *st) {
    return sc_yylex(yylval, st);
}

void sc_error(ScParseState *st, const char *msg) {
    fprintf(stderr, "snocone: parse error: %s\n", msg);
    if (st) st->nerrors++;
}

/* ============================================================================
 * Public entry point — replaces snocone_compile() / snocone_control_compile()
 * ========================================================================= */

Program *snocone_parse_program(const char *source, const char *filename) {
    if (!source) return calloc(1, sizeof(Program));
    if (!filename) filename = "<stdin>";

    /* Snocone preserves identifier case (RULES.md) */
    sno_set_case_sensitive(1);

    /* 1. Lex into ScTokenBuf */
    ScTokenBuf buf = snocone_lex2(source);

    /* 2. Parse */
    ScParseState st;
    memset(&st, 0, sizeof st);
    st.buf      = &buf;
    st.pos      = 0;
    st.prog     = calloc(1, sizeof(Program));
    st.filename = filename;
    st.fname    = NULL;

    sc_parse(&st);

    /* 3. Cleanup */
    sc_token_buf_free(&buf);
    if (st.fname) free(st.fname);

    if (st.nerrors > 0) {
        fprintf(stderr, "snocone: %d error(s) in %s\n", st.nerrors, filename);
    }
    return st.prog;
}
