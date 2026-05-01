/*
 * snocone_fsm.c -- Snocone threaded-code FSM lexer
 *                  GOAL-SNOCONE-LANG-SPACE LS-3.f.1, session 2026-04-30
 *
 * Replaces snocone.l / snocone.lex.c (flex 2.6.4 hangs on the {W}OP{W}
 * envelopes when comments are folded into {W} -- empirically verified).
 * Replaces also the two-pass hand-written snocone_lex.c per Lon
 * directive session 2026-04-30 #1: "use clean emission technique
 * just like SNOBOL4, use one pass when ever possible."
 *
 * THE FSM IS THE CODE
 * -------------------
 * sc_lex_next(ctx) returns one token kind per call.  The function
 * body is a graph of labelled blocks linked by goto statements.
 * Each line is exactly:
 *
 *     Label:   [if (cond)]   action;          goto NEXT;
 *
 * No switch, for, while, or do/until appears anywhere in the FSM
 * body.  Loops are formed by backward-pointing gotos -- they are
 * visible as cycles in the label graph, not hidden in language
 * constructs.  Comments-as-whitespace falls out for free: S_LCOMMENT
 * and S_BCOMMENT both transition back into S_WS on completion.
 *
 * STATE BETWEEN CALLS (in LexCtx):
 *   p           cursor into the source buffer (advances per call)
 *   line        1-based line number
 *   last_kind   kind of the most recently emitted token; consulted
 *               by the next call for CONCAT-trigger and bin/unary
 *               disambiguation.  Initialised to 0.
 *   text        payload buffer for the most recent value token.
 *   strbuf      intermediate buffer for string-literal accumulation.
 *   strpos      cursor into strbuf.
 *
 * PER-CALL LOCAL STATE:
 *   had_ws      set to 1 by the leading whitespace loop if any
 *               whitespace or comment was consumed before the token.
 *   tok_start   pointer to the first byte of the current token's
 *               payload (used by EMIT_V).
 *   last_value  cache of sc_kind_is_value(ctx->last_kind), used for
 *               CONCAT trigger and the {W}OP{W} bin-vs-unary test.
 *
 * THE TWO RULES, SAID PLAINLY
 * ---------------------------
 *   1. A binary operator requires whitespace on BOTH sides.  If
 *      either side is missing, the operator is unary.  Special
 *      case: '=' accepts left-only '{W}=' too, for the DYN-63
 *      end-of-line idiom 'x ='.
 *   2. T_CONCAT is emitted between two value-yielding tokens that
 *      have whitespace between them and no operator.
 *
 * Token kinds (T_*) match snobol4.tab.h -- same name everywhere
 * for any concept equivalence (RULES.md / Lon's session-#9).
 *
 * Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snocone_fsm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int is_alpha(int c)        { return ((c | 32) >= 'a' && (c | 32) <= 'z') || c == '_'; }
static inline int is_digit(int c)        { return c >= '0' && c <= '9'; }
static inline int is_idcont(int c)       { return is_alpha(c) || is_digit(c); }
/* is_value_starter -- a char that begins a fresh value token, used
 * by the CONCAT-trigger check in S_DISPATCH after a whitespace gap.
 *
 * Unary-prefix operator chars (+, -, *, &, etc.) are NOT here.
 * Each S_OP_* label decides binary-vs-unary based on right-ws, and
 * when it picks unary AND there was a leading-ws value-token, that
 * label itself emits CONCAT (cursor stays put for the next call).
 *
 * '{' is not here: '{' opens a block, no CONCAT before block.
 * '[' is not here: '[' is subscript only, regardless of whitespace. */
static inline int is_value_starter(int c) {
    return is_alpha(c) || is_digit(c) || c == '\'' || c == '"' ||
           c == '(';
}
/* is_rws -- "is right-side whitespace": the char `c` (peeked AFTER an
 * operator candidate) counts as the right-ws of the {W}OP{W} envelope
 * if it is a literal whitespace character, end-of-input, or the start
 * of a comment (// or slash-star).  The two-char comment lookahead
 * is folded in via the predicate `is_rws_at(p, n)` which knows the
 * full source.
 *
 * The dual-role unary/binary operators (& | ? $ . + - * / ^ ! @ # % ~)
 * are binary only when {W}OP{W} -- both sides whitespace.  Without
 * right-ws they are unary.  This is the SPITBOL/SNOBOL4 rule and is
 * what makes `x && y` lex as IDENT CONCAT UN_AMP UN_AMP IDENT (the
 * first '&' has no right-ws because '&' follows it, so it's unary).
 */
