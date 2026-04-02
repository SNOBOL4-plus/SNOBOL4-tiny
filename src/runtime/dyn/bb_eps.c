/*
 * bb_eps.c — EPS (epsilon), SUCCEED, and FAIL Byrd Boxes (M-DYN-2)
 *
 * EPS (epsilon):
 *   α: succeeds once with zero-width match.  Sets done=1 to prevent
 *      double-success if somehow called again without reset.
 *   β: always ω.
 *   Used as the identity element in SEQ and as placeholder for unimplemented
 *   or degenerate nodes.
 *
 * SUCCEED:
 *   Always γ, zero-width, on every call (α or β).
 *   Used for the SNOBOL4 SUCCEED pattern function.
 *   The outer scan loop provides the "infinite" retry behavior.
 *
 * FAIL:
 *   Always ω on every call.
 *   Used for the SNOBOL4 FAIL pattern function.
 *
 * Three-column layout (EPS):
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     EPS_α:              if done goto EPS_ω;
 *                         done=1; EPS=spec(Σ+Δ,0);       → EPS_γ
 *     EPS_β:                                             → EPS_ω
 *     EPS_γ:                                             return EPS;
 *     EPS_ω:                                             return spec_empty;
 *
 * State ζ (EPS): done flag.
 * State ζ (SUCCEED/FAIL): none (dummy).
 */

#include "bb_box.h"
#include <stdlib.h>

/* ── EPS ─────────────────────────────────────────────────────────────────── */
typedef struct { int done; } eps_t;

spec_t bb_eps(eps_t **ζζ, int entry)
{
    eps_t *ζ = *ζζ;

    if (entry == α) { ζ->done = 0; goto EPS_α; }
    if (entry == β)                              goto EPS_β;

    /*------------------------------------------------------------------------*/
    spec_t EPS;

    EPS_α:  if (ζ->done)                                      goto EPS_ω;
            ζ->done = 1;
            EPS = spec(Σ+Δ, 0);                               goto EPS_γ;

    EPS_β:                                                    goto EPS_ω;

    /*------------------------------------------------------------------------*/
    EPS_γ:                                                    return EPS;
    EPS_ω:                                                    return spec_empty;
}

eps_t *bb_eps_new(void)
{
                                                              return calloc(1, sizeof(eps_t));
}

/* ── SUCCEED ─────────────────────────────────────────────────────────────── */
typedef struct { int dummy; } succeed_t;

spec_t bb_succeed(succeed_t **ζζ, int entry)
{
    (void)ζζ; (void)entry;
                                                              return spec(Σ+Δ, 0);
}

succeed_t *bb_succeed_new(void)
{
                                                              return calloc(1, sizeof(succeed_t));
}

/* ── FAIL ────────────────────────────────────────────────────────────────── */
typedef struct { int dummy; } fail_t;

spec_t bb_fail(fail_t **ζζ, int entry)
{
    (void)ζζ; (void)entry;
                                                              return spec_empty;
}

fail_t *bb_fail_new(void)
{
                                                              return calloc(1, sizeof(fail_t));
}
