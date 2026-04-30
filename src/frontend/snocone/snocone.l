/*
 * snocone.l — Snocone language Flex lexer  (GOAL-SNOCONE-LANG-SPACE LS-3.a)
 *
 * Andrew Koenig .sc operator set minus &&, ||, % plus CONCAT synthetic.
 * Commit identity: LCherryholmes / lcherryh@yahoo.com
 */

%option noyywrap nounput noinput
%option reentrant
%option extra-type="LexerState *"
%option prefix="sc_"
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snocone_lex2.h"

static int  classify_keyword(const char *s);
static void push_token_impl(ScTokenBuf *buf, int kind, const char *text, int line);
static int  can_start_expr_kind(int kind);
static void update_prev(LexerState *st, int kind);
static void maybe_emit_concat(LexerState *st, int next_can_start, int lineno);
static void push_call_depth(LexerState *st);
static int  pop_call_depth(LexerState *st);

#define TOK(kind) \
    do { \
        maybe_emit_concat(yyextra, can_start_expr_kind(kind), yylineno); \
        push_token_impl(yyextra->buf, (kind), yytext, yylineno); \
        update_prev(yyextra, (kind)); \
        yyextra->whitespace_seen = 0; \
    } while(0)

%}

DIGIT    [0-9]
LETTER   [A-Za-z_]
ALNUM    [A-Za-z0-9_]
WS       [ \t\r\f]
NL       \n
EXP      [eEdD][+-]?{DIGIT}+

%x IN_SQ IN_DQ IN_BLOCK_COMMENT

%%

{WS}+                   { yyextra->whitespace_seen = 1; }
{NL}                    { yyextra->whitespace_seen = 1; }
^[+.][^\n]*{NL}         { yyextra->whitespace_seen = 1; }
"//"[^\n]*              { yyextra->whitespace_seen = 1; }
"/*"                    { BEGIN(IN_BLOCK_COMMENT); yyextra->whitespace_seen = 1; }
<IN_BLOCK_COMMENT>"*/"  { BEGIN(INITIAL); }
<IN_BLOCK_COMMENT>{NL}  { /* absorbed */ }
<IN_BLOCK_COMMENT>.     { /* absorbed */ }

\'          { BEGIN(IN_SQ); sc_strlit_begin(yyextra, '\'', yylineno); }
<IN_SQ>\'\'  { sc_strlit_append_char(yyextra, '\''); }
<IN_SQ>\'   {
    BEGIN(INITIAL);
    int ln = yyextra->strlit_start_line;
    char *s = sc_strlit_end(yyextra);
    maybe_emit_concat(yyextra, 1, ln);
    push_token_impl(yyextra->buf, SC2_STRING, s, ln);
    free(s);
    update_prev(yyextra, SC2_STRING);
    yyextra->whitespace_seen = 0;
}
<IN_SQ>{NL}  { sc_strlit_append_char(yyextra, '\n'); }
<IN_SQ>.     { sc_strlit_append_char(yyextra, yytext[0]); }

\"          { BEGIN(IN_DQ); sc_strlit_begin(yyextra, '"', yylineno); }
<IN_DQ>\"\"  { sc_strlit_append_char(yyextra, '"'); }
<IN_DQ>\"   {
    BEGIN(INITIAL);
    int ln = yyextra->strlit_start_line;
    char *s = sc_strlit_end(yyextra);
    maybe_emit_concat(yyextra, 1, ln);
    push_token_impl(yyextra->buf, SC2_STRING, s, ln);
    free(s);
    update_prev(yyextra, SC2_STRING);
    yyextra->whitespace_seen = 0;
}
<IN_DQ>{NL}  { sc_strlit_append_char(yyextra, '\n'); }
<IN_DQ>.     { sc_strlit_append_char(yyextra, yytext[0]); }

{DIGIT}+\.{DIGIT}*({EXP})?    { TOK(SC2_REAL); }
{DIGIT}+{EXP}                  { TOK(SC2_REAL); }
\.{DIGIT}+({EXP})?             { TOK(SC2_REAL); }
{DIGIT}+                       { TOK(SC2_INTEGER); }

