/*
 * icon_lex.c — Tiny-ICON lexer
 *
 * _POSIX_C_SOURCE for strdup
 *
 * Hand-rolled lexer for snobol4x Icon frontend.
 * NO auto-semicolon insertion (deliberate deviation from standard Icon).
 * Explicit ';' required everywhere.
 *
 * Token kinds defined in icon_lex.h.
 * Structural template: src/frontend/prolog/prolog_lex.c
 */

#define _POSIX_C_SOURCE 200809L
#include "icon_lex.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

static char lcur(const IcnLexer *lx) {
    return lx->src[lx->pos];
}
static char lpeek1(const IcnLexer *lx) {
    if (lx->pos + 1 >= lx->src_len) return '\0';
    return lx->src[lx->pos + 1];
}

static char ladvance(IcnLexer *lx) {
    char c = lx->src[lx->pos];
    if (c == '\n') { lx->line++; lx->col = 1; }
    else           { lx->col++; }
    if (lx->pos < lx->src_len) lx->pos++;
    return c;
}

/* Simple heap buffer append */
typedef struct { char *data; int len; int cap; } Buf;
static void buf_push(Buf *b, char c) {
    if (b->len + 2 > b->cap) {
        b->cap = b->cap ? b->cap * 2 : 32;
        b->data = realloc(b->data, (size_t)b->cap);
    }
    b->data[b->len++] = c;
    b->data[b->len]   = '\0';
}

static void skip_whitespace_and_comments(IcnLexer *lx) {
    for (;;) {
        /* Whitespace */
        while (lcur(lx) && isspace((unsigned char)lcur(lx)))
            ladvance(lx);
        /* # line comment (Icon uses # for comments) */
        if (lcur(lx) == '#') {
            while (lcur(lx) && lcur(lx) != '\n')
                ladvance(lx);
            continue;
        }
        break;
    }
}

static IcnToken make_tok(IcnTkKind kind, int line, int col) {
    IcnToken t;
    memset(&t, 0, sizeof(t));
    t.kind = kind;
    t.line = line;
    t.col  = col;
    return t;
}

static IcnToken make_error(IcnLexer *lx, int line, int col, const char *msg) {
    IcnToken t = make_tok(TK_ERROR, line, col);
    lx->had_error = 1;
    snprintf(lx->errmsg, sizeof(lx->errmsg), "lex error at %d:%d: %s", line, col, msg);
    return t;
}

/* =========================================================================
 * Escape character processing for strings/csets
 * ========================================================================= */
static int lex_escape(IcnLexer *lx, char *out) {
    char c = ladvance(lx); /* consume the char after backslash */
    switch (c) {
        case 'n':  *out = '\n'; return 1;
        case 't':  *out = '\t'; return 1;
        case 'r':  *out = '\r'; return 1;
        case 'b':  *out = '\b'; return 1;
        case 'f':  *out = '\f'; return 1;
        case '\\': *out = '\\'; return 1;
        case '"':  *out = '"';  return 1;
        case '\'': *out = '\''; return 1;
        case '0':  *out = '\0'; return 1;
        default:
            *out = c;
            return 1;
    }
}

/* =========================================================================
 * Number lexing
 * ========================================================================= */
static IcnToken lex_number(IcnLexer *lx, int line, int col) {
    Buf b = {0,0,0};

    /* Collect digits before possible decimal point */
    while (isdigit((unsigned char)lcur(lx)))
        buf_push(&b, ladvance(lx));

    /* Radix notation: 8r77, 16rff, etc. */
    if ((lcur(lx) == 'r' || lcur(lx) == 'R') && b.len > 0) {
        int radix = atoi(b.data);
        ladvance(lx); /* consume 'r' */
        free(b.data); b.data = NULL; b.len = 0; b.cap = 0;
        long val = 0;
        if (radix < 2 || radix > 36) {
            return make_error(lx, line, col, "invalid radix");
        }
        while (isalnum((unsigned char)lcur(lx))) {
            char c = ladvance(lx);
            int digit;
            if (c >= '0' && c <= '9') digit = c - '0';
            else if (c >= 'a' && c <= 'z') digit = c - 'a' + 10;
            else digit = c - 'A' + 10;
            if (digit >= radix) return make_error(lx, line, col, "digit out of range for radix");
            val = val * radix + digit;
        }
        IcnToken t = make_tok(TK_INT, line, col);
        t.val.ival = val;
        return t;
    }

    /* Floating point */
    if (lcur(lx) == '.' && isdigit((unsigned char)lpeek1(lx))) {
        buf_push(&b, ladvance(lx)); /* '.' */
        while (isdigit((unsigned char)lcur(lx)))
            buf_push(&b, ladvance(lx));
        /* exponent */
        if (lcur(lx) == 'e' || lcur(lx) == 'E') {
            buf_push(&b, ladvance(lx));
            if (lcur(lx) == '+' || lcur(lx) == '-')
                buf_push(&b, ladvance(lx));
            while (isdigit((unsigned char)lcur(lx)))
                buf_push(&b, ladvance(lx));
        }
        IcnToken t = make_tok(TK_REAL, line, col);
        t.val.fval = atof(b.data);
        free(b.data);
        return t;
    }

    /* Plain integer */
    IcnToken t = make_tok(TK_INT, line, col);
    t.val.ival = atol(b.data);
    free(b.data);
    return t;
}