static inline int is_rws_char(int c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\f' ||
           c == '\n' || c == '\0';
}
static inline int is_rws_at(const char *p, int n) {
    int c0 = (unsigned char)p[n];
    if (is_rws_char(c0)) return 1;
    if (c0 == '/') {
        int c1 = (unsigned char)p[n + 1];
        if (c1 == '/' || c1 == '*') return 1;
    }
    return 0;
}
/* ---------------------------------------------------------------- */
/* sc_kind_is_value -- table-driven; no branches at call site.      */
/* Filled at first call (idempotent).                               */
/* ---------------------------------------------------------------- */
static signed char sc_value_table[256];
static int         sc_value_table_built = 0;
static void sc_value_table_build(void) {
    sc_value_table[T_IDENT]    = 1;
    sc_value_table[T_FUNCTION] = 1;
    sc_value_table[T_INT]      = 1;
    sc_value_table[T_REAL]     = 1;
    sc_value_table[T_STR]      = 1;
    sc_value_table[T_KEYWORD]  = 1;
    sc_value_table[T_RPAREN]   = 1;
    sc_value_table[T_RBRACK]   = 1;
    sc_value_table_built       = 1;
}
int sc_kind_is_value(int kind) {
    if (!sc_value_table_built) sc_value_table_build();
    return (kind >= 0 && kind < 256) ? sc_value_table[kind] : 0;
}
/* ---------------------------------------------------------------- */
/* Keyword classifier -- tail-recursive table walk.                 */
/* GCC -O2 turns the recursion into a loop; no source-level loop.   */
/* ---------------------------------------------------------------- */
typedef struct { const char *word; int kind; } KwEntry;
static const KwEntry KW_TABLE[] = {
    { "if",       T_KW_IF       },
    { "else",     T_KW_ELSE     },
    { "while",    T_KW_WHILE    },
    { "do",       T_KW_DO       },
    { "until",    T_KW_UNTIL    },
    { "for",      T_KW_FOR      },
    { "switch",   T_KW_SWITCH   },
    { "case",     T_KW_CASE     },
    { "default",  T_KW_DEFAULT  },
    { "break",    T_KW_BREAK    },
    { "continue", T_KW_CONTINUE },
    { "goto",     T_KW_GOTO     },
    { "function", T_KW_FUNCTION },
    { "return",   T_KW_RETURN   },
    { "freturn",  T_KW_FRETURN  },
    { "nreturn",  T_KW_NRETURN  },
    { "struct",   T_KW_STRUCT   },
    { NULL,       T_IDENT       }
};
static int kw_lookup_at(const char *s, int idx) {
    if (KW_TABLE[idx].word == NULL)              return T_IDENT;
    if (strcmp(s, KW_TABLE[idx].word) == 0)      return KW_TABLE[idx].kind;
    return kw_lookup_at(s, idx + 1);
}
static int classify_keyword_range(const char *start, const char *end) {
    int n = (int)(end - start);
    if (n <= 0 || n > 32) return T_IDENT;
    char buf[64];
    memcpy(buf, start, n);
    buf[n] = '\0';
    return kw_lookup_at(buf, 0);
}
/* ---------------------------------------------------------------- */
/* Emit helpers                                                     */
/* ---------------------------------------------------------------- */
/* ADV(n)     advance cursor by n
 * PEEK(n)    read p[n] without advancing
 * EMIT(k)    return non-value token; updates last_kind and ctx->p
 * EMIT_V(k)  return value token; copies tok_start..p into ctx->text
 *
 * The standard `do { ... } while (0)` macro idiom is avoided here
 * to keep the file free of loop syntax.  These are written with
 * comma operators or as separate statements in the per-label code.
 */
