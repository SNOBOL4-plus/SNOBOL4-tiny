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

#ifndef YY_RAKU_YY_RAKU_TAB_H_INCLUDED
# define YY_RAKU_YY_RAKU_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef RAKU_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define RAKU_YYDEBUG 1
#  else
#   define RAKU_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define RAKU_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined RAKU_YYDEBUG */
#if RAKU_YYDEBUG
extern int raku_yydebug;
#endif
/* "%code requires" blocks.  */
#line 3 "raku.y"

/*
 * raku.y — Tiny-Raku Bison grammar
 *
 * FI-3: builds EXPR_t/STMT_t directly — no intermediate RakuNode AST.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
 */
#include "../../ir/ir.h"
#include "../snobol4/scrip_cc.h"

typedef struct ExprList {
    EXPR_t **items;
    int      count;
    int      cap;
} ExprList;

#line 75 "raku.tab.h"

/* Token kinds.  */
#ifndef RAKU_YYTOKENTYPE
# define RAKU_YYTOKENTYPE
  enum raku_yytokentype
  {
    RAKU_YYEMPTY = -2,
    RAKU_YYEOF = 0,                /* "end of file"  */
    RAKU_YYerror = 256,            /* error  */
    RAKU_YYUNDEF = 257,            /* "invalid token"  */
    LIT_INT = 258,                 /* LIT_INT  */
    LIT_FLOAT = 259,               /* LIT_FLOAT  */
    LIT_STR = 260,                 /* LIT_STR  */
    LIT_INTERP_STR = 261,          /* LIT_INTERP_STR  */
    LIT_REGEX = 262,               /* LIT_REGEX  */
    VAR_SCALAR = 263,              /* VAR_SCALAR  */
    VAR_ARRAY = 264,               /* VAR_ARRAY  */
    VAR_HASH = 265,                /* VAR_HASH  */
    VAR_TWIGIL = 266,              /* VAR_TWIGIL  */
    IDENT = 267,                   /* IDENT  */
    VAR_CAPTURE = 268,             /* VAR_CAPTURE  */
    VAR_NAMED_CAPTURE = 269,       /* VAR_NAMED_CAPTURE  */
    KW_MY = 270,                   /* KW_MY  */
    KW_SAY = 271,                  /* KW_SAY  */
    KW_PRINT = 272,                /* KW_PRINT  */
    KW_IF = 273,                   /* KW_IF  */
    KW_ELSE = 274,                 /* KW_ELSE  */
    KW_ELSIF = 275,                /* KW_ELSIF  */
    KW_WHILE = 276,                /* KW_WHILE  */
    KW_FOR = 277,                  /* KW_FOR  */
    KW_SUB = 278,                  /* KW_SUB  */
    KW_GATHER = 279,               /* KW_GATHER  */
    KW_TAKE = 280,                 /* KW_TAKE  */
    KW_RETURN = 281,               /* KW_RETURN  */
    KW_GIVEN = 282,                /* KW_GIVEN  */
    KW_WHEN = 283,                 /* KW_WHEN  */
    KW_DEFAULT = 284,              /* KW_DEFAULT  */
    KW_EXISTS = 285,               /* KW_EXISTS  */
    KW_DELETE = 286,               /* KW_DELETE  */
    KW_UNLESS = 287,               /* KW_UNLESS  */
    KW_UNTIL = 288,                /* KW_UNTIL  */
    KW_REPEAT = 289,               /* KW_REPEAT  */
    KW_MAP = 290,                  /* KW_MAP  */
    KW_GREP = 291,                 /* KW_GREP  */
    KW_SORT = 292,                 /* KW_SORT  */
    KW_TRY = 293,                  /* KW_TRY  */
    KW_CATCH = 294,                /* KW_CATCH  */
    KW_DIE = 295,                  /* KW_DIE  */
    KW_CLASS = 296,                /* KW_CLASS  */
    KW_METHOD = 297,               /* KW_METHOD  */
    KW_HAS = 298,                  /* KW_HAS  */
    KW_NEW = 299,                  /* KW_NEW  */
    OP_FATARROW = 300,             /* OP_FATARROW  */
    OP_RANGE = 301,                /* OP_RANGE  */
    OP_RANGE_EX = 302,             /* OP_RANGE_EX  */
    OP_ARROW = 303,                /* OP_ARROW  */
    OP_EQ = 304,                   /* OP_EQ  */
    OP_NE = 305,                   /* OP_NE  */
    OP_LE = 306,                   /* OP_LE  */
    OP_GE = 307,                   /* OP_GE  */
    OP_SEQ = 308,                  /* OP_SEQ  */
    OP_SNE = 309,                  /* OP_SNE  */
    OP_AND = 310,                  /* OP_AND  */
    OP_OR = 311,                   /* OP_OR  */
    OP_BIND = 312,                 /* OP_BIND  */
    OP_SMATCH = 313,               /* OP_SMATCH  */
    OP_DIV = 314,                  /* OP_DIV  */
    UMINUS = 315                   /* UMINUS  */
  };
  typedef enum raku_yytokentype raku_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined RAKU_YYSTYPE && ! defined RAKU_YYSTYPE_IS_DECLARED
union RAKU_YYSTYPE
{
#line 165 "raku.y"

    long      ival;
    double    dval;
    char     *sval;
    EXPR_t   *node;
    ExprList *list;

#line 160 "raku.tab.h"

};
typedef union RAKU_YYSTYPE RAKU_YYSTYPE;
# define RAKU_YYSTYPE_IS_TRIVIAL 1
# define RAKU_YYSTYPE_IS_DECLARED 1
#endif


extern RAKU_YYSTYPE raku_yylval;


int raku_yyparse (void);


#endif /* !YY_RAKU_YY_RAKU_TAB_H_INCLUDED  */