/* =========================================================================
 * Keyword table
 * ========================================================================= */
typedef struct { const char *word; IcnTkKind kind; } KwEntry;
static const KwEntry keywords[] = {
    {"to",         TK_TO},
    {"by",         TK_BY},
    {"every",      TK_EVERY},
    {"do",         TK_DO},
    {"if",         TK_IF},
    {"then",       TK_THEN},
    {"else",       TK_ELSE},
    {"while",      TK_WHILE},
    {"until",      TK_UNTIL},
    {"repeat",     TK_REPEAT},
    {"return",     TK_RETURN},
    {"suspend",    TK_SUSPEND},
    {"fail",       TK_FAIL},
    {"break",      TK_BREAK},
    {"next",       TK_NEXT},
    {"not",        TK_NOT},
    {"procedure",  TK_PROCEDURE},
    {"end",        TK_END},
    {"global",     TK_GLOBAL},
    {"local",      TK_LOCAL},
    {"static",     TK_STATIC},
    {"record",     TK_RECORD},
    {"link",       TK_LINK},
    {"invocable",  TK_INVOCABLE},
    {"case",       TK_CASE},
    {"of",         TK_OF},
    {"default",    TK_DEFAULT},
    {"create",     TK_CREATE},
    {"initial",    TK_INITIAL},
    {NULL,         TK_EOF}
};

static IcnTkKind lookup_keyword(const char *word) {
    for (int i = 0; keywords[i].word; i++) {
        if (strcmp(keywords[i].word, word) == 0)
            return keywords[i].kind;
    }
    return TK_IDENT;
}

/* =========================================================================
 * Identifier / keyword lexing
 * ========================================================================= */
static IcnToken lex_ident(IcnLexer *lx, int line, int col) {
    Buf b = {0,0,0};
    while (isalnum((unsigned char)lcur(lx)) || lcur(lx) == '_')
        buf_push(&b, ladvance(lx));

    IcnTkKind kind = lookup_keyword(b.data);
    IcnToken t = make_tok(kind, line, col);
    if (kind == TK_IDENT) {
        t.val.sval.data = b.data;
        t.val.sval.len  = (size_t)b.len;
    } else {
        free(b.data);
    }
    return t;
}

/* =========================================================================
 * String literal: "..." with backslash escapes
 * ========================================================================= */
static IcnToken lex_string(IcnLexer *lx, int line, int col) {
    ladvance(lx); /* consume opening " */
    Buf b = {0,0,0};
    while (lcur(lx) && lcur(lx) != '"') {
        if (lcur(lx) == '\\') {
            ladvance(lx); /* consume backslash */
            char esc;
            lex_escape(lx, &esc);
            buf_push(&b, esc);
        } else {
            buf_push(&b, ladvance(lx));
        }
    }
    if (!lcur(lx))
        return make_error(lx, line, col, "unterminated string literal");
    ladvance(lx); /* consume closing " */

    IcnToken t = make_tok(TK_STRING, line, col);
    t.val.sval.data = b.data ? b.data : strdup("");
    t.val.sval.len  = (size_t)(b.len);
    return t;
}

/* =========================================================================
 * Cset literal: '...' with backslash escapes
 * ========================================================================= */
static IcnToken lex_cset(IcnLexer *lx, int line, int col) {
    ladvance(lx); /* consume opening ' */
    Buf b = {0,0,0};
    while (lcur(lx) && lcur(lx) != '\'') {
        if (lcur(lx) == '\\') {
            ladvance(lx);
            char esc;
            lex_escape(lx, &esc);
            buf_push(&b, esc);
        } else {
            buf_push(&b, ladvance(lx));
        }
    }
    if (!lcur(lx))
        return make_error(lx, line, col, "unterminated cset literal");
    ladvance(lx); /* consume closing ' */

    IcnToken t = make_tok(TK_CSET, line, col);
    t.val.sval.data = b.data ? b.data : strdup("");
    t.val.sval.len  = (size_t)(b.len);
    return t;
}

/* =========================================================================
 * Public API
 * ========================================================================= */

