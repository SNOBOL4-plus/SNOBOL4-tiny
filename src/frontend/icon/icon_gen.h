/*============================================================================================================================
 * icon_gen.h — Icon Value-Generator Byrd Box Types (GOAL-ICN-BROKER B-1)
 *
 * Mirrors bb_box.h / bb_node_t for the value-typed (DESCR_t) Icon generator world.
 *
 * SNOBOL4 pattern Byrd box:   spec_t  (*bb_box_fn)(void *zeta, int entry)  — cursor-typed
 * Icon generator Byrd box:    DESCR_t (*icn_box_fn)(void *zeta, int entry) — value-typed
 *
 * Same four-signal protocol:
 *   entry == α (0)  fresh entry    — initialise state, produce first value (γ) or fail (ω)
 *   entry == β (1)  backtrack      — advance state, produce next value (γ) or fail (ω)
 *   return IS_FAIL_fn(result)  →  ω fired (exhausted)
 *   return !IS_FAIL_fn(result) →  γ fired (value = result)
 *============================================================================================================================*/

#ifndef ICON_GEN_H
#define ICON_GEN_H

#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "../../runtime/x86/snobol4.h"   /* DESCR_t, FAILDESCR, IS_FAIL_fn, α/β */

/*----------------------------------------------------------------------------------------------------------------------------
 * Entry ports — reuse α/β from bb_box.h when included together, otherwise define locally.
 *--------------------------------------------------------------------------------------------------------------------------*/
#ifndef BB_ALPHA_DEFINED
static const int α = 0;   /* fresh entry  */
static const int β = 1;   /* backtrack re-entry */
#define BB_ALPHA_DEFINED 1
#endif

/*----------------------------------------------------------------------------------------------------------------------------
 * icn_box_fn — the universal Icon generator function signature.
 *
 *   DESCR_t result = fn(zeta, α);   // fresh: allocate state, produce first value
 *   DESCR_t result = fn(zeta, β);   // backtrack: advance state, produce next value
 *   IS_FAIL_fn(result)              // ω: generator exhausted
 *--------------------------------------------------------------------------------------------------------------------------*/
typedef DESCR_t (*icn_box_fn)(void *zeta, int entry);

/*----------------------------------------------------------------------------------------------------------------------------
 * icn_gen_t — a live generator instance (function + opaque state pointer).
 * Mirrors bb_node_t.  zeta is allocated by the box on first α entry (or pre-allocated
 * by icn_eval_gen for stateful boxes).
 *--------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    icn_box_fn  fn;    /* box implementation                    */
    void       *zeta;  /* per-invocation state (box-owned, heap) */
} icn_gen_t;

/*----------------------------------------------------------------------------------------------------------------------------
 * ICN_FAIL_GEN — a generator that immediately fires ω.  Used as a sentinel / no-op.
 *--------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t icn_fail_box(void *zeta, int entry) { (void)zeta; (void)entry; return FAILDESCR; }
static const icn_gen_t ICN_FAIL_GEN = { icn_fail_box, NULL };

/*----------------------------------------------------------------------------------------------------------------------------
 * icn_gen_enter — allocate (or reuse) per-invocation state, matching bb_enter() pattern.
 *--------------------------------------------------------------------------------------------------------------------------*/
static inline void *icn_gen_enter(void **pp, size_t size) {
    void *p = *pp;
    if (size) {
        if (p) memset(p, 0, size);
        else   p = *pp = calloc(1, size);
    }
    return p;
}
#define ICN_ENTER(ref, T)  ((T *)icn_gen_enter((void **)(ref), sizeof(T)))

/*----------------------------------------------------------------------------------------------------------------------------
 * icn_broker — drive a generator through all its values, calling body_fn for each.
 * Declared here; implemented in icon_gen.c (B-2).
 *--------------------------------------------------------------------------------------------------------------------------*/
int icn_broker(icn_gen_t gen, void (*body_fn)(DESCR_t val, void *arg), void *arg);

/*----------------------------------------------------------------------------------------------------------------------------
 * Box state types — allocated by icn_eval_gen (in scrip.c) and passed as zeta
 *--------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long lo; long hi; long cur; }                                        icn_to_state_t;
typedef struct { long lo; long hi; long step; long cur; }                             icn_to_by_state_t;
typedef struct { const char *str; long len; long pos; char ch[2]; }                  icn_iterate_state_t;
typedef struct { const char *needle; const char *hay; int nlen; const char *next; }  icn_find_state_t;
typedef struct {
    ucontext_t  gen_ctx;
    ucontext_t  caller_ctx;
    char       *stack;
    DESCR_t     yielded;
    int         exhausted;
    int         started;
    void      (*trampoline)(void);
    void       *trampoline_arg;
} icn_suspend_state_t;

/*----------------------------------------------------------------------------------------------------------------------------
 * Box function declarations — implemented in icon_gen.c
 *--------------------------------------------------------------------------------------------------------------------------*/
DESCR_t icn_bb_to(void *zeta, int entry);
DESCR_t icn_bb_to_by(void *zeta, int entry);
DESCR_t icn_bb_iterate(void *zeta, int entry);
DESCR_t icn_bb_suspend(void *zeta, int entry);
DESCR_t icn_bb_find(void *zeta, int entry);

/*----------------------------------------------------------------------------------------------------------------------------
 * icn_eval_gen — walk an EXPR_t tree and return an icn_gen_t.
 * Declared here; implemented in scrip.c (B-8) where interp_eval and proc tables are visible.
 * Uses EXPR_t* — callers must have ir.h in scope.
 *--------------------------------------------------------------------------------------------------------------------------*/
#ifndef EXPR_T_DEFINED
#define EXPR_T_DEFINED
typedef struct EXPR_t EXPR_t;  /* minimal forward when ir.h not yet included */
#endif
icn_gen_t icn_eval_gen(EXPR_t *e);

#endif /* ICON_GEN_H */
