/*
 * bb_any.c — ANY(chars) and NOTANY(chars) Byrd Boxes (M-DYN-2)
 *
 * ANY(chars):    match one character if it IS in the set.
 * NOTANY(chars): match one character if it is NOT in the set.
 * Both advance Δ by 1 on γ; restore Δ by 1 on β.
 *
 * Pattern:  ANY('aeiou')  /  NOTANY('aeiou')
 * SNOBOL4:  ANY('aeiou')  /  NOTANY('aeiou')
 *
 * Three-column layout (ANY):
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     ANY_α:              if (!Σ[Δ] || Σ[Δ] ∉ chars)    → ANY_ω
 *                         ANY = spec(Σ+Δ, 1); Δ += 1;    → ANY_γ
 *     ANY_β:              Δ -= 1;                        → ANY_ω
 *     ANY_γ:                                             return ANY;
 *     ANY_ω:                                             return spec_empty;
 *
 * NOTANY is identical with the membership test inverted.
 *
 * State ζ: chars pointer only (advance is always 1).
 */

#include "bb_box.h"
#include <string.h>
#include <stdlib.h>

/* ── ANY ─────────────────────────────────────────────────────────────────── */
typedef struct { const char *chars; } any_t;

spec_t bb_any(any_t **ζζ, int entry)
{
    any_t *ζ = *ζζ;

    if (entry == α)                                 goto ANY_α;
    if (entry == β)                                 goto ANY_β;

    /*------------------------------------------------------------------------*/
    spec_t         ANY;

    ANY_α:            if (Δ >= Ω || !strchr(ζ->chars, Σ[Δ]))  goto ANY_ω;
                      ANY = spec(Σ+Δ, 1); Δ += 1;             goto ANY_γ;

    ANY_β:            Δ -= 1;                                  goto ANY_ω;

    /*------------------------------------------------------------------------*/
    ANY_γ:                                                    return ANY;
    ANY_ω:                                                    return spec_empty;
}

any_t *bb_any_new(const char *chars)
{
    any_t *ζ = calloc(1, sizeof(any_t));
    ζ->chars = chars;
                                                              return ζ;
}

/* ── NOTANY ──────────────────────────────────────────────────────────────── */
typedef struct { const char *chars; } notany_t;

spec_t bb_notany(notany_t **ζζ, int entry)
{
    notany_t *ζ = *ζζ;

    if (entry == α)                                 goto NOTANY_α;
    if (entry == β)                                 goto NOTANY_β;

    /*------------------------------------------------------------------------*/
    spec_t         NOTANY;

    NOTANY_α:         if (Δ >= Ω || strchr(ζ->chars, Σ[Δ]))   goto NOTANY_ω;
                      NOTANY = spec(Σ+Δ, 1); Δ += 1;          goto NOTANY_γ;

    NOTANY_β:         Δ -= 1;                                  goto NOTANY_ω;

    /*------------------------------------------------------------------------*/
    NOTANY_γ:                                                 return NOTANY;
    NOTANY_ω:                                                 return spec_empty;
}

notany_t *bb_notany_new(const char *chars)
{
    notany_t *ζ = calloc(1, sizeof(notany_t));
    ζ->chars = chars;
                                                              return ζ;
}
