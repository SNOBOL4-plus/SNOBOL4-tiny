/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_SC_SNOCONE_TAB_H_INCLUDED
# define YY_SC_SNOCONE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef SC_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SC_DEBUG 1
#  else
#   define SC_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SC_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SC_DEBUG */
#if SC_DEBUG
extern int sc_debug;
#endif
/* "%code requires" blocks.  */
#line 243 "snocone.y"

#include "scrip_cc.h"

/* Forward-declare LexCtx — full definition lives in snocone_fsm.h, which
 * we deliberately do NOT include here so its T_* enumerators don't
 * collide with Bison's enum sc_tokentype.  See the %code top block above. */
struct LexCtx;

/* Parser state — passed to sc_parse() via %parse-param.  Carries the
 * FSM lexer context (the single producer of tokens), the program under
 * construction, and a small error counter. */
typedef struct ScParseState {
    struct LexCtx *ctx;
    Program       *prog;
    const char    *filename;
    int            nerrors;
} ScParseState;

#line 76 "snocone.tab.h"

/* Token kinds.  */
#ifndef SC_TOKENTYPE
# define SC_TOKENTYPE
  enum sc_tokentype
  {
    SC_EMPTY = -2,
    SC_EOF = 0,                    /* "end of file"  */
    SC_error = 256,                /* error  */
    SC_UNDEF = 257,                /* "invalid token"  */
    T_IDENT = 258,                 /* T_IDENT  */
    T_KEYWORD = 259,               /* T_KEYWORD  */
    T_INT = 260,                   /* T_INT  */
    T_REAL = 261,                  /* T_REAL  */
    T_STR = 262,                   /* T_STR  */
    T_FUNCTION = 263,              /* T_FUNCTION  */
    T_ADDITION = 264,              /* T_ADDITION  */
    T_SUBTRACTION = 265,           /* T_SUBTRACTION  */
    T_MULTIPLICATION = 266,        /* T_MULTIPLICATION  */
    T_DIVISION = 267,              /* T_DIVISION  */
    T_EXPONENTIATION = 268,        /* T_EXPONENTIATION  */
    T_EQ = 269,                    /* T_EQ  */
    T_NE = 270,                    /* T_NE  */
    T_LT = 271,                    /* T_LT  */
    T_GT = 272,                    /* T_GT  */
    T_LE = 273,                    /* T_LE  */
    T_GE = 274,                    /* T_GE  */
    T_LEQ = 275,                   /* T_LEQ  */
    T_LNE = 276,                   /* T_LNE  */
    T_LLT = 277,                   /* T_LLT  */
    T_LGT = 278,                   /* T_LGT  */
    T_LLE = 279,                   /* T_LLE  */
    T_LGE = 280,                   /* T_LGE  */
    T_IDENT_OP = 281,              /* T_IDENT_OP  */
    T_DIFFER = 282,                /* T_DIFFER  */
    T_UN_PLUS = 283,               /* T_UN_PLUS  */
    T_UN_MINUS = 284,              /* T_UN_MINUS  */
    T_ASSIGNMENT = 285,            /* T_ASSIGNMENT  */
    T_PLUS_ASSIGN = 286,           /* T_PLUS_ASSIGN  */
    T_MINUS_ASSIGN = 287,          /* T_MINUS_ASSIGN  */
    T_STAR_ASSIGN = 288,           /* T_STAR_ASSIGN  */
    T_SLASH_ASSIGN = 289,          /* T_SLASH_ASSIGN  */
    T_CARET_ASSIGN = 290,          /* T_CARET_ASSIGN  */
    T_MATCH = 291,                 /* T_MATCH  */
    T_ALTERNATION = 292,           /* T_ALTERNATION  */
    T_CONCAT = 293,                /* T_CONCAT  */
    T_LPAREN = 294,                /* T_LPAREN  */
    T_RPAREN = 295,                /* T_RPAREN  */
    T_SEMICOLON = 296,             /* T_SEMICOLON  */
    T_COMMA = 297,                 /* T_COMMA  */
    T_IMMEDIATE_ASSIGN = 298,      /* T_IMMEDIATE_ASSIGN  */
    T_COND_ASSIGN = 299,           /* T_COND_ASSIGN  */
    T_AMPERSAND = 300,             /* T_AMPERSAND  */
    T_AT_SIGN = 301,               /* T_AT_SIGN  */
    T_POUND = 302,                 /* T_POUND  */
    T_PERCENT = 303,               /* T_PERCENT  */
    T_TILDE = 304,                 /* T_TILDE  */
    T_UN_ASTERISK = 305,           /* T_UN_ASTERISK  */
    T_UN_SLASH = 306,              /* T_UN_SLASH  */
    T_UN_PERCENT = 307,            /* T_UN_PERCENT  */
    T_UN_AT_SIGN = 308,            /* T_UN_AT_SIGN  */
    T_UN_TILDE = 309,              /* T_UN_TILDE  */
    T_UN_DOLLAR_SIGN = 310,        /* T_UN_DOLLAR_SIGN  */
    T_UN_PERIOD = 311,             /* T_UN_PERIOD  */
    T_UN_POUND = 312,              /* T_UN_POUND  */
    T_UN_VERTICAL_BAR = 313,       /* T_UN_VERTICAL_BAR  */
    T_UN_EQUAL = 314,              /* T_UN_EQUAL  */
    T_UN_QUESTION_MARK = 315,      /* T_UN_QUESTION_MARK  */
    T_UN_AMPERSAND = 316,          /* T_UN_AMPERSAND  */
    T_LBRACK = 317,                /* T_LBRACK  */
    T_RBRACK = 318,                /* T_RBRACK  */
    T_LBRACE = 319,                /* T_LBRACE  */
    T_RBRACE = 320,                /* T_RBRACE  */
    T_COLON = 321,                 /* T_COLON  */
    T_KW_IF = 322,                 /* T_KW_IF  */
    T_KW_ELSE = 323,               /* T_KW_ELSE  */
    T_KW_WHILE = 324,              /* T_KW_WHILE  */
    T_KW_DO = 325,                 /* T_KW_DO  */
    T_KW_UNTIL = 326,              /* T_KW_UNTIL  */
    T_KW_FOR = 327,                /* T_KW_FOR  */
    T_KW_SWITCH = 328,             /* T_KW_SWITCH  */
    T_KW_CASE = 329,               /* T_KW_CASE  */
    T_KW_DEFAULT = 330,            /* T_KW_DEFAULT  */
    T_KW_BREAK = 331,              /* T_KW_BREAK  */
    T_KW_CONTINUE = 332,           /* T_KW_CONTINUE  */
    T_KW_GOTO = 333,               /* T_KW_GOTO  */
    T_KW_FUNCTION = 334,           /* T_KW_FUNCTION  */
    T_KW_RETURN = 335,             /* T_KW_RETURN  */
    T_KW_FRETURN = 336,            /* T_KW_FRETURN  */
    T_KW_NRETURN = 337,            /* T_KW_NRETURN  */
    T_KW_STRUCT = 338,             /* T_KW_STRUCT  */
    T_UNKNOWN = 339                /* T_UNKNOWN  */
  };
  typedef enum sc_tokentype sc_token_kind_t;
#endif

/* Value type.  */
#if ! defined SC_STYPE && ! defined SC_STYPE_IS_DECLARED
union SC_STYPE
{
#line 303 "snocone.y"

    EXPR_t *expr;
    char   *str;
    long    ival;
    double  dval;

#line 184 "snocone.tab.h"

};
typedef union SC_STYPE SC_STYPE;
# define SC_STYPE_IS_TRIVIAL 1
# define SC_STYPE_IS_DECLARED 1
#endif




int sc_parse (ScParseState *st);


#endif /* !YY_SC_SNOCONE_TAB_H_INCLUDED  */
