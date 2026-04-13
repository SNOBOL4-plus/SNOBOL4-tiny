/*============================================================================================================================
 * icon_gen.c — Icon Value-Generator Byrd Box Broker + Box Implementations (GOAL-ICN-BROKER)
 *
 * Architecture mirrors SNOBOL4's exec_stmt Phase 3 broker loop (stmt_exec.c):
 *   Phase 3 (SNOBOL4):  root.fn(ζ,α) → body → root.fn(ζ,β) → … → ω
 *   icn_broker (Icon):  gen.fn(ζ,α)  → body → gen.fn(ζ,β)  → … → ω
 *
 * Value type: DESCR_t (not spec_t).  Failure sentinel: FAILDESCR / IS_FAIL_fn().
 *============================================================================================================================*/

#include "icon_gen.h"
#include "../../ir/ir.h"            /* EXPR_t, EKind, E_TO, E_TO_BY, E_ITERATE, E_SUSPEND, E_FNC */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

/*============================================================================================================================
 * B-2: icn_broker — drive a generator through all its values
 *
 * Mirrors exec_stmt Phase 3:
 *   gen.fn(ζ, α)  — fresh entry; if not FAIL → body_fn(val), then gen.fn(ζ, β) loop
 *   gen.fn(ζ, β)  — re-entry (backtrack); if not FAIL → body_fn(val), continue
 *   gen.fn(ζ, β)  → FAIL (ω) → done
 *
 * Returns: number of values produced (tick count).
 *============================================================================================================================*/
int icn_broker(icn_gen_t gen, void (*body_fn)(DESCR_t val, void *arg), void *arg) {
    if (!gen.fn) return 0;

    int ticks = 0;

    /* α — fresh entry */
    DESCR_t val = gen.fn(gen.zeta, α);
    if (IS_FAIL_fn(val)) return 0;

    body_fn(val, arg);
    ticks++;

    /* β loop — re-entries until ω */
    for (;;) {
        val = gen.fn(gen.zeta, β);
        if (IS_FAIL_fn(val)) break;
        body_fn(val, arg);
        ticks++;
    }

    return ticks;
}

/*============================================================================================================================
 * B-3: icn_bb_to — E_TO Byrd box  (i to j)
 *
 * State: lo, hi, cur.
 *   α: cur = lo; if cur > hi → ω; else return integer cur (γ).
 *   β: cur++; if cur > hi → ω; else return integer cur (γ).
 *============================================================================================================================*/
typedef struct { long lo; long hi; long cur; } icn_to_state_t;

DESCR_t icn_bb_to(void *zeta, int entry) {
    icn_to_state_t *z = (icn_to_state_t *)zeta;
    if (entry == α) z->cur = z->lo;
    else            z->cur++;
    if (z->cur > z->hi) return FAILDESCR;
    return (DESCR_t){ .v = DT_I, .i = z->cur };
}

/*============================================================================================================================
 * B-4: icn_bb_to_by — E_TO_BY Byrd box  (i to j by k)
 *
 * State: lo, hi, step, cur.
 *   α: cur = lo.
 *   β: cur += step.
 *   if step > 0: cur > hi → ω.   if step < 0: cur < hi → ω.
 *============================================================================================================================*/
typedef struct { long lo; long hi; long step; long cur; } icn_to_by_state_t;

DESCR_t icn_bb_to_by(void *zeta, int entry) {
    icn_to_by_state_t *z = (icn_to_by_state_t *)zeta;
    if (entry == α) z->cur = z->lo;
    else            z->cur += z->step;
    long step = z->step ? z->step : 1;
    if (step > 0 && z->cur > z->hi) return FAILDESCR;
    if (step < 0 && z->cur < z->hi) return FAILDESCR;
    return (DESCR_t){ .v = DT_I, .i = z->cur };
}

/*============================================================================================================================
 * B-5: icn_bb_iterate — E_ITERATE Byrd box  (!str)
 *
 * State: str, len, pos.
 *   α: pos = 0.
 *   β: pos++.
 *   if pos >= len → ω; else return single-char string at pos (γ).
 *============================================================================================================================*/
