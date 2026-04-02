/* _XNME/_XFNME  CAPTURE     $ writes on every γ; . buffers for Phase-5 commit */
#include "bb_box.h"
#include <stdlib.h>
#include <string.h>

extern void (*NV_SET_fn)(const char*, DESCR_t);
extern void *GC_MALLOC(size_t);
typedef struct {
    bb_box_fn fn; void *fz; const char *var;
    int imm; spec_t pend; int has_p;
} capture_t;

spec_t bb_capture(void *zeta, int entry)
{
    capture_t *ζ = zeta;
    spec_t child_r;
    if (entry==α)                                                               goto CAP_α;
    if (entry==β)                                                               goto CAP_β;
    CAP_α:          child_r=ζ->fn(ζ->fz,α);                                     
                    if (spec_is_empty(child_r))                                 goto CAP_ω;
                                                                                goto CAP_γ_core;
    CAP_β:          child_r=ζ->fn(ζ->fz,β);                                     
                    if (spec_is_empty(child_r))                                 goto CAP_ω;
                                                                                goto CAP_γ_core;
    CAP_γ_core:     if (ζ->var && *ζ->var && ζ->imm) {                          
                        char *s=GC_MALLOC(child_r.δ+1);                         
                        memcpy(s,child_r.σ,(size_t)child_r.δ); s[child_r.δ]=0;  
                        DESCR_t v={.v=DT_S,.slen=(uint32_t)child_r.δ,.s=s};     
                        NV_SET_fn(ζ->var,v);                                    
                    } else if (ζ->var && *ζ->var) {                             
                        ζ->pend=child_r; ζ->has_p=1; }                          
                                                                                return child_r;
    CAP_ω:          ζ->has_p=0;                                                 return spec_empty;
}

capture_t *bb_capture_new(bb_box_fn fn, void *fz, const char *var, int imm)
{ capture_t *ζ=calloc(1,sizeof(capture_t)); ζ->fn=fn; ζ->fz=fz; ζ->var=var; ζ->imm=imm; return ζ; }