"&"{LETTER}{ALNUM}*            { TOK(SC2_KEYWORD_NAME); }

{LETTER}{ALNUM}*/"("    {
    char *name = strdup(yytext);

    maybe_emit_concat(yyextra, 1, yylineno);
    push_token_impl(yyextra->buf, SC2_IDENT_LPAREN_NOSP, name, yylineno);
    free(name);
    push_call_depth(yyextra);
    update_prev(yyextra, SC2_IDENT_LPAREN_NOSP);
    yyextra->whitespace_seen = 0;
    yyextra->absorb_next_lparen = 1;  /* next '(' is the call-open, absorb silently */
}
{LETTER}{ALNUM}*       { TOK(classify_keyword(yytext)); }

":!=:"  { TOK(SC2_STR_NE); }
":<=:"  { TOK(SC2_STR_LE); }
":>=:"  { TOK(SC2_STR_GE); }
":==:"  { TOK(SC2_STR_EQ); }
":!:"   { TOK(SC2_STR_DIFFER); }
":<:"   { TOK(SC2_STR_LT); }
":>:"   { TOK(SC2_STR_GT); }
"+="    { TOK(SC2_PLUS_ASSIGN); }
"-="    { TOK(SC2_MINUS_ASSIGN); }
"*="    { TOK(SC2_STAR_ASSIGN); }
"/="    { TOK(SC2_SLASH_ASSIGN); }
"^="    { TOK(SC2_CARET_ASSIGN); }
"::"    { TOK(SC2_STR_IDENT); }
"=="    { TOK(SC2_EQ); }
"!="    { TOK(SC2_NE); }
"<="    { TOK(SC2_LE); }
">="    { TOK(SC2_GE); }
"**"    { TOK(SC2_CARET); }
"="     { TOK(SC2_ASSIGN); }
"?"     { TOK(SC2_QUESTION); }
"|"     { TOK(SC2_PIPE); }
"+"     { TOK(SC2_PLUS); }
"-"     { TOK(SC2_MINUS); }
"/"     { TOK(SC2_SLASH); }
"*"     { TOK(SC2_STAR); }
"^"     { TOK(SC2_CARET); }
"."     { TOK(SC2_PERIOD); }
"$"     { TOK(SC2_DOLLAR); }
"&"     { TOK(SC2_AMPERSAND); }
"@"     { TOK(SC2_AT); }
"~"     { TOK(SC2_TILDE); }
"<"     { TOK(SC2_LT); }
">"     { TOK(SC2_GT); }
"%"     { TOK(SC2_PERCENT); }
"!"     { TOK(SC2_BANG_EXPONENT); }

"("     {
    if (yyextra->absorb_next_lparen) {
        /* This '(' is the call-open paren implied by IDENT_LPAREN_NOSP.
         * Do NOT emit a LPAREN token — the grammar knows from the
         * IDENT_LPAREN_NOSP token that a '(' was consumed.
         * DO increment paren_depth so RPAREN tracking works. */
        yyextra->absorb_next_lparen = 0;
        yyextra->paren_depth++;
        /* prev stays PT_OPEN (set by update_prev for IDENT_LPAREN_NOSP) */
    } else {
        maybe_emit_concat(yyextra, 1, yylineno);
        push_token_impl(yyextra->buf, SC2_LPAREN, yytext, yylineno);
        yyextra->paren_depth++;
        update_prev(yyextra, SC2_LPAREN);
        yyextra->whitespace_seen = 0;
    }
}
")"     {
    int is_call = pop_call_depth(yyextra);
    (void)is_call;
    maybe_emit_concat(yyextra, 0, yylineno);
    push_token_impl(yyextra->buf, SC2_RPAREN, yytext, yylineno);
    if (yyextra->paren_depth > 0) yyextra->paren_depth--;
    update_prev(yyextra, SC2_RPAREN);
    yyextra->whitespace_seen = 0;
}
"{"     { TOK(SC2_LBRACE); }
"}"     { TOK(SC2_RBRACE); }
"["     { TOK(SC2_LBRACKET); }
"]"     { TOK(SC2_RBRACKET); }
","     { TOK(SC2_COMMA); }
";"     { TOK(SC2_SEMICOLON); }
":"     { TOK(SC2_COLON); }

