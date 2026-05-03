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
 * RS-22b (2026-05-03): Arithmetic + numeric-comparison binops lifted in.
 * E_ADD/E_SUB/E_MUL/E_DIV/E_MOD/E_POW dispatch through `shared_arith` in
 * runtime/common/coerce.c (mirrors sm_interp's SM_ADD..SM_EXP path —
 * FAIL propagation, DT_S → INT, DT_SNUL → INT 0, then shared_arith).
 * E_LT/E_LE/E_GT/E_GE/E_EQ/E_NE return the right operand on success,
 * FAILDESCR on fail (Icon goal-directed convention).  E_IDENTICAL routes
 * through `icn_descr_identical` (declared in coro_runtime.h).  Note: there
 * is no E_NOT_IDENTICAL kind — `~===` lowers as E_NOT(E_IDENTICAL(...)).
 *
 * RS-22c (2026-05-03): String concat + subscript read + section read +
 * field read lifted in.  E_CAT and E_LCONCAT share `bb_str_concat`
 * (numeric operands → string via `descr_to_str_icn`, then GC_malloc'd
 * concat).  E_IDX dispatches via `subscript_get`/`subscript_get2` (already
 * exposed in snobol4.h).  E_FIELD via `data_field_ptr`.  E_SECTION/
 * E_SECTION_PLUS/E_SECTION_MINUS share `bb_section` with Icon position
 * normalization (0 → slen+1, negative → slen+1+p) — three minor variants
 * of bound computation kept inline.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * SPRINT:  RS-17a (2026-05-03), RS-22a (2026-05-03), RS-22b (2026-05-03), RS-22c (2026-05-03)
 *==========================================================================================================================*/

#include "coro_value.h"
#include "coro_runtime.h"   /* FRAME, frame_depth, scan_pos, scan_subj, icn_descr_identical, g_lang */
#include "../../driver/interp_private.h"  /* RS-22a: icn_call_builtin, icn_string_section_assign, set_and_trace, data_field_ptr, kw_assign */
#include "../common/coerce.h"             /* RS-22b: shared_arith */
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
 * bb_arith — RS-22b arithmetic dispatch helper.
 *
 * Mirrors the SM_ADD..SM_EXP path in sm_interp.c (lines 327-353): propagate
 * DT_FAIL operands, coerce DT_S → DT_I via to_int, coerce DT_SNUL → INT(0),
 * then delegate to `shared_arith` in runtime/common/coerce.c.  One helper
 * instead of six near-identical cases — and the same code path SM mode uses.
 *----------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t bb_arith(EXPR_t *e, sm_opcode_t op)
{
    if (e->nchildren < 2) return FAILDESCR;
    DESCR_t l = bb_eval_value(e->children[0]);
    DESCR_t r = bb_eval_value(e->children[1]);
    if (l.v == DT_FAIL || r.v == DT_FAIL) return FAILDESCR;
    if (l.v == DT_S)    l = INTVAL(to_int(l));
    if (r.v == DT_S)    r = INTVAL(to_int(r));
    if (l.v == DT_SNUL) l = INTVAL(0);
    if (r.v == DT_SNUL) r = INTVAL(0);
    return shared_arith(l, r, op);
}

/*------------------------------------------------------------------------------------------------------------------------------
 * bb_numrel — RS-22b numeric relational dispatch helper.
 *
 * Mirrors NUMREL macro in interp_eval.c (lines 3375-3384): both operands
 * coerce to double (DT_R direct, DT_I cast, anything else → 0); compare;
 * succeed → return RIGHT operand (Icon goal-directed convention; right
 * operand survives so `2 < (1 to 4)` filters generators); fail → FAILDESCR.
 *----------------------------------------------------------------------------------------------------------------------------*/
typedef enum { BBR_LT, BBR_LE, BBR_GT, BBR_GE, BBR_EQ, BBR_NE } bb_relop_t;

static DESCR_t bb_numrel(EXPR_t *e, bb_relop_t op)
{
    if (e->nchildren < 2) return FAILDESCR;
    DESCR_t l = bb_eval_value(e->children[0]);
    DESCR_t r = bb_eval_value(e->children[1]);
    if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
    double lv = (l.v == DT_R) ? l.r : (double)(l.v == DT_I ? l.i : 0);
    double rv = (r.v == DT_R) ? r.r : (double)(r.v == DT_I ? r.i : 0);
    int ok;
    switch (op) {
    case BBR_LT: ok = (lv <  rv); break;
    case BBR_LE: ok = (lv <= rv); break;
    case BBR_GT: ok = (lv >  rv); break;
    case BBR_GE: ok = (lv >= rv); break;
    case BBR_EQ: ok = (lv == rv); break;
    case BBR_NE: ok = (lv != rv); break;
    default:     ok = 0;          break;
    }
    return ok ? r : FAILDESCR;
}

