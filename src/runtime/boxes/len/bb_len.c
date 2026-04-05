/* _XLNTH    LEN         match exactly n characters (UTF-8 code points) — P3C */
#include "bb_box.h"
#include <stdlib.h>
#include <string.h>
#include "../../snobol4/utf8_utils.h"

spec_t bb_len(void *zeta, int entry)
{
    len_t *ζ = zeta;
    spec_t LEN;
    if (entry==α)                                                               goto LEN_α;
    if (entry==β)                                                               goto LEN_β;
    LEN_α: {
                    /* P3C: count remaining code points — correct failure gate */
                    size_t remaining = utf8_strlen(Σ + Δ);
                    if ((size_t)ζ->n > remaining)                               goto LEN_ω;
                    int bspan = (int)utf8_char_bytes(Σ, (size_t)Ω, (size_t)Δ, (size_t)ζ->n);
                    ζ->bspan = bspan;
                    LEN = spec(Σ+Δ, bspan); Δ += bspan;                         goto LEN_γ;
           }
    LEN_β:          Δ -= ζ->bspan;                                              goto LEN_ω;
    LEN_γ:                                                                      return LEN;
    LEN_ω:                                                                      return spec_empty;
}

len_t *bb_len_new(int n)
{ len_t *ζ=calloc(1,sizeof(len_t)); ζ->n=n; return ζ; }