void icn_lex_init(IcnLexer *lx, const char *src) {
    memset(lx, 0, sizeof(*lx));
    lx->src     = src;
    lx->src_len = strlen(src);
    lx->pos     = 0;
    lx->line    = 1;
    lx->col     = 1;
}

IcnToken icn_lex_next(IcnLexer *lx) {
    skip_whitespace_and_comments(lx);

    int line = lx->line, col = lx->col;

    if (!lcur(lx) || lx->pos >= lx->src_len)
        return make_tok(TK_EOF, line, col);

    char c = lcur(lx);

    /* --- Numbers --- */
    if (isdigit((unsigned char)c))
        return lex_number(lx, line, col);

    /* --- Identifiers / keywords --- */
    if (isalpha((unsigned char)c) || c == '_')
        return lex_ident(lx, line, col);

    /* --- String literal --- */
    if (c == '"')
        return lex_string(lx, line, col);

    /* --- Cset literal --- */
    if (c == '\'')
        return lex_cset(lx, line, col);

    /* --- Multi-char operators & punctuation --- */
    ladvance(lx);
    char n = lcur(lx);

    switch (c) {
        case '+':
            if (n == ':' && lpeek1(lx) == '=') { ladvance(lx); ladvance(lx); return make_tok(TK_AUGPLUS,   line, col); }
            return make_tok(TK_PLUS, line, col);

        case '-':
            if (n == ':' && lpeek1(lx) == '=') { ladvance(lx); ladvance(lx); return make_tok(TK_AUGMINUS,  line, col); }
            return make_tok(TK_MINUS, line, col);

        case '*':
            if (n == ':' && lpeek1(lx) == '=') { ladvance(lx); ladvance(lx); return make_tok(TK_AUGSTAR,   line, col); }
            return make_tok(TK_STAR, line, col);

        case '/':
            if (n == ':' && lpeek1(lx) == '=') { ladvance(lx); ladvance(lx); return make_tok(TK_AUGSLASH,  line, col); }
            return make_tok(TK_SLASH, line, col);

        case '%':
            if (n == ':' && lpeek1(lx) == '=') { ladvance(lx); ladvance(lx); return make_tok(TK_AUGMOD,    line, col); }
            return make_tok(TK_MOD, line, col);

        case '^':
            return make_tok(TK_CARET, line, col);

        case '<':
            if (n == '<') {
                ladvance(lx);
                if (lcur(lx) == '=') { ladvance(lx); return make_tok(TK_SLE, line, col); }
                return make_tok(TK_SLT, line, col);
            }
            if (n == '=') { ladvance(lx); return make_tok(TK_LE, line, col); }
            if (n == '-') { ladvance(lx); return make_tok(TK_REVASSIGN, line, col); }
            return make_tok(TK_LT, line, col);

        case '>':
            if (n == '>') {
                ladvance(lx);
                if (lcur(lx) == '=') { ladvance(lx); return make_tok(TK_SGE, line, col); }
                return make_tok(TK_SGT, line, col);
            }
            if (n == '=') { ladvance(lx); return make_tok(TK_GE, line, col); }
            return make_tok(TK_GT, line, col);

        case '=':
            if (n == '=') { ladvance(lx); return make_tok(TK_SEQ, line, col); }
            return make_tok(TK_EQ, line, col);

        case '~':
            if (n == '=') {
                ladvance(lx);
                if (lcur(lx) == '=') { ladvance(lx); return make_tok(TK_SNE, line, col); }
                return make_tok(TK_NEQ, line, col);
            }
            return make_tok(TK_TILDE, line, col);

        case '|':
            if (n == '|') {
                ladvance(lx);
                if (lcur(lx) == '|') {
                    ladvance(lx); return make_tok(TK_LCONCAT, line, col);
                }
                if (lcur(lx) == ':' && lpeek1(lx) == '=') {
                    ladvance(lx); ladvance(lx); return make_tok(TK_AUGCONCAT, line, col);
                }
                return make_tok(TK_CONCAT, line, col);
            }
            return make_tok(TK_BAR, line, col);

        case ':':
            if (n == '=') {
                ladvance(lx);
                if (lcur(lx) == ':') { ladvance(lx); return make_tok(TK_SWAP, line, col); }
                return make_tok(TK_ASSIGN, line, col);
            }
            return make_tok(TK_COLON, line, col);

        case '&': return make_tok(TK_AND,       line, col);
        case '\\': return make_tok(TK_BACKSLASH, line, col);
        case '!':  return make_tok(TK_BANG,      line, col);
        case '?':  return make_tok(TK_QMARK,     line, col);
        case '@':  return make_tok(TK_AT,        line, col);
        case '.':  return make_tok(TK_DOT,       line, col);

        case '(':  return make_tok(TK_LPAREN,  line, col);
        case ')':  return make_tok(TK_RPAREN,  line, col);
        case '{':  return make_tok(TK_LBRACE,  line, col);
        case '}':  return make_tok(TK_RBRACE,  line, col);
        case '[':  return make_tok(TK_LBRACK,  line, col);
        case ']':  return make_tok(TK_RBRACK,  line, col);
        case ',':  return make_tok(TK_COMMA,   line, col);
        case ';':  return make_tok(TK_SEMICOL, line, col);

        default: {
            char msg[64];
            snprintf(msg, sizeof(msg), "unexpected character '%c' (0x%02x)", c, (unsigned char)c);
            return make_error(lx, line, col, msg);
        }
    }
}

