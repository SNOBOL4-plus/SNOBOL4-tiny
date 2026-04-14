#ifndef REBUS_LOWER_H
#define REBUS_LOWER_H
/*
 * rebus_lower.h — public API for Rebus → unified IR lowering pass
 *
 * Milestone: M-G5-LOWER-REBUS-FIX
 */

#include "rebus.h"
#include "../../frontend/snobol4/scrip_cc.h"

/*
 * rebus_lower(rp)
 *   Walk RProgram* produced by rebus_parse() and return a Program*
 *   whose STMT_t list is ready for asm_emit / jvm_emit / net_emit.
 *   Returns NULL on error (messages printed to stderr).
 */
Program *rebus_lower(RProgram *rp);

/*
 * rebus_compile(src, filename) — FI-1A
 *   Full pipeline: parse src string via rebus_parse(), lower via
 *   rebus_lower(), tag all STMT_t with LANG_REB. Mirrors icon_compile().
 *   Returns NULL on parse or lower error.
 */
Program *rebus_compile(const char *src, const char *filename);

#endif /* REBUS_LOWER_H */
