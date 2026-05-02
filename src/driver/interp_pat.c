/*
 * interp_pat.c — pattern-context evaluator (interp_eval_pat)
 *
 * Evaluates an EXPR_t in PATTERN context, producing a DT_P descriptor.
 * Distinct from interp_eval (value context) and interp_eval_ref (lvalue context).
 * Pattern evaluation drives SNOBOL4 match: alternation, concatenation, captures.
 *
 * Split from interp.c by RS-3 (GOAL-REWRITE-SCRIP).
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * DATE:    2026-05-02
 */

#include "interp_private.h"

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
            DESCR_t v = interp_eval(e->children[i]);
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
            DESCR_t _v = interp_eval(e);
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
                 * match time.  If we eagerly interp_eval(thx) here at
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
                            av[i] = interp_eval(arg);
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
        DESCR_t a = interp_eval(e->children[0]);
        return pat_pos((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_RPOS: {
        if (e->nchildren < 1) return pat_rpos(0);
        DESCR_t a = interp_eval(e->children[0]);
        return pat_rpos((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_TAB: {
        if (e->nchildren < 1) return pat_tab(0);
        DESCR_t a = interp_eval(e->children[0]);
        return pat_tab((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_RTAB: {
        if (e->nchildren < 1) return pat_rtab(0);
        DESCR_t a = interp_eval(e->children[0]);
        return pat_rtab((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_LEN: {
        if (e->nchildren < 1) return pat_len(0);
        DESCR_t a = interp_eval(e->children[0]);
        return pat_len((int64_t)(a.v==DT_I ? a.i : (int64_t)(a.v==DT_R ? (int64_t)a.r : 0)));
    }
    case E_ANY: {
        if (e->nchildren < 1) return pat_any_cs("");
        DESCR_t a = NAME_DEREF(interp_eval(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_any_cs(s);
    }
    case E_NOTANY: {
        if (e->nchildren < 1) return pat_notany("");
        DESCR_t a = NAME_DEREF(interp_eval(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_notany(s);
    }
    case E_SPAN: {
        if (e->nchildren < 1) return pat_span("");
        DESCR_t a = NAME_DEREF(interp_eval(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_span(s);
    }
    case E_BREAK: {
        if (e->nchildren < 1) return pat_break_("");
        DESCR_t a = NAME_DEREF(interp_eval(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_break_(s);
    }
    case E_BREAKX: {
        extern DESCR_t pat_breakx(const char *);
        if (e->nchildren < 1) return pat_breakx("");
        DESCR_t a = NAME_DEREF(interp_eval(e->children[0]));
        const char *s = (a.v==DT_S||a.v==DT_SNUL) && a.s ? a.s : "";
        return pat_breakx(s);
    }
    case E_ARBNO: {
        if (e->nchildren < 1) return pat_arb(); /* degenerate */
        DESCR_t inner = interp_eval_pat(e->children[0]);
        return pat_arbno(inner);
    }

    case E_FNC:
        /* Generic function call in pattern context -- evaluate as value,
         * let the caller coerce via PATVAL.  The SB-5c.1 guards for
         * E_FNC("ARBNO")/E_FNC("FENCE") are removed: no frontend produces
         * those; the SNOBOL4 parser uses pat_prim_kind() to emit E_ARBNO /
         * E_FENCE directly (RS-5). */
        return interp_eval(e);

    default:
        return interp_eval(e);
    }
}
