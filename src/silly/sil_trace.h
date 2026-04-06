/*
 * sil_trace.h — Tracing procedures (v311.sil §16 lines 5466–5827)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Date:    2026-04-06
 * Milestone: M16
 */

#ifndef SIL_TRACE_H
#define SIL_TRACE_H

#include "sil_types.h"

SIL_result TRACE_fn(void);    /* TRACE(V,R,T,F)         */
SIL_result STOPTR_fn(void);   /* STOPTR(V,R)            */
SIL_result FENTR_fn(void);    /* FENTR  — call trace    */
SIL_result FENTR2_fn(DESCR_t name); /* FENTR2 std entry  */
SIL_result KEYTR_fn(void);    /* KEYTR  — keyword trace */
SIL_result LABTR_fn(void);    /* LABTR  — label trace   */
SIL_result TRPHND_fn(DESCR_t atptr); /* trace handler    */
SIL_result VALTR_fn(void);    /* VALTR  — value trace   */
SIL_result FNEXTR_fn(void);   /* FNEXTR — return trace  */
SIL_result FNEXT2_fn(DESCR_t name);  /* FNEXT2 ftrace    */
SIL_result SETEXIT_fn(void);  /* SETEXIT(LBL)           */
SIL_result XITHND_fn(void);   /* SETEXIT handler        */

#endif /* SIL_TRACE_H */
