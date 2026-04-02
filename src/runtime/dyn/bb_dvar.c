/* _XDSAR/_XVAR  DVAR        *VAR/VAR — re-resolve live value on every α */
#include "bb_box.h"
#include <stdlib.h>
#include <string.h>

extern DESCR_t (*NV_GET_fn)(const char*);
extern bb_node_t bb_build(void*);
typedef struct { const char *name; bb_box_fn fn; void *fz; size_t fz_sz; } dvar_t;

spec_t bb_deferred_var(void *zeta, int entry)
{
    dvar_t *ζ = zeta;
    spec_t DVAR;
    if (entry==α)                                                               goto DVAR_α;
    if (entry==β)                                                               goto DVAR_β;
    DVAR_α:         { DESCR_t val=NV_GET_fn(ζ->name); int rebuilt=0;            
                      if (val.v==DT_P && val.p && val.p!=ζ->fz) {               
                          bb_node_t c=bb_build(val.p);                          
                          ζ->fn=c.fn; ζ->fz=c.ζ; ζ->fz_sz=c.ζ_size; rebuilt=1; }
                      else if (val.v==DT_S && val.s) {                          
                          _lit_t *lz=(_lit_t*)ζ->fz;                            
                          if (!lz||lz->lit!=val.s) {                            
                              lz=calloc(1,sizeof(_lit_t)); lz->lit=val.s; lz->len=(int)strlen(val.s);
                              ζ->fn=(bb_box_fn)bb_lit; ζ->fz=lz;                
                              ζ->fz_sz=sizeof(_lit_t); rebuilt=1; } }           
                      if (!rebuilt&&ζ->fz&&ζ->fz_sz&&ζ->fn!=(bb_box_fn)bb_lit)  
                          memset(ζ->fz,0,ζ->fz_sz); }                           
                    if (!ζ->fn)                                                 goto DVAR_ω;
                    DVAR=ζ->fn(ζ->fz,α);                                        
                    if (spec_is_empty(DVAR))                                    goto DVAR_ω;
                                                                                goto DVAR_γ;
    DVAR_β:         if (!ζ->fn)                                                 goto DVAR_ω;
                    DVAR=ζ->fn(ζ->fz,β);                                        
                    if (spec_is_empty(DVAR))                                    goto DVAR_ω;
                                                                                goto DVAR_γ;
    DVAR_γ:                                                                     return DVAR;
    DVAR_ω:                                                                     return spec_empty;
}

dvar_t *bb_dvar_new(const char *name)
{ dvar_t *ζ=calloc(1,sizeof(dvar_t)); ζ->name=name; return ζ; }
