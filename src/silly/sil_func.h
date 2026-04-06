/*
 * sil_func.h — Other functions (v311.sil §19 lines 6322–7037)
 *
 * Faithful C translation of §19 procedures.
 * Complex procs (OPSYN, CONVERT, DMP/DUMP, ARG/LOCAL/FIELDS, CNVTA/CNVAT)
 * are stubbed pending deeper infrastructure.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Date:    2026-04-06
 * Milestone: M12
 */

#ifndef SIL_FUNC_H
#define SIL_FUNC_H

#include "sil_types.h"

SIL_result APPLY_fn(void);
SIL_result ARG_fn(void);
SIL_result LOCAL_fn(void);
SIL_result FIELDS_fn(void);
SIL_result CLEAR_fn(void);
SIL_result CMA_fn(void);
SIL_result COLECT_fn(void);
SIL_result COPY_fn(void);
SIL_result CNVRT_fn(void);
SIL_result CODER_fn(void);
SIL_result DATE_fn(void);
SIL_result DT_fn(void);
SIL_result DMP_fn(void);
SIL_result DUMP_fn(void);
SIL_result DUPL_fn(void);
SIL_result OPSYN_fn(void);
SIL_result RPLACE_fn(void);
SIL_result REVERS_fn(void);
SIL_result SIZE_fn(void);
SIL_result SUBSTR_fn(void);
SIL_result TIME_fn(void);
SIL_result TRIM_fn(void);
SIL_result VDIFFR_fn(void);

#endif /* SIL_FUNC_H */
