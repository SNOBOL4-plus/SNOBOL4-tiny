/*
 * bb_tab.c — TAB and RTAB Byrd Boxes (M-DYN-5)
 *
 * TAB(n):  advance cursor TO position n if Δ ≤ n; else ω.
 *          No-advance match (zero-width) — β always ω (cursor-assertion box).
 * RTAB(n): advance cursor TO position (Ω-n) if Δ ≤ (Ω-n); else ω.
 *
 * TAB differs from POS:
 *   POS(n):  succeeds only if Δ == n  (exact position assert, no advance)
 *   TAB(n):  succeeds if   Δ <= n     (advance TO n, then assert)
 *
 * SNOBOL4 spec §4.3:
 *   TAB(n) matches the null string at cursor position n, advancing the
 *   cursor from its current position to n.  If the cursor is already past
 *   position n, the match fails.
 *
 * Three-column layout — one function per box, all ports:
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     TAB_α:              if (Δ > n)                     → TAB_ω
 *                         TAB = spec(Σ+Δ, n-Δ); Δ = n;  → TAB_γ
 *     TAB_β:              Δ -= (old advance);            → TAB_ω  (cursor assert — no retry)
 *     TAB_γ:              return TAB;
 *     TAB_ω:              return spec_empty;
 *
 * State ζ: the advance amount saved for β restore.
 */

#include "bb_box.h"
#include <stdlib.h>

/* ── TAB ─────────────────────────────────────────────────────────────────── */
typedef struct { int n; int advance; } tab_t;

spec_t bb_tab(tab_t **ζζ, int entry)
{
    tab_t *ζ = *ζζ;

    if (entry == α)                                     goto TAB_α;
    if (entry == β)                                     goto TAB_β;

    /*------------------------------------------------------------------------*/
    spec_t          TAB;

    TAB_α:          if (Δ > ζ->n)                      goto TAB_ω;
                    TAB = spec(Σ+Δ, ζ->n - Δ);
                    ζ->advance = ζ->n - Δ;
                    Δ = ζ->n;                           goto TAB_γ;

    TAB_β:          Δ -= ζ->advance;                   goto TAB_ω;

    /*------------------------------------------------------------------------*/
    TAB_γ:                                              return TAB;
    TAB_ω:                                              return spec_empty;
}

tab_t *bb_tab_new(int n)
{
    tab_t *ζ = calloc(1, sizeof(tab_t));
    ζ->n = n;
    return ζ;
}

/* ── RTAB ────────────────────────────────────────────────────────────────── */
typedef struct { int n; int advance; } rtab_t;

spec_t bb_rtab(rtab_t **ζζ, int entry)
{
    rtab_t *ζ = *ζζ;

    if (entry == α)                                     goto RTAB_α;
    if (entry == β)                                     goto RTAB_β;

    /*------------------------------------------------------------------------*/
    spec_t          RTAB;

    RTAB_α:         if (Δ > Ω - ζ->n)                  goto RTAB_ω;
                    RTAB = spec(Σ+Δ, (Ω - ζ->n) - Δ);
                    ζ->advance = (Ω - ζ->n) - Δ;
                    Δ = Ω - ζ->n;                       goto RTAB_γ;

    RTAB_β:         Δ -= ζ->advance;                   goto RTAB_ω;

    /*------------------------------------------------------------------------*/
    RTAB_γ:                                             return RTAB;
    RTAB_ω:                                             return spec_empty;
}

rtab_t *bb_rtab_new(int n)
{
    rtab_t *ζ = calloc(1, sizeof(rtab_t));
    ζ->n = n;
    return ζ;
}
