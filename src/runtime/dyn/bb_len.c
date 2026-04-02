/*
 * bb_len.c — LEN(n) Byrd Box (M-DYN-2)
 *
 * Matches exactly n characters unconditionally.
 * On backtrack (β), restores cursor by n.
 *
 * Pattern:  LEN(4)
 * SNOBOL4:  LEN(4)
 *
 * Three-column layout:
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     LEN_α:              if (Δ + n > Ω)                 → LEN_ω
 *                         LEN = spec(Σ+Δ, n); Δ += n;    → LEN_γ
 *     LEN_β:              Δ -= n;                        → LEN_ω
 *     LEN_γ:                                             return LEN;
 *     LEN_ω:                                             return spec_empty;
 *
 * State ζ: n (constant — no dynamic state beyond the length itself).
 */

#include "bb_box.h"
#include <stdlib.h>

typedef struct { int n; } len_t;

spec_t bb_len(len_t **ζζ, int entry)
{
    len_t *ζ = *ζζ;

    if (entry == α)                                 goto LEN_α;
    if (entry == β)                                 goto LEN_β;

    /*------------------------------------------------------------------------*/
    spec_t         LEN;

    LEN_α:            if (Δ + ζ->n > Ω)                      goto LEN_ω;
                      LEN = spec(Σ+Δ, ζ->n); Δ += ζ->n;      goto LEN_γ;

    LEN_β:            Δ -= ζ->n;                              goto LEN_ω;

    /*------------------------------------------------------------------------*/
    LEN_γ:                                                    return LEN;
    LEN_ω:                                                    return spec_empty;
}

len_t *bb_len_new(int n)
{
    len_t *ζ = calloc(1, sizeof(len_t));
    ζ->n = n;
                                                              return ζ;
}