#define ADV(n)    (p += (n))
#define PEEK(n)   ((unsigned char)p[n])
static inline int emit_kind(LexCtx *ctx, const char *p, int kind) {
    ctx->p = p;
    ctx->last_kind = kind;
    return kind;
}
static inline int emit_value(LexCtx *ctx, const char *p, const char *tok_start, int kind) {
    int n = (int)(p - tok_start);
    if (n >= (int)sizeof(ctx->text)) n = (int)sizeof(ctx->text) - 1;
    memcpy(ctx->text, tok_start, n);
    ctx->text[n] = '\0';
    ctx->p = p;
    ctx->last_kind = kind;
    return kind;
}
#define EMIT(k)    return emit_kind(ctx, p, (k))
#define EMIT_V(k)  return emit_value(ctx, p, tok_start, (k))
int sc_lex_next(LexCtx *ctx) {
    const char *p          = ctx->p;
    const char *tok_start  = NULL;
    int         had_ws     = 0;
    int         last_value = sc_kind_is_value(ctx->last_kind);
    /* | Label        | if                                         | action                                    | goto             | */
    /* |--------------|--------------------------------------------|-------------------------------------------|------------------| */
S_WS:
    if (PEEK(0) == ' '  )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\t' )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\r' )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\f' )                                          {  had_ws = 1; ADV(1);                                  goto S_WS;        }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; had_ws = 1; ADV(1);                     goto S_CONT;      }
    if (PEEK(0) == '/'  && PEEK(1) == '/'  )                       {  had_ws = 1; ADV(2);                                  goto S_LCOMMENT;  }
    if (PEEK(0) == '/'  && PEEK(1) == '*'  )                       {  had_ws = 1; ADV(2);                                  goto S_BCOMMENT;  }
                                                                                                                           goto S_DISPATCH;
/*--------------------------------------------------------------------------------------------------------------------*/
S_CONT:
    /* After a newline: SNOBOL4 column-1 continuation marker '+' or '.' */
    if (PEEK(0) == '+'  )                                          {  ADV(1);                                              goto S_WS;        }
    if (PEEK(0) == '.'  )                                          {  ADV(1);                                              goto S_WS;        }
                                                                                                                           goto S_WS;
