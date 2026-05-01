/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 72 "snocone_parse.y"

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

#line 269 "snocone_parse.tab.c"
/* Substitute the type names.  */
#define YYSTYPE         SC_STYPE
/* Substitute the variable and function names.  */
#define yyparse         sc_parse
#define yylex           sc_lex
#define yyerror         sc_error
#define yydebug         sc_debug
#define yynerrs         sc_nerrs


# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "snocone_parse.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_IDENT = 3,                    /* T_IDENT  */
  YYSYMBOL_T_KEYWORD = 4,                  /* T_KEYWORD  */
  YYSYMBOL_T_INT = 5,                      /* T_INT  */
  YYSYMBOL_T_REAL = 6,                     /* T_REAL  */
  YYSYMBOL_T_STR = 7,                      /* T_STR  */
  YYSYMBOL_T_CALL = 8,                     /* T_CALL  */
  YYSYMBOL_T_2PLUS = 9,                    /* T_2PLUS  */
  YYSYMBOL_T_2MINUS = 10,                  /* T_2MINUS  */
  YYSYMBOL_T_2STAR = 11,                   /* T_2STAR  */
  YYSYMBOL_T_2SLASH = 12,                  /* T_2SLASH  */
  YYSYMBOL_T_2CARET = 13,                  /* T_2CARET  */
  YYSYMBOL_T_EQ = 14,                      /* T_EQ  */
  YYSYMBOL_T_NE = 15,                      /* T_NE  */
  YYSYMBOL_T_LT = 16,                      /* T_LT  */
  YYSYMBOL_T_GT = 17,                      /* T_GT  */
  YYSYMBOL_T_LE = 18,                      /* T_LE  */
  YYSYMBOL_T_GE = 19,                      /* T_GE  */
  YYSYMBOL_T_LEQ = 20,                     /* T_LEQ  */
  YYSYMBOL_T_LNE = 21,                     /* T_LNE  */
  YYSYMBOL_T_LLT = 22,                     /* T_LLT  */
  YYSYMBOL_T_LGT = 23,                     /* T_LGT  */
  YYSYMBOL_T_LLE = 24,                     /* T_LLE  */
  YYSYMBOL_T_LGE = 25,                     /* T_LGE  */
  YYSYMBOL_T_IDENT_OP = 26,                /* T_IDENT_OP  */
  YYSYMBOL_T_DIFFER = 27,                  /* T_DIFFER  */
  YYSYMBOL_T_1PLUS = 28,                   /* T_1PLUS  */
  YYSYMBOL_T_1MINUS = 29,                  /* T_1MINUS  */
  YYSYMBOL_T_2EQUAL = 30,                  /* T_2EQUAL  */
  YYSYMBOL_T_PLUS_ASSIGN = 31,             /* T_PLUS_ASSIGN  */
  YYSYMBOL_T_MINUS_ASSIGN = 32,            /* T_MINUS_ASSIGN  */
  YYSYMBOL_T_STAR_ASSIGN = 33,             /* T_STAR_ASSIGN  */
  YYSYMBOL_T_SLASH_ASSIGN = 34,            /* T_SLASH_ASSIGN  */
  YYSYMBOL_T_CARET_ASSIGN = 35,            /* T_CARET_ASSIGN  */
  YYSYMBOL_T_2QUEST = 36,                  /* T_2QUEST  */
  YYSYMBOL_T_2PIPE = 37,                   /* T_2PIPE  */
  YYSYMBOL_T_CONCAT = 38,                  /* T_CONCAT  */
  YYSYMBOL_T_LPAREN = 39,                  /* T_LPAREN  */
  YYSYMBOL_T_RPAREN = 40,                  /* T_RPAREN  */
  YYSYMBOL_T_SEMICOLON = 41,               /* T_SEMICOLON  */
  YYSYMBOL_T_COMMA = 42,                   /* T_COMMA  */
  YYSYMBOL_T_LBRACK = 43,                  /* T_LBRACK  */
  YYSYMBOL_T_RBRACK = 44,                  /* T_RBRACK  */
  YYSYMBOL_T_2DOLLAR = 45,                 /* T_2DOLLAR  */
  YYSYMBOL_T_2DOT = 46,                    /* T_2DOT  */
  YYSYMBOL_T_2AMP = 47,                    /* T_2AMP  */
  YYSYMBOL_T_2AT = 48,                     /* T_2AT  */
  YYSYMBOL_T_2POUND = 49,                  /* T_2POUND  */
  YYSYMBOL_T_2PERCENT = 50,                /* T_2PERCENT  */
  YYSYMBOL_T_2TILDE = 51,                  /* T_2TILDE  */
  YYSYMBOL_T_1STAR = 52,                   /* T_1STAR  */
  YYSYMBOL_T_1SLASH = 53,                  /* T_1SLASH  */
  YYSYMBOL_T_1PERCENT = 54,                /* T_1PERCENT  */
  YYSYMBOL_T_1AT = 55,                     /* T_1AT  */
  YYSYMBOL_T_1TILDE = 56,                  /* T_1TILDE  */
  YYSYMBOL_T_1DOLLAR = 57,                 /* T_1DOLLAR  */
  YYSYMBOL_T_1DOT = 58,                    /* T_1DOT  */
  YYSYMBOL_T_1POUND = 59,                  /* T_1POUND  */
  YYSYMBOL_T_1PIPE = 60,                   /* T_1PIPE  */
  YYSYMBOL_T_1EQUAL = 61,                  /* T_1EQUAL  */
  YYSYMBOL_T_1QUEST = 62,                  /* T_1QUEST  */
  YYSYMBOL_T_1AMP = 63,                    /* T_1AMP  */
  YYSYMBOL_T_COLON = 64,                   /* T_COLON  */
  YYSYMBOL_T_DO = 65,                      /* T_DO  */
  YYSYMBOL_T_FOR = 66,                     /* T_FOR  */
  YYSYMBOL_T_SWITCH = 67,                  /* T_SWITCH  */
  YYSYMBOL_T_CASE = 68,                    /* T_CASE  */
  YYSYMBOL_T_DEFAULT = 69,                 /* T_DEFAULT  */
  YYSYMBOL_T_BREAK = 70,                   /* T_BREAK  */
  YYSYMBOL_T_CONTINUE = 71,                /* T_CONTINUE  */
  YYSYMBOL_T_GOTO = 72,                    /* T_GOTO  */
  YYSYMBOL_T_FUNCTION = 73,                /* T_FUNCTION  */
  YYSYMBOL_T_RETURN = 74,                  /* T_RETURN  */
  YYSYMBOL_T_FRETURN = 75,                 /* T_FRETURN  */
  YYSYMBOL_T_NRETURN = 76,                 /* T_NRETURN  */
  YYSYMBOL_T_STRUCT = 77,                  /* T_STRUCT  */
  YYSYMBOL_T_UNKNOWN = 78,                 /* T_UNKNOWN  */
  YYSYMBOL_T_LBRACE = 79,                  /* T_LBRACE  */
  YYSYMBOL_T_RBRACE = 80,                  /* T_RBRACE  */
  YYSYMBOL_T_IF = 81,                      /* T_IF  */
  YYSYMBOL_T_ELSE = 82,                    /* T_ELSE  */
  YYSYMBOL_T_WHILE = 83,                   /* T_WHILE  */
  YYSYMBOL_YYACCEPT = 84,                  /* $accept  */
  YYSYMBOL_program = 85,                   /* program  */
  YYSYMBOL_stmt_list = 86,                 /* stmt_list  */
  YYSYMBOL_stmt = 87,                      /* stmt  */
  YYSYMBOL_matched_stmt = 88,              /* matched_stmt  */
  YYSYMBOL_unmatched_stmt = 89,            /* unmatched_stmt  */
  YYSYMBOL_if_head = 90,                   /* if_head  */
  YYSYMBOL_while_head = 91,                /* while_head  */
  YYSYMBOL_do_head = 92,                   /* do_head  */
  YYSYMBOL_do_body = 93,                   /* do_body  */
  YYSYMBOL_for_head = 94,                  /* for_head  */
  YYSYMBOL_opt_head_sep = 95,              /* opt_head_sep  */
  YYSYMBOL_func_head = 96,                 /* func_head  */
  YYSYMBOL_func_arglist = 97,              /* func_arglist  */
  YYSYMBOL_func_arglist_ne = 98,           /* func_arglist_ne  */
  YYSYMBOL_else_keyword = 99,              /* else_keyword  */
  YYSYMBOL_simple_stmt = 100,              /* simple_stmt  */
  YYSYMBOL_block_stmt = 101,               /* block_stmt  */
  YYSYMBOL_expr0 = 102,                    /* expr0  */
  YYSYMBOL_expr1 = 103,                    /* expr1  */
  YYSYMBOL_expr3 = 104,                    /* expr3  */
  YYSYMBOL_expr4 = 105,                    /* expr4  */
  YYSYMBOL_expr5 = 106,                    /* expr5  */
  YYSYMBOL_expr6 = 107,                    /* expr6  */
  YYSYMBOL_expr9 = 108,                    /* expr9  */
  YYSYMBOL_expr11 = 109,                   /* expr11  */
  YYSYMBOL_expr15 = 110,                   /* expr15  */
  YYSYMBOL_exprlist = 111,                 /* exprlist  */
  YYSYMBOL_exprlist_ne = 112,              /* exprlist_ne  */
  YYSYMBOL_expr17 = 113                    /* expr17  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 304 "snocone_parse.y"

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
};
/* LS-4.g — do/while: snapshot before body (at KW_DO), cond
 * provided when the trailing while clause is parsed. */
