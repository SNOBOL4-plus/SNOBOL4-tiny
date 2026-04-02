/* _XARBN    ARBNO       zero-or-more greedy; zero-advance guard; β unwinds stack */
#include "bb_box.h"
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#define ARBNO_STACK_MAX 64
typedef struct { spec_t acc; int pos; } arbno_frame_t;
typedef struct { bb_box_fn fn; void *fz; int top; arbno_frame_t stk[ARBNO_STACK_MAX]; } arbno_t;

spec_t bb_arbno(void *zeta, int entry)
{
    arbno_t *ζ = zeta;
    spec_t ARBNO; spec_t br; arbno_frame_t *fr;
    if (entry==α)                                                               goto ARBNO_α;
    if (entry==β)                                                               goto ARBNO_β;
    ARBNO_α:        ζ->top=0; fr=&ζ->stk[0];                                    
                    fr->acc=spec(Σ+Δ,0); fr->pos=Δ;                             
    ARBNO_try:      br=ζ->fn(ζ->fz,α);                                          
                    if (spec_is_empty(br))                                      goto body_ω;
                                                                                goto body_γ;
    ARBNO_β:        if (ζ->top<=0)                                              goto ARBNO_ω;
                    ζ->top--; fr=&ζ->stk[ζ->top]; Δ=fr->pos;                    goto ARBNO_γ;
    body_γ:         fr=&ζ->stk[ζ->top];                                         
                    if (Δ==fr->pos)                                             goto ARBNO_γ_now;
                    ARBNO=spec_cat(fr->acc,br);                                 
                    if (ζ->top+1<ARBNO_STACK_MAX) {                             
                        ζ->top++; fr=&ζ->stk[ζ->top];                           
                        fr->acc=ARBNO; fr->pos=Δ; }                             goto ARBNO_try;
    body_ω:         ARBNO=ζ->stk[ζ->top].acc;                                   goto ARBNO_γ;
    ARBNO_γ_now:    ARBNO=ζ->stk[ζ->top].acc;                                   goto ARBNO_γ;
    ARBNO_γ:                                                                    return ARBNO;
    ARBNO_ω:                                                                    return spec_empty;
}

arbno_t *bb_arbno_new(bb_box_fn fn, void *fz)
{ arbno_t *ζ=calloc(1,sizeof(arbno_t)); ζ->fn=fn; ζ->fz=fz; return ζ; }
