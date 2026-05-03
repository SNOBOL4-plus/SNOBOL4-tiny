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
 * RS-22d (2026-05-03): Unary + augmented-assign kinds lifted in.
 * E_MNS (unary `-` — Icon parser uses E_MNS, not the rung-text "E_NEG"),
 * E_PLS (unary `+`), E_NOT (`not`), E_NULL (`/`), E_NONNULL (`\`),
 * E_SIZE (`*`), E_RANDOM (`?`) all dispatched directly.  E_AUGOP (the
 * actual IR kind name; rung-text "E_AUGASSIGN" was a label rather than
 * the literal kind) handles all three IR-mode paths: bang-iterate lvalue
 * (`!container OP:= rhs`), generator-RHS drive (`every sum +:= (1 to n)`
 * via coro_eval + bb_node_t.fn ticks), and plain `lv OP rv` then
 * writeback.  Two helpers — `bb_augop_compute` (pure compute given lv,
 * rv, op token) and `bb_augop_writeback` (write to E_VAR slot / E_IDX /
 * E_FIELD lhs) — replace IR-mode's AUGOP_APPLY / AUGOP_CELL macros.
 * Unsupported tokens (TK_AUGPOW, TK_AUGCSET_*, TK_AUGSCAN) fall through
 * to the integer-add default — same coverage as IR-mode.
 *
 * RS-22e (2026-05-03): Fallthrough survey.  smoke_icon hits zero
 * unhandled kinds — the rung's stated gate is met.  Full Icon corpus
 * (271 programs) hits 16 distinct kinds totaling 62 fallthroughs, in
 * five categories: generators (E_TO/E_ALTERNATE/E_ITERATE/E_LIMIT/
 * E_SEQ), string relops (E_LEQ/E_LNE/E_LGE/E_LLT plus untriggered
 * E_LGT/E_LLE peers), cset arithmetic (E_CSET/E_CSET_COMPL/_DIFF/
 * _INTER), and three mid-size kinds (E_MAKELIST, E_SCAN, E_CASE).
 * Hardening the fallthrough to FAILDESCR causes 4 unified_broker FAILs
 * (notably palindrome.icn via E_LNE), so per the rung the abort is
 * reverted; full inventory in docs/RS-22e-fallthrough-survey.md.
 * RS-22f-or-RS-23 closes the remaining 16 kinds; only after that can
 * the `interp_eval` extern be dropped (RS-23) and coro_value.c
 * promoted into the isolation gate.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * SPRINT:  RS-17a (2026-05-03), RS-22a (2026-05-03), RS-22b (2026-05-03), RS-22c (2026-05-03), RS-22d (2026-05-03), RS-22e (2026-05-03)
 *==========================================================================================================================*/

#include "coro_value.h"
#include "coro_runtime.h"   /* FRAME, frame_depth, scan_pos, scan_subj, icn_descr_identical, g_lang, is_suspendable, coro_eval */
#include "../../driver/interp_private.h"  /* RS-22a: icn_call_builtin, icn_string_section_assign, set_and_trace, data_field_ptr, kw_assign; RS-22d: IcnTkKind via icon_lex.h */
#include "../common/coerce.h"             /* RS-22b: shared_arith */
#include "../x86/bb_broker.h"             /* RS-22d: α, β, bb_node_t for E_AUGOP generator-RHS path */
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
 * bb_augop_compute — RS-22d pure compute step for E_AUGOP.
 *
 * Given (lv, rv, op_token), produce the augop result (FAILDESCR if
 * the op fails — e.g. division by zero, or a comparison-augop whose
 * predicate is false).  Mirrors the AUGOP_APPLY inner switch in
 * interp_eval.c:3740.  No writeback here — separate helper.
 *
 * Coverage: TK_AUGPLUS/MINUS/STAR/SLASH/MOD, TK_AUGCONCAT, the numeric
 * comparison-augops (=:=, ~=:=, <:=, <=:=, >:=, >=:=), the string
 * comparison-augops (==:=, ~==:=, <<:=, <<=:=, >>:=, >>=:=).
 * Unsupported tokens (TK_AUGPOW, TK_AUGCSET_*, TK_AUGSCAN) fall through
 * to integer-add default — same coverage as IR-mode.
 *----------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t bb_augop_compute(DESCR_t lv, DESCR_t rv, IcnTkKind op)
{
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
    long li = IS_INT_fn(lv) ? lv.i : (long)lv.r;
    long ri = IS_INT_fn(rv) ? rv.i : (long)rv.r;
    switch (op) {
    case TK_AUGPLUS:   return INTVAL(li + ri);
    case TK_AUGMINUS:  return INTVAL(li - ri);
    case TK_AUGSTAR:   return INTVAL(li * ri);
    case TK_AUGSLASH:  return ri ? INTVAL(li / ri) : FAILDESCR;
    case TK_AUGMOD:    return ri ? INTVAL(li % ri) : FAILDESCR;
    case TK_AUGCONCAT: {
        const char *ls = VARVAL_fn(lv), *rs = VARVAL_fn(rv);
        if (!ls) ls = ""; if (!rs) rs = "";
        size_t ll = strlen(ls), rl = strlen(rs);
        char *buf = GC_malloc(ll + rl + 1);
        memcpy(buf, ls, ll); memcpy(buf + ll, rs, rl); buf[ll + rl] = '\0';
        return STRVAL(buf);
    }
    /* Numeric comparison-augops — `lv OP rv` evaluates the relation;
     * on success the augop result is rv (and writeback stores it),
     * on failure the augop fails (alternation `| fallback` runs). */
    case TK_AUGEQ: return (li == ri) ? rv : FAILDESCR;
    case TK_AUGNE: return (li != ri) ? rv : FAILDESCR;
    case TK_AUGLT: return (li <  ri) ? rv : FAILDESCR;
    case TK_AUGLE: return (li <= ri) ? rv : FAILDESCR;
    case TK_AUGGT: return (li >  ri) ? rv : FAILDESCR;
    case TK_AUGGE: return (li >= ri) ? rv : FAILDESCR;
    /* String comparison-augops — strcmp on string values. */
    case TK_AUGSEQ: case TK_AUGSNE:
    case TK_AUGSLT: case TK_AUGSLE: case TK_AUGSGT: case TK_AUGSGE: {
        const char *ls = VARVAL_fn(lv), *rs = VARVAL_fn(rv);
        if (!ls) ls = ""; if (!rs) rs = "";
        int cmp = strcmp(ls, rs);
        int ok = 0;
        switch (op) {
        case TK_AUGSEQ: ok = (cmp == 0); break;
        case TK_AUGSNE: ok = (cmp != 0); break;
        case TK_AUGSLT: ok = (cmp <  0); break;
        case TK_AUGSLE: ok = (cmp <= 0); break;
        case TK_AUGSGT: ok = (cmp >  0); break;
        case TK_AUGSGE: ok = (cmp >= 0); break;
        default:        break;
        }
        return ok ? rv : FAILDESCR;
    }
    default:           return INTVAL(li + ri);   /* same default as IR-mode */
    }
}

/*------------------------------------------------------------------------------------------------------------------------------
 * bb_augop_writeback — RS-22d write augop result back to lhs.
 *
 * lhs may be E_VAR (slot/global), E_IDX (subscript), or E_FIELD (record
 * field).  E_VAR&-keyword left untouched for parity with IR-mode (the
 * AUGOP_APPLY macro never wrote back to keyword lvalues).  Callers
 * already checked !IS_FAIL_fn(res).
 *----------------------------------------------------------------------------------------------------------------------------*/
static void bb_augop_writeback(EXPR_t *lhs, DESCR_t res)
{
    if (!lhs) return;
    if (lhs->kind == E_VAR) {
        int slot = (int)lhs->ival;
        if (frame_depth > 0 && slot >= 0 && slot < FRAME.env_n)
            FRAME.env[slot] = res;
        else if (slot < 0 && lhs->sval && lhs->sval[0] != '&')
            set_and_trace(lhs->sval, res);
    } else if (lhs->kind == E_IDX && lhs->nchildren >= 2) {
        DESCR_t base = bb_eval_value(lhs->children[0]);
        DESCR_t idx  = bb_eval_value(lhs->children[1]);
        if (!IS_FAIL_fn(base) && !IS_FAIL_fn(idx))
            subscript_set(base, idx, res);
    } else if (lhs->kind == E_FIELD && lhs->sval && lhs->nchildren >= 1) {
        DESCR_t obj = bb_eval_value(lhs->children[0]);
        if (!IS_FAIL_fn(obj)) {
            DESCR_t *cell = data_field_ptr(lhs->sval, obj);
            if (cell) *cell = res;
        }
    }
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

    /* RS-22d: unary minus / plus.  Mirrors interp_eval.c:2501-2540.
     * E_PLS is more elaborate than `pos()` — try integer parse first,
     * then real, fall back to INTVAL(0).  Match exactly. */
    case E_MNS: {
        if (e->nchildren < 1) return FAILDESCR;
        return neg(bb_eval_value(e->children[0]));
    }
    case E_PLS: {
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (IS_INT_fn(v) || IS_REAL_fn(v)) return v;
        const char *s = VARVAL_fn(v);
        if (!s || !*s) return INTVAL(0);
        char *end = NULL;
        long long iv = strtoll(s, &end, 10);
        if (end && *end == '\0') return INTVAL(iv);
        double dv = strtod(s, &end);
        if (end && *end == '\0') return REALVAL(dv);
        return INTVAL(0);
    }

    /* RS-22d: boolean not.  `not E` succeeds with &null iff E fails;
     * fails iff E succeeds.  Mirrors interp_eval.c:3124. */
    case E_NOT: {
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(v)) return NULVCL;
        return FAILDESCR;
    }

    /* RS-22d: `/E` succeed-if-null. Mirrors interp_eval.c:3629. */
    case E_NULL: {
        if (e->nchildren < 1) return NULVCL;
        DESCR_t v = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (v.v == DT_SNUL) return NULVCL;
        if (v.v == DT_S && (!v.s || v.s[0] == '\0')) return NULVCL;
        return FAILDESCR;
    }

    /* RS-22d: `\E` succeed-if-non-null. Mirrors interp_eval.c:3646. */
    case E_NONNULL: {
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (v.v == DT_SNUL) return FAILDESCR;
        if (v.v == DT_S && (!v.s || v.s[0] == '\0')) return FAILDESCR;
        return v;
    }

    /* RS-22d: `*E` size — string/list/table.  Mirrors interp_eval.c:3133. */
    case E_SIZE: {
        if (e->nchildren < 1) return INTVAL(0);
        DESCR_t v = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (v.v == DT_T) return INTVAL(v.tbl ? v.tbl->size : 0);
        if (v.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(v, "icn_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0)
                return INTVAL((int)FIELD_GET_fn(v, "frame_size").i);
        }
        if (IS_INT_fn(v)) return INTVAL(0);
        if (IS_REAL_fn(v)) return INTVAL(0);
        const char *s = VARVAL_fn(v);
        if (!s) return INTVAL(0);
        if (strchr(s, '\x01')) {     /* SOH-delimited Raku/Icon array */
            long n = 1;
            for (const char *p = s; *p; p++) if (*p == '\x01') n++;
            return INTVAL(n);
        }
        long len = v.slen > 0 ? v.slen : (long)strlen(s);
        return INTVAL(len);
    }

    /* RS-22d: `?E` random selector. Mirrors interp_eval.c:3659.
     * Uses local static LCG (Knuth MMIX constants).  NOTE: this RNG
     * state is independent from interp_eval's — if a program runs
     * partly through bb_eval_value and partly through interp_eval,
     * the two RNGs interleave separately.  In pure BB (post-RS-22e)
     * this becomes the single source. */
    case E_RANDOM: {
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = bb_eval_value(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        static unsigned long _rnd_seed = 12345UL;
        _rnd_seed = _rnd_seed * 6364136223846793005UL + 1442695040888963407UL;
        unsigned long _rnd = _rnd_seed >> 33;
        if (v.v == DT_T) {
            if (!v.tbl || v.tbl->size <= 0) return FAILDESCR;
            int target = (int)(_rnd % (unsigned long)v.tbl->size);
            int seen = 0;
            for (int b = 0; b < TABLE_BUCKETS; b++) {
                for (TBPAIR_t *p = v.tbl->buckets[b]; p; p = p->next) {
                    if (seen == target) return p->val;
                    seen++;
                }
            }
            return FAILDESCR;
        }
        if (v.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(v, "icn_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                int n = (int)FIELD_GET_fn(v, "frame_size").i;
                if (n <= 0) return FAILDESCR;
                DESCR_t ea = FIELD_GET_fn(v, "frame_elems");
                if (ea.v != DT_DATA || !ea.ptr) return FAILDESCR;
                DESCR_t *elems = (DESCR_t *)ea.ptr;
                return elems[_rnd % (unsigned long)n];
            }
            if (v.u && v.u->type && v.u->type->nfields > 0 && v.u->fields) {
                int n = v.u->type->nfields;
                return v.u->fields[_rnd % (unsigned long)n];
            }
            return FAILDESCR;
        }
        if (IS_INT_fn(v)) {
            long long n = v.i;
            if (n <= 0) return FAILDESCR;
            return INTVAL((long long)((_rnd % (unsigned long)n) + 1));
        }
        if (v.v == DT_SNUL) return FAILDESCR;
        const char *s = VARVAL_fn(v);
        if (!s || !*s) return FAILDESCR;
        long slen = v.slen > 0 ? v.slen : (long)strlen(s);
        if (slen <= 0) return FAILDESCR;
        char *out = (char *)GC_malloc(2);
        out[0] = s[_rnd % (unsigned long)slen];
        out[1] = '\0';
        return STRVAL(out);
    }

    /* RS-22d: augmented assignment.  Three execution paths mirroring
     * interp_eval.c:3729-3870:
     *   (1) `!container OP:= rhs`  (lhs is E_ITERATE) — bang-iterate,
     *       apply OP to every cell of T/list/record in place.
     *   (2) RHS suspendable          — drive via coro_eval+bb_node_t,
     *       apply OP per tick, re-reading lhs each tick.  Implements
     *       `every sum +:= (1 to n)`.
     *   (3) Plain                     — single lv OP rv, then writeback.
     * `bb_augop_compute` is the shared compute step; `bb_augop_writeback`
     * is the shared writeback (E_VAR slot / E_IDX / E_FIELD). */
    case E_AUGOP: {
        if (e->nchildren < 2) return NULVCL;
        EXPR_t *lhs = e->children[0];
        EXPR_t *rhs = e->children[1];
        IcnTkKind op = (IcnTkKind)e->ival;
        DESCR_t result = NULVCL;

        /* (1) `!container OP:= rhs` — bang-iterate lvalue. */
        if (lhs && lhs->kind == E_ITERATE && lhs->nchildren >= 1) {
            DESCR_t cv = bb_eval_value(lhs->children[0]);
            DESCR_t rv = bb_eval_value(rhs);
            if (IS_FAIL_fn(cv) || IS_FAIL_fn(rv)) return FAILDESCR;
            #define BB_AUGOP_CELL(cell_) do { \
                DESCR_t _r = bb_augop_compute((cell_), rv, op); \
                if (!IS_FAIL_fn(_r)) { (cell_) = _r; result = _r; } \
            } while (0)
            if (cv.v == DT_T && cv.tbl) {
                for (int b = 0; b < TABLE_BUCKETS; b++)
                    for (TBPAIR_t *p = cv.tbl->buckets[b]; p; p = p->next)
                        BB_AUGOP_CELL(p->val);
            } else if (cv.v == DT_DATA) {
                DESCR_t tag = FIELD_GET_fn(cv, "icn_type");
                if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                    DESCR_t ea = FIELD_GET_fn(cv, "frame_elems");
                    int n = (int)FIELD_GET_fn(cv, "frame_size").i;
                    DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                    if (elems && n > 0)
                        for (int i = 0; i < n; i++) BB_AUGOP_CELL(elems[i]);
                } else if (cv.u && cv.u->type && cv.u->type->nfields > 0 && cv.u->fields) {
                    for (int i = 0; i < cv.u->type->nfields; i++)
                        BB_AUGOP_CELL(cv.u->fields[i]);
                }
            }
            #undef BB_AUGOP_CELL
            return result;
        }

        /* (2) RHS suspendable — drive generator, apply OP per tick. */
        if (rhs && is_suspendable(rhs)) {
            bb_node_t rbox = coro_eval(rhs);
            DESCR_t tick = rbox.fn(rbox.ζ, α);
            while (!IS_FAIL_fn(tick) && !FRAME.loop_break && !FRAME.returning) {
                DESCR_t cur_lv = bb_eval_value(lhs);
                DESCR_t res    = bb_augop_compute(cur_lv, tick, op);
                if (!IS_FAIL_fn(res)) {
                    bb_augop_writeback(lhs, res);
                    result = res;
                }
                tick = rbox.fn(rbox.ζ, β);
            }
            return result;
        }

        /* (3) Plain — single compute + writeback. */
        DESCR_t lv = bb_eval_value(lhs);
        DESCR_t rv = bb_eval_value(rhs);
        DESCR_t res = bb_augop_compute(lv, rv, op);
        if (!IS_FAIL_fn(res)) {
            bb_augop_writeback(lhs, res);
            result = res;
        }
        return result;
    }

    default:
        break;
    }

    /* RS-22e (2026-05-03): fallthrough survey complete.
     *
     *   smoke_icon       : 0 unhandled kinds  ✅
     *   merge gate (unified_broker) : 4 FAILs hardened — palindrome.icn
     *                       (E_LNE), cross_lang.scrip and the Raku tests
     *                       depend on kinds outside our switch.
     *   full Icon corpus (271 programs)      : 16 distinct unhandled
     *                       kinds, totaling 62 calls.
     *
     * Hardening forced merge-gate regression, so per the rung we revert
     * to interp_eval fallthrough and capture the 16 kinds as the work
     * boundary in docs/RS-22e-fallthrough-survey.md.  Five categories:
     *
     *   Generators (legitimate first-value-via-coro_eval semantics —
     *   should be lifted to coro_eval dispatch, not direct evaluation):
     *     E_TO E_TO_BY (only TO seen — TO_BY untriggered) E_ALTERNATE
     *     E_ITERATE E_LIMIT E_SEQ E_EVERY (E_EVERY not in survey but
     *     same shape).
     *
     *   Easy lifts (peers of kinds we already have — copy the pattern):
     *     E_LEQ  E_LNE  E_LGE  E_LLT  E_LGT  E_LLE  (string relops —
     *     mirror of RS-22b's NUMREL via STRREL macro at interp_eval.c
     *     :3397).  E_LCONCAT was already done; these are six remaining
     *     string-comparison peers.
     *
     *   Cset arithmetic (Icon-specific value ops):
     *     E_CSET (literal — trivial: return e->sval ? STRVAL : NULVCL)
     *     E_CSET_COMPL E_CSET_DIFF E_CSET_INTER E_CSET_UNION
     *
     *   Mid-size:
     *     E_MAKELIST  (DT_DATA list constructor — uses DEFDAT_fn /
     *                  DATCON_fn; ~12-line lift)
     *     E_SCAN      (drives a generator chain via coro/exec_stmt;
     *                  needs careful first-value contract definition)
     *     E_CASE      (statement-shaped; reaches value context only via
     *                  case-as-expression — small lift)
     *
     * RS-22f or RS-23 will close these; the rung-23 gate ("remove
     * extern interp_eval, promote into isolation gate") cannot fire
     * until then.  Until then, fallthrough preserves behavior. */
    return interp_eval(e);
}
