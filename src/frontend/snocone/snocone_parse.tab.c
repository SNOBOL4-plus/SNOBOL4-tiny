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
#line 71 "snocone_parse.y"

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

#line 267 "snocone_parse.tab.c"
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
  YYSYMBOL_T_FUNCTION = 8,                 /* T_FUNCTION  */
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
  YYSYMBOL_T_2DOLLAR = 43,                 /* T_2DOLLAR  */
  YYSYMBOL_T_2DOT = 44,                    /* T_2DOT  */
  YYSYMBOL_T_2AMP = 45,                    /* T_2AMP  */
  YYSYMBOL_T_2AT = 46,                     /* T_2AT  */
  YYSYMBOL_T_2POUND = 47,                  /* T_2POUND  */
  YYSYMBOL_T_2PERCENT = 48,                /* T_2PERCENT  */
  YYSYMBOL_T_2TILDE = 49,                  /* T_2TILDE  */
  YYSYMBOL_T_1STAR = 50,                   /* T_1STAR  */
  YYSYMBOL_T_1SLASH = 51,                  /* T_1SLASH  */
  YYSYMBOL_T_1PERCENT = 52,                /* T_1PERCENT  */
  YYSYMBOL_T_1AT = 53,                     /* T_1AT  */
  YYSYMBOL_T_1TILDE = 54,                  /* T_1TILDE  */
  YYSYMBOL_T_1DOLLAR = 55,                 /* T_1DOLLAR  */
  YYSYMBOL_T_1DOT = 56,                    /* T_1DOT  */
  YYSYMBOL_T_1POUND = 57,                  /* T_1POUND  */
  YYSYMBOL_T_1PIPE = 58,                   /* T_1PIPE  */
  YYSYMBOL_T_1EQUAL = 59,                  /* T_1EQUAL  */
  YYSYMBOL_T_1QUEST = 60,                  /* T_1QUEST  */
  YYSYMBOL_T_1AMP = 61,                    /* T_1AMP  */
  YYSYMBOL_T_LBRACK = 62,                  /* T_LBRACK  */
  YYSYMBOL_T_RBRACK = 63,                  /* T_RBRACK  */
  YYSYMBOL_T_LBRACE = 64,                  /* T_LBRACE  */
  YYSYMBOL_T_RBRACE = 65,                  /* T_RBRACE  */
  YYSYMBOL_T_COLON = 66,                   /* T_COLON  */
  YYSYMBOL_T_KW_IF = 67,                   /* T_KW_IF  */
  YYSYMBOL_T_KW_ELSE = 68,                 /* T_KW_ELSE  */
  YYSYMBOL_T_KW_WHILE = 69,                /* T_KW_WHILE  */
  YYSYMBOL_T_KW_DO = 70,                   /* T_KW_DO  */
  YYSYMBOL_T_KW_UNTIL = 71,                /* T_KW_UNTIL  */
  YYSYMBOL_T_KW_FOR = 72,                  /* T_KW_FOR  */
  YYSYMBOL_T_KW_SWITCH = 73,               /* T_KW_SWITCH  */
  YYSYMBOL_T_KW_CASE = 74,                 /* T_KW_CASE  */
  YYSYMBOL_T_KW_DEFAULT = 75,              /* T_KW_DEFAULT  */
  YYSYMBOL_T_KW_BREAK = 76,                /* T_KW_BREAK  */
  YYSYMBOL_T_KW_CONTINUE = 77,             /* T_KW_CONTINUE  */
  YYSYMBOL_T_KW_GOTO = 78,                 /* T_KW_GOTO  */
  YYSYMBOL_T_KW_FUNCTION = 79,             /* T_KW_FUNCTION  */
  YYSYMBOL_T_KW_RETURN = 80,               /* T_KW_RETURN  */
  YYSYMBOL_T_KW_FRETURN = 81,              /* T_KW_FRETURN  */
  YYSYMBOL_T_KW_NRETURN = 82,              /* T_KW_NRETURN  */
  YYSYMBOL_T_KW_STRUCT = 83,               /* T_KW_STRUCT  */
  YYSYMBOL_T_UNKNOWN = 84,                 /* T_UNKNOWN  */
  YYSYMBOL_YYACCEPT = 85,                  /* $accept  */
  YYSYMBOL_program = 86,                   /* program  */
  YYSYMBOL_stmt_list = 87,                 /* stmt_list  */
  YYSYMBOL_stmt = 88,                      /* stmt  */
  YYSYMBOL_expr0 = 89,                     /* expr0  */
  YYSYMBOL_expr1 = 90,                     /* expr1  */
  YYSYMBOL_expr3 = 91,                     /* expr3  */
  YYSYMBOL_expr4 = 92,                     /* expr4  */
  YYSYMBOL_expr5 = 93,                     /* expr5  */
  YYSYMBOL_expr6 = 94,                     /* expr6  */
  YYSYMBOL_expr9 = 95,                     /* expr9  */
  YYSYMBOL_expr11 = 96,                    /* expr11  */
  YYSYMBOL_exprlist = 97,                  /* exprlist  */
  YYSYMBOL_exprlist_ne = 98,               /* exprlist_ne  */
  YYSYMBOL_expr17 = 99                     /* expr17  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 291 "snocone_parse.y"

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

#line 443 "snocone_parse.tab.c"

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
typedef yytype_int8 yy_state_t;

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
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   86

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  93

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   339


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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
};

#if SC_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   400,   400,   401,   404,   405,   408,   409,   451,   453,
     457,   461,   465,   469,   473,   486,   488,   502,   507,   522,
     527,   541,   544,   547,   550,   553,   556,   559,   562,   565,
     568,   571,   574,   577,   580,   583,   587,   589,   591,   595,
     597,   599,   604,   606,   617,   620,   623,   625,   638,   645,
     649,   653,   655,   657,   659,   661,   663
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
  "T_INT", "T_REAL", "T_STR", "T_FUNCTION", "T_2PLUS", "T_2MINUS",
  "T_2STAR", "T_2SLASH", "T_2CARET", "T_EQ", "T_NE", "T_LT", "T_GT",
  "T_LE", "T_GE", "T_LEQ", "T_LNE", "T_LLT", "T_LGT", "T_LLE", "T_LGE",
  "T_IDENT_OP", "T_DIFFER", "T_1PLUS", "T_1MINUS", "T_2EQUAL",
  "T_PLUS_ASSIGN", "T_MINUS_ASSIGN", "T_STAR_ASSIGN", "T_SLASH_ASSIGN",
  "T_CARET_ASSIGN", "T_2QUEST", "T_2PIPE", "T_CONCAT", "T_LPAREN",
  "T_RPAREN", "T_SEMICOLON", "T_COMMA", "T_2DOLLAR", "T_2DOT", "T_2AMP",
  "T_2AT", "T_2POUND", "T_2PERCENT", "T_2TILDE", "T_1STAR", "T_1SLASH",
  "T_1PERCENT", "T_1AT", "T_1TILDE", "T_1DOLLAR", "T_1DOT", "T_1POUND",
  "T_1PIPE", "T_1EQUAL", "T_1QUEST", "T_1AMP", "T_LBRACK", "T_RBRACK",
  "T_LBRACE", "T_RBRACE", "T_COLON", "T_KW_IF", "T_KW_ELSE", "T_KW_WHILE",
  "T_KW_DO", "T_KW_UNTIL", "T_KW_FOR", "T_KW_SWITCH", "T_KW_CASE",
  "T_KW_DEFAULT", "T_KW_BREAK", "T_KW_CONTINUE", "T_KW_GOTO",
  "T_KW_FUNCTION", "T_KW_RETURN", "T_KW_FRETURN", "T_KW_NRETURN",
  "T_KW_STRUCT", "T_UNKNOWN", "$accept", "program", "stmt_list", "stmt",
  "expr0", "expr1", "expr3", "expr4", "expr5", "expr6", "expr9", "expr11",
  "exprlist", "exprlist_ne", "expr17", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-39)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -1,   -39,   -39,   -39,   -39,   -39,   -38,     5,     5,     5,
     -39,    35,    -1,   -39,    -2,   -15,     0,    -6,    31,    32,
      62,   -39,    30,     5,   -39,   -39,    39,   -39,   -39,   -39,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,   -39,    40,
      41,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,    -6,
      31,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      32,    32,    32,    32,    32,    62,    62,   -39,   -39,   -39,
     -39,     5,   -39
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    49,    50,    51,    52,    53,     0,     0,     0,     0,
       7,     0,     2,     5,     0,    14,    16,    18,    20,    35,
      38,    41,    43,    45,    55,    56,     0,     1,     4,     6,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
      44,    54,     8,     9,    10,    11,    12,    13,    15,    17,
      19,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    36,    37,    39,    40,    42,
      48,     0,    46
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -39,   -39,   -39,    69,    -9,    48,   -39,    49,    47,    20,
      22,   -26,   -39,   -39,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    59,    60,    22
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      26,    23,     1,     2,     3,     4,     5,     6,     1,     2,
       3,     4,     5,     6,    58,    30,    31,    32,    33,    34,
      35,    62,    63,    64,    65,    66,    67,     7,     8,    87,
      88,    89,    38,     7,     8,    27,    36,    37,     9,    29,
      10,    53,    54,    57,     9,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    55,    56,    85,    86,    24,    25,    61,
      90,    28,    92,    91,    68,    70,    69
};

static const yytype_int8 yycheck[] =
{
       9,    39,     3,     4,     5,     6,     7,     8,     3,     4,
       5,     6,     7,     8,    23,    30,    31,    32,    33,    34,
      35,    30,    31,    32,    33,    34,    35,    28,    29,    55,
      56,    57,    38,    28,    29,     0,    36,    37,    39,    41,
      41,     9,    10,    13,    39,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    11,    12,    53,    54,     7,     8,    40,
      40,    12,    91,    42,    36,    38,    37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    28,    29,    39,
      41,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    99,    39,    99,    99,    89,     0,    88,    41,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     9,    10,    11,    12,    13,    89,    97,
      98,    40,    89,    89,    89,    89,    89,    89,    90,    92,
      93,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    95,    95,    96,    96,    96,
      40,    42,    89
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    85,    86,    86,    87,    87,    88,    88,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    94,    94,    94,    95,
      95,    95,    96,    96,    97,    97,    98,    98,    99,    99,
      99,    99,    99,    99,    99,    99,    99
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     1,     3,     3,
       3,     3,     3,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     1,     1,     0,     3,     1,     4,     1,
       1,     1,     1,     1,     3,     2,     2
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
  case 6: /* stmt: expr0 T_SEMICOLON  */