typedef struct { const char *str; long len; long pos; char ch[2]; } icn_iterate_state_t;

DESCR_t icn_bb_iterate(void *zeta, int entry) {
    icn_iterate_state_t *z = (icn_iterate_state_t *)zeta;
    if (entry == α) z->pos = 0;
    else            z->pos++;
    if (z->pos >= z->len) return FAILDESCR;
    z->ch[0] = z->str[z->pos];
    z->ch[1] = '\0';
    return (DESCR_t){ .v = DT_S, .slen = 1, .s = z->ch };
}

/*============================================================================================================================
 * B-6: icn_bb_suspend — E_SUSPEND Byrd box (coroutine wrapper)
 *
 * Wraps existing ucontext coroutine machinery from scrip.c.
 * The zeta pointer carries an opaque Icn_coro_entry* already set up by the caller.
 * α: start (fresh call) — swapcontext into coroutine.
 * β: resume — swapcontext into coroutine again.
 * ω: coroutine set exhausted=1.
 *
 * This box does NOT own the coroutine — icn_eval_gen (B-8) wires it up.
 * The zeta is cast to icn_suspend_state_t which the broker caller populates.
 *============================================================================================================================*/
typedef struct {
    ucontext_t  gen_ctx;
    ucontext_t  caller_ctx;
    char       *stack;
    DESCR_t     yielded;
    int         exhausted;
    int         started;
    void      (*trampoline)(void);   /* entry function for makecontext */
    void       *trampoline_arg;      /* passed via global before makecontext */
} icn_suspend_state_t;

DESCR_t icn_bb_suspend(void *zeta, int entry) {
    icn_suspend_state_t *z = (icn_suspend_state_t *)zeta;
    if (z->exhausted) return FAILDESCR;
    if (entry == α && !z->started) {
        /* First α: set up and enter the coroutine */
        z->started = 1;
        getcontext(&z->gen_ctx);
        z->gen_ctx.uc_stack.ss_sp   = z->stack;
        z->gen_ctx.uc_stack.ss_size = 256 * 1024;
        z->gen_ctx.uc_link          = NULL;
        makecontext(&z->gen_ctx, z->trampoline, 0);
        swapcontext(&z->caller_ctx, &z->gen_ctx);
    } else {
        /* β or α-after-started: resume */
        swapcontext(&z->caller_ctx, &z->gen_ctx);
    }
    if (z->exhausted) return FAILDESCR;
    return z->yielded;
}

/*============================================================================================================================
 * B-7: icn_bb_find — find() generator Byrd box
 *
 * State: needle, haystack, pos (byte offset into haystack, 0-based).
 *   α: pos = 0, find first match.
 *   β: advance past last match, find next.
 *   returns 1-based position of match, or ω.
 *============================================================================================================================*/
typedef struct { const char *needle; const char *hay; int nlen; const char *next; } icn_find_state_t;

DESCR_t icn_bb_find(void *zeta, int entry) {
    icn_find_state_t *z = (icn_find_state_t *)zeta;
    if (entry == α) z->next = z->hay;
    const char *hit = strstr(z->next, z->needle);
    if (!hit) return FAILDESCR;
    long pos1 = (long)(hit - z->hay) + 1;   /* 1-based */
    z->next = hit + (z->nlen > 0 ? z->nlen : 1);
    return (DESCR_t){ .v = DT_I, .i = pos1 };
}

/*============================================================================================================================
 * icn_eval_gen — forward declaration (B-8); stub until then
 *============================================================================================================================*/
icn_gen_t icn_eval_gen(struct _EXPR_t *e) {
    (void)e;
    return ICN_FAIL_GEN;   /* replaced fully in B-8 */
}

/*============================================================================================================================
 * Unit tests: B-2 constant box, B-3 icn_bb_to, B-4 icn_bb_to_by, B-5 icn_bb_iterate, B-7 icn_bb_find
 *============================================================================================================================*/
#ifdef ICON_GEN_UNIT_TEST