struct DoHead {
    STMT_t *before_body;   /* tail snapshot at KW_DO */
    int     lineno;
};
/* LS-4.g — for (init; cond; step): snapshot after init emits, before
 * the loop-top label and cond-stmt are spliced. */
struct ForHead {
    STMT_t *before_loop;   /* tail snapshot after init stmt appended */
    EXPR_t *cond;          /* loop condition */
    EXPR_t *step;          /* step expression (emitted at loop bottom) */
    int     lineno;
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

#line 554 "snocone_parse.tab.c"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined SC_STYPE_IS_TRIVIAL && SC_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  80
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   536

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  84
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  201

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   338


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83
};

#if SC_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   537,   537,   538,   541,   542,   568,   569,   573,   574,
     575,   577,   584,   587,   590,   592,   597,   599,   601,   604,
     608,   612,   619,   628,   629,   637,   650,   651,   673,   683,
     684,   685,   690,   694,   706,   709,   710,   712,   713,   714,
     715,   718,   719,   762,   764,   768,   772,   776,   780,   784,
     797,   799,   813,   818,   833,   838,   852,   855,   858,   861,
     864,   867,   870,   873,   876,   879,   882,   885,   888,   891,
     894,   898,   900,   902,   906,   908,   910,   917,   919,   947,
     954,   965,   968,   971,   973,   986,   993,   997,  1001,  1003,
    1005,  1007,  1009,  1011,  1015,  1017,  1019,  1021,  1023,  1025,
    1027,  1030,  1032,  1034,  1036,  1038
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if SC_DEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_IDENT", "T_KEYWORD",
  "T_INT", "T_REAL", "T_STR", "T_CALL", "T_2PLUS", "T_2MINUS", "T_2STAR",
  "T_2SLASH", "T_2CARET", "T_EQ", "T_NE", "T_LT", "T_GT", "T_LE", "T_GE",
  "T_LEQ", "T_LNE", "T_LLT", "T_LGT", "T_LLE", "T_LGE", "T_IDENT_OP",
  "T_DIFFER", "T_1PLUS", "T_1MINUS", "T_2EQUAL", "T_PLUS_ASSIGN",
  "T_MINUS_ASSIGN", "T_STAR_ASSIGN", "T_SLASH_ASSIGN", "T_CARET_ASSIGN",
  "T_2QUEST", "T_2PIPE", "T_CONCAT", "T_LPAREN", "T_RPAREN", "T_SEMICOLON",
  "T_COMMA", "T_LBRACK", "T_RBRACK", "T_2DOLLAR", "T_2DOT", "T_2AMP",
  "T_2AT", "T_2POUND", "T_2PERCENT", "T_2TILDE", "T_1STAR", "T_1SLASH",
  "T_1PERCENT", "T_1AT", "T_1TILDE", "T_1DOLLAR", "T_1DOT", "T_1POUND",
  "T_1PIPE", "T_1EQUAL", "T_1QUEST", "T_1AMP", "T_COLON", "T_DO", "T_FOR",
  "T_SWITCH", "T_CASE", "T_DEFAULT", "T_BREAK", "T_CONTINUE", "T_GOTO",
  "T_FUNCTION", "T_RETURN", "T_FRETURN", "T_NRETURN", "T_STRUCT",
  "T_UNKNOWN", "T_LBRACE", "T_RBRACE", "T_IF", "T_ELSE", "T_WHILE",
  "$accept", "program", "stmt_list", "stmt", "matched_stmt",
  "unmatched_stmt", "if_head", "while_head", "do_head", "do_body",
  "for_head", "opt_head_sep", "func_head", "func_arglist",
  "func_arglist_ne", "else_keyword", "simple_stmt", "block_stmt", "expr0",
  "expr1", "expr3", "expr4", "expr5", "expr6", "expr9", "expr11", "expr15",
  "exprlist", "exprlist_ne", "expr17", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-164)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     364,  -164,  -164,  -164,  -164,  -164,   -30,   445,   445,   445,
    -164,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,  -164,   -23,    29,   403,   -21,    10,    68,
      14,    18,    49,   364,  -164,  -164,  -164,   364,   364,   -19,
     364,   -15,  -164,  -164,    24,     9,    -2,    28,   495,    46,
      47,  -164,    -5,  -164,   445,  -164,  -164,    27,  -164,  -164,
    -164,  -164,  -164,  -164,  -164,  -164,  -164,  -164,  -164,  -164,
     445,    38,  -164,    39,  -164,  -164,  -164,   149,   445,   445,
    -164,  -164,  -164,   -14,  -164,  -164,   192,    -4,  -164,  -164,
     235,  -164,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,  -164,    41,    48,  -164,    50,     7,  -164,  -164,    52,
      53,  -164,   364,  -164,   278,    55,  -164,   321,  -164,  -164,
    -164,  -164,  -164,  -164,  -164,    28,   495,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    47,    47,  -164,  -164,  -164,    45,  -164,   445,   445,
       8,  -164,    60,    12,    60,    60,  -164,  -164,  -164,   445,
    -164,  -164,  -164,    59,  -164,    98,  -164,  -164,  -164,    99,
    -164,  -164,    64,   445,  -164,  -164,    67,    65,  -164,    60,
    -164
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    86,    87,    88,    89,    90,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     5,     6,     7,     0,     0,     0,
       0,     0,     8,     9,     0,    49,    51,    53,    55,    70,
      73,    76,    78,    80,    82,    92,    93,     0,    94,   102,
     101,    97,    98,    96,    95,   103,   104,   105,    99,   100,
       0,     0,    38,     0,    39,    40,    42,     0,     0,     0,
       1,     4,    16,     6,    11,    18,     0,     0,    13,    19,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,    84,     0,    81,    91,     0,     0,    37,    41,     0,
       0,    34,     0,    24,     0,     0,    15,     0,    43,    44,
      45,    46,    47,    48,    50,    52,    54,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    71,    72,    74,    75,    77,     0,    85,     0,     0,
       0,    29,    26,     0,    26,    26,    10,    17,    23,     0,
      14,    79,    83,     0,    30,     0,    27,    28,    31,     0,
      20,    21,     0,     0,    32,    33,     0,     0,    12,    26,
      25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -164,  -164,    -8,   -31,   -33,   -37,  -164,  -164,  -164,  -164,
    -164,  -163,  -164,  -164,  -164,  -164,  -164,  -164,    -9,    15,
    -164,    13,    16,   422,   -53,  -104,  -164,   -10,  -164,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    32,    33,    34,    35,    36,    37,    38,    39,    87,
      40,   187,    41,   172,   173,   132,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,   122,   123,    53
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,    85,    81,    89,    83,    84,    82,    88,   119,    54,
     170,   190,   191,   163,   164,   165,    70,    73,    55,    56,
      74,    77,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    98,    99,   200,    71,   120,    92,
      93,    94,    95,    96,    97,   121,    81,   171,   184,    80,
     185,    75,   188,    78,   189,   115,   116,    79,   117,   118,
      86,   125,   161,   162,    90,    91,   100,   124,   131,   129,
     130,     1,     2,     3,     4,     5,     6,   126,   134,   135,
     127,   167,   137,   138,   139,   140,   141,   142,   143,   181,
     168,   169,   174,   175,   179,   177,     7,     8,   186,   176,
     193,   194,   195,    81,   196,   199,    81,     9,   198,    10,
     166,   121,   145,   144,     0,     0,   146,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     0,    23,    24,     0,     0,     0,     0,     0,
       0,    25,    26,    27,    28,     0,     0,    29,    76,    30,
       0,    31,     1,     2,     3,     4,     5,     6,     0,   182,
     183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,   197,     0,     0,     0,     9,     0,
      10,     0,     0,     0,     0,     1,     2,     3,     4,     5,
       6,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,     0,    23,    24,     0,     0,     0,     0,
       7,     8,    25,    26,    27,    28,     0,     0,    29,   128,
      30,     9,    31,    10,     0,     0,     0,     0,     1,     2,
       3,     4,     5,     6,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     0,    23,    24,     0,
       0,     0,     0,     7,     8,    25,    26,    27,    28,     0,
       0,    29,   133,    30,     9,    31,    10,     0,     0,     0,
       0,     1,     2,     3,     4,     5,     6,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,     0,
      23,    24,     0,     0,     0,     0,     7,     8,    25,    26,
      27,    28,     0,     0,    29,   136,    30,     9,    31,    10,
       0,     0,     0,     0,     1,     2,     3,     4,     5,     6,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     0,    23,    24,     0,     0,     0,     0,     7,
       8,    25,    26,    27,    28,     0,     0,    29,   178,    30,
       9,    31,    10,     0,     0,     0,     0,     1,     2,     3,
       4,     5,     6,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,     0,    23,    24,     0,     0,
       0,     0,     7,     8,    25,    26,    27,    28,     0,     0,
      29,   180,    30,     9,    31,    10,     1,     2,     3,     4,
       5,     6,     0,     0,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     0,    23,
      24,     7,     8,     0,     0,     0,     0,    25,    26,    27,
      28,     0,     9,    29,    72,    30,     0,    31,     1,     2,
       3,     4,     5,     6,     0,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160
};

