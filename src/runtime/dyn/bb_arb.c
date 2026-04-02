/*
 * bb_arb.c — ARB and REM Byrd Boxes (M-DYN-2)
 *
 * ARB: matches 0, 1, 2, … characters lazily.
 *   α: succeeds immediately with 0 chars (Δ unchanged).
 *   β: tries one more character each time; fails when Δ+tried > Ω.
 *   Cursor is restored to `start` on each β, then advanced by `tried`.
 *
 * REM: matches the entire remainder of the subject.
 *   α: succeeds with spec(Σ+Δ, Ω-Δ), advances Δ to Ω.
 *   β: always ω (no backtrack — REM is a one-shot consume).
 *
 * Three-column layout (ARB):
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     ARB_α:              tried=0; start=Δ;              → ARB_γ (0-width)
 *     ARB_β:              tried++; if start+tried>Ω      → ARB_ω
 *                         Δ=start; Δ+=tried;             → ARB_γ
 *     ARB_γ:                                             return spec(Σ+start, tried);
 *     ARB_ω:                                             return spec_empty;
 *
 * State ζ (ARB): tried count + start cursor.
 * State ζ (REM): none (dummy).
 */

#include "bb_box.h"
#include <stdlib.h>

/* ── ARB ─────────────────────────────────────────────────────────────────── */
typedef struct { int tried; int start; } arb_t;

spec_t bb_arb(arb_t **ζζ, int entry)
{
    arb_t *ζ = *ζζ;

    if (entry == α)                                 goto ARB_α;
    if (entry == β)                                 goto ARB_β;

    /*------------------------------------------------------------------------*/
    spec_t         ARB;

    ARB_α:        ζ->tried = 0;
                  ζ->start = Δ;
                  ARB = spec(Σ+Δ, 0);                         goto ARB_γ;

    ARB_β:        ζ->tried++;
                  if (ζ->start + ζ->tried > Ω)                goto ARB_ω;
                  Δ = ζ->start;
                  ARB = spec(Σ+Δ, ζ->tried);
                  Δ += ζ->tried;                              goto ARB_γ;

    /*------------------------------------------------------------------------*/
    ARB_γ:                                                    return ARB;
    ARB_ω:                                                    return spec_empty;
}

arb_t *bb_arb_new(void)
{
                                                              return calloc(1, sizeof(arb_t));
}

/* ── REM ─────────────────────────────────────────────────────────────────── */
typedef struct { int dummy; } rem_t;

spec_t bb_rem(rem_t **ζζ, int entry)
{
    (void)ζζ;

    if (entry == α)                                 goto REM_α;
    if (entry == β)                                 goto REM_β;

    /*------------------------------------------------------------------------*/
    spec_t         REM;

    REM_α:        REM = spec(Σ+Δ, Ω-Δ); Δ = Ω;               goto REM_γ;

    REM_β:                                                    goto REM_ω;

    /*------------------------------------------------------------------------*/
    REM_γ:                                                    return REM;
    REM_ω:                                                    return spec_empty;
}

rem_t *bb_rem_new(void)
{
                                                              return calloc(1, sizeof(rem_t));
}
