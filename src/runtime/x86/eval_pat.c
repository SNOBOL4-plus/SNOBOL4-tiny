/*
 * eval_pat.c — pattern-context expression evaluator (RS-16).
 *
 * Evaluates an EXPR_t in PATTERN context, producing a DT_P descriptor.
 * Pattern evaluation drives SNOBOL4 match: alternation, concatenation,
 * captures, primitive patterns (LEN, TAB, ANY, BREAK, ARBNO, ...).
 *
 * SHARED across all four execution modes. Value-context subexpressions
 * route through eval_node (eval_code.c) — never through interp_eval, so
 * this file has no dependency on src/driver/ (mode 1's home).
 *
 * History: lifted from src/driver/interp_pat.c by RS-16. The original was
 * split off from interp.c by RS-3.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.7
 * Date: 2026-05-03
 */

#include "snobol4.h"
#include "sil_macros.h"
#include "../../ir/ir.h"

/* eval_node is in eval_code.c (sibling). pat_* helpers, NV_GET_fn, APPLY_fn,
 * PATVAL_fn, NULVCL, FAILDESCR, IS_FAIL_fn — all in snobol4.h. */
extern DESCR_t eval_node(EXPR_t *e);

/* RS-16: local copy of NAME_DEREF (originally inline in interp_private.h —
 * mode-1 only). This helper is needed for value-context arg eval inside
 * pattern primitives that take name-bearing arguments. */
static inline DESCR_t NAME_DEREF(DESCR_t d) {
    if (IS_NAME(d)) {
        if (IS_NAMEPTR(d)) return NAME_DEREF_PTR(d);
        if (IS_NAMEVAL(d)) return NV_GET_fn(d.s);
    }
    return d;
}