typedef struct { DESCR_t value; int fired; } const_box_state_t;

static DESCR_t const_box_fn(void *zeta, int entry) {
    const_box_state_t *z = (const_box_state_t *)zeta;
    if (entry == α && !z->fired) { z->fired = 1; return z->value; }
    return FAILDESCR;
}

typedef struct { long *vals; int n; int cap; } collector_t;
static void collect_int(DESCR_t val, void *arg) {
    collector_t *c = (collector_t *)arg;
    if (c->n < c->cap) c->vals[c->n++] = val.i;
}

typedef struct { char *buf; int idx; int cap; } str_collector_t;
static void collect_str(DESCR_t val, void *arg) {
    str_collector_t *c = (str_collector_t *)arg;
    if (c->idx < c->cap && val.s) c->buf[c->idx++] = val.s[0];
}

static int test_fail = 0;
#define ASSERT(cond, msg) do { if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); test_fail=1; } } while(0)

int main(void) {
    /* B-2: constant box → 1 tick, value=42 */
    {
        const_box_state_t *s = calloc(1, sizeof(*s));
        s->value = (DESCR_t){ .v = DT_I, .i = 42 };
        icn_gen_t gen = { const_box_fn, s };
        long vals[8]; collector_t c = { vals, 0, 8 };
        int ticks = icn_broker(gen, collect_int, &c);
        ASSERT(ticks == 1, "B-2: ticks==1");
        ASSERT(c.n == 1 && vals[0] == 42, "B-2: value==42");
        free(s);
    }

    /* B-3: icn_bb_to (1 to 5) → 1,2,3,4,5 */
    {
        icn_to_state_t *s = calloc(1, sizeof(*s));
        s->lo = 1; s->hi = 5;
        icn_gen_t gen = { icn_bb_to, s };
        long vals[8]; collector_t c = { vals, 0, 8 };
        int ticks = icn_broker(gen, collect_int, &c);
        ASSERT(ticks == 5, "B-3: ticks==5");
        ASSERT(c.n==5 && vals[0]==1 && vals[4]==5, "B-3: values 1..5");
        free(s);
    }

    /* B-4: icn_bb_to_by (1 to 10 by 2) → 1,3,5,7,9 */
    {
        icn_to_by_state_t *s = calloc(1, sizeof(*s));
        s->lo = 1; s->hi = 10; s->step = 2;
        icn_gen_t gen = { icn_bb_to_by, s };
        long vals[8]; collector_t c = { vals, 0, 8 };
        int ticks = icn_broker(gen, collect_int, &c);
        ASSERT(ticks == 5, "B-4: ticks==5");
        ASSERT(vals[0]==1 && vals[1]==3 && vals[4]==9, "B-4: values 1,3,5,7,9");
        free(s);
    }

    /* B-5: icn_bb_iterate !("abc") → 'a','b','c' */
    {
        icn_iterate_state_t *s = calloc(1, sizeof(*s));
        s->str = "abc"; s->len = 3;
        icn_gen_t gen = { icn_bb_iterate, s };
        char got[4] = {0};
        str_collector_t sc = { got, 0, 3 };
        icn_broker(gen, collect_str, &sc);
        ASSERT(strcmp(got, "abc") == 0, "B-5: iterate abc");
        free(s);
    }

    /* B-7: icn_bb_find find("is","this is it") → 3,6 */
    {
        icn_find_state_t *s = calloc(1, sizeof(*s));
        s->needle = "is"; s->hay = "this is it"; s->nlen = 2; s->next = s->hay;
        icn_gen_t gen = { icn_bb_find, s };
        long vals[8]; collector_t c = { vals, 0, 8 };
        int ticks = icn_broker(gen, collect_int, &c);
        ASSERT(ticks == 2, "B-7: find ticks==2");
        ASSERT(vals[0]==3 && vals[1]==6, "B-7: find positions 3,6");
        free(s);
    }

    if (!test_fail) printf("PASS: all box gates\n");
    return test_fail;
}

#endif /* ICON_GEN_UNIT_TEST */