<<EOF>> {
    push_token_impl(yyextra->buf, SC2_EOF, "", yylineno);
    yyterminate();
}

.       {
    push_token_impl(yyextra->buf, SC2_UNKNOWN, yytext, yylineno);
    yyextra->whitespace_seen = 0;
}

%%

typedef struct { const char *word; int kind; } KwEntry;
static const KwEntry KW_TABLE[] = {
    { "if",       SC2_KW_IF       },
    { "else",     SC2_KW_ELSE     },
    { "while",    SC2_KW_WHILE    },
    { "do",       SC2_KW_DO       },
    { "until",    SC2_KW_UNTIL    },
    { "for",      SC2_KW_FOR      },
    { "switch",   SC2_KW_SWITCH   },
    { "case",     SC2_KW_CASE     },
    { "default",  SC2_KW_DEFAULT  },
    { "break",    SC2_KW_BREAK    },
    { "continue", SC2_KW_CONTINUE },
    { "goto",     SC2_KW_GOTO     },
    { "function", SC2_KW_FUNCTION },
    { "return",   SC2_KW_RETURN   },
    { "freturn",  SC2_KW_FRETURN  },
    { "nreturn",  SC2_KW_NRETURN  },
    { "struct",   SC2_KW_STRUCT   },
    { NULL,       SC2_IDENT       }
};

static int classify_keyword(const char *s) {
    for (const KwEntry *e = KW_TABLE; e->word; e++)
        if (strcmp(s, e->word) == 0) return e->kind;
    return SC2_IDENT;
}

static int can_start_expr_kind(int kind) {
    /* Only tokens that are UNAMBIGUOUSLY the start of a new operand trigger
     * CONCAT emission.  Tokens that double as binary operators (?, &, +, -, *,
     * ., $) do NOT trigger CONCAT — in a post-value position they are binary.
     * The grammar handles precedence from there. */
    switch (kind) {
    case SC2_IDENT:
    case SC2_IDENT_LPAREN_NOSP:
    case SC2_INTEGER:
    case SC2_REAL:
    case SC2_STRING:
    case SC2_KEYWORD_NAME:
    case SC2_LPAREN:
    case SC2_LBRACKET:
    /* Unambiguously unary — have no binary role in the operator table */
    case SC2_TILDE:
    case SC2_AT:
        return 1;
    default:
        return 0;
    }
}

static void update_prev(LexerState *st, int kind) {
    switch (kind) {
    case SC2_IDENT: case SC2_INTEGER: case SC2_REAL: case SC2_STRING:
    case SC2_KEYWORD_NAME: case SC2_RPAREN: case SC2_RBRACKET:
        st->prev = PT_VALUE; break;
    case SC2_IDENT_LPAREN_NOSP:
        st->prev = PT_OPEN; break;
    case SC2_LPAREN: case SC2_LBRACE: case SC2_LBRACKET:
        st->prev = PT_OPEN; break;
    case SC2_COMMA: case SC2_SEMICOLON: case SC2_COLON: case SC2_RBRACE:
        st->prev = PT_NONE; break;
    case SC2_AMPERSAND: case SC2_AT: case SC2_TILDE:
        st->prev = PT_UNARY_PREFIX; break;
    case SC2_KW_IF: case SC2_KW_WHILE: case SC2_KW_FOR: case SC2_KW_DO:
    case SC2_KW_UNTIL: case SC2_KW_SWITCH: case SC2_KW_CASE:
    case SC2_KW_DEFAULT: case SC2_KW_FUNCTION: case SC2_KW_ELSE:
    case SC2_KW_STRUCT:
        st->prev = PT_KW_BLOCK_OPENER; break;
    case SC2_KW_BREAK: case SC2_KW_CONTINUE: case SC2_KW_GOTO:
    case SC2_KW_RETURN: case SC2_KW_FRETURN: case SC2_KW_NRETURN:
        st->prev = PT_KW_LOOPCTRL; break;
    default:
        st->prev = PT_BINOP; break;
    }
}

