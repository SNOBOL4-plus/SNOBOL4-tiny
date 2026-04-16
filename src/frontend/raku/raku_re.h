/*============================================================= raku_re.h ====
 * Raku regex engine — table-driven NFA (Thompson construction)
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
 *==========================================================================*/
#ifndef RAKU_RE_H
#define RAKU_RE_H

/*------------------------------------------------------------------------
 * Character class bitmap (256 bits)
 *----------------------------------------------------------------------*/
typedef struct { unsigned char bits[32]; } Raku_cc;

int raku_cc_test(const Raku_cc *cc, unsigned char c);

/*------------------------------------------------------------------------
 * NFA state kinds
 *----------------------------------------------------------------------*/
typedef enum {
    NK_EPS,         /* epsilon */
    NK_SPLIT,       /* fork (alternation / quantifier) */
    NK_CHAR,        /* match literal ch */
    NK_ANY,         /* match any char except newline */
    NK_CLASS,       /* match char class bitmap */
    NK_ANCHOR_BOL,  /* ^ zero-width */
    NK_ANCHOR_EOL,  /* $ zero-width */
    NK_CAP_OPEN,    /* ( or <name> — record group start */
    NK_CAP_CLOSE,   /* ) or end of <name> — record group end */
    NK_ACCEPT
} Nfa_kind;

#define NFA_NULL   (-1)
#define MAX_GROUPS  16

/*------------------------------------------------------------------------
 * NFA state
 *----------------------------------------------------------------------*/
typedef struct {
    int       id;
    Nfa_kind  kind;
    unsigned char ch;
    Raku_cc   cc;
    int       out1;
    int       out2;
    int       cap_idx;    /* NK_CAP_OPEN/CLOSE: group index */
    int       bb_id;      /* reserved for Phase-3 BB lifter */
} Nfa_state;

/*------------------------------------------------------------------------
 * Match result
 *----------------------------------------------------------------------*/
typedef struct {
    int matched;
    int full_start;
    int full_end;
    int  group_start[MAX_GROUPS];
    int  group_end[MAX_GROUPS];
    char group_name[MAX_GROUPS][64];  /* "" = positional, else named */
    int  ngroups;
} Raku_match;

/*------------------------------------------------------------------------
 * NFA graph (opaque)
 *----------------------------------------------------------------------*/
typedef struct Raku_nfa Raku_nfa;

Raku_nfa  *raku_nfa_build(const char *pattern);
int        raku_nfa_state_count(const Raku_nfa *nfa);
int        raku_nfa_ngroups(const Raku_nfa *nfa);
int        raku_nfa_match(const Raku_nfa *nfa, const char *subject);
void       raku_nfa_exec(const Raku_nfa *nfa, const char *subject, Raku_match *result);
void       raku_nfa_free(Raku_nfa *nfa);
Nfa_state *raku_nfa_states(Raku_nfa *nfa);

/* Look up group index by name (-1 if not found) */
int        raku_nfa_group_by_name(const Raku_nfa *nfa, const char *name);

#endif /* RAKU_RE_H */
