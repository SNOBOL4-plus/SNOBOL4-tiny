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
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         RAKU_YYSTYPE
/* Substitute the variable and function names.  */
#define yyparse         raku_yyparse
#define yylex           raku_yylex
#define yyerror         raku_yyerror
#define yydebug         raku_yydebug
#define yynerrs         raku_yynerrs
#define yylval          raku_yylval
#define yychar          raku_yychar

/* First part of user prologue.  */
#line 21 "raku.y"

#include "../../ir/ir.h"
#include "../snobol4/scrip_cc.h"
#include "raku.tab.h"   /* pulls in ExprList from %code requires */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int  raku_yylex(void);
extern int  raku_get_lineno(void);
void raku_yyerror(const char *msg) {
    fprintf(stderr, "raku parse error line %d: %s\n", raku_get_lineno(), msg);
}

/*--------------------------------------------------------------------
 * ExprList helpers
 *--------------------------------------------------------------------*/
static ExprList *exprlist_new(void) {
    ExprList *l = calloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
static ExprList *exprlist_append(ExprList *l, EXPR_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = realloc(l->items, l->cap * sizeof(EXPR_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
static void exprlist_free(ExprList *l) { if (l) { free(l->items); free(l); } }

/*--------------------------------------------------------------------
 * Build helpers (logic from raku_lower.c, inlined for direct IR)
 *--------------------------------------------------------------------*/
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
static EXPR_t *leaf_sval(EKind k, const char *s) {
    EXPR_t *e = expr_new(k); e->sval = intern(s); return e;
}
static EXPR_t *var_node(const char *name) {
    return leaf_sval(E_VAR, strip_sigil(name));
}
/* make_call: E_FNC + children[0]=E_VAR(name) for icn_interp_eval layout */
static EXPR_t *make_call(const char *name) {
    EXPR_t *e = leaf_sval(E_FNC, name);
    EXPR_t *n = expr_new(E_VAR); n->sval = intern(name);
    expr_add_child(e, n);
    return e;
}
/* make_seq: ExprList → E_SEQ_EXPR, frees list */
static EXPR_t *make_seq(ExprList *stmts) {
    EXPR_t *seq = expr_new(E_SEQ_EXPR);
    if (stmts) {
        for (int i = 0; i < stmts->count; i++) expr_add_child(seq, stmts->items[i]);
        exprlist_free(stmts);
    }
    return seq;
}
/* lower_interp_str: "hello $var" → left-associative E_CAT chain */
static EXPR_t *lower_interp_str(const char *s) {
    int len = s ? (int)strlen(s) : 0;
    EXPR_t *result = NULL;
    char litbuf[4096]; int litpos = 0, i = 0;
    while (i < len) {
        if (s[i]=='$' && i+1<len &&
            (s[i+1]=='_'||(s[i+1]>='A'&&s[i+1]<='Z')||(s[i+1]>='a'&&s[i+1]<='z'))) {
            if (litpos>0) { litbuf[litpos]='\0';
                EXPR_t *lit=leaf_sval(E_QLIT,litbuf);
                result=result?expr_binary(E_CAT,result,lit):lit; litpos=0; }
            i++;
            char vname[256]; int vlen=0;
            while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
                { if(vlen<255) vname[vlen++]=s[i]; i++; }
            vname[vlen]='\0';
            EXPR_t *var=leaf_sval(E_VAR,vname);
            result=result?expr_binary(E_CAT,result,var):var;
        } else { if(litpos<4095) litbuf[litpos++]=s[i]; i++; }
    }
    if (litpos>0) { litbuf[litpos]='\0';
        EXPR_t *lit=leaf_sval(E_QLIT,litbuf);
        result=result?expr_binary(E_CAT,result,lit):lit; }
    return result ? result : leaf_sval(E_QLIT,"");
}
/* make_for_range: for lo..hi -> $v body → explicit while-loop */
static EXPR_t *make_for_range(EXPR_t *lo, EXPR_t *hi, const char *vname, EXPR_t *body_seq) {
    EXPR_t *init = expr_binary(E_ASSIGN, leaf_sval(E_VAR,vname), lo);
    EXPR_t *cond = expr_binary(E_LE, leaf_sval(E_VAR,vname), hi);
    EXPR_t *one  = expr_new(E_ILIT); one->ival = 1;
    EXPR_t *incr = expr_binary(E_ADD, leaf_sval(E_VAR,vname), one);
    expr_add_child(body_seq, expr_binary(E_ASSIGN, leaf_sval(E_VAR,vname), incr));
    EXPR_t *wloop = expr_binary(E_WHILE, cond, body_seq);
    EXPR_t *seq   = expr_new(E_SEQ_EXPR);
    expr_add_child(seq, init); expr_add_child(seq, wloop);
    return seq;
}

/*--------------------------------------------------------------------
 * Program output
 *--------------------------------------------------------------------*/
Program *raku_prog_result = NULL;

static void add_proc(EXPR_t *e) {
    if (!e) return;
    if (!raku_prog_result) raku_prog_result = calloc(1, sizeof(Program));
    STMT_t *st = calloc(1, sizeof(STMT_t));
    st->subject = e; st->lineno = 0; st->lang = LANG_RAKU;
    if (!raku_prog_result->head) raku_prog_result->head = raku_prog_result->tail = st;
    else { raku_prog_result->tail->next = st; raku_prog_result->tail = st; }
    raku_prog_result->nstmts++;
}

/* SUB_TAG: sentinel bit to distinguish sub defs from body stmts in stmt_list */
#define SUB_TAG 0x40000000

/* RK-26: Raku method table — maps "ClassName::method" → E_FNC proc name */
#define RAKU_METH_MAX 256
typedef struct { char key[128]; char procname[128]; } RakuMethEntry;
static RakuMethEntry raku_meth_table[RAKU_METH_MAX];
static int           raku_meth_ntypes = 0;

static void raku_meth_register(const char *classname, const char *methname, const char *procname) {
    if (raku_meth_ntypes >= RAKU_METH_MAX) return;
    RakuMethEntry *e = &raku_meth_table[raku_meth_ntypes++];
    snprintf(e->key,      sizeof e->key,      "%s::%s", classname, methname);
    snprintf(e->procname, sizeof e->procname,  "%s",     procname);
}

/* Emit the extern declaration so interp.c can call raku_meth_lookup */
const char *raku_meth_lookup(const char *classname, const char *methname) {
    char key[128];
    snprintf(key, sizeof key, "%s::%s", classname, methname);
    for (int i = 0; i < raku_meth_ntypes; i++)
        if (strcmp(raku_meth_table[i].key, key) == 0)
            return raku_meth_table[i].procname;
    return NULL;
}



#line 223 "raku.tab.c"

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

#include "raku.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LIT_INT = 3,                    /* LIT_INT  */
  YYSYMBOL_LIT_FLOAT = 4,                  /* LIT_FLOAT  */
  YYSYMBOL_LIT_STR = 5,                    /* LIT_STR  */
  YYSYMBOL_LIT_INTERP_STR = 6,             /* LIT_INTERP_STR  */
  YYSYMBOL_LIT_REGEX = 7,                  /* LIT_REGEX  */
  YYSYMBOL_VAR_SCALAR = 8,                 /* VAR_SCALAR  */
  YYSYMBOL_VAR_ARRAY = 9,                  /* VAR_ARRAY  */
  YYSYMBOL_VAR_HASH = 10,                  /* VAR_HASH  */
  YYSYMBOL_VAR_TWIGIL = 11,                /* VAR_TWIGIL  */
  YYSYMBOL_IDENT = 12,                     /* IDENT  */
  YYSYMBOL_KW_MY = 13,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 14,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 15,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 16,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 17,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 18,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 19,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 20,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 21,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 22,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 23,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 24,                 /* KW_RETURN  */
  YYSYMBOL_KW_GIVEN = 25,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 26,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 27,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 28,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 29,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 30,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 31,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 32,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 33,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 34,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 35,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 36,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 37,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 38,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 39,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 40,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 41,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 42,                    /* KW_NEW  */
  YYSYMBOL_OP_FATARROW = 43,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 44,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 45,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 46,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 47,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 48,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 49,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 50,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 51,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 52,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 53,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 54,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 55,                   /* OP_BIND  */
  YYSYMBOL_OP_SMATCH = 56,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 57,                    /* OP_DIV  */
  YYSYMBOL_58_ = 58,                       /* '='  */
  YYSYMBOL_59_ = 59,                       /* '!'  */
  YYSYMBOL_60_ = 60,                       /* '<'  */
  YYSYMBOL_61_ = 61,                       /* '>'  */
  YYSYMBOL_62_ = 62,                       /* '~'  */
  YYSYMBOL_63_ = 63,                       /* '+'  */
  YYSYMBOL_64_ = 64,                       /* '-'  */
  YYSYMBOL_65_ = 65,                       /* '*'  */
  YYSYMBOL_66_ = 66,                       /* '/'  */
  YYSYMBOL_67_ = 67,                       /* '%'  */
  YYSYMBOL_UMINUS = 68,                    /* UMINUS  */
  YYSYMBOL_69_ = 69,                       /* '.'  */
  YYSYMBOL_70_ = 70,                       /* ';'  */
  YYSYMBOL_71_ = 71,                       /* '['  */
  YYSYMBOL_72_ = 72,                       /* ']'  */
  YYSYMBOL_73_ = 73,                       /* '{'  */
  YYSYMBOL_74_ = 74,                       /* '}'  */
  YYSYMBOL_75_ = 75,                       /* '('  */
  YYSYMBOL_76_ = 76,                       /* ')'  */
  YYSYMBOL_77_ = 77,                       /* ','  */
  YYSYMBOL_YYACCEPT = 78,                  /* $accept  */
  YYSYMBOL_program = 79,                   /* program  */
  YYSYMBOL_stmt_list = 80,                 /* stmt_list  */
  YYSYMBOL_stmt = 81,                      /* stmt  */
  YYSYMBOL_if_stmt = 82,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 83,                /* while_stmt  */
  YYSYMBOL_unless_stmt = 84,               /* unless_stmt  */
  YYSYMBOL_until_stmt = 85,                /* until_stmt  */
  YYSYMBOL_repeat_stmt = 86,               /* repeat_stmt  */
  YYSYMBOL_for_stmt = 87,                  /* for_stmt  */
  YYSYMBOL_given_stmt = 88,                /* given_stmt  */
  YYSYMBOL_when_list = 89,                 /* when_list  */
  YYSYMBOL_sub_decl = 90,                  /* sub_decl  */
  YYSYMBOL_class_decl = 91,                /* class_decl  */
  YYSYMBOL_class_body_list = 92,           /* class_body_list  */
  YYSYMBOL_named_arg_list = 93,            /* named_arg_list  */
  YYSYMBOL_param_list = 94,                /* param_list  */
  YYSYMBOL_block = 95,                     /* block  */
  YYSYMBOL_closure = 96,                   /* closure  */
  YYSYMBOL_expr = 97,                      /* expr  */
  YYSYMBOL_cmp_expr = 98,                  /* cmp_expr  */
  YYSYMBOL_range_expr = 99,                /* range_expr  */
  YYSYMBOL_add_expr = 100,                 /* add_expr  */
  YYSYMBOL_mul_expr = 101,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 102,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 103,             /* postfix_expr  */
  YYSYMBOL_call_expr = 104,                /* call_expr  */
  YYSYMBOL_arg_list = 105,                 /* arg_list  */
  YYSYMBOL_atom = 106                      /* atom  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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
typedef yytype_int16 yy_state_t;

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
         || (defined RAKU_YYSTYPE_IS_TRIVIAL && RAKU_YYSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   571

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  126
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  304

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


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
       2,     2,     2,    59,     2,     2,     2,    67,     2,     2,
      75,    76,    65,    63,    77,    64,    69,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    70,
      60,    58,    61,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,    74,    62,     2,     2,     2,
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
      55,    56,    57,    68
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   217,   217,   247,   248,   252,   254,   256,   259,   261,
     263,   265,   267,   269,   271,   273,   275,   277,   279,   281,
     284,   289,   292,   295,   298,   301,   304,   305,   306,   307,
     308,   310,   313,   316,   317,   318,   319,   320,   324,   326,
     328,   333,   339,   341,   347,   353,   359,   372,   378,   393,
     411,   412,   421,   429,   441,   474,   475,   478,   481,   492,
     504,   508,   515,   516,   520,   525,   529,   530,   553,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     573,   577,   578,   579,   583,   584,   585,   586,   590,   591,
     592,   593,   594,   598,   599,   600,   603,   606,   611,   613,
     619,   624,   631,   637,   644,   647,   650,   653,   656,   659,
     663,   664,   668,   669,   670,   671,   672,   673,   674,   675,
     677,   679,   681,   683,   685,   687,   692
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if RAKU_YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LIT_INT", "LIT_FLOAT",
  "LIT_STR", "LIT_INTERP_STR", "LIT_REGEX", "VAR_SCALAR", "VAR_ARRAY",
  "VAR_HASH", "VAR_TWIGIL", "IDENT", "KW_MY", "KW_SAY", "KW_PRINT",
  "KW_IF", "KW_ELSE", "KW_ELSIF", "KW_WHILE", "KW_FOR", "KW_SUB",
  "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_GIVEN", "KW_WHEN", "KW_DEFAULT",
  "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_MAP",
  "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH", "KW_DIE", "KW_CLASS",
  "KW_METHOD", "KW_HAS", "KW_NEW", "OP_FATARROW", "OP_RANGE",
  "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ",
  "OP_SNE", "OP_AND", "OP_OR", "OP_BIND", "OP_SMATCH", "OP_DIV", "'='",
  "'!'", "'<'", "'>'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "'.'", "';'", "'['", "']'", "'{'", "'}'", "'('", "')'", "','", "$accept",
  "program", "stmt_list", "stmt", "if_stmt", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "class_decl", "class_body_list", "named_arg_list",
  "param_list", "block", "closure", "expr", "cmp_expr", "range_expr",
  "add_expr", "mul_expr", "unary_expr", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-49)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -49,    18,   339,   -49,   -49,   -49,   -49,   -49,    -2,   -25,
     -47,   -49,    13,   120,   458,   458,   -48,   -27,   458,    41,
      -7,   458,   377,   458,    45,    73,    39,    63,    -7,    58,
      58,   415,    -7,   458,   104,   496,   496,   458,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,    75,     8,
     -49,   105,    70,   -49,   -49,   -49,    95,   458,   128,   458,
     162,   458,   133,    30,   118,   119,   122,    94,   123,   107,
     -45,   112,   116,   458,   458,   -22,   109,   -49,   -49,   124,
     -49,   125,   115,   -44,   -23,   458,   458,   -49,   458,   458,
     458,   458,   -49,   155,   -49,   126,   -49,   -49,   -49,   121,
     -49,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   189,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   186,   130,   145,   132,   144,   134,   131,   -49,   -49,
      31,   458,   458,   458,   -26,    42,    43,   458,   458,   195,
     458,   -49,   -49,   135,   149,   201,   -49,    -3,   282,   -49,
     -49,   -49,   217,   458,   218,   458,   156,   157,   161,   -49,
     -49,   -49,    -7,   -49,   -49,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,   -49,    96,    96,    70,    70,
      70,   -49,   -49,   -49,   -49,   163,   -49,   458,   178,   179,
     181,    -6,   -49,   458,   170,   171,   172,   458,   -49,   458,
     -49,   458,   -49,   -49,   173,   182,   174,    -7,    -7,    -7,
     -49,    -7,    34,   -49,     4,   183,   175,   185,   176,    -7,
      -7,   -49,   -49,   -20,    87,   177,   458,   458,   458,   208,
     -49,    57,   -49,   -49,   -49,   -49,   188,   190,   192,   -49,
     -49,   -49,   236,   -49,   -49,   -49,    -7,   247,   458,    -7,
     -49,   -49,   -49,   193,   194,   239,   -49,   253,    14,   -49,
     -49,    66,   -49,   196,   197,   198,   458,   -49,   257,   -49,
     -49,   -49,    -4,   -49,   -49,    -7,   199,   -49,   -49,    -7,
     184,   200,   202,   -49,   -49,   -49,   -49,   -49,   228,   -49,
     -49,   -49,   -49,   -49,     3,   -49,   -49,   458,    -7,    71,
     -49,   -49,    -7,   -49
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,   112,   113,   114,   115,   116,   117,
     118,   125,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,    27,
      28,    33,    34,    35,    29,    30,    36,    37,     0,    68,
      80,    83,    87,    92,    95,    96,   109,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,     0,     0,     0,     0,     0,     0,     3,    67,     0,
      18,     0,     0,     0,     0,     0,     0,    45,     0,     0,
       0,     0,   107,    31,   104,     0,   116,    94,    93,     0,
      26,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,   110,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,     0,     0,     0,    47,     0,     0,    16,
      17,    50,     0,     0,     0,     0,     0,     0,     0,   105,
     106,   108,     0,    55,   126,    69,    70,    81,    82,    71,
      72,    75,    76,    77,    78,    79,    73,    74,    86,    84,
      85,    91,    88,    89,    90,   103,    19,     0,   119,   120,
     121,     0,    97,     0,     0,     0,     0,     0,    11,     0,
      12,     0,    13,    66,     0,     0,     0,     0,     0,     0,
      62,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,    65,    32,     0,     0,     0,     0,     0,     0,     0,
     100,     0,   111,     5,     6,     7,     0,     0,     0,   119,
     120,   121,    38,    41,    46,    53,     0,     0,     0,     0,
      48,   122,   123,     0,     0,    42,    44,     0,     0,    54,
     102,     0,    20,     0,     0,     0,     0,    99,     0,     8,
       9,    10,     0,    52,    63,     0,     0,    24,    25,     0,
       0,     0,     0,   101,    21,    22,    23,    60,     0,    40,
      39,    51,    49,    43,     0,    57,    56,     0,     0,     0,
      61,    59,     0,    58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -49,   -49,   204,   -49,     2,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -19,   -18,    50,   -14,
     -49,   -49,   114,    56,   -33,   -49,   -49,    52,   -49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    38,    39,    40,    41,    42,    43,    44,
      45,   214,    46,    47,   223,   231,   212,    78,    89,    48,
      49,    50,    51,    52,    53,    54,    55,   130,    56
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,    72,    97,    98,    75,   210,   229,    79,    81,    82,
      87,   210,    16,    60,    93,   139,   152,    92,     3,    94,
     257,   258,   281,    99,   145,   282,    61,    73,   140,   153,
     248,   249,   197,     4,     5,     6,     7,   154,    68,    69,
      70,    11,    12,   122,   198,   124,    59,   126,    74,   129,
     155,    77,    20,    76,   259,    83,    57,   146,    24,   143,
     144,   101,   102,    29,    30,    31,    77,    58,    33,    77,
     230,   156,   157,   211,   158,   159,   160,   161,   250,   298,
      90,    91,    62,    84,   181,   182,   183,   184,    63,    35,
       4,     5,     6,     7,    36,    68,    69,    70,    11,    12,
     199,   201,   134,   135,   136,    37,   128,   192,   193,    20,
     246,   247,   200,   202,    85,    24,    95,   194,   195,   196,
      29,    30,    31,   203,   204,    33,   206,   117,    64,    65,
      66,    88,    67,   267,   268,   118,   119,   120,    86,   216,
     123,   218,   283,   193,   222,   100,    35,   302,   247,   103,
     104,    36,   105,   106,   107,   108,   109,   110,   114,   115,
     116,   111,    37,   260,   121,   112,   113,   114,   115,   116,
     178,   179,   180,   225,   125,   127,   131,   132,   138,   232,
     133,   137,   141,   236,   147,   237,   142,   238,   151,   242,
     243,   244,   162,   245,   149,   150,   175,   164,   185,   163,
     186,   255,   256,   187,   188,   189,   191,   205,   190,   209,
     129,   207,   263,   264,   265,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   208,   176,   177,   273,   215,
     217,   276,   219,   220,   275,   221,   226,   227,   224,   228,
     233,   234,   235,   240,   251,   239,   253,   262,   241,   252,
     254,   266,   287,   272,   290,   274,   279,   291,   269,   294,
     270,   293,   271,   277,   278,   280,   284,   285,   286,   288,
     295,   297,   296,   292,   289,   299,   261,     0,     0,     0,
     301,   148,     0,   300,   303,     4,     5,     6,     7,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,     0,
       0,    17,    18,    19,    20,    21,    22,    23,     0,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     0,
      33,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,     4,     5,     6,     7,    36,     8,     9,    10,
      11,    12,    13,    14,    15,    16,   213,    37,    17,    18,
      19,    20,    21,    22,    23,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,     0,    33,    34,     0,
       4,     5,     6,     7,     0,    68,    69,    70,    11,    12,
       0,     0,     0,     0,     0,     0,     0,     0,    35,    20,
       0,     0,     0,    36,     0,    24,     0,     0,     0,     0,
      29,    30,    31,     0,    37,    33,     0,     0,     4,     5,
       6,     7,     0,    68,    69,    70,    11,    12,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    20,     0,     0,
       0,    36,     0,    24,     0,     0,     0,    80,    29,    30,
      31,     0,    37,    33,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,    68,    69,    70,    11,
      12,     0,     0,     0,    35,     0,     0,     0,     0,    36,
      20,     0,     0,     0,     0,     0,    24,     0,    88,     0,
      37,    29,    30,    31,     0,     0,    33,     0,     0,     4,
       5,     6,     7,     0,    96,    69,    70,    11,    12,     0,
       0,     0,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    36,     0,    24,     0,     0,     0,     0,    29,
      30,    31,     0,    37,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37
};

static const yytype_int16 yycheck[] =
{
      14,    15,    35,    36,    18,     8,    12,    21,    22,    23,
      28,     8,    16,    60,    32,    60,    60,    31,     0,    33,
      40,    41,     8,    37,    46,    11,    73,    75,    73,    73,
      26,    27,    58,     3,     4,     5,     6,    60,     8,     9,
      10,    11,    12,    57,    70,    59,    71,    61,    75,    63,
      73,    73,    22,    12,    74,    10,    58,    75,    28,    73,
      74,    53,    54,    33,    34,    35,    73,    69,    38,    73,
      76,    85,    86,    76,    88,    89,    90,    91,    74,    76,
      30,    31,    69,    10,   117,   118,   119,   120,    75,    59,
       3,     4,     5,     6,    64,     8,     9,    10,    11,    12,
      58,    58,     8,     9,    10,    75,    76,    76,    77,    22,
      76,    77,    70,    70,    75,    28,    12,   131,   132,   133,
      33,    34,    35,   137,   138,    38,   140,    57,     8,     9,
      10,    73,    12,    76,    77,    65,    66,    67,    75,   153,
      12,   155,    76,    77,   162,    70,    59,    76,    77,    44,
      45,    64,    47,    48,    49,    50,    51,    52,    62,    63,
      64,    56,    75,    76,    69,    60,    61,    62,    63,    64,
     114,   115,   116,   187,    12,    42,    58,    58,    71,   193,
      58,    58,    70,   197,    75,   199,    70,   201,    73,   207,
     208,   209,    37,   211,    70,    70,     7,    76,    12,    73,
      70,   219,   220,    58,    72,    61,    75,    12,    74,     8,
     224,    76,   226,   227,   228,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    76,   112,   113,   246,    12,
      12,   249,    76,    76,   248,    74,    58,    58,    75,    58,
      70,    70,    70,    61,    61,    72,    61,    70,    74,    74,
      74,    43,   266,    17,   272,     8,    17,   275,    70,    75,
      70,   279,    70,    70,    70,    12,    70,    70,    70,    12,
      70,    43,    70,    74,   272,   294,   224,    -1,    -1,    -1,
     298,    77,    -1,   297,   302,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,     3,     4,     5,     6,    64,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    74,    75,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    22,
      -1,    -1,    -1,    64,    -1,    28,    -1,    -1,    -1,    -1,
      33,    34,    35,    -1,    75,    38,    -1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    22,    -1,    -1,
      -1,    64,    -1,    28,    -1,    -1,    -1,    70,    33,    34,
      35,    -1,    75,    38,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    64,
      22,    -1,    -1,    -1,    -1,    -1,    28,    -1,    73,    -1,
      75,    33,    34,    35,    -1,    -1,    38,    -1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    64,    -1,    28,    -1,    -1,    -1,    -1,    33,
      34,    35,    -1,    75,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    79,    80,     0,     3,     4,     5,     6,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    19,    20,    21,
      22,    23,    24,    25,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    38,    39,    59,    64,    75,    81,    82,
      83,    84,    85,    86,    87,    88,    90,    91,    97,    98,
      99,   100,   101,   102,   103,   104,   106,    58,    69,    71,
      60,    73,    69,    75,     8,     9,    10,    12,     8,     9,
      10,    97,    97,    75,    75,    97,    12,    73,    95,    97,
      70,    97,    97,    10,    10,    75,    75,    95,    73,    96,
      96,    96,    97,    95,    97,    12,     8,   102,   102,    97,
      70,    53,    54,    44,    45,    47,    48,    49,    50,    51,
      52,    56,    60,    61,    62,    63,    64,    57,    65,    66,
      67,    69,    97,    12,    97,    12,    97,    42,    76,    97,
     105,    58,    58,    58,     8,     9,    10,    58,    71,    60,
      73,    70,    70,    97,    97,    46,    95,    75,    80,    70,
      70,    73,    60,    73,    60,    73,    97,    97,    97,    97,
      97,    97,    37,    73,    76,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,     7,   100,   100,   101,   101,
     101,   102,   102,   102,   102,    12,    70,    58,    72,    61,
      74,    75,    76,    77,    97,    97,    97,    58,    70,    58,
      70,    58,    70,    97,    97,    12,    97,    76,    76,     8,
       8,    76,    94,    74,    89,    12,    97,    12,    97,    76,
      76,    74,    95,    92,    75,    97,    58,    58,    58,    12,
      76,    93,    97,    70,    70,    70,    97,    97,    97,    72,
      61,    74,    95,    95,    95,    95,    76,    77,    26,    27,
      74,    61,    74,    61,    74,    95,    95,    40,    41,    74,
      76,   105,    70,    97,    97,    97,    43,    76,    77,    70,
      70,    70,    17,    95,     8,    97,    95,    70,    70,    17,
      12,     8,    11,    76,    70,    70,    70,    97,    12,    82,
      95,    95,    74,    95,    75,    70,    70,    43,    76,    94,
      97,    95,    76,    95
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    78,    79,    80,    80,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    82,
      82,    83,    84,    84,    85,    86,    87,    87,    88,    88,
      89,    89,    90,    90,    91,    92,    92,    92,    92,    92,
      93,    93,    94,    94,    95,    96,    97,    97,    97,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    99,    99,    99,   100,   100,   100,   100,   101,   101,
     101,   101,   101,   102,   102,   102,   103,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     105,   105,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     5,     5,     6,     6,
       6,     4,     4,     4,     3,     3,     3,     3,     2,     4,
       6,     7,     7,     7,     6,     6,     2,     1,     1,     1,
       1,     2,     4,     1,     1,     1,     1,     1,     5,     7,
       7,     5,     5,     7,     5,     2,     5,     3,     5,     7,
       0,     4,     6,     5,     5,     0,     4,     4,     7,     6,
       3,     5,     1,     3,     3,     3,     3,     2,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     3,     3,
       3,     3,     1,     2,     2,     1,     1,     4,     3,     6,
       5,     6,     5,     3,     2,     3,     3,     2,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     5,     5,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = RAKU_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == RAKU_YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use RAKU_YYerror or RAKU_YYUNDEF. */
#define YYERRCODE RAKU_YYUNDEF


/* Enable debugging if requested.  */
#if RAKU_YYDEBUG

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
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
                 int yyrule)
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !RAKU_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !RAKU_YYDEBUG */


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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
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

  yychar = RAKU_YYEMPTY; /* Cause a token to be read.  */

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
  if (yychar == RAKU_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= RAKU_YYEOF)
    {
      yychar = RAKU_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == RAKU_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = RAKU_YYUNDEF;
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
  yychar = RAKU_YYEMPTY;
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
  case 2: /* program: stmt_list  */
#line 218 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            /* Partition: subs (ival & SUB_TAG) vs body stmts */
            if (all) {
                /* Pass 1: emit sub defs */
                for (int i = 0; i < all->count; i++) {
                    EXPR_t *e = all->items[i];
                    if (!e || !(e->kind==E_FNC && (e->ival & SUB_TAG))) continue;
                    e->ival &= ~SUB_TAG;   /* restore real nparams */
                    add_proc(e);
                    all->items[i] = NULL;  /* mark consumed */
                }
                /* Pass 2: wrap remaining body stmts in synthetic "main" E_FNC */
                int has_body = 0;
                for (int i = 0; i < all->count; i++) if (all->items[i]) { has_body=1; break; }
                if (has_body) {
                    EXPR_t *mf = leaf_sval(E_FNC, "main"); mf->ival = 0;
                    EXPR_t *mn = expr_new(E_VAR); mn->sval = intern("main");
                    expr_add_child(mf, mn);
                    for (int i = 0; i < all->count; i++)
                        if (all->items[i]) expr_add_child(mf, all->items[i]);
                    add_proc(mf);
                }
                exprlist_free(all);
            }
        }
#line 1593 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 247 "raku.y"
                     { (yyval.list) = exprlist_new(); }
#line 1599 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 248 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1605 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 253 "raku.y"
        { (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1611 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 255 "raku.y"
        { (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1617 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 257 "raku.y"
        { (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1623 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 260 "raku.y"
        { free((yyvsp[-4].sval)); (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1629 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 262 "raku.y"
        { free((yyvsp[-4].sval)); (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1635 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 264 "raku.y"
        { free((yyvsp[-4].sval)); (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1641 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 266 "raku.y"
        { free((yyvsp[-2].sval)); (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-1].sval)), leaf_sval(E_QLIT, "")); }
#line 1647 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 268 "raku.y"
        { free((yyvsp[-2].sval)); (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-1].sval)), leaf_sval(E_QLIT, "")); }
#line 1653 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 270 "raku.y"
        { free((yyvsp[-2].sval)); (yyval.node) = expr_binary(E_ASSIGN, var_node((yyvsp[-1].sval)), leaf_sval(E_QLIT, "")); }
#line 1659 "raku.tab.c"
    break;

  case 14: /* stmt: KW_SAY expr ';'  */
#line 272 "raku.y"
        { EXPR_t *c=make_call("write"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1665 "raku.tab.c"
    break;

  case 15: /* stmt: KW_PRINT expr ';'  */
#line 274 "raku.y"
        { EXPR_t *c=make_call("writes"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1671 "raku.tab.c"
    break;

  case 16: /* stmt: KW_TAKE expr ';'  */
#line 276 "raku.y"
        { (yyval.node)=expr_unary(E_SUSPEND,(yyvsp[-1].node)); }
#line 1677 "raku.tab.c"
    break;

  case 17: /* stmt: KW_RETURN expr ';'  */
#line 278 "raku.y"
        { EXPR_t *r=expr_new(E_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1683 "raku.tab.c"
    break;

  case 18: /* stmt: KW_RETURN ';'  */
#line 280 "raku.y"
        { (yyval.node)=expr_new(E_RETURN); }
#line 1689 "raku.tab.c"
    break;

  case 19: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 282 "raku.y"
        { (yyval.node)=expr_binary(E_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1695 "raku.tab.c"
    break;

  case 20: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 285 "raku.y"
        { EXPR_t *fe=expr_new(E_FIELD);
          fe->sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(E_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1704 "raku.tab.c"
    break;

  case 21: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 290 "raku.y"
        { EXPR_t *c=make_call("arr_set");
          expr_add_child(c,var_node((yyvsp[-6].sval))); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1711 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 293 "raku.y"
        { EXPR_t *c=make_call("hash_set");
          expr_add_child(c,var_node((yyvsp[-6].sval))); expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-4].sval))); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1718 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 296 "raku.y"
        { EXPR_t *c=make_call("hash_set");
          expr_add_child(c,var_node((yyvsp[-6].sval))); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1725 "raku.tab.c"
    break;

  case 24: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 299 "raku.y"
        { EXPR_t *c=make_call("hash_delete");
          expr_add_child(c,var_node((yyvsp[-4].sval))); expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 1732 "raku.tab.c"
    break;

  case 25: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 302 "raku.y"
        { EXPR_t *c=make_call("hash_delete");
          expr_add_child(c,var_node((yyvsp[-4].sval))); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1739 "raku.tab.c"
    break;

  case 26: /* stmt: expr ';'  */
#line 304 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 1745 "raku.tab.c"
    break;

  case 27: /* stmt: if_stmt  */
#line 305 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1751 "raku.tab.c"
    break;

  case 28: /* stmt: while_stmt  */
#line 306 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1757 "raku.tab.c"
    break;

  case 29: /* stmt: for_stmt  */
#line 307 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1763 "raku.tab.c"
    break;

  case 30: /* stmt: given_stmt  */
#line 308 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1769 "raku.tab.c"
    break;

  case 31: /* stmt: KW_TRY block  */
#line 311 "raku.y"
        { EXPR_t *c=make_call("raku_try");
          expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 1776 "raku.tab.c"
    break;

  case 32: /* stmt: KW_TRY block KW_CATCH block  */
#line 314 "raku.y"
        { EXPR_t *c=make_call("raku_try");
          expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 1783 "raku.tab.c"
    break;

  case 33: /* stmt: unless_stmt  */
#line 316 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1789 "raku.tab.c"
    break;

  case 34: /* stmt: until_stmt  */
#line 317 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1795 "raku.tab.c"
    break;

  case 35: /* stmt: repeat_stmt  */
#line 318 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1801 "raku.tab.c"
    break;

  case 36: /* stmt: sub_decl  */
#line 319 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1807 "raku.tab.c"
    break;

  case 37: /* stmt: class_decl  */
#line 320 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1813 "raku.tab.c"
    break;

  case 38: /* if_stmt: KW_IF '(' expr ')' block  */
#line 325 "raku.y"
        { EXPR_t *e=expr_new(E_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1819 "raku.tab.c"
    break;

  case 39: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 327 "raku.y"
        { EXPR_t *e=expr_new(E_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1825 "raku.tab.c"
    break;

  case 40: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 329 "raku.y"
        { EXPR_t *e=expr_new(E_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1831 "raku.tab.c"
    break;

  case 41: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 334 "raku.y"
        { (yyval.node)=expr_binary(E_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 1837 "raku.tab.c"
    break;

  case 42: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 340 "raku.y"
        { EXPR_t *e=expr_new(E_IF); expr_add_child(e,expr_unary(E_NOT,(yyvsp[-2].node))); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1843 "raku.tab.c"
    break;

  case 43: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 342 "raku.y"
        { EXPR_t *e=expr_new(E_IF); expr_add_child(e,expr_unary(E_NOT,(yyvsp[-4].node))); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1849 "raku.tab.c"
    break;

  case 44: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 348 "raku.y"
        { EXPR_t *e=expr_new(E_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1855 "raku.tab.c"
    break;

  case 45: /* repeat_stmt: KW_REPEAT block  */
#line 354 "raku.y"
        { EXPR_t *e=expr_new(E_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1861 "raku.tab.c"
    break;

  case 46: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 360 "raku.y"
        { EXPR_t *iter=(yyvsp[-3].node); const char *vname=strip_sigil((yyvsp[-1].sval));
          if (iter->kind==E_TO) {
              /* range case: lo=children[0], hi=children[1] */
              (yyval.node) = make_for_range(iter->children[0], iter->children[1], vname, (yyvsp[0].node));
          } else {
              /* Always wrap in E_ITERATE so loopvar goes on the wrapper node.
               * RK-16/RK-21: gen->sval = loopvar name for icn_drive / E_EVERY binding. */
              const char *vn = intern(strip_sigil((yyvsp[-1].sval)));
              EXPR_t *gen = expr_unary(E_ITERATE, iter);
              gen->sval = (char *)vn;
              (yyval.node)=expr_binary(E_EVERY,gen,(yyvsp[0].node));
          } }
#line 1878 "raku.tab.c"
    break;

  case 47: /* for_stmt: KW_FOR expr block  */
#line 373 "raku.y"
        { EXPR_t *gen=((yyvsp[-1].node)->kind==E_VAR)?expr_unary(E_ITERATE,(yyvsp[-1].node)):(yyvsp[-1].node);
          (yyval.node)=expr_binary(E_EVERY,gen,(yyvsp[0].node)); }
#line 1885 "raku.tab.c"
    break;

  case 48: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 379 "raku.y"
        { /* RK-18d: E_CASE[ topic, cmpnode0, val0, body0, cmpnode1, val1, body1, ... ]
           * cmp kind stored in separate E_ILIT node (ival=EKind) to avoid corrupting val->ival. */
          EXPR_t *ec=expr_new(E_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++){
              EXPR_t *pair=whens->items[i];
              EKind cmp=(EKind)(pair->ival);
              EXPR_t *val=pair->children[0], *body=pair->children[1];
              EXPR_t *cn=expr_new(E_ILIT); cn->ival=(long)cmp;
              expr_add_child(ec,cn); expr_add_child(ec,val); expr_add_child(ec,body);
          }
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 1904 "raku.tab.c"
    break;

  case 49: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 394 "raku.y"
        { /* RK-18d: E_CASE with default: E_NUL cmpnode + E_NUL val + body at end. */
          EXPR_t *ec=expr_new(E_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++){
              EXPR_t *pair=whens->items[i];
              EKind cmp=(EKind)(pair->ival);
              EXPR_t *val=pair->children[0], *body=pair->children[1];
              EXPR_t *cn=expr_new(E_ILIT); cn->ival=(long)cmp;
              expr_add_child(ec,cn); expr_add_child(ec,val); expr_add_child(ec,body);
          }
          exprlist_free(whens);
          expr_add_child(ec,expr_new(E_NUL)); expr_add_child(ec,expr_new(E_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 1923 "raku.tab.c"
    break;

  case 50: /* when_list: %empty  */
#line 411 "raku.y"
                   { (yyval.list)=exprlist_new(); }
#line 1929 "raku.tab.c"
    break;

  case 51: /* when_list: when_list KW_WHEN expr block  */
#line 413 "raku.y"
        { EKind cmp=((yyvsp[-1].node)->kind==E_QLIT)?E_LEQ:E_EQ;
          EXPR_t *pair=expr_new(E_SEQ_EXPR);
          pair->ival=(long)cmp;
          expr_add_child(pair,(yyvsp[-1].node)); expr_add_child(pair,(yyvsp[0].node));
          (yyval.list)=exprlist_append((yyvsp[-3].list),pair); }
#line 1939 "raku.tab.c"
    break;

  case 52: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 422 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          EXPR_t *e=leaf_sval(E_FNC,(yyvsp[-4].sval)); e->ival=(long)np|SUB_TAG;
          EXPR_t *nn=expr_new(E_VAR); nn->sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          EXPR_t *body=(yyvsp[0].node);
          for(int i=0;i<body->nchildren;i++) expr_add_child(e,body->children[i]);
          (yyval.node)=e; }
#line 1951 "raku.tab.c"
    break;

  case 53: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 430 "raku.y"
        { EXPR_t *e=leaf_sval(E_FNC,(yyvsp[-3].sval)); e->ival=(long)0|SUB_TAG;
          EXPR_t *nn=expr_new(E_VAR); nn->sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          EXPR_t *body=(yyvsp[0].node);
          for(int i=0;i<body->nchildren;i++) expr_add_child(e,body->children[i]);
          (yyval.node)=e; }
#line 1961 "raku.tab.c"
    break;

  case 54: /* class_decl: KW_CLASS IDENT '{' class_body_list '}'  */
#line 442 "raku.y"
        {
            const char *cname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            EXPR_t *rec = expr_new(E_RECORD);
            rec->sval = (char *)cname;
            if (body) {
                for (int i = 0; i < body->count; i++) {
                    EXPR_t *item = body->items[i];
                    if (!item) continue;
                    if (item->kind == E_VAR) {
                        expr_add_child(rec, item);
                    } else if (item->kind == E_FNC && (item->ival & SUB_TAG)) {
                        char fullname[256];
                        snprintf(fullname, sizeof fullname, "%s__%s", cname, item->sval);
                        const char *fname = intern(fullname);
                        raku_meth_register(cname, item->sval, fname);
                        item->sval = (char *)fname;
                        if (item->nchildren > 0 && item->children[0]->kind == E_VAR)
                            item->children[0]->sval = (char *)fname;
                        item->ival &= ~SUB_TAG;
                        add_proc(item);
                        body->items[i] = NULL;
                    }
                }
                exprlist_free(body);
            }
            add_proc(rec);
            (yyval.node) = expr_new(E_NUL);
        }
#line 1995 "raku.tab.c"
    break;

  case 55: /* class_body_list: %empty  */
#line 474 "raku.y"
                   { (yyval.list) = exprlist_new(); }
#line 2001 "raku.tab.c"
    break;

  case 56: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 476 "raku.y"
        { EXPR_t *fv = leaf_sval(E_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2008 "raku.tab.c"
    break;

  case 57: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 479 "raku.y"
        { EXPR_t *fv = leaf_sval(E_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2015 "raku.tab.c"
    break;

  case 58: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 482 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          EXPR_t *e = leaf_sval(E_FNC, (yyvsp[-4].sval));
          e->ival = (long)(np + 1) | SUB_TAG;
          EXPR_t *nn = expr_new(E_VAR); nn->sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          expr_add_child(e, leaf_sval(E_VAR, "self"));
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          EXPR_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->nchildren; i++) expr_add_child(e, body->children[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2030 "raku.tab.c"
    break;

  case 59: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 493 "raku.y"
        { EXPR_t *e = leaf_sval(E_FNC, (yyvsp[-3].sval));
          e->ival = (long)(1) | SUB_TAG;
          EXPR_t *nn = expr_new(E_VAR); nn->sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          expr_add_child(e, leaf_sval(E_VAR, "self"));
          EXPR_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->nchildren; i++) expr_add_child(e, body->children[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2043 "raku.tab.c"
    break;

  case 60: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 505 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(E_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2051 "raku.tab.c"
    break;

  case 61: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 509 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(E_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2059 "raku.tab.c"
    break;

  case 62: /* param_list: VAR_SCALAR  */
#line 515 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2065 "raku.tab.c"
    break;

  case 63: /* param_list: param_list ',' VAR_SCALAR  */
#line 516 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2071 "raku.tab.c"
    break;

  case 64: /* block: '{' stmt_list '}'  */
#line 520 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2077 "raku.tab.c"
    break;

  case 65: /* closure: '{' expr '}'  */
#line 525 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2083 "raku.tab.c"
    break;

  case 66: /* expr: VAR_SCALAR '=' expr  */
#line 529 "raku.y"
                           { (yyval.node)=expr_binary(E_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2089 "raku.tab.c"
    break;

  case 67: /* expr: KW_GATHER block  */
#line 530 "raku.y"
                           {
          /* RK-21: gather { block } → anonymous coroutine sub + call.
           * 1. Build E_FNC def with SUB_TAG (like sub_decl) named __gather_N.
           * 2. add_proc() so it lands in the proc table.
           * 3. Return a call E_FNC (no SUB_TAG) so icn_eval_gen wraps it as
           *    icn_bb_suspend — a BB_PUMP coroutine collecting E_SUSPEND (take) values. */
          static int gather_seq = 0;
          char gname[32]; snprintf(gname, sizeof gname, "__gather_%d", gather_seq++);
          /* Build the def node */
          EXPR_t *def = leaf_sval(E_FNC, gname); def->ival = (long)0 | SUB_TAG;
          EXPR_t *dn  = expr_new(E_VAR); dn->sval = intern(gname);
          expr_add_child(def, dn);
          /* Splice block children into def */
          EXPR_t *blk = (yyvsp[0].node);
          for (int i = 0; i < blk->nchildren; i++) expr_add_child(def, blk->children[i]);
          def->ival &= ~SUB_TAG;   /* strip sentinel — restore real nparams (0) for icn_call_proc */
          add_proc(def);
          /* Build the call node (no SUB_TAG) */
          EXPR_t *call = leaf_sval(E_FNC, gname);
          EXPR_t *cn   = expr_new(E_VAR); cn->sval = intern(gname);
          expr_add_child(call, cn);
          (yyval.node) = call;
      }
#line 2117 "raku.tab.c"
    break;

  case 68: /* expr: cmp_expr  */
#line 553 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2123 "raku.tab.c"
    break;

  case 69: /* cmp_expr: cmp_expr OP_AND add_expr  */
#line 557 "raku.y"
                                { (yyval.node)=expr_binary(E_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2129 "raku.tab.c"
    break;

  case 70: /* cmp_expr: cmp_expr OP_OR add_expr  */
#line 558 "raku.y"
                                { (yyval.node)=expr_binary(E_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2135 "raku.tab.c"
    break;

  case 71: /* cmp_expr: add_expr OP_EQ add_expr  */
#line 559 "raku.y"
                                { (yyval.node)=expr_binary(E_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2141 "raku.tab.c"
    break;

  case 72: /* cmp_expr: add_expr OP_NE add_expr  */
#line 560 "raku.y"
                                { (yyval.node)=expr_binary(E_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2147 "raku.tab.c"
    break;

  case 73: /* cmp_expr: add_expr '<' add_expr  */
#line 561 "raku.y"
                                { (yyval.node)=expr_binary(E_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2153 "raku.tab.c"
    break;

  case 74: /* cmp_expr: add_expr '>' add_expr  */
#line 562 "raku.y"
                                { (yyval.node)=expr_binary(E_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2159 "raku.tab.c"
    break;

  case 75: /* cmp_expr: add_expr OP_LE add_expr  */
#line 563 "raku.y"
                                { (yyval.node)=expr_binary(E_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2165 "raku.tab.c"
    break;

  case 76: /* cmp_expr: add_expr OP_GE add_expr  */
#line 564 "raku.y"
                                { (yyval.node)=expr_binary(E_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2171 "raku.tab.c"
    break;

  case 77: /* cmp_expr: add_expr OP_SEQ add_expr  */
#line 565 "raku.y"
                                { (yyval.node)=expr_binary(E_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2177 "raku.tab.c"
    break;

  case 78: /* cmp_expr: add_expr OP_SNE add_expr  */
#line 566 "raku.y"
                                { (yyval.node)=expr_binary(E_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2183 "raku.tab.c"
    break;

  case 79: /* cmp_expr: add_expr OP_SMATCH LIT_REGEX  */
#line 568 "raku.y"
        { /* RK-23: $s ~~ /pattern/ — emit make_call("raku_match", subj, pat_str) */
          EXPR_t *c = make_call("raku_match");
          expr_add_child(c, (yyvsp[-2].node));
          expr_add_child(c, leaf_sval(E_QLIT, (yyvsp[0].sval)));
          (yyval.node) = c; }
#line 2193 "raku.tab.c"
    break;

  case 80: /* cmp_expr: range_expr  */
#line 573 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2199 "raku.tab.c"
    break;

  case 81: /* range_expr: add_expr OP_RANGE add_expr  */
#line 577 "raku.y"
                                    { (yyval.node)=expr_binary(E_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2205 "raku.tab.c"
    break;

  case 82: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 578 "raku.y"
                                    { (yyval.node)=expr_binary(E_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2211 "raku.tab.c"
    break;

  case 83: /* range_expr: add_expr  */
#line 579 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2217 "raku.tab.c"
    break;

  case 84: /* add_expr: add_expr '+' mul_expr  */
#line 583 "raku.y"
                             { (yyval.node)=expr_binary(E_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2223 "raku.tab.c"
    break;

  case 85: /* add_expr: add_expr '-' mul_expr  */
#line 584 "raku.y"
                             { (yyval.node)=expr_binary(E_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2229 "raku.tab.c"
    break;

  case 86: /* add_expr: add_expr '~' mul_expr  */
#line 585 "raku.y"
                             { (yyval.node)=expr_binary(E_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2235 "raku.tab.c"
    break;

  case 87: /* add_expr: mul_expr  */
#line 586 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2241 "raku.tab.c"
    break;

  case 88: /* mul_expr: mul_expr '*' unary_expr  */
#line 590 "raku.y"
                                  { (yyval.node)=expr_binary(E_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2247 "raku.tab.c"
    break;

  case 89: /* mul_expr: mul_expr '/' unary_expr  */
#line 591 "raku.y"
                                  { (yyval.node)=expr_binary(E_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2253 "raku.tab.c"
    break;

  case 90: /* mul_expr: mul_expr '%' unary_expr  */
#line 592 "raku.y"
                                  { (yyval.node)=expr_binary(E_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2259 "raku.tab.c"
    break;

  case 91: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 593 "raku.y"
                                  { (yyval.node)=expr_binary(E_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2265 "raku.tab.c"
    break;

  case 92: /* mul_expr: unary_expr  */
#line 594 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2271 "raku.tab.c"
    break;

  case 93: /* unary_expr: '-' unary_expr  */
#line 598 "raku.y"
                                   { (yyval.node)=expr_unary(E_MNS,(yyvsp[0].node)); }
#line 2277 "raku.tab.c"
    break;

  case 94: /* unary_expr: '!' unary_expr  */
#line 599 "raku.y"
                                   { (yyval.node)=expr_unary(E_NOT,(yyvsp[0].node)); }
#line 2283 "raku.tab.c"
    break;

  case 95: /* unary_expr: postfix_expr  */
#line 600 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2289 "raku.tab.c"
    break;

  case 96: /* postfix_expr: call_expr  */
#line 603 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2295 "raku.tab.c"
    break;

  case 97: /* call_expr: IDENT '(' arg_list ')'  */
#line 607 "raku.y"
        { EXPR_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2304 "raku.tab.c"
    break;

  case 98: /* call_expr: IDENT '(' ')'  */
#line 611 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2310 "raku.tab.c"
    break;

  case 99: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 614 "raku.y"
        { EXPR_t *c=make_call("raku_new");
          expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs=(yyvsp[-1].list);
          if(nargs){ for(int i=0;i<nargs->count;i++) expr_add_child(c,nargs->items[i]); exprlist_free(nargs); }
          (yyval.node)=c; }
#line 2320 "raku.tab.c"
    break;

  case 100: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 620 "raku.y"
        { EXPR_t *c=make_call("raku_new");
          expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node)=c; }
#line 2328 "raku.tab.c"
    break;

  case 101: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 625 "raku.y"
        { EXPR_t *c=make_call("raku_mcall");
          expr_add_child(c,(yyvsp[-5].node));
          expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(c,args->items[i]); exprlist_free(args); }
          (yyval.node)=c; }
#line 2339 "raku.tab.c"
    break;

  case 102: /* call_expr: atom '.' IDENT '(' ')'  */
#line 632 "raku.y"
        { EXPR_t *c=make_call("raku_mcall");
          expr_add_child(c,(yyvsp[-4].node));
          expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node)=c; }
#line 2348 "raku.tab.c"
    break;

  case 103: /* call_expr: atom '.' IDENT  */
#line 638 "raku.y"
        { EXPR_t *fe=expr_new(E_FIELD);
          fe->sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 2357 "raku.tab.c"
    break;

  case 104: /* call_expr: KW_DIE expr  */
#line 645 "raku.y"
        { EXPR_t *c=make_call("raku_die");
          expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 2364 "raku.tab.c"
    break;

  case 105: /* call_expr: KW_MAP closure expr  */
#line 648 "raku.y"
        { EXPR_t *c=make_call("raku_map");
          expr_add_child(c,(yyvsp[-1].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 2371 "raku.tab.c"
    break;

  case 106: /* call_expr: KW_GREP closure expr  */
#line 651 "raku.y"
        { EXPR_t *c=make_call("raku_grep");
          expr_add_child(c,(yyvsp[-1].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 2378 "raku.tab.c"
    break;

  case 107: /* call_expr: KW_SORT expr  */
#line 654 "raku.y"
        { EXPR_t *c=make_call("raku_sort");
          expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 2385 "raku.tab.c"
    break;

  case 108: /* call_expr: KW_SORT closure expr  */
#line 657 "raku.y"
        { EXPR_t *c=make_call("raku_sort");
          expr_add_child(c,(yyvsp[-1].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 2392 "raku.tab.c"
    break;

  case 109: /* call_expr: atom  */
#line 659 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 2398 "raku.tab.c"
    break;

  case 110: /* arg_list: expr  */
#line 663 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 2404 "raku.tab.c"
    break;

  case 111: /* arg_list: arg_list ',' expr  */
#line 664 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 2410 "raku.tab.c"
    break;

  case 112: /* atom: LIT_INT  */
#line 668 "raku.y"
                      { EXPR_t *e=expr_new(E_ILIT); e->ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 2416 "raku.tab.c"
    break;

  case 113: /* atom: LIT_FLOAT  */
#line 669 "raku.y"
                      { EXPR_t *e=expr_new(E_FLIT); e->dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 2422 "raku.tab.c"
    break;

  case 114: /* atom: LIT_STR  */
#line 670 "raku.y"
                      { (yyval.node)=leaf_sval(E_QLIT,(yyvsp[0].sval)); }
#line 2428 "raku.tab.c"
    break;

  case 115: /* atom: LIT_INTERP_STR  */
#line 671 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 2434 "raku.tab.c"
    break;

  case 116: /* atom: VAR_SCALAR  */
#line 672 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2440 "raku.tab.c"
    break;

  case 117: /* atom: VAR_ARRAY  */
#line 673 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2446 "raku.tab.c"
    break;

  case 118: /* atom: VAR_HASH  */
#line 674 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2452 "raku.tab.c"
    break;

  case 119: /* atom: VAR_ARRAY '[' expr ']'  */
#line 676 "raku.y"
        { EXPR_t *c=make_call("arr_get"); expr_add_child(c,var_node((yyvsp[-3].sval))); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2458 "raku.tab.c"
    break;

  case 120: /* atom: VAR_HASH '<' IDENT '>'  */
#line 678 "raku.y"
        { EXPR_t *c=make_call("hash_get"); expr_add_child(c,var_node((yyvsp[-3].sval))); expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2464 "raku.tab.c"
    break;

  case 121: /* atom: VAR_HASH '{' expr '}'  */
#line 680 "raku.y"
        { EXPR_t *c=make_call("hash_get"); expr_add_child(c,var_node((yyvsp[-3].sval))); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2470 "raku.tab.c"
    break;

  case 122: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 682 "raku.y"
        { EXPR_t *c=make_call("hash_exists"); expr_add_child(c,var_node((yyvsp[-3].sval))); expr_add_child(c,leaf_sval(E_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2476 "raku.tab.c"
    break;

  case 123: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 684 "raku.y"
        { EXPR_t *c=make_call("hash_exists"); expr_add_child(c,var_node((yyvsp[-3].sval))); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2482 "raku.tab.c"
    break;

  case 124: /* atom: IDENT  */
#line 685 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2488 "raku.tab.c"
    break;

  case 125: /* atom: VAR_TWIGIL  */
#line 688 "raku.y"
        { EXPR_t *fe=expr_new(E_FIELD);
          fe->sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe, leaf_sval(E_VAR, "self"));
          (yyval.node)=fe; }
#line 2497 "raku.tab.c"
    break;

  case 126: /* atom: '(' expr ')'  */
#line 692 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 2503 "raku.tab.c"
    break;


#line 2507 "raku.tab.c"

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
  yytoken = yychar == RAKU_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= RAKU_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == RAKU_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = RAKU_YYEMPTY;
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
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
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != RAKU_YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 695 "raku.y"


/* ── Parse entry (sets up flex buffer and calls yyparse) ─────────────── */
extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);

Program *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