/*------------------------------------------------------------------------------------------------------------------------------
 * bb_str_concat — RS-22c string-concat helper (E_CAT + E_LCONCAT).
 *
 * Icon `||` (E_CAT) and `|||` (E_LCONCAT) both reach here via the BB
 * adapter.  Mirrors the IR-mode E_LCONCAT case at interp_eval.c:4037 —
 * coerce numeric operands via descr_to_str_icn (round-trip-correct real
 * formatting), VARVAL_fn for everything else, GC_malloc'd concat.
 *
 * Pattern operands: do not occur in BB-engine call sites today (Icon
 * never produces them; SNOBOL4's pattern-context paths never reach
 * bb_eval_value).  If one ever did, descr_to_str_icn would fail-through
 * to FAILDESCR rather than producing garbage.
 *----------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t bb_str_concat(EXPR_t *e)
{
    if (e->nchildren < 2) return NULVCL;
    DESCR_t a = bb_eval_value(e->children[0]);
    DESCR_t b = bb_eval_value(e->children[1]);
    if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) return FAILDESCR;
    DESCR_t as = descr_to_str_icn(a);
    DESCR_t bs = descr_to_str_icn(b);
    const char *asp = (as.v == DT_S || as.v == DT_SNUL) ? VARVAL_fn(as) : NULL;
    const char *bsp = (bs.v == DT_S || bs.v == DT_SNUL) ? VARVAL_fn(bs) : NULL;
    if (!asp) asp = "";
    if (!bsp) bsp = "";
    size_t al = strlen(asp), bl = strlen(bsp);
    char *buf = GC_malloc(al + bl + 1);
    memcpy(buf, asp, al);
    memcpy(buf + al, bsp, bl);
    buf[al + bl] = '\0';
    return STRVAL(buf);
}

/*------------------------------------------------------------------------------------------------------------------------------
 * bb_section — RS-22c string section helper.
 *
 * Mirrors interp_eval.c:4070-4125 for E_SECTION (s[i:j]), E_SECTION_PLUS
 * (s[i+:n]), E_SECTION_MINUS (s[i-:n]).  Icon position rules:
 *   p ≥ 1     → 1-based position (1 is before first char)
 *   p == 0    → position past last char (= slen+1)
 *   p < 0     → slen+1+p   (-1 → slen, -2 → slen-1, ...)
 * Out-of-bounds after normalization → FAILDESCR.
 *----------------------------------------------------------------------------------------------------------------------------*/
typedef enum { BBS_RANGE, BBS_PLUS, BBS_MINUS } bb_section_t;