/*--------------------------------------------------------------------------------------------------------------------*/
S_LCOMMENT:
    if (PEEK(0) == '\0' )                                                                                                  goto S_DISPATCH;
    if (PEEK(0) == '\n' )                                                                                                  goto S_WS;
                                                                   {  ADV(1);                                              goto S_LCOMMENT;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_BCOMMENT:
    if (PEEK(0) == '\0' )                                                                                                  goto S_DISPATCH;
    if (PEEK(0) == '*'  )                                          {  ADV(1);                                              goto S_BC_STAR;   }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; ADV(1);                                 goto S_BCOMMENT;  }
                                                                   {  ADV(1);                                              goto S_BCOMMENT;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_BC_STAR:
    if (PEEK(0) == '/'  )                                          {  ADV(1);                                              goto S_WS;        }
    if (PEEK(0) == '*'  )                                          {  ADV(1);                                              goto S_BC_STAR;   }
    if (PEEK(0) == '\0' )                                                                                                  goto S_DISPATCH;
                                                                   {  ADV(1);                                              goto S_BCOMMENT;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_DISPATCH:
    /* CONCAT trigger: prev was a value, gap had whitespace, next
     * char unambiguously starts a fresh value-token.  '.DIGIT' is
     * also a value-starter (real number, e.g. .5 after `x`). */
    if (had_ws && last_value && is_value_starter(PEEK(0)))         {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
    if (had_ws && last_value && PEEK(0) == '.' && is_digit(PEEK(1))) { ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                }
    if (PEEK(0) == '\0' )                                                                                                  goto E_EOF;
    if (PEEK(0) == '\'' )                                          {  ctx->strpos = 0; ADV(1);                             goto S_STR1;      }
    if (PEEK(0) == '"'  )                                          {  ctx->strpos = 0; ADV(1);                             goto S_STR2;      }
    if (is_alpha(PEEK(0)))                                         {  tok_start = p; ADV(1);                               goto S_IDENT;     }
    if (is_digit(PEEK(0)))                                         {  tok_start = p; ADV(1);                               goto S_INT;       }
    if (PEEK(0) == '.'  && is_digit(PEEK(1)))                      {  tok_start = p; ADV(1);                                goto S_FRAC;      }
    if (PEEK(0) == '.'  )                                                                                                  goto S_OP_DOT;
    if (PEEK(0) == '&'  && is_alpha(PEEK(1)))                      {  ADV(1); tok_start = p;                               goto S_KEYWORD;   }
    if (PEEK(0) == '('  )                                          {  ADV(1);                                              goto E_LPAREN;    }
    if (PEEK(0) == ')'  )                                          {  ADV(1);                                              goto E_RPAREN;    }
    if (PEEK(0) == '['  )                                          {  ADV(1);                                              goto E_LBRACK;    }
    if (PEEK(0) == ']'  )                                          {  ADV(1);                                              goto E_RBRACK;    }
    if (PEEK(0) == '{'  )                                          {  ADV(1);                                              goto E_LBRACE;    }
    if (PEEK(0) == '}'  )                                          {  ADV(1);                                              goto E_RBRACE;    }
    if (PEEK(0) == ','  )                                          {  ADV(1);                                              goto E_COMMA;     }
    if (PEEK(0) == ';'  )                                          {  ADV(1);                                              goto E_SEMICOLON; }
    if (PEEK(0) == ':'  )                                                                                                  goto S_OP_COLON;
    if (PEEK(0) == '='  )                                                                                                  goto S_OP_EQ;
    if (PEEK(0) == '!'  )                                                                                                  goto S_OP_BANG;
    if (PEEK(0) == '<'  )                                                                                                  goto S_OP_LT;
    if (PEEK(0) == '>'  )                                                                                                  goto S_OP_GT;
    if (PEEK(0) == '+'  )                                                                                                  goto S_OP_PLUS;
    if (PEEK(0) == '-'  )                                                                                                  goto S_OP_MINUS;
    if (PEEK(0) == '*'  )                                                                                                  goto S_OP_STAR;
    if (PEEK(0) == '/'  )                                                                                                  goto S_OP_SLASH;
    if (PEEK(0) == '^'  )                                                                                                  goto S_OP_CARET;
    if (PEEK(0) == '|'  )                                                                                                  goto S_OP_PIPE;
    if (PEEK(0) == '?'  )                                                                                                  goto S_OP_QUEST;
    if (PEEK(0) == '$'  )                                                                                                  goto S_OP_DOLLAR;
    if (PEEK(0) == '&'  )                                                                                                  goto S_OP_AMP;
    if (PEEK(0) == '@'  )                                                                                                  goto S_OP_AT;
    if (PEEK(0) == '#'  )                                                                                                  goto S_OP_POUND;
    if (PEEK(0) == '%'  )                                                                                                  goto S_OP_PERCENT;
    if (PEEK(0) == '~'  )                                                                                                  goto S_OP_TILDE;
                                                                   {  tok_start = p; ADV(1);                               goto E_UNKNOWN;   }
/*--------------------------------------------------------------------------------------------------------------------*/
S_IDENT:
    if (is_idcont(PEEK(0)))                                        {  ADV(1);                                              goto S_IDENT;     }
    if (PEEK(0) == '('  )                                                                                                  goto E_FUNCTION;
                                                                   {                                                       goto E_IDENT;     }
/*--------------------------------------------------------------------------------------------------------------------*/
S_KEYWORD:
    if (is_idcont(PEEK(0)))                                        {  ADV(1);                                              goto S_KEYWORD;   }
                                                                                                                           goto E_KEYWORD;
/*--------------------------------------------------------------------------------------------------------------------*/
S_INT:
    if (is_digit(PEEK(0)))                                         {  ADV(1);                                              goto S_INT;       }
    if (PEEK(0) == '.' && is_digit(PEEK(1)))                       {  ADV(1);                                              goto S_FRAC;      }
    if (PEEK(0) == '.' )                                           {  ADV(1);                                              goto E_REAL;      }
    if (PEEK(0) == 'e' || PEEK(0) == 'E')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
    if (PEEK(0) == 'd' || PEEK(0) == 'D')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
                                                                                                                           goto E_INT;
/*--------------------------------------------------------------------------------------------------------------------*/
S_FRAC:
    if (is_digit(PEEK(0)))                                         {  ADV(1);                                              goto S_FRAC;      }
    if (PEEK(0) == 'e' || PEEK(0) == 'E')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
    if (PEEK(0) == 'd' || PEEK(0) == 'D')                          {  ADV(1);                                              goto S_EXP_SIGN;  }
                                                                                                                           goto E_REAL;
/*--------------------------------------------------------------------------------------------------------------------*/
S_EXP_SIGN:
    if (PEEK(0) == '+' || PEEK(0) == '-')                          {  ADV(1);                                              goto S_EXP_DIG;   }
                                                                                                                           goto S_EXP_DIG;
/*--------------------------------------------------------------------------------------------------------------------*/
S_EXP_DIG:
    if (is_digit(PEEK(0)))                                         {  ADV(1);                                              goto S_EXP_DIG;   }
                                                                                                                           goto E_REAL;
/*--------------------------------------------------------------------------------------------------------------------*/
S_STR1:
    if (PEEK(0) == '\0' )                                                                                                  goto E_STR;
    if (PEEK(0) == '\'' && PEEK(1) == '\'' )                       {  ctx->strbuf[ctx->strpos++] = '\''; ADV(2);           goto S_STR1;      }
    if (PEEK(0) == '\'' )                                          {  ADV(1);                                              goto E_STR;       }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; ctx->strbuf[ctx->strpos++] = '\n'; ADV(1); goto S_STR1;   }
                                                                   {  ctx->strbuf[ctx->strpos++] = *p; ADV(1);              goto S_STR1;     }
/*--------------------------------------------------------------------------------------------------------------------*/
S_STR2:
    if (PEEK(0) == '\0' )                                                                                                  goto E_STR;
    if (PEEK(0) == '"'  && PEEK(1) == '"'  )                       {  ctx->strbuf[ctx->strpos++] = '"';  ADV(2);           goto S_STR2;      }
    if (PEEK(0) == '"'  )                                          {  ADV(1);                                              goto E_STR;       }
    if (PEEK(0) == '\n' )                                          {  ctx->line++; ctx->strbuf[ctx->strpos++] = '\n'; ADV(1); goto S_STR2;   }
                                                                   {  ctx->strbuf[ctx->strpos++] = *p; ADV(1);              goto S_STR2;     }
    /* Each S_OP_* label disambiguates multi-char forms (++, +=,    */
    /* etc.), then chooses BINARY vs UNARY based on:                */
    /*    BINARY needs left-ws + last-was-value (had_ws+last_value).*/
    /*    UNARY  is the default.                                    */
    /* The {W}OP{W} right-ws check is implicit: with no right-ws,   */
    /* the left-side condition still fires correctly because the    */
    /* parser will see ADD/etc and the absence of right-ws is fine. */
    /* (Snobol4 / SPITBOL convention: x+ is x followed by unary +.) */
    /* ============================================================ */
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_COLON:
    if (PEEK(1) == ':' )                                           {  ADV(2);                                              goto E_IDENT_OP;  }
    if (PEEK(1) == '!' && PEEK(2) == ':' )                         {  ADV(3);                                              goto E_DIFFER;    }
    if (PEEK(1) == '<' && PEEK(2) == '=' && PEEK(3) == ':' )       {  ADV(4);                                              goto E_LLE;       }
    if (PEEK(1) == '>' && PEEK(2) == '=' && PEEK(3) == ':' )       {  ADV(4);                                              goto E_LGE;       }
    if (PEEK(1) == '=' && PEEK(2) == '=' && PEEK(3) == ':' )       {  ADV(4);                                              goto E_LEQ;       }
    if (PEEK(1) == '!' && PEEK(2) == '=' && PEEK(3) == ':' )       {  ADV(4);                                              goto E_LNE;       }
    if (PEEK(1) == '<' && PEEK(2) == ':' )                         {  ADV(3);                                              goto E_LLT;       }
    if (PEEK(1) == '>' && PEEK(2) == ':' )                         {  ADV(3);                                              goto E_LGT;       }
                                                                   {  ADV(1);                                              goto E_COLON;     }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_EQ:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_EQ;        }
    if (had_ws && last_value)                                      {  ADV(1);                                              goto E_ASSIGN;    }
                                                                   {  ADV(1);                                              goto E_UN_EQUAL;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_BANG:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_NE;        }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_EXP;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_EXP;       }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_LT:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_LE;        }
                                                                   {  ADV(1);                                              goto E_LT;        }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_GT:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_GE;        }
                                                                   {  ADV(1);                                              goto E_GT;        }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_PLUS:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_PLUS_ASSIGN;  }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_ADD;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_PLUS;   }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_MINUS:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_MINUS_ASSIGN; }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_SUB;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_MINUS;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_STAR:
    if (PEEK(1) == '*' && is_rws_at(p, 2))                         {  ADV(2);                                              goto E_EXP;       }
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_STAR_ASSIGN;  }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_MUL;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
    if (PEEK(1) == '*' )                                           {  ADV(2);                                              goto E_EXP;       }
                                                                   {  ADV(1);                                              goto E_UN_STAR;   }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_SLASH:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_SLASH_ASSIGN; }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_DIV;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_SLASH;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_CARET:
    if (PEEK(1) == '=' )                                           {  ADV(2);                                              goto E_CARET_ASSIGN; }
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_EXP;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_EXP;       }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_PIPE:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_ALT;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_PIPE;   }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_QUEST:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_MATCH;     }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_QUEST;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_DOLLAR:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_IMM_ASSIGN;}
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_DOLLAR; }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_DOT:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_COND_ASSIGN;  }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_DOT;    }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_AMP:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_AMP;       }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_AMP;    }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_AT:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_AT;        }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_AT;     }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_POUND:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_POUND;     }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_POUND;  }
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_PERCENT:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_PERCENT;   }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_PERCENT;}
/*--------------------------------------------------------------------------------------------------------------------*/
S_OP_TILDE:
    if (had_ws && last_value && is_rws_at(p, 1))                   {  ADV(1);                                              goto E_TILDE;     }
    if (had_ws && last_value)                                      {  ctx->p = p; ctx->last_kind = T_CONCAT; return T_CONCAT;                 }
                                                                   {  ADV(1);                                              goto E_UN_TILDE;  }
