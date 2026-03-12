/*
 * snoc_helpers.c — C implementations of beauty.sno's -INCLUDE helper libraries.
 *
 * These replace the compiled SNOBOL4 from:
 *   stack.sno, counter.sno, ShiftReduce.sno, tree.sno (partial),
 *   is.sno, case.sno, ReadWrite.sno, assign.sno, match.sno,
 *   TDump.sno, XDump.sno, trace.sno (stubs)
 *
 * Call snoc_helpers_init() from sno_runtime_init() BEFORE any SNOBOL4
 * DEFINE statements execute.  The C stubs win because they register first;
 * SNOBOL4 DEFINE calls from -INCLUDE files are silently ignored when the
 * name is already registered.
 *
 * Session 30 eureka (Lon Cherryholmes, 2026-03-12):
 *   ~905 lines of -INCLUDE SNOBOL4 → ~370 lines of C.
 *   debug/trace helpers are no-ops (xTrace=0 by default).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snobol4.h"
#include "GC.h"

/* ============================================================
 * TREE data structure
 * DATA('tree(t,v,n,c)')
 * t = type string, v = value SnoVal, n = child count int, c = SnoVal array
 * ============================================================ */

typedef struct SnoTree {
    char       *t;       /* type */
    SnoVal      v;       /* value */
    int         n;       /* child count */
    SnoVal     *c;       /* children array (1-based, c[1..n]) */
} SnoTree;

/* Allocate a tree node */
static SnoTree *tree_alloc(const char *t, SnoVal v, int n, SnoVal *c) {
    SnoTree *tr = GC_malloc(sizeof(SnoTree));
    tr->t = t ? GC_strdup(t) : GC_strdup("");
    tr->v = v;
    tr->n = n;
    if (n > 0 && c) {
        tr->c = GC_malloc((n + 1) * sizeof(SnoVal));
        for (int i = 1; i <= n; i++) tr->c[i] = c[i];
    } else {
        tr->c = NULL;
    }
    return tr;
}

static SnoVal tree_to_val(SnoTree *tr) {
    SnoVal v;
    v.type = SNO_OBJECT;
    v.obj  = (void*)tr;
    /* tag it so we can identify tree objects */
    v.tag  = 0xBEEF;
    return v;
}

static SnoTree *val_to_tree(SnoVal v) {
    if (v.type == SNO_OBJECT && v.tag == 0xBEEF)
        return (SnoTree*)v.obj;
    return NULL;
}

/* tree(t, v, n, c) constructor */
static SnoVal _b_tree(SnoVal *args, int nargs) {
    const char *t = (nargs > 0) ? sno_to_str(args[0]) : "";
    SnoVal      v = (nargs > 1) ? args[1] : SNO_NULL_VAL;
    int         n = (nargs > 2) ? (int)sno_to_int(args[2]) : 0;
    SnoVal     *c = NULL;
    if (nargs > 3 && args[3].type == SNO_ARRAY) {
        c = GC_malloc((n + 2) * sizeof(SnoVal));
        for (int i = 1; i <= n; i++)
            c[i] = sno_array_get(args[3].a, i);
    }
    return tree_to_val(tree_alloc(t, v, n, c));
}

/* Field accessors: t(x), v(x), n(x), c(x) */
static SnoVal _b_tree_t(SnoVal *args, int nargs) {
    if (nargs < 1) return SNO_FAIL_VAL;
    SnoTree *tr = val_to_tree(args[0]);
    if (!tr) return SNO_FAIL_VAL;
    return SNO_STR_VAL(tr->t ? tr->t : "");
}
static SnoVal _b_tree_v(SnoVal *args, int nargs) {
    if (nargs < 1) return SNO_FAIL_VAL;
    SnoTree *tr = val_to_tree(args[0]);
    if (!tr) return SNO_FAIL_VAL;
    return tr->v;
}
static SnoVal _b_tree_n(SnoVal *args, int nargs) {
    if (nargs < 1) return SNO_FAIL_VAL;
    SnoTree *tr = val_to_tree(args[0]);
    if (!tr) return SNO_NULL_VAL; /* n() of non-tree = null (0) */
    return sno_int(tr->n);
}
static SnoVal _b_tree_c(SnoVal *args, int nargs) {
    if (nargs < 1) return SNO_FAIL_VAL;
    SnoTree *tr = val_to_tree(args[0]);
    if (!tr || tr->n == 0) return SNO_NULL_VAL;
    /* return as a SnoVal array */
    SnoArray *a = sno_array_new(tr->n);
    for (int i = 1; i <= tr->n; i++)
        sno_array_set(a, i, tr->c[i]);
    SnoVal r; r.type = SNO_ARRAY; r.a = a;
    return r;
}

