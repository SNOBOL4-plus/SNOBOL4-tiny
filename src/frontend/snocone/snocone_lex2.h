/*
 * snocone_lex2.h — Snocone new lexer public API
 *
 * GOAL-SNOCONE-LANG-SPACE LS-3.a companion header.
 *
 * This replaces snocone_lex.h once LS-3 is complete.
 * During the transition, both headers coexist; the old
 * snocone_lex.h/.c serve the existing parser; the new
 * snocone_lex2.h / snocone.l serve the new LS-4 Bison parser.
 *
 * Token set: Andrew Koenig's .sc self-host operator set
 * (snocone.sc lines 32-60), minus && and || and %,
 * plus CONCAT synthetic, plus C-style keywords.
 */

#ifndef SNOCONE_LEX2_H
#define SNOCONE_LEX2_H

#include <stddef.h>

/* -----------------------------------------------------------------------
 * Token kinds
 * ----------------------------------------------------------------------- */
typedef enum {
    /* Literals and identifiers */
    SC2_INTEGER = 1,
    SC2_REAL,
    SC2_STRING,
    SC2_IDENT,
    SC2_IDENT_LPAREN_NOSP,  /* identifier immediately followed by '(' — call form */
    SC2_KEYWORD_NAME,       /* &IDENT with no whitespace — e.g. &FULLSCAN */

    /* Synthetic operator (never a literal character) */
    SC2_CONCAT,             /* emitted by prev/next-token rule when space-as-concat fires */

    /* Binary operators — precedence follows SPITBOL Ch.15 */
    SC2_ASSIGN,             /* =    pri 0  right-assoc */
    SC2_QUESTION,           /* ?    pri 1  left;  also unary (interrogation) */
    SC2_PIPE,               /* |    pri 3  right (pattern alternation) */
    /* SC2_CONCAT above -- SPACE pri 4 right (concat) */
    SC2_EQ,                 /* ==   pri 6  left  → EQ(a,b)      */
    SC2_NE,                 /* !=   pri 6  left  → NE(a,b)      */
    SC2_LT,                 /* <    pri 6  left  → LT(a,b)      */
    SC2_GT,                 /* >    pri 6  left  → GT(a,b)      */
    SC2_LE,                 /* <=   pri 6  left  → LE(a,b)      */
    SC2_GE,                 /* >=   pri 6  left  → GE(a,b)      */
    SC2_STR_IDENT,          /* ::   pri 6  left  → IDENT(a,b)   — Andrew .sc line 45 */
    SC2_STR_DIFFER,         /* :!:  pri 6  left  → DIFFER(a,b)  — Andrew .sc line 46 */
    SC2_STR_LT,             /* :<:  pri 6  left  → LLT(a,b)     */
    SC2_STR_GT,             /* :>:  pri 6  left  → LGT(a,b)     */
    SC2_STR_LE,             /* :<=: pri 6  left  → LLE(a,b)     */
    SC2_STR_GE,             /* :>=: pri 6  left  → LGE(a,b)     */
    SC2_STR_EQ,             /* :==: pri 6  left  → LEQ(a,b)     */
    SC2_STR_NE,             /* :!=: pri 6  left  → LNE(a,b)     */
    SC2_PLUS,               /* +    pri 6  left;  also unary (pos) */
    SC2_MINUS,              /* -    pri 6  left;  also unary (neg) */
    SC2_SLASH,              /* /    pri 8  left  */
    SC2_STAR,               /* *    pri 9  left;  also unary (defer) */
    SC2_CARET,              /* ^ ** pri 11 right (exponentiation) */
    SC2_BANG_EXPONENT,      /* !    pri 11 right (SPITBOL synonym for ^) */
    SC2_PERIOD,             /* .    pri 12 left;  also unary (name-of) */
    SC2_DOLLAR,             /* $    pri 12 left;  also unary (indirection) */

    /* Unary-only operators */
    SC2_AT,                 /* @   unary (cursor position); binary OPSYN slot pri 5 */
    SC2_TILDE,              /* ~   unary (negate success/failure); binary OPSYN slot pri 13 */
    SC2_AMPERSAND,          /* &   unary (keyword access); binary OPSYN slot pri 2 */
    SC2_PERCENT,            /* %   OPSYN slot (no built-in semantics — user OPSYN only) */

    /* Compound-assign operators */
    SC2_PLUS_ASSIGN,        /* +=  → a = a + rhs */
    SC2_MINUS_ASSIGN,       /* -=  → a = a - rhs */
    SC2_STAR_ASSIGN,        /* *=  → a = a * rhs */
    SC2_SLASH_ASSIGN,       /* /=  → a = a / rhs */
    SC2_CARET_ASSIGN,       /* ^=  → a = a ^ rhs */

    /* Punctuation */
    SC2_LPAREN,
    SC2_RPAREN,
    SC2_LBRACE,
    SC2_RBRACE,
    SC2_LBRACKET,
    SC2_RBRACKET,
    SC2_COMMA,
    SC2_SEMICOLON,
    SC2_COLON,

    /* Keywords */
    SC2_KW_IF,
    SC2_KW_ELSE,
    SC2_KW_WHILE,
    SC2_KW_DO,
    SC2_KW_UNTIL,       /* NEW — do { } until (cond); */
    SC2_KW_FOR,
    SC2_KW_SWITCH,      /* NEW */
    SC2_KW_CASE,        /* NEW */
    SC2_KW_DEFAULT,     /* NEW */
    SC2_KW_BREAK,
    SC2_KW_CONTINUE,
    SC2_KW_GOTO,
    SC2_KW_FUNCTION,    /* RENAMED from procedure — session #7 */
    SC2_KW_RETURN,
    SC2_KW_FRETURN,
    SC2_KW_NRETURN,
    SC2_KW_STRUCT,

    /* Sentinel */
    SC2_EOF,
    SC2_UNKNOWN
} Sc2Kind;

