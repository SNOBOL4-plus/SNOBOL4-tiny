/*
 * snocone_lex2.h — Snocone lexer public API (LS-3)
 *
 * Token names align with snobol4.tab.h conventions: same name everywhere
 * for any concept equivalence (Lon session #9 directive).
 */
#ifndef SNOCONE_LEX2_H
#define SNOCONE_LEX2_H

#include <stddef.h>

typedef enum {
    /* Atoms — match snobol4.tab.h */
    T_INT = 1,
    T_REAL,
    T_STR,
    T_IDENT,
    T_FUNCTION,            /* ident immediately followed by '(' */
    T_KEYWORD,             /* &IDENT */

    /* Synthetic */
    T_CONCAT,              /* whitespace between two atoms */

    /* Binary operators (mirror snobol4.l names) */
    T_ASSIGNMENT,          /* =      pri 0 */
    T_MATCH,               /* ?      pri 1 */
    T_ALTERNATION,         /* |      pri 3 */
    /* T_CONCAT — pri 4 (above) */
    T_EQ, T_NE, T_LT, T_GT, T_LE, T_GE,    /* numeric compare pri 6 */
    T_LEQ, T_LNE, T_LLT, T_LGT, T_LLE, T_LGE, /* string compare :==: etc. pri 6 */
    T_IDENT_OP,            /* ::    pri 6 IDENT() */
    T_DIFFER,              /* :!:   pri 6 DIFFER() */
    T_ADDITION,            /* +     pri 6 */
    T_SUBTRACTION,         /* -     pri 6 */
    T_DIVISION,            /* /     pri 8 */
    T_MULTIPLICATION,      /* *     pri 9 */
    T_EXPONENTIATION,      /* ^ ** ! pri 11 right */
    T_IMMEDIATE_ASSIGN,    /* $     pri 12 (binary) */
    T_COND_ASSIGN,         /* .     pri 12 (binary) */
    T_AMPERSAND,           /* &     OPSYN slot pri 2 */
    T_AT_SIGN,             /* @     OPSYN slot pri 5 */
    T_POUND,               /* #     OPSYN slot pri 7 */
    T_PERCENT,             /* %     OPSYN slot pri 10 */
    T_TILDE,               /* ~     OPSYN slot pri 13 */

    /* Compound-assign (Snocone extension; match snobol4 if added there) */
    T_PLUS_ASSIGN, T_MINUS_ASSIGN, T_STAR_ASSIGN, T_SLASH_ASSIGN, T_CARET_ASSIGN,

    /* Unary operators — match snobol4.l T_UN_* names */
    T_UN_PLUS, T_UN_MINUS, T_UN_ASTERISK, T_UN_SLASH, T_UN_PERCENT,
    T_UN_AT_SIGN, T_UN_TILDE, T_UN_DOLLAR_SIGN, T_UN_PERIOD, T_UN_POUND,
    T_UN_VERTICAL_BAR, T_UN_EQUAL, T_UN_QUESTION_MARK, T_UN_AMPERSAND,

    /* Punctuation — match snobol4.l names */
    T_LPAREN, T_RPAREN,
    T_LBRACK, T_RBRACK,    /* [ ]   — name matches snobol4 (LBRACK not LBRACKET) */
    T_LBRACE, T_RBRACE,    /* { }   — Snocone-only */
    T_COMMA,
    T_SEMICOLON,
    T_COLON,

    /* Keywords (Snocone-only) */
    T_KW_IF, T_KW_ELSE, T_KW_WHILE, T_KW_DO, T_KW_UNTIL, T_KW_FOR,
    T_KW_SWITCH, T_KW_CASE, T_KW_DEFAULT,
    T_KW_BREAK, T_KW_CONTINUE, T_KW_GOTO,
    T_KW_FUNCTION, T_KW_RETURN, T_KW_FRETURN, T_KW_NRETURN,
    T_KW_STRUCT,

    /* Sentinel */
    T_EOF,
    T_UNKNOWN
} ScKind;

typedef struct {
    int    kind;
    char  *text;
    int    line;
} ScToken2;

typedef struct {
    ScToken2 *tokens;
    int       count;
    int       capacity;
} ScTokenBuf;

typedef struct LexerState {
    ScTokenBuf *buf;
} LexerState;

/* Public API */
ScTokenBuf  snocone_lex2(const char *source);
void        sc_token_buf_free(ScTokenBuf *buf);
const char *sc2_kind_name(int kind);
int         sc_kind_is_value(int kind);

#endif