DESCR_t interp_eval_pat(EXPR_t *e)
{
    if (!e) return NULVCL;
    switch (e->kind) {
    case E_SEQ:
    case E_CAT: {
        if (e->nchildren == 0) return NULVCL;
        DESCR_t acc = interp_eval_pat(e->children[0]);
        if (IS_FAIL_fn(acc)) return FAILDESCR;
        for (int i = 1; i < e->nchildren; i++) {
            DESCR_t nxt = interp_eval_pat(e->children[i]);
            if (IS_FAIL_fn(nxt)) return FAILDESCR;
            acc = pat_cat(acc, nxt);
        }
        return acc;
    }
    case E_ALT: {
        /* pattern alternation: p1 | p2 | ... — each child evaluated in
         * pattern context so that E_DEFER(E_VAR) children become XDSAR
         * nodes rather than frozen DT_E values. */
        if (e->nchildren == 0) return pat_epsilon();
        DESCR_t acc = interp_eval_pat(e->children[0]);
        if (IS_FAIL_fn(acc)) return FAILDESCR;
        for (int i = 1; i < e->nchildren; i++) {
            DESCR_t nxt = interp_eval_pat(e->children[i]);
            if (IS_FAIL_fn(nxt)) return FAILDESCR;
            acc = pat_alt(acc, nxt);
        }
        return acc;
    }
    case E_VLIST: {
        /* Goal-directed value-context disjunction in pattern context.
         * Paren-list `(a, b, c)` is a value-level construct even when it
         * appears inside a pattern; the result is then coerced to pattern
         * by the surrounding context (pat_cat, pat_alt, etc.) via
         * pat_to_patnd.  Try children left-to-right; return first
         * non-failing value; fail if all fail. */
        if (e->nchildren == 0) return FAILDESCR;
        for (int i = 0; i < e->nchildren; i++) {
            DESCR_t v = eval_node(e->children[i]);
            if (!IS_FAIL_fn(v)) return v;
        }
        return FAILDESCR;
    }
    case E_VAR:
        if (e->sval && *e->sval) {
            if (_is_pat_fnc_name(e->sval)) {
                DESCR_t _fr = APPLY_fn(e->sval, NULL, 0);
                if (!IS_FAIL_fn(_fr)) return _fr;
            }
            DESCR_t _v = eval_node(e);
            /* PATVAL coerce: DT_N → deref; DT_E(null) → epsilon; DT_I/DT_R → literal; DT_E → thaw; DT_P/DT_S → pass. */
            if (_v.v == DT_N) {
                if (_v.slen == 1 && _v.ptr) _v = *(DESCR_t *)_v.ptr;
                else if (_v.slen == 0 && _v.s) _v = NV_GET_fn(_v.s);
                else _v = NULVCL;
            }
            if (_v.v == DT_E && !_v.ptr) return NULVCL;  /* null frozen expr → unset var */
            if (_v.v == DT_E || _v.v == DT_I || _v.v == DT_R) return PATVAL_fn(_v);
            return _v;
        }
        return NULVCL;
    case E_DEFER:
        /* *expr in pattern context — two sub-cases:
         *
         * 1. *func(args)  — E_DEFER(E_FNC): build a deferred T_FUNC pattern node
         *    (XATP via pat_user_call) so the function fires at MATCH time as a
         *    zero-width side-effect.  Mirrors SIL *X where X is a user function.
         *
         * 2. *var         — E_DEFER(E_VAR): look up the variable NOW and return
         *    its stored pattern value (the pattern was built at assignment time).
         *    (Contrast: E_DEFER in value context produces DT_E via interp_eval.) */
        if (e->nchildren < 1) return pat_epsilon();
        {
            EXPR_t *child = e->children[0];
            if (child->kind == E_FNC && child->sval) {
                /* *func(args) — build deferred XATP pattern node.
                 *
                 * SN-26c-parseerr-c (Bug B): args that are themselves function
                 * calls (E_FNC) or other non-trivial expressions must be
                 * DEFERRED to match time, not eagerly evaluated here.  Beauty's
                 * snoParse production has  ("'snoParse'" & 'nTop()')  which
                 * builds  EVAL("epsilon . *Reduce('snoParse', nTop())").
                 * Per SPITBOL semantics, nTop() must fire at match time
                 * (after ARBNO has bumped the counter), not at pattern-build
                 * time (when counter is just-pushed = 0).
                 *
                 * Mechanism: wrap the arg child as DT_E (frozen EXPR_t*); the
                 * match-time path (bb_usercall in stmt_exec.c) thaws each DT_E
                 * via EVAL_fn before invoking the user function.
                 *
                 * Plain E_LIT args don't need this — E_LIT already has
                 * its constant value baked in.
                 *
                 * SN-26c-parseerr-d: extend deferral to E_VAR.  The earlier
                 * "Plain E_VAR args don't need this" reasoning was wrong:
                 * in  p . thx . *Shift_t('idtag', thx)  the cursor capture
                 * `. thx` writes the matched substring into thx ONLY at
                 * match time.  If we eagerly eval_node(thx) here at
                 * pattern-build time, we capture the stale value (typically
                 * empty), and the match-time call to Shift_t receives that
                 * stale value instead of the captured cursor substring.
                 * Wrapping E_VAR as DT_E with the EXPR_t* itself defers the
                 * lookup to bb_usercall's thaw loop, which calls EVAL_fn ->
                 * eval_node -> NV_GET_fn AT MATCH TIME. */
                int na = child->nchildren;
                DESCR_t *av = NULL;
                if (na > 0) {
                    av = GC_malloc(na * sizeof(DESCR_t));
                    for (int i = 0; i < na; i++) {
                        EXPR_t *arg = child->children[i];
                        if (arg && (arg->kind == E_FNC || arg->kind == E_VAR)) {
                            /* Defer: wrap as DT_E for match-time EVAL_fn thaw. */
                            av[i].v = DT_E;
                            av[i].ptr = arg;
                            av[i].slen = 0;
                        } else {
                            av[i] = eval_node(arg);
                        }
                    }
                }
                if (getenv("ONE4ALL_USERCALL_TRACE")) {
                    fprintf(stderr, "PAT_USER_CALL_BUILD name=%s nargs=%d\n",
                            child->sval, na);
                }
                return pat_user_call(child->sval, av, na);
            }
            /* *var — build XDSAR deferred-ref node so the variable is
             * resolved at MATCH time (not now).  This is required for
             * self-/mutually-recursive patterns like:
             *   factor = addop *factor . *Unary() | *primary
             * where *factor must not be resolved while factor is still
             * being assigned.  pat_ref() creates an XDSAR node; the
             * materialise() path in snobol4_pattern.c resolves it with
             * cycle detection at match time. */
            if (child->kind == E_VAR && child->sval)
                return pat_ref(child->sval);
            /* Non-VAR, non-FNC child: if it contains no pattern-only nodes,
             * it is a pure value expression — freeze as DT_E for EVAL() to
             * thaw later.  E.g. *('abc' 'def') or *'str' → DT_E, not STRING.
             * If it IS a pattern tree (E_ALT etc.) evaluate in pat context. */
            if (!_expr_is_pat(child)) {
                DESCR_t d; d.v = DT_E; d.ptr = child; d.slen = 0;
                return d;
            }
            DESCR_t r = interp_eval_pat(child);
            if (IS_NAMEPTR(r)) r = NAME_DEREF_PTR(r);
            return r;
        }

    /* PARSER-SN-INFRA-7a: E_CAPT_COND_ASGN / E_CAPT_IMMED_ASGN evaluated in
     * pattern context.  When the capture target is a deferred function call
     * — E_DEFER(E_FNC) for `pat . *func(args)` or E_INDIRECT(E_FNC) for the
     * Snocone `pat . *func(args)` lowering — we MUST route through
     * pat_assign_callcap / pat_assign_callcap_named (XCALLCAP nodes) so the
     * function fires at flush time with deferred args.  Mirrors the
     * driver-side routing in interp_eval.c (E_CAPT_COND_ASGN, lines ~3219).
     *
     * Without this case, E_CAPT_COND_ASGN inside a pattern subexpression
     * (e.g.  `(str ? (POS(0) LEN(1) . *assign(.var, val)))`  built inline,
     * not pre-assigned) fell through to default → eval_node → eval_code.c's
     * E_CAPT_COND_ASGN, which builds a plain XNME node via pat_assign_cond
     * and silently drops the deferred call.  Resulting symptom: the call
     * never fires, the variable is never written, and the only visible
     * effect was a successful match with no side effect.
     *
     * Non-deferred targets (E_VAR, E_INDIRECT, E_NAME) are still routed
     * through eval_node so eval_code.c's existing logic handles them. */
    case E_CAPT_COND_ASGN:
    case E_CAPT_IMMED_ASGN: {
        if (e->nchildren < 2) return FAILDESCR;
        EXPR_t *tgt = e->children[1];
        EXPR_t *fnc = NULL;
        int is_imm = (e->kind == E_CAPT_IMMED_ASGN);
        /* Detect E_DEFER(E_FNC) and E_INDIRECT(E_FNC) deferred-call targets. */
        if (tgt && tgt->kind == E_DEFER && tgt->nchildren == 1
                && tgt->children[0] && tgt->children[0]->kind == E_FNC
                && tgt->children[0]->sval) {
            fnc = tgt->children[0];
        } else if (tgt && tgt->kind == E_INDIRECT && tgt->nchildren == 1
                && tgt->children[0] && tgt->children[0]->kind == E_FNC
                && tgt->children[0]->sval) {
            fnc = tgt->children[0];
        }
        if (fnc) {
            /* Build pattern child in pattern context (so any nested E_DEFER /
             * captures in it get the right treatment), then dispatch to the
             * appropriate XCALLCAP builder. */
            DESCR_t pat = interp_eval_pat(e->children[0]);
            if (IS_FAIL_fn(pat)) return FAILDESCR;
            int na = fnc->nchildren;
            /* TL-2 fast path: when every arg is a plain E_VAR, store names
             * for flush-time NV_GET_fn lookup so earlier in-pattern captures
             * are seen.  Mirrors interp_eval.c lines 3228-3237. */
            int all_vars = (na > 0);
            for (int i = 0; i < na; i++) {
                EXPR_t *c = fnc->children[i];
                if (!c || c->kind != E_VAR || !c->sval) { all_vars = 0; break; }
            }
            if (all_vars) {
                char **names = (char **)GC_malloc(na * sizeof(char *));
                for (int i = 0; i < na; i++) names[i] = (char *)fnc->children[i]->sval;
                if (is_imm)
                    return pat_assign_callcap_named_imm(pat, fnc->sval, NULL, 0, names, na);
                return pat_assign_callcap_named(pat, fnc->sval, NULL, 0, names, na);
            }
            /* Mixed args: defer everything except E_QLIT via DT_E wrap.
             * Mirrors interp_eval.c lines 3251-3267. */
            DESCR_t *av = na > 0 ? GC_malloc(na * sizeof(DESCR_t)) : NULL;
            for (int i = 0; i < na; i++) {
                EXPR_t *arg = fnc->children[i];
                if (arg && arg->kind == E_QLIT) {
                    av[i] = eval_node(arg);
                } else if (arg) {
                    av[i].v = DT_E;
                    av[i].ptr = arg;
                    av[i].slen = 0;
                } else {
                    av[i] = NULVCL;
                }
            }
            if (is_imm)
                return pat_assign_callcap_named_imm(pat, fnc->sval, av, na, NULL, 0);
            return pat_assign_callcap(pat, fnc->sval, av, na);
        }
        /* Non-deferred target — fall through to value-context eval_node,
         * whose E_CAPT_COND_ASGN / E_CAPT_IMMED_ASGN handlers already do
         * the right thing for E_VAR, E_INDIRECT(E_QLIT), etc. */
        return eval_node(e);
    }

    /* Zero-argument pattern primitives.
     * Typed E_* nodes produced by the SNOBOL4 parser via pat_prim_kind().
     * Belong here in interp_eval_pat, not in interp_eval
     * (moved from DYN-55 location in interp_eval.c -- RS-5). */
    case E_ARB:     return pat_arb();
    case E_REM:     return pat_rem();
    case E_FAIL:    return pat_fail();
    case E_SUCCEED: return pat_succeed();
    case E_FENCE:
        if (e->nchildren > 0) {
            DESCR_t _inner = interp_eval_pat(e->children[0]);
            if (IS_FAIL_fn(_inner)) return FAILDESCR;
            return pat_fence_p(_inner);
        }
        return pat_fence();
    case E_ABORT:   return pat_abort();
    case E_BAL:     return pat_bal();

    /* One-argument pattern primitives.
     * POS(n), RPOS(n), TAB(n), RTAB(n), LEN(n) take integer args.
     * ANY(s), NOTANY(s), SPAN(s), BREAK(s), BREAKX(s) take string args. */
    case E_POS: {
        if (e->nchildren < 1) return pat_pos(0);
        DESCR_t a = eval_node(e->children[0]);
        return pat_pos((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_RPOS: {
        if (e->nchildren < 1) return pat_rpos(0);
        DESCR_t a = eval_node(e->children[0]);
        return pat_rpos((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_TAB: {
        if (e->nchildren < 1) return pat_tab(0);
        DESCR_t a = eval_node(e->children[0]);
        return pat_tab((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_RTAB: {
        if (e->nchildren < 1) return pat_rtab(0);
        DESCR_t a = eval_node(e->children[0]);
        return pat_rtab((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_LEN: {
        if (e->nchildren < 1) return pat_len(0);
        DESCR_t a = eval_node(e->children[0]);
        return pat_len((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_ANY: {
        if (e->nchildren < 1) return pat_any_cs("");
        DESCR_t a = NAME_DEREF(eval_node(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_any_cs(s);
    }
    case E_NOTANY: {
        if (e->nchildren < 1) return pat_notany("");
        DESCR_t a = NAME_DEREF(eval_node(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_notany(s);
    }
    case E_SPAN: {
        if (e->nchildren < 1) return pat_span("");
        DESCR_t a = NAME_DEREF(eval_node(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_span(s);
    }
    case E_BREAK: {
        if (e->nchildren < 1) return pat_break_("");
        DESCR_t a = NAME_DEREF(eval_node(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_break_(s);
    }
    case E_BREAKX: {
        extern DESCR_t pat_breakx(const char *);
        if (e->nchildren < 1) return pat_breakx("");
        DESCR_t a = NAME_DEREF(eval_node(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_breakx(s);
    }
    case E_ARBNO: {
        if (e->nchildren < 1) return pat_arb(); /* degenerate */
        DESCR_t inner = interp_eval_pat(e->children[0]);
        return pat_arbno(inner);
    }

    case E_FNC:
        /* Snocone-mode pattern context: ARBNO(P) / FENCE(P) are E_FNC
         * (Snocone has no keyword form — `ARBNO(p)` is just a function
         * call).  When E_SCAN reaches us via interp_eval_pat, the inner
         * pattern argument MUST be evaluated in pattern context so that
         * E_DEFER(E_VAR) / E_DEFER(E_FNC) children become proper deferred
         * ref nodes (XDSAR / pat-deferred T_FUNC) rather than frozen
         * value-form DT_E.  Without this, deferred calls inside
         * ARBNO(...) fire zero times because the pattern is baked at
         * build time.  Mirrors the value-context fix at interp_eval
         * E_FNC ARBNO/FENCE (interp_eval.c ~line 2951).  The SNOBOL4
         * frontend's `ARBNO`/`FENCE` keywords still take the E_ARBNO /
         * E_FENCE direct paths above; this guard only catches Snocone's
         * E_FNC encoding. */
        if (e->nchildren > 0 && e->sval) {
            if (strcmp(e->sval, "ARBNO") == 0) {
                DESCR_t _inner = interp_eval_pat(e->children[0]);
                if (IS_FAIL_fn(_inner)) return FAILDESCR;
                return pat_arbno(_inner);
            }
            if (strcmp(e->sval, "FENCE") == 0) {
                DESCR_t _inner = interp_eval_pat(e->children[0]);
                if (IS_FAIL_fn(_inner)) return FAILDESCR;
                return pat_fence_p(_inner);
            }
        }
        /* Generic function call in pattern context -- evaluate as value,
         * let the caller coerce via PATVAL. */
        return eval_node(e);

    case E_INDIRECT:
        /* $expr in pattern context — eval value, then PATVAL coerce.
         * Mirrors case E_VAR's DT_E thaw at line 95-96.  Fixes recursive-rule
         * binding regression from $'  ' = *White alias (Step 3d-bug):
         * INDIR_GET retrieves a DT_E (frozen EXPR_t*); without this branch
         * the default: path returned DT_E as-is, never coercing to DT_P. */
        {
            DESCR_t _v = eval_node(e);   /* INDIR_GET equivalent */
            if (_v.v == DT_E && !_v.ptr) return NULVCL;
            if (_v.v == DT_E || _v.v == DT_I || _v.v == DT_R) return PATVAL_fn(_v);
            return _v;
        }

    default:
        return eval_node(e);
    }
}