/* Field setters: n(x) = val, v(x) = val, c(x) = val */
/* These are called as assignments in SNOBOL4: n(x) = n(x) + 1 */
/* In snoc they compile as sno_field_set("n", x, val) */
static SnoVal _b_tree_set_n(SnoVal obj, SnoVal val) {
    SnoTree *tr = val_to_tree(obj);
    if (!tr) return SNO_FAIL_VAL;
    tr->n = (int)sno_to_int(val);
    return val;
}
static SnoVal _b_tree_set_v(SnoVal obj, SnoVal val) {
    SnoTree *tr = val_to_tree(obj);
    if (!tr) return SNO_FAIL_VAL;
    tr->v = val;
    return val;
}
static SnoVal _b_tree_set_c(SnoVal obj, SnoVal val) {
    SnoTree *tr = val_to_tree(obj);
    if (!tr) return SNO_FAIL_VAL;
    if (val.type == SNO_ARRAY) {
        tr->c = GC_malloc((tr->n + 2) * sizeof(SnoVal));
        for (int i = 1; i <= tr->n; i++)
            tr->c[i] = sno_array_get(val.a, i);
    }
    return val;
}

/* ============================================================
 * STACK  (global head: sno var "@S" holds a link object)
 * DATA('link(next,value)')
 * Push(x), Pop(var), Top()
 * ============================================================ */

typedef struct SnoLink {
    SnoVal next;
    SnoVal value;
} SnoLink;

static SnoVal link_to_val(SnoLink *lk) {
    SnoVal v; v.type = SNO_OBJECT; v.obj = lk; v.tag = 0xABCD; return v;
}
static SnoLink *val_to_link(SnoVal v) {
    if (v.type == SNO_OBJECT && v.tag == 0xABCD) return (SnoLink*)v.obj;
    return NULL;
}
/* link(next,value) constructor */
static SnoVal _b_link(SnoVal *args, int nargs) {
    SnoLink *lk = GC_malloc(sizeof(SnoLink));
    lk->next  = (nargs > 0) ? args[0] : SNO_NULL_VAL;
    lk->value = (nargs > 1) ? args[1] : SNO_NULL_VAL;
    return link_to_val(lk);
}
/* field accessors */
static SnoVal _b_link_next(SnoVal *args, int nargs) {
    if (nargs < 1) return SNO_FAIL_VAL;
    SnoLink *lk = val_to_link(args[0]);
    return lk ? lk->next : SNO_FAIL_VAL;
}
static SnoVal _b_link_value(SnoVal *args, int nargs) {
    if (nargs < 1) return SNO_FAIL_VAL;
    SnoLink *lk = val_to_link(args[0]);
    return lk ? lk->value : SNO_FAIL_VAL;
}

/* InitStack() — clear @S */
static SnoVal _b_InitStack(SnoVal *args, int nargs) {
    sno_var_set("@S", SNO_NULL_VAL);
    return SNO_NULL_VAL; /* NRETURN semantics */
}

/* Push(x) — push onto @S */
static SnoVal _b_Push(SnoVal *args, int nargs) {
    SnoVal x = (nargs > 0) ? args[0] : SNO_NULL_VAL;
    SnoVal head = sno_var_get("@S");
    SnoLink *lk = GC_malloc(sizeof(SnoLink));
    lk->next  = head;
    lk->value = x;
    sno_var_set("@S", link_to_val(lk));
    return SNO_NULL_VAL; /* NRETURN */
}

/* Pop() — pop from @S, return value; FRETURN if empty */
static SnoVal _b_Pop(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("@S");
    SnoLink *lk = val_to_link(head);
    if (!lk) return SNO_FAIL_VAL; /* FRETURN */
    SnoVal val = lk->value;
    sno_var_set("@S", lk->next);
    return val;
}