#line 408 "snocone_parse.y"
                                               { sc_append_stmt(st, (yyvsp[-1].expr)); }
#line 1475 "snocone_parse.tab.c"
    break;

  case 7: /* stmt: T_SEMICOLON  */
#line 409 "snocone_parse.y"
                                               { /* empty stmt */         }
#line 1481 "snocone_parse.tab.c"
    break;

  case 8: /* expr0: expr1 T_2EQUAL expr0  */
#line 452 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1487 "snocone_parse.tab.c"
    break;

  case 9: /* expr0: expr1 T_PLUS_ASSIGN expr0  */
#line 454 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_ADD, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1495 "snocone_parse.tab.c"
    break;

  case 10: /* expr0: expr1 T_MINUS_ASSIGN expr0  */
#line 458 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_SUB, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1503 "snocone_parse.tab.c"
    break;

  case 11: /* expr0: expr1 T_STAR_ASSIGN expr0  */
#line 462 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_MUL, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1511 "snocone_parse.tab.c"
    break;

  case 12: /* expr0: expr1 T_SLASH_ASSIGN expr0  */
#line 466 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_DIV, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1519 "snocone_parse.tab.c"
    break;

  case 13: /* expr0: expr1 T_CARET_ASSIGN expr0  */
#line 470 "snocone_parse.y"
                                { EXPR_t *cl = sc_clone_expr_simple((yyvsp[-2].expr));
                                  EXPR_t *rhs = expr_binary(E_POW, cl, (yyvsp[0].expr));
                                  (yyval.expr) = expr_binary(E_ASSIGN, (yyvsp[-2].expr), rhs); }