IcnToken icn_lex_peek(IcnLexer *lx) {
    /* Save state, lex one token, restore state */
    size_t  saved_pos  = lx->pos;
    int     saved_line = lx->line;
    int     saved_col  = lx->col;

    IcnToken t = icn_lex_next(lx);

    lx->pos  = saved_pos;
    lx->line = saved_line;
    lx->col  = saved_col;
    return t;
}

/* =========================================================================
 * Token name table (for diagnostics / tests)
 * ========================================================================= */
const char *icn_tk_name(IcnTkKind kind) {
    switch (kind) {
        case TK_EOF:       return "EOF";
        case TK_ERROR:     return "ERROR";
        case TK_INT:       return "INT";
        case TK_REAL:      return "REAL";
        case TK_STRING:    return "STRING";
        case TK_CSET:      return "CSET";
        case TK_IDENT:     return "IDENT";
        case TK_PLUS:      return "+";
        case TK_MINUS:     return "-";
        case TK_STAR:      return "*";
        case TK_SLASH:     return "/";
        case TK_MOD:       return "%";
        case TK_CARET:     return "^";
        case TK_LT:        return "<";
        case TK_LE:        return "<=";
        case TK_GT:        return ">";
        case TK_GE:        return ">=";
        case TK_EQ:        return "=";
        case TK_NEQ:       return "~=";
        case TK_SLT:       return "<<";
        case TK_SLE:       return "<<=";
        case TK_SGT:       return ">>";
        case TK_SGE:       return ">>=";
        case TK_SEQ:       return "==";
        case TK_SNE:       return "~==";
        case TK_CONCAT:    return "||";
        case TK_LCONCAT:   return "|||";
        case TK_ASSIGN:    return ":=";
        case TK_SWAP:      return ":=:";
        case TK_REVASSIGN: return "<-";
        case TK_AUGPLUS:   return "+:=";
        case TK_AUGMINUS:  return "-:=";
        case TK_AUGSTAR:   return "*:=";
        case TK_AUGSLASH:  return "/:=";
        case TK_AUGMOD:    return "%:=";
        case TK_AUGCONCAT: return "||:=";
        case TK_AND:       return "&";
        case TK_BAR:       return "|";
        case TK_BACKSLASH: return "\\";
        case TK_BANG:      return "!";
        case TK_QMARK:     return "?";
        case TK_AT:        return "@";
        case TK_TILDE:     return "~";
        case TK_DOT:       return ".";
        case TK_TO:        return "to";
        case TK_BY:        return "by";
        case TK_EVERY:     return "every";
        case TK_DO:        return "do";
        case TK_IF:        return "if";
        case TK_THEN:      return "then";
        case TK_ELSE:      return "else";
        case TK_WHILE:     return "while";
        case TK_UNTIL:     return "until";
        case TK_REPEAT:    return "repeat";
        case TK_RETURN:    return "return";
        case TK_SUSPEND:   return "suspend";
        case TK_FAIL:      return "fail";
        case TK_BREAK:     return "break";
        case TK_NEXT:      return "next";
        case TK_NOT:       return "not";
        case TK_PROCEDURE: return "procedure";
        case TK_END:       return "end";
        case TK_GLOBAL:    return "global";
        case TK_LOCAL:     return "local";
        case TK_STATIC:    return "static";
        case TK_RECORD:    return "record";
        case TK_LINK:      return "link";
        case TK_INVOCABLE: return "invocable";
        case TK_CASE:      return "case";
        case TK_OF:        return "of";
        case TK_DEFAULT:   return "default";
        case TK_CREATE:    return "create";
        case TK_INITIAL:   return "initial";
        case TK_LPAREN:    return "(";
        case TK_RPAREN:    return ")";
        case TK_LBRACE:    return "{";
        case TK_RBRACE:    return "}";
        case TK_LBRACK:    return "[";
        case TK_RBRACK:    return "]";
        case TK_COMMA:     return ",";
        case TK_SEMICOL:   return ";";
        case TK_COLON:     return ":";
        case TK_COUNT:     return "<COUNT>";
        default:           return "<unknown>";
    }
}
