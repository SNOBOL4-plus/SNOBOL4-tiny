/*
 * bb_brk.c — BREAK(chars) and BREAKX(chars) Byrd Boxes (M-DYN-2)
 *
 * BREAK(chars):  scan forward until a character IN `chars` is found.
 *                Matches the prefix BEFORE that character (may be empty
 *                if Σ[Δ] is already in the set — succeeds with δ=0).
 *                Fails if end-of-subject is reached without hitting a
 *                break character.
 *
 * BREAKX(chars): same as BREAK, but also fails if δ==0 (zero advance).
 *                Used in the BREAKX pattern which is intended to always
 *                advance at least one character.
 *
 * Three-column layout (BREAK):
 *
 *     LABEL:              ACTION                          GOTO
 *     ─────────────────────────────────────────────────────────
 *     BRK_α:              scan until Σ[Δ+δ] ∈ chars → δ → BRK_ω if EOS
 *                         BRK = spec(Σ+Δ, δ); Δ += δ;    → BRK_γ
 *     BRK_β:              Δ -= δ;                        → BRK_ω
 *     BRK_γ:                                             return BRK;
 *     BRK_ω:                                             return spec_empty;
 *
 * BREAKX adds: if (δ == 0) goto BRK_ω; after the scan.
 *
 * State ζ: chars pointer + saved advance δ (for β restore).
 */

#include "bb_box.h"
#include <string.h>
#include <stdlib.h>

/* ── BREAK ───────────────────────────────────────────────────────────────── */
typedef struct { const char *chars; int δ; } brk_t;

spec_t bb_brk(brk_t **ζζ, int entry)
{
    brk_t *ζ = *ζζ;

    if (entry == α)                                 goto BRK_α;
    if (entry == β)                                 goto BRK_β;

    /*------------------------------------------------------------------------*/
    spec_t         BRK;

    BRK_α:        for (ζ->δ = 0; Δ+ζ->δ < Ω; ζ->δ++)
                      if (strchr(ζ->chars, Σ[Δ+ζ->δ])) break;
                  if (Δ + ζ->δ >= Ω)                         goto BRK_ω;
                  BRK = spec(Σ+Δ, ζ->δ); Δ += ζ->δ;          goto BRK_γ;

    BRK_β:        Δ -= ζ->δ;                                  goto BRK_ω;

    /*------------------------------------------------------------------------*/
    BRK_γ:                                                    return BRK;
    BRK_ω:                                                    return spec_empty;
}

brk_t *bb_brk_new(const char *chars)
{
    brk_t *ζ = calloc(1, sizeof(brk_t));
    ζ->chars = chars;
                                                              return ζ;
}

/* ── BREAKX ──────────────────────────────────────────────────────────────── */
typedef struct { const char *chars; int δ; } brkx_t;

spec_t bb_breakx(brkx_t **ζζ, int entry)
{
    brkx_t *ζ = *ζζ;

    if (entry == α)                                 goto BRKX_α;
    if (entry == β)                                 goto BRKX_β;

    /*------------------------------------------------------------------------*/
    spec_t         BRKX;

    BRKX_α:       for (ζ->δ = 0; Δ+ζ->δ < Ω; ζ->δ++)
                      if (strchr(ζ->chars, Σ[Δ+ζ->δ])) break;
                  if (ζ->δ == 0)                            goto BRKX_ω;  /* zero advance → fail */
                  if (Δ + ζ->δ >= Ω)                        goto BRKX_ω;
                  BRKX = spec(Σ+Δ, ζ->δ); Δ += ζ->δ;        goto BRKX_γ;

    BRKX_β:       Δ -= ζ->δ;                                goto BRKX_ω;

    /*------------------------------------------------------------------------*/
    BRKX_γ:                                                   return BRKX;
    BRKX_ω:                                                   return spec_empty;
}

brkx_t *bb_breakx_new(const char *chars)
{
    brkx_t *ζ = calloc(1, sizeof(brkx_t));
    ζ->chars = chars;
                                                              return ζ;
}