/* -----------------------------------------------------------------------
 * Token struct
 * ----------------------------------------------------------------------- */
typedef struct {
    int    kind;    /* Sc2Kind */
    char  *text;    /* heap-allocated verbatim source text (caller frees via sc_token_buf_free) */
    int    line;    /* 1-based physical line */
} ScToken2;

/* -----------------------------------------------------------------------
 * Token buffer (dynamic array returned by snocone_lex2)
 * ----------------------------------------------------------------------- */
typedef struct {
    ScToken2 *tokens;
    int       count;
    int       capacity;
} ScTokenBuf;

/* -----------------------------------------------------------------------
 * Previous-token state for CONCAT emission
 * ----------------------------------------------------------------------- */
#define SC2_MAX_CALL_DEPTH 64

typedef enum {
    PT_NONE,              /* file start, after ; , ( [ { etc.               */
    PT_VALUE,             /* IDENT, INTEGER, REAL, STRING, KEYWORD_NAME,
                           * RPAREN_of_expr, RBRACKET, RPAREN_of_call       */
    PT_BINOP,             /* binary op just emitted                          */
    PT_UNARY_PREFIX,      /* unary prefix op just emitted                    */
    PT_OPEN,              /* ( or [ or { just emitted                        */
    PT_KW_BLOCK_OPENER,   /* if/while/for/switch/do/case/function keyword    */
    PT_KW_LOOPCTRL,       /* break/continue/goto/return/freturn/nreturn      */
    PT_LABEL_COLON_PENDING /* IDENT followed by : — label or goto-field     */
} PrevTokenClass;

typedef struct {
    PrevTokenClass  prev;
    int             whitespace_seen;      /* >0 if any whitespace since last token */
    int             paren_depth;          /* total ( [ { nesting */
    int             call_stack[SC2_MAX_CALL_DEPTH]; /* paren_depth at which each call-( opened */
    int             call_stack_top;       /* index into call_stack; -1 = empty */
    /* String literal accumulation */
    char            strlit_quote;
    int             strlit_start_line;
    char           *strlit_buf;
    int             strlit_len;
    int             strlit_cap;
    /* Pointer back to the token buffer (set by snocone_lex2 before scan) */
    ScTokenBuf     *buf;
    /* Flag: when set, the very next '(' token is the implicit call-open for
     * an IDENT_LPAREN_NOSP token just emitted.  Absorb it silently. */
    int             absorb_next_lparen;
} LexerState;

/* -----------------------------------------------------------------------
 * String literal helpers (called from Flex rules)
 * ----------------------------------------------------------------------- */
void  sc_strlit_begin(LexerState *st, char quote, int line);
void  sc_strlit_append_char(LexerState *st, char c);
void  sc_strlit_append(LexerState *st, char c);
char *sc_strlit_end(LexerState *st);

/* (Internal helpers push_call_depth, pop_call_depth, update_prev are
 *  defined as static functions inside snocone.lex.c — not exposed here.) */

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

/*
 * snocone_lex2(source)
 *   Tokenise a complete Snocone source string.
 *   Returns a heap-allocated ScTokenBuf terminated by SC2_EOF.
 *   Free with sc_token_buf_free().
 */
ScTokenBuf  snocone_lex2(const char *source);

/*
 * sc_token_buf_free(buf)
 *   Free all memory owned by the token buffer.
 */
void sc_token_buf_free(ScTokenBuf *buf);

/*
 * sc2_kind_name(kind)
 *   Return a static string name for debugging / test output.
 */
const char *sc2_kind_name(int kind);

#endif /* SNOCONE_LEX2_H */