static void maybe_emit_concat(LexerState *st, int next_can_start, int lineno) {
    if (st->prev == PT_VALUE && next_can_start && st->whitespace_seen)
        push_token_impl(st->buf, SC2_CONCAT, "", lineno);
}

static void push_call_depth(LexerState *st) {
    /* The '(' is NOT yet consumed (trailing context) so paren_depth has not
     * been incremented yet.  Record paren_depth+1 as the target depth — that
     * is the depth the call's RPAREN will see when it arrives. */
    int target = st->paren_depth + 1;
    if (st->call_stack_top < SC2_MAX_CALL_DEPTH - 1)
        st->call_stack[++st->call_stack_top] = target;
}

static int pop_call_depth(LexerState *st) {
    if (st->call_stack_top >= 0
            && st->call_stack[st->call_stack_top] == st->paren_depth) {
        st->call_stack_top--;
        return 1;
    }
    return 0;
}

static void push_token_impl(ScTokenBuf *buf, int kind, const char *text, int line) {
    if (buf->count >= buf->capacity) {
        buf->capacity = buf->capacity ? buf->capacity * 2 : 64;
        buf->tokens = realloc(buf->tokens, buf->capacity * sizeof(ScToken2));
    }
    ScToken2 *t = &buf->tokens[buf->count++];
    t->kind = kind;
    t->text = strdup(text);
    t->line = line;
}

void sc_strlit_begin(LexerState *st, char quote, int line) {
    st->strlit_quote = quote;
    st->strlit_start_line = line;
    st->strlit_len = 0;
    st->strlit_cap = 64;
    st->strlit_buf = malloc(st->strlit_cap);
    st->strlit_buf[0] = '\0';
}

void sc_strlit_append_char(LexerState *st, char c) {
    if (st->strlit_len + 2 >= st->strlit_cap) {
        st->strlit_cap *= 2;
        st->strlit_buf = realloc(st->strlit_buf, st->strlit_cap);
    }
    st->strlit_buf[st->strlit_len++] = c;
    st->strlit_buf[st->strlit_len] = '\0';
}

void sc_strlit_append(LexerState *st, char c) { sc_strlit_append_char(st, c); }

char *sc_strlit_end(LexerState *st) { return strdup(st->strlit_buf); }

ScTokenBuf snocone_lex2(const char *source) {
    LexerState st;
    memset(&st, 0, sizeof(st));
    st.prev = PT_NONE;
    st.call_stack_top = -1;

    ScTokenBuf buf;
    memset(&buf, 0, sizeof(buf));
    st.buf = &buf;

    yyscan_t scanner;
    sc_lex_init_extra(&st, &scanner);
    YY_BUFFER_STATE ybuf = sc__scan_string(source, scanner);
    sc_lex(scanner);
    sc__delete_buffer(ybuf, scanner);
    sc_lex_destroy(scanner);

    if (st.strlit_buf) free(st.strlit_buf);
    return buf;
}

void sc_token_buf_free(ScTokenBuf *buf) {
    for (int i = 0; i < buf->count; i++) free(buf->tokens[i].text);
    free(buf->tokens);
    buf->tokens = NULL; buf->count = 0; buf->capacity = 0;
}

