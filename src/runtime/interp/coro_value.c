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
 * RS-22a (2026-05-03): E_ASSIGN and E_FNC ported out of interp_eval's Icon-frame
 * switch.  All interp_eval(child) recurses replaced with bb_eval_value(child).
 * E_FNC builtins route through icn_call_builtin (already IR-free).
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * SPRINT:  RS-17a (2026-05-03), RS-22a (2026-05-03)
 *==========================================================================================================================*/

#include "coro_value.h"
#include "coro_runtime.h"   /* FRAME, frame_depth, scan_pos, scan_subj */
#include "../../driver/interp_private.h"  /* RS-22a: icn_call_builtin, icn_string_section_assign, set_and_trace, data_field_ptr, kw_assign */
#include "snobol4.h"
#include <string.h>
#include <gc/gc.h>

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

    /* RS-22a: E_ASSIGN — slot store, IDX, FIELD, ITERATE, RANDOM lvalue paths.
     * Mirrors interp_eval.c lines 373-474; all interp_eval(child) replaced
     * with bb_eval_value(child). */
    case E_ASSIGN: {
        if (e->nchildren < 2) return NULVCL;
        DESCR_t val = bb_eval_value(e->children[1]);
        if (IS_FAIL_fn(val)) return FAILDESCR;
        EXPR_t *lhs = e->children[0];
        if (lhs && (lhs->kind == E_SECTION || lhs->kind == E_SECTION_PLUS ||
                    lhs->kind == E_SECTION_MINUS)) {
            if (icn_string_section_assign(lhs, val)) return val;
            return FAILDESCR;
        }
        if (lhs && lhs->kind == E_IDX && lhs->nchildren >= 2) {
            if (icn_string_section_assign(lhs, val)) return val;
            { DESCR_t _b = bb_eval_value(lhs->children[0]);
              if (_b.v == DT_S || _b.v == DT_SNUL) return FAILDESCR; }
        }
        if (lhs && lhs->kind == E_VAR) {
            if (lhs->sval && lhs->sval[0] == '&') {
                if (!kw_assign(lhs->sval + 1, val)) return FAILDESCR;
                return val;
            }
            int slot = (int)lhs->ival;
            if (slot >= 0 && slot < FRAME.env_n) { FRAME.env[slot] = val; return val; }
            if (slot < 0 && lhs->sval && lhs->sval[0] != '&') set_and_trace(lhs->sval, val);
        } else if (lhs && lhs->kind == E_IDX && lhs->nchildren >= 2) {
            DESCR_t base = bb_eval_value(lhs->children[0]);
            if (!IS_FAIL_fn(base)) {
                DESCR_t idx = bb_eval_value(lhs->children[1]);
                if (!IS_FAIL_fn(idx)) subscript_set(base, idx, val);
            }
        } else if (lhs && lhs->kind == E_FIELD && lhs->sval && lhs->nchildren >= 1) {
            DESCR_t obj = bb_eval_value(lhs->children[0]);
            if (!IS_FAIL_fn(obj)) {
                DESCR_t *cell = data_field_ptr(lhs->sval, obj);
                if (cell) *cell = val;
            }
        } else if (lhs && lhs->kind == E_ITERATE && lhs->nchildren >= 1) {
            DESCR_t cv = bb_eval_value(lhs->children[0]);
            if (!IS_FAIL_fn(cv)) {
                if (cv.v == DT_T && cv.tbl) {
                    for (int b = 0; b < TABLE_BUCKETS; b++)
                        for (TBPAIR_t *p = cv.tbl->buckets[b]; p; p = p->next)
                            p->val = val;
                } else if (cv.v == DT_DATA) {
                    DESCR_t tag = FIELD_GET_fn(cv, "icn_type");
                    if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                        DESCR_t ea = FIELD_GET_fn(cv, "frame_elems");
                        int n = (int)FIELD_GET_fn(cv, "frame_size").i;
                        DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                        if (elems && n > 0) for (int i = 0; i < n; i++) elems[i] = val;
                    } else if (cv.u && cv.u->type && cv.u->type->nfields > 0 && cv.u->fields) {
                        for (int i = 0; i < cv.u->type->nfields; i++) cv.u->fields[i] = val;
                    }
                }
            }
        }
        return val;
    }

    /* RS-22a: E_FNC — user-proc path dispatches through proc_table → coro_call.
     * Builtin path evaluates args through bb_eval_value then calls icn_call_builtin
     * (already IR-free).  Mirror of interp_eval.c E_FNC case but recursion-safe. */
    case E_FNC: {
        if (e->nchildren < 1) return NULVCL;
        const char *fn = e->children[0] ? e->children[0]->sval : NULL;
        if (!fn) return NULVCL;
        int nargs = e->nchildren - 1;
        /* User-proc path: look up in proc_table and call via coro_call. */
        for (int i = 0; i < proc_count; i++) {
            if (strcmp(proc_table[i].name, fn) != 0) continue;
            DESCR_t *args = nargs > 0 ? GC_malloc((size_t)nargs * sizeof(DESCR_t)) : NULL;
            for (int j = 0; j < nargs; j++) args[j] = bb_eval_value(e->children[1+j]);
            DESCR_t result = coro_call(proc_table[i].proc, args, nargs);
            return result;
        }
        /* Builtin path: evaluate args through bb_eval_value, then icn_call_builtin. */
        DESCR_t *args = nargs > 0 ? GC_malloc((size_t)nargs * sizeof(DESCR_t)) : NULL;
        for (int j = 0; j < nargs; j++) {
            args[j] = bb_eval_value(e->children[1+j]);
            if (IS_FAIL_fn(args[j])) return FAILDESCR;
        }
        return icn_call_builtin(e, args, nargs);
    }

    default:
        break;
    }

    /* Fallthrough: RS-17a-cont rungs migrate kinds from here into the switch
     * above, replacing this call site by site. */
    return interp_eval(e);
}
