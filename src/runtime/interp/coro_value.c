/*============================================================================================================================
 * coro_value.c — RS-17a: pure-BB value-context evaluator for Icon Byrd boxes.
 *
 * See coro_value.h for the contract and migration plan.
 *
 * Today this delegates to `eval_node` for kinds that are SNOBOL4/Icon-identical
 * (literals, keywords) and adds an Icon-frame-aware E_VAR shim that reads the
 * slot-indexed local from FRAME.env when frame_depth > 0.  All other kinds
 * fall through to `interp_eval` — that fallthrough is intentional scaffolding
 * for the incremental migration of coro_runtime.c call sites (RS-17a-cont).
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * SPRINT:  RS-17a (2026-05-03)
 *==========================================================================================================================*/

#include "coro_value.h"
#include "coro_runtime.h"   /* FRAME, frame_depth, scan_pos, scan_subj */
#include "snobol4.h"
#include <string.h>

/* eval_node lives in src/runtime/x86/eval_code.c — IR-free expression evaluator. */
extern DESCR_t eval_node(EXPR_t *e);

/* interp_eval is the IR-mode tree-walker; bb_eval_value falls through here for
 * kinds it does not yet handle directly.  RS-17a-cont rungs replace each
 * fallthrough with an in-this-file branch until this declaration goes away
 * (and the isolation gate can promote coro_runtime.c). */
extern DESCR_t interp_eval(EXPR_t *e);

/*------------------------------------------------------------------------------------------------------------------------------
 * bb_eval_value — evaluate e in value context.
 *
 * Value context means: produce a single DESCR_t result.  Generator-context
 * sub-expressions (E_TO/E_TO_BY/E_ALTERNATE/E_FNC user-proc/etc.) fall through
 * to interp_eval today; routing them through coro_eval + a single bb_broker
 * BB_ONCE pump is RS-17a-cont work.
 *
 * Icon-frame E_VAR shim mirrors interp_eval.c lines 353-372: when frame_depth>0
 * we read scan/letter keywords directly and use slot-indexed locals from
 * FRAME.env[ival].  Outside an Icon frame, E_VAR delegates to eval_node which
 * does the SNOBOL4 NV_GET_fn lookup.
 *----------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bb_eval_value(EXPR_t *e)
{
    if (!e) return NULVCL;

    /* Icon-frame-aware E_VAR: slot read when inside an Icon procedure call. */
    if (e->kind == E_VAR && frame_depth > 0) {
        if (e->sval && e->sval[0] == '&') {
            const char *kw = e->sval + 1;
            if (!strcmp(kw, "subject")) return scan_subj ? STRVAL(scan_subj) : NULVCL;
            if (!strcmp(kw, "pos"))     return INTVAL(scan_pos);
            if (!strcmp(kw, "letters")) return STRVAL("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
            if (!strcmp(kw, "ucase"))   return STRVAL("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            if (!strcmp(kw, "lcase"))   return STRVAL("abcdefghijklmnopqrstuvwxyz");
            if (!strcmp(kw, "digits"))  return STRVAL("0123456789");
            if (!strcmp(kw, "null"))    return NULVCL;
            if (!strcmp(kw, "fail"))    return FAILDESCR;
            return NULVCL;
        }
        int slot = (int)e->ival;
        if (slot >= 0 && slot < FRAME.env_n) return FRAME.env[slot];
        if (slot < 0 && e->sval && e->sval[0] != '&') return NV_GET_fn(e->sval);
        return NULVCL;
    }

    /* Kinds that eval_node already handles identically for SNOBOL4 and Icon
     * outside-of-frame: literals, &keywords, NULVCL.  E_VAR outside an Icon
     * frame goes through eval_node's NV_GET_fn path. */
    switch (e->kind) {
    case E_ILIT:
    case E_FLIT:
    case E_QLIT:
    case E_NUL:
    case E_KEYWORD:
        return eval_node(e);
    case E_VAR:
        /* frame_depth == 0: SNOBOL4-style lookup via eval_node */
        return eval_node(e);
    default:
        break;
    }

    /* Fallthrough: RS-17a-cont rungs migrate kinds from here into the switch
     * above, replacing this call site by site. */
    return interp_eval(e);
}
