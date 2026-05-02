#pragma once
/* coerce.h — shared numeric-to-string coercion helpers (D-1, RS-6)
 * Used by icn_runtime.c, icon_gen.c, and any future frontend needing
 * int/real → DT_S conversion with Icon round-trip real formatting.
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6 (RS-6, 2026-05-02) */

/* snobol4.h provides DESCR_t, DT_*, IS_INT_fn, IS_REAL_fn, IS_STR_fn,
 * STRVAL, FAILDESCR — all reachable via -I$(RT)/x86 */
#include "snobol4.h"

/* descr_to_str_icn — coerce DT_I or DT_R to DT_S using Icon semantics.
 * DT_S / DT_SNUL returned as-is.  All others return FAILDESCR.
 * GC_malloc is used for the result buffer — safe in GC-managed heap. */
DESCR_t descr_to_str_icn(DESCR_t d);