const char *sc2_kind_name(int kind) {
    switch (kind) {
    case SC2_INTEGER:           return "INTEGER";
    case SC2_REAL:              return "REAL";
    case SC2_STRING:            return "STRING";
    case SC2_IDENT:             return "IDENT";
    case SC2_IDENT_LPAREN_NOSP: return "IDENT_LPAREN_NOSP";
    case SC2_KEYWORD_NAME:      return "KEYWORD_NAME";
    case SC2_CONCAT:            return "CONCAT";
    case SC2_ASSIGN:            return "ASSIGN";
    case SC2_QUESTION:          return "QUESTION";
    case SC2_PIPE:              return "PIPE";
    case SC2_EQ:                return "EQ";
    case SC2_NE:                return "NE";
    case SC2_LT:                return "LT";
    case SC2_GT:                return "GT";
    case SC2_LE:                return "LE";
    case SC2_GE:                return "GE";
    case SC2_STR_IDENT:         return "STR_IDENT";
    case SC2_STR_DIFFER:        return "STR_DIFFER";
    case SC2_STR_LT:            return "STR_LT";
    case SC2_STR_GT:            return "STR_GT";
    case SC2_STR_LE:            return "STR_LE";
    case SC2_STR_GE:            return "STR_GE";
    case SC2_STR_EQ:            return "STR_EQ";
    case SC2_STR_NE:            return "STR_NE";
    case SC2_PLUS:              return "PLUS";
    case SC2_MINUS:             return "MINUS";
    case SC2_SLASH:             return "SLASH";
    case SC2_STAR:              return "STAR";
    case SC2_PERCENT:           return "PERCENT";
    case SC2_CARET:             return "CARET";
    case SC2_BANG_EXPONENT:     return "BANG_EXPONENT";
    case SC2_PERIOD:            return "PERIOD";
    case SC2_DOLLAR:            return "DOLLAR";
    case SC2_AT:                return "AT";
    case SC2_TILDE:             return "TILDE";
    case SC2_AMPERSAND:         return "AMPERSAND";
    case SC2_PLUS_ASSIGN:       return "PLUS_ASSIGN";
    case SC2_MINUS_ASSIGN:      return "MINUS_ASSIGN";
    case SC2_STAR_ASSIGN:       return "STAR_ASSIGN";
    case SC2_SLASH_ASSIGN:      return "SLASH_ASSIGN";
    case SC2_CARET_ASSIGN:      return "CARET_ASSIGN";
    case SC2_LPAREN:            return "LPAREN";
    case SC2_RPAREN:            return "RPAREN";
    case SC2_LBRACE:            return "LBRACE";
    case SC2_RBRACE:            return "RBRACE";
    case SC2_LBRACKET:          return "LBRACKET";
    case SC2_RBRACKET:          return "RBRACKET";
    case SC2_COMMA:             return "COMMA";
    case SC2_SEMICOLON:         return "SEMICOLON";
    case SC2_COLON:             return "COLON";
    case SC2_KW_IF:             return "KW_IF";
    case SC2_KW_ELSE:           return "KW_ELSE";
    case SC2_KW_WHILE:          return "KW_WHILE";
    case SC2_KW_DO:             return "KW_DO";
    case SC2_KW_UNTIL:          return "KW_UNTIL";
    case SC2_KW_FOR:            return "KW_FOR";
    case SC2_KW_SWITCH:         return "KW_SWITCH";
    case SC2_KW_CASE:           return "KW_CASE";
    case SC2_KW_DEFAULT:        return "KW_DEFAULT";
    case SC2_KW_BREAK:          return "KW_BREAK";
    case SC2_KW_CONTINUE:       return "KW_CONTINUE";
    case SC2_KW_GOTO:           return "KW_GOTO";
    case SC2_KW_FUNCTION:       return "KW_FUNCTION";
    case SC2_KW_RETURN:         return "KW_RETURN";
    case SC2_KW_FRETURN:        return "KW_FRETURN";
    case SC2_KW_NRETURN:        return "KW_NRETURN";
    case SC2_KW_STRUCT:         return "KW_STRUCT";
    case SC2_EOF:               return "EOF";
    case SC2_UNKNOWN:           return "UNKNOWN";
    default:                    return "???";
    }
}