static const yytype_int16 yycheck[] =
{
       9,    38,    33,    40,    37,    38,    37,    40,    13,    39,
       3,   174,   175,   117,   118,   119,    39,    26,     7,     8,
      41,    29,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    36,    37,   199,     8,    43,    30,
      31,    32,    33,    34,    35,    54,    77,    40,    40,     0,
      42,    41,    40,    39,    42,     9,    10,    39,    11,    12,
      79,    70,   115,   116,    79,    41,    38,    40,    82,    78,
      79,     3,     4,     5,     6,     7,     8,    39,    86,    83,
      41,    40,    90,    92,    93,    94,    95,    96,    97,    44,
      42,    41,    40,    40,    39,   132,    28,    29,    38,   132,
      41,     3,     3,   134,    40,    40,   137,    39,    41,    41,
     120,   120,    99,    98,    -1,    -1,   100,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    75,    76,    -1,    -1,    79,    80,    81,
      -1,    83,     3,     4,     5,     6,     7,     8,    -1,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,    39,    -1,
      41,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      28,    29,    73,    74,    75,    76,    -1,    -1,    79,    80,
      81,    39,    83,    41,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    28,    29,    73,    74,    75,    76,    -1,
      -1,    79,    80,    81,    39,    83,    41,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    28,    29,    73,    74,
      75,    76,    -1,    -1,    79,    80,    81,    39,    83,    41,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    28,
      29,    73,    74,    75,    76,    -1,    -1,    79,    80,    81,
      39,    83,    41,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    28,    29,    73,    74,    75,    76,    -1,    -1,
      79,    80,    81,    39,    83,    41,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    28,    29,    -1,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    39,    79,    41,    81,    -1,    83,     3,     4,
       5,     6,     7,     8,    -1,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    28,    29,    39,
      41,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    65,    66,    73,    74,    75,    76,    79,
      81,    83,    85,    86,    87,    88,    89,    90,    91,    92,
      94,    96,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   113,    39,   113,   113,   102,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
      39,     8,    41,   102,    41,    41,    80,    86,    39,    39,
       0,    87,    87,    88,    88,    89,    79,    93,    88,    89,
      79,    41,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     9,    10,    11,    12,    13,
      43,   102,   111,   112,    40,   102,    39,    41,    80,   102,
     102,    82,    99,    80,    86,    83,    80,    86,   102,   102,
     102,   102,   102,   102,   103,   105,   106,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   108,   108,   109,   109,   109,   111,    40,    42,    41,
       3,    40,    97,    98,    40,    40,    88,    89,    80,    39,
      80,    44,   102,   102,    40,    42,    38,    95,    40,    42,
      95,    95,   102,    41,     3,     3,    40,   102,    41,    40,
      95
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    84,    85,    85,    86,    86,    87,    87,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    89,    89,
      90,    91,    92,    93,    93,    94,    95,    95,    96,    97,
      97,    97,    98,    98,    99,   100,   100,   100,   100,   100,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   107,   107,   107,   108,   108,   108,   109,   109,   110,
     110,   111,   111,   112,   112,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       4,     2,     7,     2,     4,     3,     2,     4,     2,     2,
       5,     5,     1,     3,     2,     9,     0,     1,     5,     1,
       2,     2,     3,     3,     1,     2,     1,     3,     2,     2,
       2,     3,     2,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     1,     4,
       1,     1,     0,     3,     1,     4,     1,     1,     1,     1,
       1,     3,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = SC_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == SC_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (st, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SC_error or SC_UNDEF. */
#define YYERRCODE SC_UNDEF


/* Enable debugging if requested.  */
#if SC_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, st); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ScParseState *st)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (st);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ScParseState *st)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, st);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, ScParseState *st)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], st);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, st); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !SC_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !SC_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, ScParseState *st)
{
  YY_USE (yyvaluep);
  YY_USE (st);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (ScParseState *st)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = SC_EMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == SC_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, st);
    }

  if (yychar <= SC_EOF)
    {
      yychar = SC_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == SC_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = SC_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = SC_EMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 10: /* matched_stmt: if_head matched_stmt else_keyword matched_stmt  */
#line 576 "snocone_parse.y"
                                        { sc_finalize_if_else(st, (yyvsp[-3].ifhead), (yyvsp[-1].stmt_ptr)); }
#line 1728 "snocone_parse.tab.c"
    break;

  case 11: /* matched_stmt: while_head matched_stmt  */
#line 578 "snocone_parse.y"
                                        { sc_finalize_while(st, (yyvsp[-1].whilehead)); }
#line 1734 "snocone_parse.tab.c"
    break;

  case 12: /* matched_stmt: do_head do_body T_WHILE T_LPAREN expr0 T_RPAREN T_SEMICOLON  */
#line 585 "snocone_parse.y"
                                        { sc_finalize_do_while(st, (yyvsp[-6].dohead), (yyvsp[-2].expr)); }
#line 1740 "snocone_parse.tab.c"
    break;

  case 13: /* matched_stmt: for_head matched_stmt  */
#line 588 "snocone_parse.y"
                                        { sc_finalize_for(st, (yyvsp[-1].forhead)); }
#line 1746 "snocone_parse.tab.c"
    break;

  case 14: /* matched_stmt: func_head T_LBRACE stmt_list T_RBRACE  */
#line 591 "snocone_parse.y"
                                        { sc_finalize_function(st, (yyvsp[-3].funchead)); }
#line 1752 "snocone_parse.tab.c"
    break;

  case 15: /* matched_stmt: func_head T_LBRACE T_RBRACE  */
#line 593 "snocone_parse.y"
                                        { sc_finalize_function(st, (yyvsp[-2].funchead)); }
#line 1758 "snocone_parse.tab.c"
    break;

  case 16: /* unmatched_stmt: if_head stmt  */
#line 598 "snocone_parse.y"
                                        { sc_finalize_if_no_else(st, (yyvsp[-1].ifhead)); }
#line 1764 "snocone_parse.tab.c"
    break;

  case 17: /* unmatched_stmt: if_head matched_stmt else_keyword unmatched_stmt  */
#line 600 "snocone_parse.y"
                                        { sc_finalize_if_else(st, (yyvsp[-3].ifhead), (yyvsp[-1].stmt_ptr)); }
#line 1770 "snocone_parse.tab.c"
    break;

  case 18: /* unmatched_stmt: while_head unmatched_stmt  */
#line 602 "snocone_parse.y"
                                        { sc_finalize_while(st, (yyvsp[-1].whilehead)); }
#line 1776 "snocone_parse.tab.c"
    break;

  case 19: /* unmatched_stmt: for_head unmatched_stmt  */
#line 605 "snocone_parse.y"
                                        { sc_finalize_for(st, (yyvsp[-1].forhead)); }
#line 1782 "snocone_parse.tab.c"
    break;

  case 20: /* if_head: T_IF T_LPAREN expr0 T_RPAREN opt_head_sep  */
#line 609 "snocone_parse.y"
                                        { (yyval.ifhead) = sc_if_head_new(st, (yyvsp[-2].expr)); }
#line 1788 "snocone_parse.tab.c"
    break;

  case 21: /* while_head: T_WHILE T_LPAREN expr0 T_RPAREN opt_head_sep  */
#line 613 "snocone_parse.y"
                                        { (yyval.whilehead) = sc_while_head_new(st, (yyvsp[-2].expr)); }
#line 1794 "snocone_parse.tab.c"
    break;

  case 22: /* do_head: T_DO  */
#line 619 "snocone_parse.y"
                                    { (yyval.dohead) = sc_do_head_new(st); }
#line 1800 "snocone_parse.tab.c"
    break;

  case 25: /* for_head: T_FOR T_LPAREN expr0 T_SEMICOLON expr0 T_SEMICOLON expr0 T_RPAREN opt_head_sep  */
#line 638 "snocone_parse.y"
                                        { sc_append_stmt(st, (yyvsp[-6].expr));
                                          (yyval.forhead) = sc_for_head_new(st, (yyvsp[-4].expr), (yyvsp[-2].expr)); }
#line 1807 "snocone_parse.tab.c"
    break;

  case 28: /* func_head: T_FUNCTION T_CALL T_LPAREN func_arglist opt_head_sep  */
#line 674 "snocone_parse.y"
                                        { (yyval.funchead) = sc_func_head_new(st, (yyvsp[-3].str), (yyvsp[-1].str)); free((yyvsp[-3].str)); free((yyvsp[-1].str)); }
#line 1813 "snocone_parse.tab.c"
    break;

  case 29: /* func_arglist: T_RPAREN  */
#line 683 "snocone_parse.y"
                                       { (yyval.str) = strdup(""); }
#line 1819 "snocone_parse.tab.c"
    break;

  case 30: /* func_arglist: T_IDENT T_RPAREN  */
#line 684 "snocone_parse.y"
                                       { (yyval.str) = strdup((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1825 "snocone_parse.tab.c"
    break;

  case 31: /* func_arglist: func_arglist_ne T_RPAREN  */
#line 685 "snocone_parse.y"
                                       { (yyval.str) = (yyvsp[-1].str); }
#line 1831 "snocone_parse.tab.c"
    break;

  case 32: /* func_arglist_ne: T_IDENT T_COMMA T_IDENT  */
#line 691 "snocone_parse.y"
                { int len = strlen((yyvsp[-2].str)) + 1 + strlen((yyvsp[0].str)) + 1;
                  char *s = malloc(len); snprintf(s, len, "%s,%s", (yyvsp[-2].str), (yyvsp[0].str));
                  free((yyvsp[-2].str)); free((yyvsp[0].str)); (yyval.str) = s; }
#line 1839 "snocone_parse.tab.c"
    break;

  case 33: /* func_arglist_ne: func_arglist_ne T_COMMA T_IDENT  */
#line 695 "snocone_parse.y"
                { int len = strlen((yyvsp[-2].str)) + 1 + strlen((yyvsp[0].str)) + 1;
                  char *s = malloc(len); snprintf(s, len, "%s,%s", (yyvsp[-2].str), (yyvsp[0].str));
                  free((yyvsp[-2].str)); free((yyvsp[0].str)); (yyval.str) = s; }
#line 1847 "snocone_parse.tab.c"
    break;

  case 34: /* else_keyword: T_ELSE  */
#line 706 "snocone_parse.y"
                                     { (yyval.stmt_ptr) = st->code->tail; }
#line 1853 "snocone_parse.tab.c"
    break;

  case 35: /* simple_stmt: expr0 T_SEMICOLON  */
#line 709 "snocone_parse.y"
                                               { sc_append_stmt(st, (yyvsp[-1].expr)); }
#line 1859 "snocone_parse.tab.c"
    break;

  case 36: /* simple_stmt: T_SEMICOLON  */
#line 710 "snocone_parse.y"
                                               { /* empty stmt */         }
#line 1865 "snocone_parse.tab.c"
    break;

  case 37: /* simple_stmt: T_RETURN expr0 T_SEMICOLON  */
#line 712 "snocone_parse.y"
                                            { sc_append_return(st, (yyvsp[-1].expr)); }
#line 1871 "snocone_parse.tab.c"
    break;

  case 38: /* simple_stmt: T_RETURN T_SEMICOLON  */
#line 713 "snocone_parse.y"
                                            { sc_append_return(st, NULL); }
#line 1877 "snocone_parse.tab.c"
    break;

  case 39: /* simple_stmt: T_FRETURN T_SEMICOLON  */
#line 714 "snocone_parse.y"
                                            { sc_append_freturn(st); }
#line 1883 "snocone_parse.tab.c"
    break;

  case 40: /* simple_stmt: T_NRETURN T_SEMICOLON  */
#line 715 "snocone_parse.y"
                                            { sc_append_nreturn(st); }
#line 1889 "snocone_parse.tab.c"
    break;

  case 41: /* block_stmt: T_LBRACE stmt_list T_RBRACE  */
#line 718 "snocone_parse.y"
                                               { /* statements already appended */ }
#line 1895 "snocone_parse.tab.c"
    break;

  case 42: /* block_stmt: T_LBRACE T_RBRACE  */
#line 719 "snocone_parse.y"
                                               { /* empty block */                  }
#line 1901 "snocone_parse.tab.c"
    break;

  case 43: /* expr0: expr1 T_2EQUAL expr0  */
#line 763 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1907 "snocone_parse.tab.c"
    break;

  case 44: /* expr0: expr1 T_PLUS_ASSIGN expr0  */
#line 765 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_ADD, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1915 "snocone_parse.tab.c"
    break;

  case 45: /* expr0: expr1 T_MINUS_ASSIGN expr0  */
#line 769 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_SUB, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1923 "snocone_parse.tab.c"
    break;

  case 46: /* expr0: expr1 T_STAR_ASSIGN expr0  */
#line 773 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_MUL, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1931 "snocone_parse.tab.c"
    break;

  case 47: /* expr0: expr1 T_SLASH_ASSIGN expr0  */
#line 777 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_DIV, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1939 "snocone_parse.tab.c"
    break;

  case 48: /* expr0: expr1 T_CARET_ASSIGN expr0  */
#line 781 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_POW, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1947 "snocone_parse.tab.c"
    break;

  case 49: /* expr0: expr1  */
#line 785 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1953 "snocone_parse.tab.c"
    break;

  case 50: /* expr1: expr3 T_2QUEST expr1  */
#line 798 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_SCAN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1959 "snocone_parse.tab.c"
    break;

  case 51: /* expr1: expr3  */
#line 800 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1965 "snocone_parse.tab.c"
    break;

  case 52: /* expr3: expr3 T_2PIPE expr4  */
#line 814 "snocone_parse.y"
                                { if ((yyvsp[-2].expr)->kind == E_ALT) { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
                                  else { EXPR_t *a = expr_new(E_ALT);
                                         expr_add_child(a, (yyvsp[-2].expr)); expr_add_child(a, (yyvsp[0].expr));
                                         (yyval.expr) = a; } }
#line 1974 "snocone_parse.tab.c"
    break;

  case 53: /* expr3: expr4  */
#line 819 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1980 "snocone_parse.tab.c"
    break;

  case 54: /* expr4: expr4 T_CONCAT expr5  */
#line 834 "snocone_parse.y"
                                { if ((yyvsp[-2].expr)->kind == E_SEQ) { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
                                  else { EXPR_t *s = expr_new(E_SEQ);
                                         expr_add_child(s, (yyvsp[-2].expr)); expr_add_child(s, (yyvsp[0].expr));
                                         (yyval.expr) = s; } }
#line 1989 "snocone_parse.tab.c"
    break;

  case 55: /* expr4: expr5  */
#line 839 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1995 "snocone_parse.tab.c"
    break;

  case 56: /* expr5: expr5 T_EQ expr6  */
#line 853 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("EQ");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2002 "snocone_parse.tab.c"
    break;

  case 57: /* expr5: expr5 T_NE expr6  */
#line 856 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("NE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2009 "snocone_parse.tab.c"
    break;

  case 58: /* expr5: expr5 T_LT expr6  */
#line 859 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2016 "snocone_parse.tab.c"
    break;

  case 59: /* expr5: expr5 T_GT expr6  */
#line 862 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("GT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2023 "snocone_parse.tab.c"
    break;

  case 60: /* expr5: expr5 T_LE expr6  */
#line 865 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2030 "snocone_parse.tab.c"
    break;

  case 61: /* expr5: expr5 T_GE expr6  */
#line 868 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("GE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2037 "snocone_parse.tab.c"
    break;

  case 62: /* expr5: expr5 T_LEQ expr6  */
#line 871 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LEQ");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2044 "snocone_parse.tab.c"
    break;

  case 63: /* expr5: expr5 T_LNE expr6  */
#line 874 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LNE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2051 "snocone_parse.tab.c"
    break;

  case 64: /* expr5: expr5 T_LLT expr6  */
#line 877 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LLT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2058 "snocone_parse.tab.c"
    break;

  case 65: /* expr5: expr5 T_LGT expr6  */
#line 880 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LGT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2065 "snocone_parse.tab.c"
    break;

  case 66: /* expr5: expr5 T_LLE expr6  */
#line 883 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LLE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2072 "snocone_parse.tab.c"
    break;

  case 67: /* expr5: expr5 T_LGE expr6  */
#line 886 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LGE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2079 "snocone_parse.tab.c"
    break;

  case 68: /* expr5: expr5 T_IDENT_OP expr6  */
#line 889 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("IDENT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2086 "snocone_parse.tab.c"
    break;

  case 69: /* expr5: expr5 T_DIFFER expr6  */
#line 892 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("DIFFER");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 2093 "snocone_parse.tab.c"
    break;

  case 70: /* expr5: expr6  */
#line 895 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2099 "snocone_parse.tab.c"
    break;

  case 71: /* expr6: expr6 T_2PLUS expr9  */
#line 899 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2105 "snocone_parse.tab.c"
    break;

  case 72: /* expr6: expr6 T_2MINUS expr9  */
#line 901 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2111 "snocone_parse.tab.c"
    break;

  case 73: /* expr6: expr9  */
#line 903 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2117 "snocone_parse.tab.c"
    break;

  case 74: /* expr9: expr9 T_2STAR expr11  */
#line 907 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2123 "snocone_parse.tab.c"
    break;

  case 75: /* expr9: expr9 T_2SLASH expr11  */
#line 909 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2129 "snocone_parse.tab.c"
    break;

  case 76: /* expr9: expr11  */
#line 911 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2135 "snocone_parse.tab.c"
    break;

  case 77: /* expr11: expr15 T_2CARET expr11  */
#line 918 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_POW, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2141 "snocone_parse.tab.c"
    break;

  case 78: /* expr11: expr15  */
#line 920 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2147 "snocone_parse.tab.c"
    break;

  case 79: /* expr15: expr15 T_LBRACK exprlist T_RBRACK  */
#line 948 "snocone_parse.y"
                                { EXPR_t *idx = expr_new(E_IDX);
                                  expr_add_child(idx, (yyvsp[-3].expr));
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(idx, (yyvsp[-1].expr)->children[i]);
                                  free((yyvsp[-1].expr)->children); free((yyvsp[-1].expr));
                                  (yyval.expr) = idx; }
#line 2158 "snocone_parse.tab.c"
    break;

  case 80: /* expr15: expr17  */
#line 955 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2164 "snocone_parse.tab.c"
    break;

  case 81: /* exprlist: exprlist_ne  */
#line 966 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2170 "snocone_parse.tab.c"
    break;

  case 82: /* exprlist: %empty  */
#line 968 "snocone_parse.y"
                                { (yyval.expr) = expr_new(E_NUL); }
#line 2176 "snocone_parse.tab.c"
    break;

  case 83: /* exprlist_ne: exprlist_ne T_COMMA expr0  */
#line 972 "snocone_parse.y"
                                { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
#line 2182 "snocone_parse.tab.c"
    break;

  case 84: /* exprlist_ne: expr0  */
#line 974 "snocone_parse.y"
                                { EXPR_t *l = expr_new(E_NUL); expr_add_child(l, (yyvsp[0].expr)); (yyval.expr) = l; }
#line 2188 "snocone_parse.tab.c"
    break;

  case 85: /* expr17: T_CALL T_LPAREN exprlist T_RPAREN  */
#line 987 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC);
                                  e->sval = (yyvsp[-3].str);             /* takes ownership */
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(e, (yyvsp[-1].expr)->children[i]);
                                  free((yyvsp[-1].expr)->children); free((yyvsp[-1].expr));
                                  (yyval.expr) = e; }
#line 2199 "snocone_parse.tab.c"
    break;

  case 86: /* expr17: T_IDENT  */
#line 994 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_VAR);
                                  e->sval = (yyvsp[0].str);             /* takes ownership */
                                  (yyval.expr) = e; }
#line 2207 "snocone_parse.tab.c"
    break;

  case 87: /* expr17: T_KEYWORD  */
#line 998 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_KEYWORD);
                                  e->sval = (yyvsp[0].str);
                                  (yyval.expr) = e; }
#line 2215 "snocone_parse.tab.c"
    break;

  case 88: /* expr17: T_INT  */
#line 1002 "snocone_parse.y"
                                { (yyval.expr) = sc_int_literal((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 2221 "snocone_parse.tab.c"
    break;

  case 89: /* expr17: T_REAL  */
#line 1004 "snocone_parse.y"
                                { (yyval.expr) = sc_real_literal((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 2227 "snocone_parse.tab.c"
    break;

  case 90: /* expr17: T_STR  */
#line 1006 "snocone_parse.y"
                                { (yyval.expr) = sc_str_literal((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 2233 "snocone_parse.tab.c"
    break;

  case 91: /* expr17: T_LPAREN expr0 T_RPAREN  */
#line 1008 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 2239 "snocone_parse.tab.c"
    break;

  case 92: /* expr17: T_1PLUS expr17  */
#line 1010 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_PLS, (yyvsp[0].expr)); }
#line 2245 "snocone_parse.tab.c"
    break;

  case 93: /* expr17: T_1MINUS expr17  */
#line 1012 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_MNS, (yyvsp[0].expr)); }
#line 2251 "snocone_parse.tab.c"
    break;

  case 94: /* expr17: T_1STAR expr17  */
#line 1015 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_DEFER,       (yyvsp[0].expr)); }
#line 2257 "snocone_parse.tab.c"
    break;

  case 95: /* expr17: T_1DOT expr17  */
#line 1017 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_NAME,        (yyvsp[0].expr)); }
#line 2263 "snocone_parse.tab.c"
    break;

  case 96: /* expr17: T_1DOLLAR expr17  */
#line 1019 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_INDIRECT,    (yyvsp[0].expr)); }
#line 2269 "snocone_parse.tab.c"
    break;

  case 97: /* expr17: T_1AT expr17  */
#line 1021 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_CAPT_CURSOR, (yyvsp[0].expr)); }
#line 2275 "snocone_parse.tab.c"
    break;

  case 98: /* expr17: T_1TILDE expr17  */
#line 1023 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_NOT,         (yyvsp[0].expr)); }
#line 2281 "snocone_parse.tab.c"
    break;

  case 99: /* expr17: T_1QUEST expr17  */
#line 1025 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_INTERROGATE, (yyvsp[0].expr)); }
#line 2287 "snocone_parse.tab.c"
    break;

  case 100: /* expr17: T_1AMP expr17  */
#line 1027 "snocone_parse.y"
                                { EXPR_t *_e = expr_unary(E_OPSYN, (yyvsp[0].expr));
                                  _e->sval = strdup("&"); (yyval.expr) = _e; }
#line 2294 "snocone_parse.tab.c"
    break;

  case 101: /* expr17: T_1PERCENT expr17  */
#line 1030 "snocone_parse.y"
                                { EXPR_t *_e = expr_unary(E_OPSYN, (yyvsp[0].expr));
                                  _e->sval = strdup("%"); (yyval.expr) = _e; }
#line 2301 "snocone_parse.tab.c"
    break;

  case 102: /* expr17: T_1SLASH expr17  */
#line 1032 "snocone_parse.y"
                                { EXPR_t *_e = expr_unary(E_OPSYN, (yyvsp[0].expr));
                                  _e->sval = strdup("/"); (yyval.expr) = _e; }
#line 2308 "snocone_parse.tab.c"
    break;

  case 103: /* expr17: T_1POUND expr17  */
#line 1034 "snocone_parse.y"
                                { EXPR_t *_e = expr_unary(E_OPSYN, (yyvsp[0].expr));
                                  _e->sval = strdup("#"); (yyval.expr) = _e; }
#line 2315 "snocone_parse.tab.c"
    break;

  case 104: /* expr17: T_1PIPE expr17  */
#line 1036 "snocone_parse.y"
                                { EXPR_t *_e = expr_unary(E_OPSYN, (yyvsp[0].expr));
                                  _e->sval = strdup("|"); (yyval.expr) = _e; }
#line 2322 "snocone_parse.tab.c"
    break;

  case 105: /* expr17: T_1EQUAL expr17  */
#line 1038 "snocone_parse.y"
                                { EXPR_t *_e = expr_unary(E_OPSYN, (yyvsp[0].expr));
                                  _e->sval = strdup("="); (yyval.expr) = _e; }
#line 2329 "snocone_parse.tab.c"
    break;


#line 2333 "snocone_parse.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == SC_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (st, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= SC_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == SC_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, st);
          yychar = SC_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, st);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (st, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != SC_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, st);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, st);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1042 "snocone_parse.y"


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
    return h;
}

/* LS-4.g — do_head: snapshot before the do-body is parsed. */
static struct DoHead *sc_do_head_new(ScParseState *st) {
    struct DoHead *h = calloc(1, sizeof *h);
    h->before_body = st->code->tail;
    h->lineno      = st->ctx ? st->ctx->line : 0;
    return h;
}

/* LS-4.g — for_head: called AFTER the init expr is emitted, so
 * before_loop snaps the tail that now includes the init stmt. */
static struct ForHead *sc_for_head_new(ScParseState *st, EXPR_t *cond, EXPR_t *step) {
    struct ForHead *h = calloc(1, sizeof *h);
    h->before_loop = st->code->tail;
    h->cond        = cond;
    h->step        = step;
    h->lineno      = st->ctx ? st->ctx->line : 0;
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
    STMT_t *s = stmt_new();
    s->lineno = st->ctx ? st->ctx->line : 0;
    s->stno   = ++st->code->nstmts;
    s->go     = sgoto_new();
    s->go->uncond = strdup("FRETURN");
    if (!st->code->head) st->code->head = st->code->tail = s;
    else { st->code->tail->next = s; st->code->tail = s; }
}

static void sc_append_nreturn(ScParseState *st) {
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
 * `fail_target`.  Takes ownership of both. */
static STMT_t *sc_make_cond_fail_stmt(ScParseState *st, EXPR_t *cond, char *fail_target, int lineno) {
    STMT_t *s = stmt_new();
    s->lineno = lineno;
    s->stno   = ++st->code->nstmts;
    s->subject = cond;
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
    char   *Ltop      = sc_label_new(st, "_Ltop");
    char   *Lend      = sc_label_new(st, "_Lend");
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
 * `succ_target`.  Takes ownership of both. */
static STMT_t *sc_make_cond_succ_stmt(ScParseState *st, EXPR_t *cond, char *succ_target, int lineno) {
    STMT_t *s = stmt_new();
    s->lineno  = lineno;
    s->stno    = ++st->code->nstmts;
    s->subject = cond;
    s->go      = sgoto_new();
    s->go->onsuccess = succ_target;
    return s;
}

static void sc_finalize_do_while(ScParseState *st, struct DoHead *h, EXPR_t *cond) {
    char   *Ltop      = sc_label_new(st, "_Ltop");
    char   *Lend      = sc_label_new(st, "_Lend");
    STMT_t *top_pad   = sc_make_label_stmt(st, Ltop);
    STMT_t *cond_stmt = sc_make_cond_succ_stmt(st, cond, strdup(Ltop), h->lineno);
    STMT_t *end_pad   = sc_make_label_stmt(st, Lend);
    /* Splice Ltop-pad right before the do-body. */
    sc_splice_after(st, h->before_body, top_pad, top_pad);
    /* Append cond :S(Ltop) and then Lend. */
    cond_stmt->next = end_pad;
    sc_append_chain(st, cond_stmt, end_pad);
    free(h);
}

static void sc_finalize_for(ScParseState *st, struct ForHead *h) {
    char   *Ltop      = sc_label_new(st, "_Ltop");
    char   *Lend      = sc_label_new(st, "_Lend");
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
    /* Append step, goto Ltop, Lend. */
    step_stmt->next = goto_top;
    goto_top->next  = end_pad;
    sc_append_chain(st, step_stmt, end_pad);
    free(h);
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
