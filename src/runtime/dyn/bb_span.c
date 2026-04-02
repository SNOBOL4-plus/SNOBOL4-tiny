/*
 * bb_span.c — SPAN(chars) Byrd Box (M-DYN-2)
 *
 * Matches the longest prefix of the subject consisting entirely of
 * characters in the set `chars`.  Requires at least one character.
 * On backtrack (β), restores cursor by the saved advance δ.
 *
 * Pattern:  SPAN('aeiou')
 * SNOBOL4:  SPAN('aeiou')
 *
 * Three-column layout:
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     SPAN_α:             scan chars in set → δ          → SPAN_ω if δ==0
 *                         SPAN = spec(Σ+Δ, δ); Δ += δ;   → SPAN_γ
 *     SPAN_β:             Δ -= δ;                        → SPAN_ω
 *     SPAN_γ:                                            return SPAN;
 *     SPAN_ω:                                            return spec_empty;
 *
 * State ζ: chars pointer + saved advance δ (for β restore).
 *
 * NOTE: SPAN succeeds exactly once per α — it is a maximal scan.
 * β simply undoes that single advance; there is no partial-length
 * retry (that would be ANY's job in a loop).
 */

#include "bb_box.h"
#include <string.h>
#include <stdlib.h>

typedef struct { const char *chars; int δ; } span_t;

spec_t bb_span(span_t **ζζ, int entry)
{
    span_t *ζ = *ζζ;

    if (entry == α)                                 goto SPAN_α;
    if (entry == β)                                 goto SPAN_β;

    /*------------------------------------------------------------------------*/
    spec_t         SPAN;

    SPAN_α:       for (ζ->δ = 0; Δ+ζ->δ < Ω && strchr(ζ->chars, Σ[Δ+ζ->δ]); ζ->δ++);
                  if (ζ->δ <= 0)                            goto SPAN_ω;
                  SPAN = spec(Σ+Δ, ζ->δ); Δ += ζ->δ;        goto SPAN_γ;

    SPAN_β:       Δ -= ζ->δ;                                goto SPAN_ω;

    /*------------------------------------------------------------------------*/
    SPAN_γ:                                                   return SPAN;
    SPAN_ω:                                                   return spec_empty;
}

span_t *bb_span_new(const char *chars)
{
    span_t *ζ = calloc(1, sizeof(span_t));
    ζ->chars = chars;
                                                              return ζ;
}