#line 1527 "snocone_parse.tab.c"
    break;

  case 14: /* expr0: expr1  */
#line 474 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1533 "snocone_parse.tab.c"
    break;

  case 15: /* expr1: expr3 T_2QUEST expr1  */
#line 487 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_SCAN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1539 "snocone_parse.tab.c"
    break;

  case 16: /* expr1: expr3  */
#line 489 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1545 "snocone_parse.tab.c"
    break;

  case 17: /* expr3: expr3 T_2PIPE expr4  */
#line 503 "snocone_parse.y"
                                { if ((yyvsp[-2].expr)->kind == E_ALT) { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
                                  else { EXPR_t *a = expr_new(E_ALT);
                                         expr_add_child(a, (yyvsp[-2].expr)); expr_add_child(a, (yyvsp[0].expr));
                                         (yyval.expr) = a; } }
#line 1554 "snocone_parse.tab.c"
    break;

  case 18: /* expr3: expr4  */
#line 508 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1560 "snocone_parse.tab.c"
    break;

  case 19: /* expr4: expr4 T_CONCAT expr5  */
#line 523 "snocone_parse.y"
                                { if ((yyvsp[-2].expr)->kind == E_SEQ) { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
                                  else { EXPR_t *s = expr_new(E_SEQ);
                                         expr_add_child(s, (yyvsp[-2].expr)); expr_add_child(s, (yyvsp[0].expr));
                                         (yyval.expr) = s; } }
#line 1569 "snocone_parse.tab.c"
    break;

  case 20: /* expr4: expr5  */
#line 528 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1575 "snocone_parse.tab.c"
    break;

  case 21: /* expr5: expr5 T_EQ expr6  */
#line 542 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("EQ");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1582 "snocone_parse.tab.c"
    break;

  case 22: /* expr5: expr5 T_NE expr6  */
#line 545 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("NE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1589 "snocone_parse.tab.c"
    break;

  case 23: /* expr5: expr5 T_LT expr6  */
#line 548 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1596 "snocone_parse.tab.c"
    break;

  case 24: /* expr5: expr5 T_GT expr6  */
#line 551 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("GT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1603 "snocone_parse.tab.c"
    break;

  case 25: /* expr5: expr5 T_LE expr6  */
#line 554 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1610 "snocone_parse.tab.c"
    break;

  case 26: /* expr5: expr5 T_GE expr6  */
#line 557 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("GE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1617 "snocone_parse.tab.c"
    break;

  case 27: /* expr5: expr5 T_LEQ expr6  */
#line 560 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LEQ");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1624 "snocone_parse.tab.c"
    break;

  case 28: /* expr5: expr5 T_LNE expr6  */
#line 563 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LNE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1631 "snocone_parse.tab.c"
    break;

  case 29: /* expr5: expr5 T_LLT expr6  */
#line 566 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LLT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1638 "snocone_parse.tab.c"
    break;

  case 30: /* expr5: expr5 T_LGT expr6  */
#line 569 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LGT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1645 "snocone_parse.tab.c"
    break;

  case 31: /* expr5: expr5 T_LLE expr6  */
#line 572 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LLE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1652 "snocone_parse.tab.c"
    break;

  case 32: /* expr5: expr5 T_LGE expr6  */
#line 575 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("LGE");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1659 "snocone_parse.tab.c"
    break;

  case 33: /* expr5: expr5 T_IDENT_OP expr6  */
#line 578 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("IDENT");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1666 "snocone_parse.tab.c"
    break;

  case 34: /* expr5: expr5 T_DIFFER expr6  */
#line 581 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC); e->sval = strdup("DIFFER");
                                  expr_add_child(e, (yyvsp[-2].expr)); expr_add_child(e, (yyvsp[0].expr)); (yyval.expr) = e; }
#line 1673 "snocone_parse.tab.c"
    break;

  case 35: /* expr5: expr6  */
#line 584 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1679 "snocone_parse.tab.c"
    break;

  case 36: /* expr6: expr6 T_2PLUS expr9  */
#line 588 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1685 "snocone_parse.tab.c"
    break;

  case 37: /* expr6: expr6 T_2MINUS expr9  */
#line 590 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1691 "snocone_parse.tab.c"
    break;

  case 38: /* expr6: expr9  */
#line 592 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1697 "snocone_parse.tab.c"
    break;

  case 39: /* expr9: expr9 T_2STAR expr11  */
#line 596 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1703 "snocone_parse.tab.c"
    break;

  case 40: /* expr9: expr9 T_2SLASH expr11  */
#line 598 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1709 "snocone_parse.tab.c"
    break;

  case 41: /* expr9: expr11  */
#line 600 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1715 "snocone_parse.tab.c"
    break;

  case 42: /* expr11: expr17 T_2CARET expr11  */
#line 605 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(E_POW, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1721 "snocone_parse.tab.c"
    break;

  case 43: /* expr11: expr17  */
#line 607 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1727 "snocone_parse.tab.c"
    break;

  case 44: /* exprlist: exprlist_ne  */
#line 618 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1733 "snocone_parse.tab.c"
    break;

  case 45: /* exprlist: %empty  */
#line 620 "snocone_parse.y"
                                { (yyval.expr) = expr_new(E_NUL); }
#line 1739 "snocone_parse.tab.c"
    break;

  case 46: /* exprlist_ne: exprlist_ne T_COMMA expr0  */
#line 624 "snocone_parse.y"
                                { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
#line 1745 "snocone_parse.tab.c"
    break;

  case 47: /* exprlist_ne: expr0  */
#line 626 "snocone_parse.y"
                                { EXPR_t *l = expr_new(E_NUL); expr_add_child(l, (yyvsp[0].expr)); (yyval.expr) = l; }
#line 1751 "snocone_parse.tab.c"
    break;

  case 48: /* expr17: T_FUNCTION T_LPAREN exprlist T_RPAREN  */
#line 639 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_FNC);
                                  e->sval = (yyvsp[-3].str);             /* takes ownership */
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(e, (yyvsp[-1].expr)->children[i]);
                                  free((yyvsp[-1].expr)->children); free((yyvsp[-1].expr));
                                  (yyval.expr) = e; }
#line 1762 "snocone_parse.tab.c"
    break;

  case 49: /* expr17: T_IDENT  */
#line 646 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_VAR);
                                  e->sval = (yyvsp[0].str);             /* takes ownership */
                                  (yyval.expr) = e; }
