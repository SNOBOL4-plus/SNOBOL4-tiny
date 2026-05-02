/* coerce.c — shared numeric-to-string coercion helpers (D-1, RS-6)
 *
 * Consolidates the int/real → DT_S coercion pattern that appeared
 * independently in icn_runtime.c (IC-8 iterate path, two sites) and
 * icon_gen.c (ICN_BINOP_CONCAT).  A single implementation using
 * icn_real_str() for round-trip-correct real formatting.
 *
 * RS-5 finding D-1: the two icn_runtime.c sites both used inline
 * snprintf + icn_real_str; icon_gen.c used %.15g which does not
 * round-trip for values needing 16 or 17 significant digits (D-2).
 * Both defects are fixed here by routing through icn_real_str.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6 (RS-6, 2026-05-02)
 */
#include "runtime/common/coerce.h"
#include <string.h>
#include <stdio.h>
#include <gc/gc.h>

/* Forward declaration — defined in src/driver/interp_eval.c, declared in
 * src/driver/interp_private.h and src/runtime/interp/icn_runtime.h */
const char *icn_real_str(double r, char *buf, int bufsz);

/* descr_to_str_icn — coerce DT_I or DT_R to DT_S using Icon semantics.
 * DT_S / DT_SNUL returned as-is.  DT_I: snprintf %lld.
 * DT_R: icn_real_str adaptive 15-17 digit round-trip formatter.
 * All other types return FAILDESCR. */
DESCR_t descr_to_str_icn(DESCR_t d)
{
    if (IS_INT_fn(d)) {
        char *nbuf = GC_malloc(32);
        snprintf(nbuf, 32, "%lld", (long long)d.i);
        return STRVAL(nbuf);
    }
    if (IS_REAL_fn(d)) {
        char tmp[64];
        icn_real_str(d.r, tmp, sizeof tmp);
        size_t len = strlen(tmp);
        char *nbuf = GC_malloc(len + 1);
        memcpy(nbuf, tmp, len + 1);
        return STRVAL(nbuf);
    }
    /* DT_S, DT_SNUL — identity */
    if (IS_STR_fn(d) || d.v == DT_SNUL) return d;
    return FAILDESCR;
}