static DESCR_t bb_section(EXPR_t *e, bb_section_t kind)
{
    if (e->nchildren < 3) return NULVCL;
    DESCR_t sd = bb_eval_value(e->children[0]);
    if (IS_FAIL_fn(sd)) return FAILDESCR;
    const char *s = VARVAL_fn(sd);
    if (!s) s = "";
    int slen = (int)strlen(s);
    DESCR_t a1 = bb_eval_value(e->children[1]);
    DESCR_t a2 = bb_eval_value(e->children[2]);
    if (IS_FAIL_fn(a1) || IS_FAIL_fn(a2)) return FAILDESCR;
    int i = (int)to_int(a1);
    int x = (int)to_int(a2);   /* j for RANGE, n for PLUS/MINUS */
    if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
    int lo, hi;
    if (kind == BBS_RANGE) {
        if (x == 0) x = slen + 1; else if (x < 0) x = slen + 1 + x;
        if (i < 1 || i > slen+1 || x < 1 || x > slen+1) return FAILDESCR;
        lo = i < x ? i : x;
        hi = i < x ? x : i;
    } else if (kind == BBS_PLUS) {
        if (i < 1 || i > slen+1) return FAILDESCR;
        if (x >= 0) { lo = i;     hi = i + x; }
        else        { lo = i + x; hi = i;     }
        if (lo < 1 || hi > slen+1) return FAILDESCR;
    } else /* BBS_MINUS */ {
        if (i < 1 || i > slen+1) return FAILDESCR;
        if (x >= 0) { lo = i - x; hi = i;     }
        else        { lo = i;     hi = i - x; }
        if (lo < 1 || hi > slen+1) return FAILDESCR;
    }
    int len = hi - lo;
    char *buf = GC_malloc(len + 1);
    memcpy(buf, s + lo - 1, len);
    buf[len] = '\0';
    return STRVAL(buf);
}

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

    /* RS-22b: arithmetic binops — bb_arith handles FAIL prop, string→int,
     * SNUL→0, then dispatches via shared_arith (the same path SM mode uses).
     * E_POW: shared_arith maps SM_EXP → integer for non-negative int^int,
     * else REALVAL(pow(...)).  This matches Icon `^` (always real if any
     * operand is real) and SNOBOL4 `**` (int when both ints, exp >= 0). */
    case E_ADD: return bb_arith(e, SM_ADD);
    case E_SUB: return bb_arith(e, SM_SUB);
    case E_MUL: return bb_arith(e, SM_MUL);
    case E_DIV: return bb_arith(e, SM_DIV);
    case E_MOD: return bb_arith(e, SM_MOD);
    case E_POW: return bb_arith(e, SM_EXP);

    /* RS-22b: numeric relational binops — succeed → return right operand,
     * fail → FAILDESCR.  Right-operand-on-success is Icon goal-directed
     * convention (lets `2 < (1 to 4)` filter a generator chain). */
    case E_LT: return bb_numrel(e, BBR_LT);
    case E_LE: return bb_numrel(e, BBR_LE);
    case E_GT: return bb_numrel(e, BBR_GT);
    case E_GE: return bb_numrel(e, BBR_GE);
    case E_EQ: return bb_numrel(e, BBR_EQ);
    case E_NE: return bb_numrel(e, BBR_NE);

    /* RS-22b: deep identity (Icon `===`).  Right-operand-on-success again.
     * Note: `~===` does NOT lower to a distinct EXPR kind — Icon's parser
     * emits E_NOT(E_IDENTICAL(a, b)).  When E_NOT is lifted (RS-22d), the
     * full `~===` path becomes IR-free; until then E_NOT still falls through
     * to interp_eval and walks back here for its E_IDENTICAL child. */
    case E_IDENTICAL: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = bb_eval_value(e->children[0]);
        DESCR_t r = bb_eval_value(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        return icn_descr_identical(l, r) ? r : FAILDESCR;
    }

    /* RS-22c: string concat — Icon `||` (E_CAT) and `|||` (E_LCONCAT)
     * share bb_str_concat.  In Icon BB context neither produces patterns,
     * so the simple coerce-and-concat path is correct (mirrors IR-mode
     * E_LCONCAT at interp_eval.c:4037).  SNOBOL4 E_CAT in pattern context
     * does not arrive here — that path is interp_eval_pat-only. */
    case E_CAT:
    case E_LCONCAT:
        return bb_str_concat(e);

    /* RS-22c: subscript read — table/list/record/string index.
     * Two-arg form → subscript_get; three-arg form (s[i:j] lowered as
     * E_IDX with two index children) → subscript_get2.  Mirrors
     * interp_eval.c:3084. */
    case E_IDX: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t base = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(base)) return FAILDESCR;
        if (e->nchildren == 2) {
            DESCR_t idx = bb_eval_value(e->children[1]);
            if (IS_FAIL_fn(idx)) return FAILDESCR;
            return subscript_get(base, idx);
        }
        DESCR_t i1 = bb_eval_value(e->children[1]);
        DESCR_t i2 = bb_eval_value(e->children[2]);
        if (IS_FAIL_fn(i1) || IS_FAIL_fn(i2)) return FAILDESCR;
        return subscript_get2(base, i1, i2);
    }

    /* RS-22c: record field read.  e->sval = field name, child[0] = object. */
    case E_FIELD: {
        if (!e->sval || e->nchildren < 1) return NULVCL;
        DESCR_t obj = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(obj)) return FAILDESCR;
        DESCR_t *cell = data_field_ptr(e->sval, obj);
        if (!cell) return FAILDESCR;
        return *cell;
    }

    /* RS-22c: string section (Icon s[i:j], s[i+:n], s[i-:n]) — bb_section.
     * Three minor variants of bound computation share one helper. */
    case E_SECTION:        return bb_section(e, BBS_RANGE);
    case E_SECTION_PLUS:   return bb_section(e, BBS_PLUS);
    case E_SECTION_MINUS:  return bb_section(e, BBS_MINUS);

    default:
        break;
    }

    /* Fallthrough: RS-17a-cont rungs migrate kinds from here into the switch
     * above, replacing this call site by site. */
    return interp_eval(e);
}