#line 1770 "snocone_parse.tab.c"
    break;

  case 50: /* expr17: T_KEYWORD  */
#line 650 "snocone_parse.y"
                                { EXPR_t *e = expr_new(E_KEYWORD);
                                  e->sval = (yyvsp[0].str);
                                  (yyval.expr) = e; }
#line 1778 "snocone_parse.tab.c"
    break;

  case 51: /* expr17: T_INT  */
#line 654 "snocone_parse.y"
                                { (yyval.expr) = sc_int_literal((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1784 "snocone_parse.tab.c"
    break;

  case 52: /* expr17: T_REAL  */
#line 656 "snocone_parse.y"
                                { (yyval.expr) = sc_real_literal((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1790 "snocone_parse.tab.c"
    break;

  case 53: /* expr17: T_STR  */
#line 658 "snocone_parse.y"
                                { (yyval.expr) = sc_str_literal((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1796 "snocone_parse.tab.c"
    break;

  case 54: /* expr17: T_LPAREN expr0 T_RPAREN  */
#line 660 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 1802 "snocone_parse.tab.c"
    break;

  case 55: /* expr17: T_1PLUS expr17  */
#line 662 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_PLS, (yyvsp[0].expr)); }
#line 1808 "snocone_parse.tab.c"
    break;

  case 56: /* expr17: T_1MINUS expr17  */
#line 664 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(E_MNS, (yyvsp[0].expr)); }
#line 1814 "snocone_parse.tab.c"
    break;


#line 1818 "snocone_parse.tab.c"

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

#line 667 "snocone_parse.y"


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