/*--------------------------------------------------------------------------------------------------------------------*/
E_EOF:           EMIT(T_EOF);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LPAREN:        EMIT(T_LPAREN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_RPAREN:        EMIT(T_RPAREN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LBRACK:        EMIT(T_LBRACK);
/*--------------------------------------------------------------------------------------------------------------------*/
E_RBRACK:        EMIT(T_RBRACK);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LBRACE:        EMIT(T_LBRACE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_RBRACE:        EMIT(T_RBRACE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_COMMA:         EMIT(T_COMMA);
/*--------------------------------------------------------------------------------------------------------------------*/
E_SEMICOLON:     EMIT(T_SEMICOLON);
/*--------------------------------------------------------------------------------------------------------------------*/
E_COLON:         EMIT(T_COLON);
/*--------------------------------------------------------------------------------------------------------------------*/
E_ASSIGN:        EMIT(T_ASSIGNMENT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_MATCH:         EMIT(T_MATCH);
/*--------------------------------------------------------------------------------------------------------------------*/
E_ALT:           EMIT(T_ALTERNATION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LEQ:           EMIT(T_LEQ);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LNE:           EMIT(T_LNE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LLE:           EMIT(T_LLE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LGE:           EMIT(T_LGE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LLT:           EMIT(T_LLT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LGT:           EMIT(T_LGT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_DIFFER:        EMIT(T_DIFFER);
/*--------------------------------------------------------------------------------------------------------------------*/
E_IDENT_OP:      EMIT(T_IDENT_OP);
/*--------------------------------------------------------------------------------------------------------------------*/
E_EQ:            EMIT(T_EQ);
/*--------------------------------------------------------------------------------------------------------------------*/
E_NE:            EMIT(T_NE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LE:            EMIT(T_LE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_GE:            EMIT(T_GE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_LT:            EMIT(T_LT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_GT:            EMIT(T_GT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_ADD:           EMIT(T_ADDITION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_SUB:           EMIT(T_SUBTRACTION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_MUL:           EMIT(T_MULTIPLICATION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_DIV:           EMIT(T_DIVISION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_EXP:           EMIT(T_EXPONENTIATION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_IMM_ASSIGN:    EMIT(T_IMMEDIATE_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_COND_ASSIGN:   EMIT(T_COND_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_AMP:           EMIT(T_AMPERSAND);
/*--------------------------------------------------------------------------------------------------------------------*/
E_AT:            EMIT(T_AT_SIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_POUND:         EMIT(T_POUND);
/*--------------------------------------------------------------------------------------------------------------------*/
E_PERCENT:       EMIT(T_PERCENT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_TILDE:         EMIT(T_TILDE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_PLUS_ASSIGN:   EMIT(T_PLUS_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_MINUS_ASSIGN:  EMIT(T_MINUS_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_STAR_ASSIGN:   EMIT(T_STAR_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_SLASH_ASSIGN:  EMIT(T_SLASH_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_CARET_ASSIGN:  EMIT(T_CARET_ASSIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_PLUS:       EMIT(T_UN_PLUS);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_MINUS:      EMIT(T_UN_MINUS);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_STAR:       EMIT(T_UN_ASTERISK);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_SLASH:      EMIT(T_UN_SLASH);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_PERCENT:    EMIT(T_UN_PERCENT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_AT:         EMIT(T_UN_AT_SIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_TILDE:      EMIT(T_UN_TILDE);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_DOLLAR:     EMIT(T_UN_DOLLAR_SIGN);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_DOT:        EMIT(T_UN_PERIOD);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_POUND:      EMIT(T_UN_POUND);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_PIPE:       EMIT(T_UN_VERTICAL_BAR);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_EQUAL:      EMIT(T_UN_EQUAL);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_QUEST:      EMIT(T_UN_QUESTION_MARK);
/*--------------------------------------------------------------------------------------------------------------------*/
E_UN_AMP:        EMIT(T_UN_AMPERSAND);
/*--------------------------------------------------------------------------------------------------------------------*/
E_INT:           EMIT_V(T_INT);
/*--------------------------------------------------------------------------------------------------------------------*/
E_REAL:          EMIT_V(T_REAL);
/*--------------------------------------------------------------------------------------------------------------------*/
E_FUNCTION:      EMIT_V(T_FUNCTION);
/*--------------------------------------------------------------------------------------------------------------------*/
E_IDENT:
    {
        int kind = classify_keyword_range(tok_start, p);
        int n    = (int)(p - tok_start);
        if (n >= (int)sizeof(ctx->text)) n = (int)sizeof(ctx->text) - 1;
        memcpy(ctx->text, tok_start, n);
        ctx->text[n] = '\0';
        ctx->p = p; ctx->last_kind = kind; return kind;
    }
/*--------------------------------------------------------------------------------------------------------------------*/
E_KEYWORD:
    {
        int n = (int)(p - tok_start);
        if (n >= (int)sizeof(ctx->text)) n = (int)sizeof(ctx->text) - 1;
        memcpy(ctx->text, tok_start, n);
        ctx->text[n] = '\0';
        ctx->p = p; ctx->last_kind = T_KEYWORD; return T_KEYWORD;
    }
/*--------------------------------------------------------------------------------------------------------------------*/
E_STR:
    {
        ctx->strbuf[ctx->strpos] = '\0';
        int n = ctx->strpos;
        if (n >= (int)sizeof(ctx->text)) n = (int)sizeof(ctx->text) - 1;
        memcpy(ctx->text, ctx->strbuf, n);
        ctx->text[n] = '\0';
        ctx->p = p; ctx->last_kind = T_STR; return T_STR;
    }
/*--------------------------------------------------------------------------------------------------------------------*/
E_UNKNOWN:       EMIT_V(T_UNKNOWN);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *sc_name_table[256];
static int         sc_name_table_built = 0;
static void sc_name_table_build(void) {
    sc_name_table[T_INT]              = "T_INT";
    sc_name_table[T_REAL]             = "T_REAL";
    sc_name_table[T_STR]              = "T_STR";
    sc_name_table[T_IDENT]            = "T_IDENT";
    sc_name_table[T_FUNCTION]         = "T_FUNCTION";
    sc_name_table[T_KEYWORD]          = "T_KEYWORD";
    sc_name_table[T_CONCAT]           = "T_CONCAT";
    sc_name_table[T_ASSIGNMENT]       = "T_ASSIGNMENT";
    sc_name_table[T_MATCH]            = "T_MATCH";
    sc_name_table[T_ALTERNATION]      = "T_ALTERNATION";
    sc_name_table[T_ADDITION]         = "T_ADDITION";
    sc_name_table[T_SUBTRACTION]      = "T_SUBTRACTION";
    sc_name_table[T_MULTIPLICATION]   = "T_MULTIPLICATION";
    sc_name_table[T_DIVISION]         = "T_DIVISION";
    sc_name_table[T_EXPONENTIATION]   = "T_EXPONENTIATION";
    sc_name_table[T_IMMEDIATE_ASSIGN] = "T_IMMEDIATE_ASSIGN";
    sc_name_table[T_COND_ASSIGN]      = "T_COND_ASSIGN";
    sc_name_table[T_AMPERSAND]        = "T_AMPERSAND";
    sc_name_table[T_AT_SIGN]          = "T_AT_SIGN";
    sc_name_table[T_POUND]            = "T_POUND";
    sc_name_table[T_PERCENT]          = "T_PERCENT";
    sc_name_table[T_TILDE]            = "T_TILDE";
    sc_name_table[T_EQ]               = "T_EQ";
    sc_name_table[T_NE]               = "T_NE";
    sc_name_table[T_LT]               = "T_LT";
    sc_name_table[T_GT]               = "T_GT";
    sc_name_table[T_LE]               = "T_LE";
    sc_name_table[T_GE]               = "T_GE";
    sc_name_table[T_LEQ]              = "T_LEQ";
    sc_name_table[T_LNE]              = "T_LNE";
    sc_name_table[T_LLT]              = "T_LLT";
    sc_name_table[T_LGT]              = "T_LGT";
    sc_name_table[T_LLE]              = "T_LLE";
    sc_name_table[T_LGE]              = "T_LGE";
    sc_name_table[T_IDENT_OP]         = "T_IDENT_OP";
    sc_name_table[T_DIFFER]           = "T_DIFFER";
    sc_name_table[T_PLUS_ASSIGN]      = "T_PLUS_ASSIGN";
    sc_name_table[T_MINUS_ASSIGN]     = "T_MINUS_ASSIGN";
    sc_name_table[T_STAR_ASSIGN]      = "T_STAR_ASSIGN";
    sc_name_table[T_SLASH_ASSIGN]     = "T_SLASH_ASSIGN";
    sc_name_table[T_CARET_ASSIGN]     = "T_CARET_ASSIGN";
    sc_name_table[T_UN_PLUS]          = "T_UN_PLUS";
    sc_name_table[T_UN_MINUS]         = "T_UN_MINUS";
    sc_name_table[T_UN_ASTERISK]      = "T_UN_ASTERISK";
    sc_name_table[T_UN_SLASH]         = "T_UN_SLASH";
    sc_name_table[T_UN_PERCENT]       = "T_UN_PERCENT";
    sc_name_table[T_UN_AT_SIGN]       = "T_UN_AT_SIGN";
    sc_name_table[T_UN_TILDE]         = "T_UN_TILDE";
    sc_name_table[T_UN_DOLLAR_SIGN]   = "T_UN_DOLLAR_SIGN";
    sc_name_table[T_UN_PERIOD]        = "T_UN_PERIOD";
    sc_name_table[T_UN_POUND]         = "T_UN_POUND";
    sc_name_table[T_UN_VERTICAL_BAR]  = "T_UN_VERTICAL_BAR";
    sc_name_table[T_UN_EQUAL]         = "T_UN_EQUAL";
    sc_name_table[T_UN_QUESTION_MARK] = "T_UN_QUESTION_MARK";
    sc_name_table[T_UN_AMPERSAND]     = "T_UN_AMPERSAND";
    sc_name_table[T_LPAREN]           = "T_LPAREN";
    sc_name_table[T_RPAREN]           = "T_RPAREN";
    sc_name_table[T_LBRACE]           = "T_LBRACE";
    sc_name_table[T_RBRACE]           = "T_RBRACE";
    sc_name_table[T_LBRACK]           = "T_LBRACK";
    sc_name_table[T_RBRACK]           = "T_RBRACK";
    sc_name_table[T_COMMA]            = "T_COMMA";
    sc_name_table[T_SEMICOLON]        = "T_SEMICOLON";
    sc_name_table[T_COLON]            = "T_COLON";
    sc_name_table[T_KW_IF]            = "T_KW_IF";
    sc_name_table[T_KW_ELSE]          = "T_KW_ELSE";
    sc_name_table[T_KW_WHILE]         = "T_KW_WHILE";
    sc_name_table[T_KW_DO]            = "T_KW_DO";
    sc_name_table[T_KW_UNTIL]         = "T_KW_UNTIL";
    sc_name_table[T_KW_FOR]           = "T_KW_FOR";
    sc_name_table[T_KW_SWITCH]        = "T_KW_SWITCH";
    sc_name_table[T_KW_CASE]          = "T_KW_CASE";
    sc_name_table[T_KW_DEFAULT]       = "T_KW_DEFAULT";
    sc_name_table[T_KW_BREAK]         = "T_KW_BREAK";
    sc_name_table[T_KW_CONTINUE]      = "T_KW_CONTINUE";
    sc_name_table[T_KW_GOTO]          = "T_KW_GOTO";
    sc_name_table[T_KW_FUNCTION]      = "T_KW_FUNCTION";
    sc_name_table[T_KW_RETURN]        = "T_KW_RETURN";
    sc_name_table[T_KW_FRETURN]       = "T_KW_FRETURN";
    sc_name_table[T_KW_NRETURN]       = "T_KW_NRETURN";
    sc_name_table[T_KW_STRUCT]        = "T_KW_STRUCT";
    sc_name_table[T_EOF]              = "T_EOF";
    sc_name_table[T_UNKNOWN]          = "T_UNKNOWN";
    sc_name_table_built               = 1;
}
const char *sc2_kind_name(int kind) {
    if (!sc_name_table_built) sc_name_table_build();
    if (kind < 0 || kind >= 256) return "T_???";
    const char *s = sc_name_table[kind];
    return s ? s : "T_???";
}