/* Top() — peek top of @S; FRETURN if empty */
static SnoVal _b_Top(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("@S");
    SnoLink *lk = val_to_link(head);
    if (!lk) return SNO_FAIL_VAL;
    return lk->value;
}

/* ============================================================
 * COUNTER STACK  (global head: sno var "#N")
 * DATA('link_counter(next,value)')
 * PushCounter(), IncCounter(), DecCounter(), TopCounter(), PopCounter()
 * ============================================================ */

typedef struct SnoLinkC {
    SnoVal next;
    long   value;
} SnoLinkC;

static SnoVal linkc_to_val(SnoLinkC *lk) {
    SnoVal v; v.type = SNO_OBJECT; v.obj = lk; v.tag = 0xCCCC; return v;
}
static SnoLinkC *val_to_linkc(SnoVal v) {
    if (v.type == SNO_OBJECT && v.tag == 0xCCCC) return (SnoLinkC*)v.obj;
    return NULL;
}

static SnoVal _b_PushCounter(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("#N");
    SnoLinkC *lk = GC_malloc(sizeof(SnoLinkC));
    lk->next  = head; lk->value = 0;
    sno_var_set("#N", linkc_to_val(lk));
    return SNO_NULL_VAL;
}
static SnoVal _b_IncCounter(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("#N");
    SnoLinkC *lk = val_to_linkc(head);
    if (!lk) return SNO_NULL_VAL;
    lk->value++;
    return SNO_NULL_VAL;
}
static SnoVal _b_DecCounter(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("#N");
    SnoLinkC *lk = val_to_linkc(head);
    if (!lk) return SNO_NULL_VAL;
    lk->value--;
    return SNO_NULL_VAL;
}
static SnoVal _b_TopCounter(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("#N");
    SnoLinkC *lk = val_to_linkc(head);
    if (!lk) return SNO_FAIL_VAL;
    return sno_int(lk->value);
}
static SnoVal _b_PopCounter(SnoVal *args, int nargs) {
    SnoVal head = sno_var_get("#N");
    SnoLinkC *lk = val_to_linkc(head);
    if (!lk) return SNO_FAIL_VAL;
    sno_var_set("#N", lk->next);
    return SNO_NULL_VAL;
}
static SnoVal _b_InitCounter(SnoVal *args, int nargs) {
    sno_var_set("#N", SNO_NULL_VAL);
    return SNO_NULL_VAL;
}

/* ============================================================
 * SHIFT / REDUCE  (the parse stack engine)
 * Shift(t,v)   — build tree(t,v), Push it
 * Reduce(t,n)  — pop n trees, build tree(t,_,n,children), Push it
 * ============================================================ */

static SnoVal _b_Shift(SnoVal *args, int nargs) {
    const char *t = (nargs > 0) ? sno_to_str(args[0]) : "";
    SnoVal      v = (nargs > 1) ? args[1] : SNO_NULL_VAL;
    SnoTree *tr = tree_alloc(t ? t : "", v, 0, NULL);
    SnoVal node = tree_to_val(tr);
    /* Push onto stack */
    SnoVal head = sno_var_get("@S");
    SnoLink *lk = GC_malloc(sizeof(SnoLink));
    lk->next = head; lk->value = node;
    sno_var_set("@S", link_to_val(lk));
    return SNO_NULL_VAL; /* NRETURN */
}

static SnoVal _b_Reduce(SnoVal *args, int nargs) {
    const char *t = (nargs > 0) ? sno_to_str(args[0]) : "";
    int         n = (nargs > 1) ? (int)sno_to_int(args[1]) : 0;
    if (n < 0) n = 0;

    /* Pop n items from stack into children array (reverse order) */
    SnoVal *children = GC_malloc((n + 2) * sizeof(SnoVal));
    for (int i = n; i >= 1; i--) {
        SnoVal head = sno_var_get("@S");
        SnoLink *lk = val_to_link(head);
        if (!lk) { children[i] = SNO_NULL_VAL; continue; }
        children[i] = lk->value;
        sno_var_set("@S", lk->next);
    }

    SnoTree *tr = tree_alloc(t ? t : "", SNO_NULL_VAL, n, children);
    SnoVal node = tree_to_val(tr);

    /* Push result */
    SnoVal head =