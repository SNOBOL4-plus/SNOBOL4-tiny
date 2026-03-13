#ifndef LEX_H
#define LEX_H

/* Token kinds */
typedef enum {
    T_IDENT, T_INT, T_REAL, T_STR, T_KEYWORD,
    T_WS,
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_PCT,
    T_CARET, T_BANG, T_STARSTAR,
    T_AMP, T_AT, T_TILDE, T_DOLLAR, T_DOT,
    T_HASH, T_PIPE, T_EQ, T_QMARK,
    T_COMMA, T_LPAREN, T_RPAREN,
    T_LBRACKET, T_RBRACKET,
    T_LANGLE, T_RANGLE,
    T_COLON, T_SGOTO, T_FGOTO,
    T_END,
    T_EOF, T_ERR
} TokKind;

typedef struct {
    TokKind     kind;
    const char *sval;
    long        ival;
    double      dval;
    int         lineno;
} Token;

/* Lexer — struct exposed so parse.c can inject T_WS via peek slot */
typedef struct Lex {
    const char *src;
    int         pos;
    int         len;
    int         lineno;
    Token       peek;
    int         peeked;
} Lex;

void  lex_open_str(Lex *lx, const char *s, int len, int lineno);
Token lex_next(Lex *lx);
Token lex_peek(Lex *lx);
int   lex_at_end(Lex *lx);

/* SnoLine — split source line (lex.c produces, parse.c consumes) */
typedef struct SnoLine {
    char *label;
    char *body;
    char *goto_str;
    int   lineno;
    int   is_end;
} SnoLine;

typedef struct {
    SnoLine *a;
    int      n, cap;
} LineArray;

#endif /* LEX_H */
