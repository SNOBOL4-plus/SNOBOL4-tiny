/*
 * interp_eval.c — expression evaluator (interp_eval)
 *
 * Split from interp.c by RS-3 (GOAL-REWRITE-SCRIP).
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * DATE:    2026-05-02
 */

#include "interp_private.h"





/* T-0: set_and_trace — NV_SET_fn + VALUE trace hook for monitor.
 * Use at every plain-variable assignment site in the --ir-run path.
 * Keywords (&STLIMIT etc.) excluded: trace_is_active only fires on
 * names registered via TRACE(var,'VALUE'), never on &-keywords.
 * KW-RETFIX: capture return-value writes into frame->retval_cell to bypass
 * NV keyword collision when user procedure name matches a keyword (e.g. "Trim"). */
void set_and_trace(const char *name, DESCR_t val) {
    /* SN-3: if this name is in the shadow table, update shadow and skip NV_SET_fn
     * (which would be ignored for pattern-primitive names anyway). */
    if (shadow_has(name)) { shadow_set_cur(name, val); goto trace_hook; }
    NV_SET_fn(name, val);
trace_hook:
    if (call_depth > 0) {
        CallFrame *fr = &call_stack[call_depth - 1];
        /* SN-19 stage-2: names arrive canonical, plain strcmp is correct */
        if (name && fr->fname[0] && strcmp(name, fr->fname) == 0) {
            fr->retval_cell = val;
            fr->retval_set  = 1;
        }
    }
    /* SN-26-binmon-3way: comm_var now fires post-commit inside NV_SET_fn,
     * so an additional call here would double-emit on every assignment.
     * The shadow path needs comm_var because shadow_set_cur doesn't pass
     * through NV_SET_fn. */
    if (shadow_has(name) && name && name[0] != '&' && trace_is_active(name))
        comm_var(name, val);
}

/* DYN-57: E_FNC names that always yield a pattern value.
 * Mirrors PAT_FNC_NAMES in SJ-17 (sno-interp.js ec6c0b3).
 * Scoped to _expr_is_pat only — do NOT intercept E_VAR (breaks 210_indirect_ref)
 * and do NOT touch S=PR split/has_eq guard (breaks 062_capture_replacement). */
static const char *PAT_FNC_NAMES[] = {
    "ANY","NOTANY","SPAN","BREAK","BREAKX","LEN","POS","RPOS","TAB","RTAB",
    "ARB","ARBNO","REM","FAIL","SUCCEED","FENCE","ABORT","BAL","CALL", NULL
};
int _is_pat_fnc_name(const char *s) {
    if (!s) return 0;
    /* SN-19 stage-2: AST sval arrives canonical from lexer fold, plain strcmp correct */
    for (int i = 0; PAT_FNC_NAMES[i]; i++)
        if (strcmp(s, PAT_FNC_NAMES[i]) == 0) return 1;
    return 0;
}

/* DYN-54: returns 1 if expr tree contains any pattern-only node.
 * Mirrors is_pat() in snobol4.y but accessible at eval time. */
int _expr_is_pat(EXPR_t *e) {
    if (!e) return 0;
    switch (e->kind) {
        case E_ARB: case E_ARBNO: case E_CAPT_COND_ASGN:
        case E_CAPT_IMMED_ASGN: case E_CAPT_CURSOR: case E_DEFER:
            return 1;
        default: break;
    }
    /* DYN-57: E_FNC whose name is a pattern primitive (LEN, POS, TAB, ARB, etc.) */
    if (e->kind == E_FNC && _is_pat_fnc_name(e->sval)) return 1;
    /* DYN-58: E_VAR whose name is a zero-arg pattern primitive (ARB, REM, FAIL, etc.)
     * Only in _expr_is_pat — do NOT change the general E_VAR eval path (breaks 210). */
    if (e->kind == E_VAR && _is_pat_fnc_name(e->sval)) return 1;
    for (int i = 0; i < e->nchildren; i++)
        if (_expr_is_pat(e->children[i])) return 1;
    return 0;
}

/* BP-1: return interior ptr into DATA instance field, or NULL if not found.
 * SN-19 arch fix: case-policy-neutral shared runtime. Each frontend enforces
 * its own case policy at ingest (SNOBOL4 lex-folds + _builtin_DATA pre-folds;
 * Icon/Raku preserve). Within one language, stored fields and lookup keys
 * have identical case convention — plain strcmp is correct by construction. */
DESCR_t *data_field_ptr(const char *fname, DESCR_t inst) {
    if (inst.v < DT_DATA || !inst.u) return NULL;
    DATBLK_t *blk = inst.u->type;
    if (!blk) return NULL;
    for (int i = 0; i < blk->nfields; i++)
        if (blk->fields[i] && strcmp(blk->fields[i], fname) == 0)
            return &inst.u->fields[i];
    return NULL;
}

/* IC-9: Icon string-section assignment.
 * Implements `s[i:j] := v`, `s[i+:n] := v`, `s[i-:n] := v`, and `s[i] := v`
 * when `s` is a string variable.
 *
 * Strings in Icon are immutable values — but variables holding strings can
 * have a section "replaced" by rebuilding the whole string and writing it
 * back to the underlying variable cell.
 *
 * Returns 1 on success (cell written), 0 on FAIL (OOB) or if the LHS shape
 * is not handled here.  Caller should treat 0 as FAIL of the whole assign
 * unless the LHS kind is E_IDX (in which case caller falls back to
 * subscript_set for list/table semantics).
 *
 * Only handles the simple case: lhs->children[0] is an addressable lvalue
 * (E_VAR / E_FIELD / E_NAME / E_INDIRECT) whose current value is a string.
 * Nested patterns like `t[k][i:j] := v` are not (yet) supported. */
int icn_string_section_assign(EXPR_t *lhs, DESCR_t val) {
    if (!lhs) return 0;
    int kind = lhs->kind;
    if (kind != E_SECTION && kind != E_SECTION_PLUS &&
        kind != E_SECTION_MINUS && kind != E_IDX) return 0;
    if (lhs->nchildren < 2) return 0;
    if (kind == E_SECTION && lhs->nchildren < 3) return 0;

    /* Get a pointer to the underlying cell (so we can write back).  Prefer
     * local slot for E_VAR (when in an icon frame), falling back to NV via
     * interp_eval_ref.  This mirrors the read-side logic in case E_VAR. */
    EXPR_t *bch = lhs->children[0];
    DESCR_t *cell = NULL;
    if (bch && bch->kind == E_VAR && frame_depth > 0) {
        int sl = (int)bch->ival;
        if (sl >= 0 && sl < FRAME.env_n) cell = &FRAME.env[sl];
    }
    if (!cell) cell = interp_eval_ref(bch);
    if (!cell) return 0;
    DESCR_t base = *cell;
    /* For E_IDX: only handle string base — list/table goes through subscript_set. */
    if (kind == E_IDX) {
        if (base.v != DT_S && base.v != DT_SNUL) return 0;
    }
    const char *s = (base.v == DT_SNUL) ? "" : VARVAL_fn(base);
    if (!s) s = "";
    int slen = (int)strlen(s);

    /* Compute lo, hi (1-based section bounds, lo ≤ hi, range [lo, hi)). */
    int lo = 0, hi = 0;
    if (kind == E_SECTION) {
        int i = (int)to_int(interp_eval(lhs->children[1]));
        int j = (int)to_int(interp_eval(lhs->children[2]));
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (j == 0) j = slen + 1; else if (j < 0) j = slen + 1 + j;
        if (i < 1 || i > slen+1 || j < 1 || j > slen+1) return 0;
        lo = i < j ? i : j; hi = i < j ? j : i;
    } else if (kind == E_SECTION_PLUS) {
        int i = (int)to_int(interp_eval(lhs->children[1]));
        int n = (int)to_int(interp_eval(lhs->children[2]));
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen+1) return 0;
        if (n < 0) return 0;
        if (i + n > slen + 1) return 0;
        lo = i; hi = i + n;
    } else if (kind == E_SECTION_MINUS) {
        int i = (int)to_int(interp_eval(lhs->children[1]));
        int n = (int)to_int(interp_eval(lhs->children[2]));
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen+1) return 0;
        if (n < 0) return 0;
        if (i - n < 1) return 0;
        lo = i - n; hi = i;
    } else { /* E_IDX */
        int i = (int)to_int(interp_eval(lhs->children[1]));
        if (i == 0) return 0;
        if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen) return 0;  /* single-char index must point at a real char */
        lo = i; hi = i + 1;
    }

    /* Build new string = s[1..lo) + val + s[hi..slen+1). */
    const char *vs = VARVAL_fn(val); if (!vs) vs = "";
    int vlen = (int)strlen(vs);
    int prefix = lo - 1;             /* chars before the section */
    int suffix = slen - (hi - 1);    /* chars after the section */
    int newlen = prefix + vlen + suffix;
    char *buf = (char *)GC_malloc((size_t)newlen + 1);
    if (prefix > 0) memcpy(buf, s, (size_t)prefix);
    if (vlen > 0)   memcpy(buf + prefix, vs, (size_t)vlen);
    if (suffix > 0) memcpy(buf + prefix + vlen, s + hi - 1, (size_t)suffix);
    buf[newlen] = '\0';

    /* Write back to the cell.  If the base is a global variable, also route
     * through set_and_trace so VALUE traces fire — mirrors E_ASSIGN. */
    EXPR_t *base_expr = lhs->children[0];
    if (base_expr && base_expr->kind == E_VAR && base_expr->sval &&
        base_expr->sval[0] != '&' &&
        !(frame_depth > 0 && base_expr->ival >= 0 && base_expr->ival < FRAME.env_n))
    {
        set_and_trace(base_expr->sval, STRVAL(buf));
    } else {
        *cell = STRVAL(buf);
    }
    return 1;
}



/* ══════════════════════════════════════════════════════════════════════════
 * eval_node_interp — thin wrapper; reuses eval_node from eval_code.c
 * via eval_expr (we call it by re-parsing for non-trivial exprs,
 * but for the common case we use NV_GET_fn / NV_SET_fn directly).
 *
 * For the interpreter we need direct EXPR_t evaluation, not string
 * re-parse.  We replicate the minimal logic needed here rather than
 * exposing eval_node (which is static in eval_code.c).
 * ══════════════════════════════════════════════════════════════════════════ */

/* find_leaf_suspendable — walk an expr tree and return the first generator-kind node.
 * Used by E_EVERY special-case to find the raw E_TO (or similar) inside
 * compound exprs like E_ADD(E_VAR(total), E_TO(1,n)), so we can drive only
 * the generator and inject via coro_drive_node, letting interp_eval re-read
 * mutable variables (e.g. frame locals) fresh each tick. */
static EXPR_t *find_leaf_suspendable(EXPR_t *e) {
    if (!e) return NULL;
    switch (e->kind) {
        case E_TO: case E_TO_BY: case E_ITERATE: case E_ALTERNATE:
        case E_SUSPEND: case E_LIMIT: case E_EVERY: case E_BANG_BINARY: case E_SEQ_EXPR:
            return e;
        case E_FNC: return e;   /* user proc or builtin — treat as leaf generator */
        default: break;
    }
    for (int i = 0; i < e->nchildren; i++) {
        EXPR_t *found = find_leaf_suspendable(e->children[i]);
        if (found) return found;
    }
    return NULL;
}

/* real_str — format a real for Icon output using shortest round-trip representation.
 * Tries precisions 15..17 and picks the shortest that parses back to the same double. */
const char *real_str(double r, char *buf, int bufsz) {
    for (int p = 15; p <= 17; p++) {
        snprintf(buf, bufsz, "%.*g", p, r);
        char *end; double back = strtod(buf, &end);
        if (back == r) break;   /* shortest precision that round-trips */
    }
    if (!strchr(buf, '.') && !strchr(buf, 'e') && !strchr(buf, 'E') && !strchr(buf, 'n') && !strchr(buf, 'N'))
        strncat(buf, ".0", bufsz - strlen(buf) - 1);
    return buf;
}

/* icn_call_builtin — call a builtin E_FNC with pre-resolved args array.
 * Used by coro_bb_fnc to avoid re-evaluating generator children.
 * Dispatches write/writes/upto/find/any/many/upto/tab/move/match by name.
 * For user procs, calls coro_call directly. */
DESCR_t icn_call_builtin(EXPR_t *call, DESCR_t *args, int nargs) {
    if (!call || call->nchildren < 1 || !call->children[0]) return NULVCL;
    const char *fn = call->children[0]->sval;
    if (!fn) return NULVCL;
    DESCR_t a0 = nargs > 0 ? args[0] : NULVCL;
    DESCR_t a1 = nargs > 1 ? args[1] : NULVCL;
    /* write(x1,...,xN) — concatenate all args, append newline.
     * Icon semantics: any FAIL arg propagates; &null arg writes empty. */
    if (!strcmp(fn, "write")) {
        for (int _wi = 0; _wi < nargs; _wi++) {
            DESCR_t av = args[_wi];
            if (IS_FAIL_fn(av)) return FAILDESCR;
            if (av.v == DT_SNUL) continue;   /* &null → empty */
            if (IS_INT_fn(av))       printf("%lld", (long long)av.i);
            else if (IS_REAL_fn(av)) { char _rb[64]; printf("%s", real_str(av.r,_rb,sizeof _rb)); }
            else { const char *s = VARVAL_fn(av); if (s) fputs(s, stdout); }
        }
        putchar('\n');
        return nargs > 0 ? args[nargs-1] : NULVCL;
    }
    /* writes(x1,...,xN) — same but no newline */
    if (!strcmp(fn, "writes")) {
        for (int _wi = 0; _wi < nargs; _wi++) {
            DESCR_t av = args[_wi];
            if (IS_FAIL_fn(av)) return FAILDESCR;
            if (av.v == DT_SNUL) continue;
            if (IS_INT_fn(av))       printf("%lld", (long long)av.i);
            else if (IS_REAL_fn(av)) { char _rb[64]; printf("%s", real_str(av.r,_rb,sizeof _rb)); }
            else { const char *s = VARVAL_fn(av); if (s) fputs(s, stdout); }
        }
        return nargs > 0 ? args[nargs-1] : NULVCL;
    }
    /* User proc — call directly with resolved args */
    for (int i = 0; i < proc_count; i++) {
        if (!strcmp(proc_table[i].name, fn))
            return coro_call(proc_table[i].proc, args, nargs);
    }
    /* Fallback: re-evaluate whole call (args ignored — last resort) */
    return interp_eval(call);
}

/* Forward declaration (also declared above for call_user_function) */

/* IC-9 (2026-05-01): Icon-keyword write helper.
 * Returns 1 on success, 0 on FAIL (out-of-range &pos write).
 * Keywords without a write contract (e.g. &letters, &lcase) are read-only
 * and silently no-op (return 1) — Icon's own behaviour is "no error, no effect".
 *
 * &pos := N semantics:
 *   subj length = L; valid N is in [-L, L+1]; N == 0 → after-last (= L+1);
 *   N < 0 → L+1+N (so -1 → L; -2 → L-1 …).  Out of range → FAIL,
 *   pos unchanged.
 *
 * &subject := s semantics:
 *   set scan subject to string form of s; reset &pos to 1 (Icon spec). */
int kw_assign(const char *kw, DESCR_t val) {
    if (!strcmp(kw, "pos")) {
        long n = to_int(val);
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        long norm;
        if (n == 0)      norm = slen + 1;
        else if (n < 0)  norm = slen + 1 + n;
        else             norm = n;
        if (norm < 1 || norm > slen + 1) return 0;
        scan_pos = norm;
        return 1;
    }
    if (!strcmp(kw, "subject")) {
        const char *s = VARVAL_fn(val); if (!s) s = "";
        scan_subj = GC_strdup(s);
        scan_pos = 1;
        return 1;
    }
    /* Other keywords: silently accept (no write contract here) */
    return 1;
}

/* IC-9 (session #26): probe variant of kw_assign — answers "would the
 * write succeed?" without performing it.  Used by atomic E_SWAP / E_REVSWAP
 * to detect OOB-on-keyword cases where neither side should be written. */
int icn_kw_can_assign(const char *kw, DESCR_t val) {
    if (!strcmp(kw, "pos")) {
        long n = to_int(val);
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        long norm;
        if (n == 0)      norm = slen + 1;
        else if (n < 0)  norm = slen + 1 + n;
        else             norm = n;
        return (norm >= 1 && norm <= slen + 1) ? 1 : 0;
    }
    /* &subject, others: always accept */
    return 1;
}

DESCR_t interp_eval(EXPR_t *e)
{
    if (!e) return NULVCL;
    /* coro_drive_node injection: if this exact node is being driven as a generator
     * (set by E_EVERY leaf-gen injection or coro_drive_fnc), return the staged value
     * directly without recursing into children.  Covers E_TO, E_FNC, and any other
     * node kind that find_leaf_suspendable or coro_drive_fnc selects as the leaf. */
    if (coro_drive_node && e == coro_drive_node) return coro_drive_val;

    /* OE-5: Icon frame dispatch — E_VAR/E_ASSIGN/E_FNC differ between SNO and ICN.
     * All other EKinds fall through to the shared switch (already has Icon cases
     * from OE-3/OE-4). Guard: only active inside an Icon call frame. */
    if (frame_depth > 0) {
        switch (e->kind) {
        case E_VAR: {
            if (e->sval && e->sval[0] == '&') {
                const char *kw = e->sval + 1;
                if (!strcmp(kw,"subject")) return scan_subj ? STRVAL(scan_subj) : NULVCL;
                if (!strcmp(kw,"pos"))     return INTVAL(scan_pos);
                if (!strcmp(kw,"letters")) return STRVAL("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
                if (!strcmp(kw,"ucase"))   return STRVAL("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                if (!strcmp(kw,"lcase"))   return STRVAL("abcdefghijklmnopqrstuvwxyz");
                if (!strcmp(kw,"digits"))  return STRVAL("0123456789");
                if (!strcmp(kw,"null"))    return NULVCL;
                if (!strcmp(kw,"fail"))    return FAILDESCR;
                return NULVCL;
            }
            int slot = (int)e->ival;
            if (slot >= 0 && slot < FRAME.env_n) return FRAME.env[slot];
            if (slot < 0 && e->sval && e->sval[0] != '&') return NV_GET_fn(e->sval);
            return NULVCL;
        }
        case E_ASSIGN: {
            if (e->nchildren < 2) return NULVCL;
            DESCR_t val = interp_eval(e->children[1]);
            if (IS_FAIL_fn(val)) return FAILDESCR;
            EXPR_t *lhs = e->children[0];
            /* IC-9: string-section / string-index lvalue.  Try this first;
             * if it returns 1 the assign happened.  Returns 0 for FAIL (OOB)
             * or for E_IDX with non-string base (falls through to subscript_set). */
            if (lhs && (lhs->kind == E_SECTION || lhs->kind == E_SECTION_PLUS ||
                        lhs->kind == E_SECTION_MINUS)) {
                if (icn_string_section_assign(lhs, val)) return val;
                return FAILDESCR;
            }
            if (lhs && lhs->kind == E_IDX && lhs->nchildren >= 2) {
                /* Try string-index first; if base is non-string, fall through. */
                if (icn_string_section_assign(lhs, val)) return val;
                /* If base IS a string but assign returned 0, that's an OOB → FAIL. */
                {
                    DESCR_t _b = interp_eval(lhs->children[0]);
                    if (_b.v == DT_S || _b.v == DT_SNUL) return FAILDESCR;
                }
            }
            if (lhs && lhs->kind == E_VAR) {
                /* IC-9 (2026-05-01): Icon keyword lvalue — &pos := N, &subject := s */
                if (lhs->sval && lhs->sval[0] == '&') {
                    if (!kw_assign(lhs->sval + 1, val)) return FAILDESCR;
                    return val;
                }
                int slot = (int)lhs->ival;
                if (slot >= 0 && slot < FRAME.env_n) { FRAME.env[slot] = val; return val; }
                /* SS-MON: route through set_and_trace so VALUE traces fire on
                 * plain `var = expr` (previously they fired only on pattern-
                 * match replacement).  set_and_trace internally calls NV_SET_fn
                 * AND comm_var.  &-keywords skipped here for parity with the
                 * pattern-match path. */
                if (slot < 0 && lhs->sval && lhs->sval[0] != '&') set_and_trace(lhs->sval, val);
            } else if (lhs && lhs->kind == E_IDX && lhs->nchildren >= 2) {
                /* t["key"] := val  — subscript assignment in Icon frame.
                 * IC-9: if the index child is generative (e.g. t[key(t)] := 99),
                 * drive the gen and write val into every generated cell.  Single
                 * `interp_eval` pass — under `every` this fires once and assigns
                 * all cells, mirroring the E_ITERATE LHS branch below. */
                DESCR_t base = interp_eval(lhs->children[0]);
                if (!IS_FAIL_fn(base)) {
                    if (is_suspendable(lhs->children[1])) {
                        bb_node_t ig = coro_eval(lhs->children[1]);
                        DESCR_t k = ig.fn(ig.ζ, α);
                        while (!IS_FAIL_fn(k)) {
                            subscript_set(base, k, val);
                            k = ig.fn(ig.ζ, β);
                        }
                    } else {
                        DESCR_t idx = interp_eval(lhs->children[1]);
                        if (!IS_FAIL_fn(idx)) subscript_set(base, idx, val);
                    }
                }
            } else if (lhs && lhs->kind == E_FIELD && lhs->sval && lhs->nchildren >= 1) {
                /* p.x := val  — record field assignment in Icon frame */
                DESCR_t obj = interp_eval(lhs->children[0]);
                if (!IS_FAIL_fn(obj)) {
                    DESCR_t *cell = data_field_ptr(lhs->sval, obj);
                    if (cell) *cell = val;
                }
            } else if (lhs && lhs->kind == E_ITERATE && lhs->nchildren >= 1) {
                /* IC-9: !container := val — bang-iterate as lvalue.
                 * Walks every element/entry of the container and writes val into each cell.
                 * Single pass — under `every` this fires once and assigns all cells.
                 *   !T := v  (table) — write v into every TBPAIR_t::val
                 *   !L := v  (list)  — write v into every frame_elems[i]
                 *   !s := v  (string)— not supported (immutable), silently no-op
                 */
                DESCR_t cv = interp_eval(lhs->children[0]);
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
                            /* IC-9 (2026-05-01): every !record := V — write val into every field cell */
                            for (int i = 0; i < cv.u->type->nfields; i++) cv.u->fields[i] = val;
                        }
                    }
                }
            } else if (lhs && lhs->kind == E_RANDOM && lhs->nchildren >= 1) {
                /* IC-9 (2026-05-01): ?container := V — write val into a random cell.
                 * Mirrors E_RANDOM read: deterministic LCG (Knuth MMIX) so output
                 * is reproducible. Currently only DT_DATA records — table/list random
                 * write would need its own RNG state to match read-side picks (the
                 * E_RANDOM read at line ~4313 has its own static seed; sharing it
                 * here would be incorrect since reads and writes interleave). */
                DESCR_t cv = interp_eval(lhs->children[0]);
                if (!IS_FAIL_fn(cv) && cv.v == DT_DATA && cv.u && cv.u->type
                    && cv.u->type->nfields > 0 && cv.u->fields) {
                    static unsigned long _rnd_w_seed = 67890UL;
                    _rnd_w_seed = _rnd_w_seed * 6364136223846793005UL + 1442695040888963407UL;
                    int n = cv.u->type->nfields;
                    cv.u->fields[(_rnd_w_seed >> 33) % (unsigned long)n] = val;
                }
            }
            return val;
        }
        case E_REVASSIGN: {
            /* IC-9: x <- v  — reversible assign, standalone path.
             * Outside `every`, no driver backtracks the operation, so we just
             * perform the assign and succeed.  The revert semantics live in
             * coro_bb_revassign (icn_runtime.c), reached only when coro_eval
             * is asked for a box (every / alt-driven contexts).
             * Mirrors E_ASSIGN's E_VAR / E_IDX / E_FIELD branches; the rare
             * E_ITERATE LHS isn't meaningful with `<-` (you'd be saving and
             * reverting an immediately-discarded sequence). */
            if (e->nchildren < 2) return NULVCL;
            DESCR_t val = interp_eval(e->children[1]);
            if (IS_FAIL_fn(val)) return FAILDESCR;
            EXPR_t *lhs = e->children[0];
            if (lhs && lhs->kind == E_VAR) {
                int slot = (int)lhs->ival;
                if (slot >= 0 && slot < FRAME.env_n) FRAME.env[slot] = val;
                else if (slot < 0 && lhs->sval && lhs->sval[0] != '&') set_and_trace(lhs->sval, val);
            } else if (lhs && lhs->kind == E_IDX && lhs->nchildren >= 2) {
                DESCR_t base = interp_eval(lhs->children[0]);
                if (!IS_FAIL_fn(base)) {
                    DESCR_t idx = interp_eval(lhs->children[1]);
                    if (!IS_FAIL_fn(idx)) subscript_set(base, idx, val);
                }
            } else if (lhs && lhs->kind == E_FIELD && lhs->sval && lhs->nchildren >= 1) {
                DESCR_t obj = interp_eval(lhs->children[0]);
                if (!IS_FAIL_fn(obj)) {
                    DESCR_t *cell = data_field_ptr(lhs->sval, obj);
                    if (cell) *cell = val;
                }
            }
            return val;
        }
        case E_FNC: {
            /* Icon call nodes: sval=NULL, name in children[0]->sval */
            if (e->nchildren < 1) return NULVCL;
            const char *fn = e->children[0] ? e->children[0]->sval : NULL;
            if (!fn) return NULVCL;
            int nargs = e->nchildren - 1;
            if (!strcmp(fn,"write")) {
                if (nargs == 0) { printf("\n"); return NULVCL; }
                /* Icon write/writes: evaluate ALL args first; fail (no output) if any fails.
                 * IC-9: pre-fix evaluated and printed incrementally, so writes("x",fail())
                 * would print "x" before failing — wrong.  Now buffers args first. */
                DESCR_t *vals = (DESCR_t *)GC_malloc((size_t)nargs * sizeof(DESCR_t));
                for (int _wi = 0; _wi < nargs; _wi++) {
                    vals[_wi] = interp_eval(e->children[1+_wi]);
                    if (IS_FAIL_fn(vals[_wi])) return FAILDESCR;
                }
                DESCR_t last = NULVCL;
                for (int _wi = 0; _wi < nargs; _wi++) {
                    DESCR_t a = vals[_wi];
                    last = a;
                    if (a.v == DT_SNUL) continue;
                    if (IS_INT_fn(a)) printf("%lld",(long long)a.i);
                    else if (IS_REAL_fn(a)) { char _rb[64]; printf("%s",real_str(a.r,_rb,sizeof _rb)); }
                    else { const char *s=VARVAL_fn(a); if (s) fputs(s, stdout); }
                }
                putchar('\n');
                return last;
            }
            if (!strcmp(fn,"writes")) {
                if (nargs == 0) return NULVCL;
                DESCR_t *vals = (DESCR_t *)GC_malloc((size_t)nargs * sizeof(DESCR_t));
                for (int _wi = 0; _wi < nargs; _wi++) {
                    vals[_wi] = interp_eval(e->children[1+_wi]);
                    if (IS_FAIL_fn(vals[_wi])) return FAILDESCR;
                }
                DESCR_t last = NULVCL;
                for (int _wi = 0; _wi < nargs; _wi++) {
                    DESCR_t a = vals[_wi];
                    last = a;
                    if (a.v == DT_SNUL) continue;
                    if (IS_INT_fn(a)) printf("%lld",(long long)a.i);
                    else if (IS_REAL_fn(a)) { char _rb[64]; printf("%s",real_str(a.r,_rb,sizeof _rb)); }
                    else { const char *s=VARVAL_fn(a); if (s) fputs(s, stdout); }
                }
                return last;
            }
            if (!strcmp(fn,"read") && nargs == 0) {
                /* Icon read() — read one line from stdin, strip trailing newline.
                 * Fails on EOF. */
                char buf[4096];
                if (!fgets(buf, sizeof buf, stdin)) return FAILDESCR;
                size_t len = strlen(buf);
                if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
                if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
                char *r = GC_malloc(len + 1); memcpy(r, buf, len + 1);
                return STRVAL(r);
            }
            if (!strcmp(fn,"reads") && nargs == 1) {
                /* Icon reads(n) — read n bytes from stdin, fail on EOF. */
                DESCR_t nd = interp_eval(e->children[1]);
                int n = (int)to_int(nd);
                if (n <= 0) return FAILDESCR;
                char *buf = GC_malloc(n + 1);
                int got = (int)fread(buf, 1, (size_t)n, stdin);
                if (got <= 0) return FAILDESCR;
                buf[got] = '\0';
                DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
                return r;
            }
            if (!strcmp(fn,"stop"))  { exit(0); }
            if (!strcmp(fn,"any") && nargs>=1 && (scan_pos>0||nargs>=2)) {
                DESCR_t cs=interp_eval(e->children[1]);
                const char *cv=VARVAL_fn(cs);
                if(!cv) return FAILDESCR;
                const char *s; int p, slen, end;
                if(nargs>=2) {
                    DESCR_t sv=interp_eval(e->children[2]); s=VARVAL_fn(sv); if(!s) s="";
                    slen=(int)strlen(s);
                    int i1=(nargs>=3)?(int)interp_eval(e->children[3]).i:scan_pos;
                    int i2=(nargs>=4)?(int)interp_eval(e->children[4]).i:slen+1;
                    if(i1<=0||i1>slen) return FAILDESCR;
                    if(i2<=0) i2=slen+1;
                    p=i1-1; end=i2-1;
                } else { s=scan_subj; if(!s) return FAILDESCR; slen=(int)strlen(s); p=scan_pos-1; end=slen; }
                if(p<0||p>=slen||p>=end||!strchr(cv,s[p])) return FAILDESCR;
                if(nargs<2) { scan_pos++; return INTVAL(scan_pos); }
                return INTVAL(p+2);
            }
            if (!strcmp(fn,"many") && nargs>=1 && (scan_pos>0||nargs>=2)) {
                DESCR_t cs=interp_eval(e->children[1]);
                const char *cv=VARVAL_fn(cs);
                if(!cv) return FAILDESCR;
                const char *s; int p, slen, end;
                if(nargs>=2) {
                    DESCR_t sv=interp_eval(e->children[2]); s=VARVAL_fn(sv); if(!s) s="";
                    slen=(int)strlen(s);
                    int i1=(nargs>=3)?(int)interp_eval(e->children[3]).i:scan_pos;
                    int i2=(nargs>=4)?(int)interp_eval(e->children[4]).i:slen+1;
                    if(i1<=0||i1>slen) return FAILDESCR;
                    if(i2<=0) i2=slen+1;
                    p=i1-1; end=i2-1;
                } else { s=scan_subj; if(!s) return FAILDESCR; slen=(int)strlen(s); p=scan_pos-1; end=slen; }
                if(p<0||p>=slen||p>=end||!strchr(cv,s[p])) return FAILDESCR;
                while(p<end&&p<slen&&strchr(cv,s[p])) p++;
                if(nargs<2) { scan_pos=p+1; return INTVAL(scan_pos); }
                return INTVAL(p+1);
            }
            if (!strcmp(fn,"upto") && nargs>=1 && (scan_pos>0||nargs>=2)) {
                DESCR_t cs=interp_eval(e->children[1]);
                const char *cv=VARVAL_fn(cs);
                if(!cv) return FAILDESCR;
                const char *s; int p, slen, end;
                if(nargs>=2) {
                    DESCR_t sv=interp_eval(e->children[2]); s=VARVAL_fn(sv); if(!s) s="";
                    slen=(int)strlen(s);
                    int i1=(nargs>=3)?(int)interp_eval(e->children[3]).i:scan_pos;
                    int i2=(nargs>=4)?(int)interp_eval(e->children[4]).i:slen+1;
                    if(i1<=0) i1=1; if(i2<=0) i2=slen+1;
                    p=i1-1; end=i2-1;
                } else { s=scan_subj; if(!s) return FAILDESCR; slen=(int)strlen(s); p=scan_pos-1; end=slen; }
                while(p<end&&p<slen&&!strchr(cv,s[p])) p++;
                if(p>=end||p>=slen) return FAILDESCR;
                if(nargs<2) { /* scan context: upto does NOT advance pos, returns current matching pos */
                    return INTVAL(p+1);
                }
                return INTVAL(p+1);
            }
            if (!strcmp(fn,"move") && nargs>=1 && scan_pos>0) {
                DESCR_t nv=interp_eval(e->children[1]); int n=(int)nv.i;
                int newp=scan_pos+n;
                if(!scan_subj||newp<1||newp>(int)strlen(scan_subj)+1) return FAILDESCR;
                int old=scan_pos; scan_pos=newp;
                size_t len=(size_t)(n>=0?n:-n); int start=(n>=0?old:newp);
                char *buf=GC_malloc(len+1); memcpy(buf,scan_subj+start-1,len); buf[len]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"tab") && nargs>=1 && scan_pos>0) {
                DESCR_t nv=interp_eval(e->children[1]); if(IS_FAIL_fn(nv)) return FAILDESCR;
                int slen=scan_subj?(int)strlen(scan_subj):0;
                int newp=(int)nv.i;
                if(newp==0) newp=slen+1;
                else if(newp<0) newp=slen+1+newp;
                if(!scan_subj||newp<scan_pos||newp<1||newp>slen+1) return FAILDESCR;
                int old=scan_pos; scan_pos=newp; size_t len=(size_t)(newp-old);
                char *buf=GC_malloc(len+1); memcpy(buf,scan_subj+old-1,len); buf[len]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"pos") && nargs>=1 && scan_pos>0) {
                DESCR_t nv=interp_eval(e->children[1]); if(IS_FAIL_fn(nv)) return FAILDESCR;
                int slen=scan_subj?(int)strlen(scan_subj):0;
                int p=(int)nv.i;
                if(p==0) p=slen+1;
                else if(p<0) p=slen+1+p;
                if(p<1||p>slen+1) return FAILDESCR;
                return (scan_pos==p) ? INTVAL(scan_pos) : FAILDESCR;
            }
            if (!strcmp(fn,"rpos") && nargs>=1 && scan_pos>0) {
                DESCR_t nv=interp_eval(e->children[1]); if(IS_FAIL_fn(nv)) return FAILDESCR;
                int slen=scan_subj?(int)strlen(scan_subj):0;
                int p=slen+1-(int)nv.i;
                if(p<1||p>slen+1) return FAILDESCR;
                return (scan_pos==p) ? INTVAL(scan_pos) : FAILDESCR;
            }
            if (!strcmp(fn,"match") && nargs>=1 && scan_pos>0) {
                DESCR_t sv=interp_eval(e->children[1]);
                const char *needle=VARVAL_fn(sv),*hay=scan_subj?scan_subj:"";
                if(!needle) return FAILDESCR;
                int p=scan_pos-1,nl=(int)strlen(needle);
                if(strncmp(hay+p,needle,nl)!=0) return FAILDESCR;
                scan_pos+=nl; return INTVAL(scan_pos);
            }
            if (!strcmp(fn,"bal") && nargs>=1) {
                /* bal(c1, c2, c3, s, i1, i2) — find first position where c1-chars appear
                   at nesting depth 0 w.r.t. c2/c3 open/close delimiters.
                   Scalar path; generator path is in coro_eval via coro_bb_bal. */
                DESCR_t cd=interp_eval(e->children[1]);
                const char *c1=VARVAL_fn(cd); if(!c1) return FAILDESCR;
                const char *c2="(", *c3=")";
                if(nargs>=2){DESCR_t t=interp_eval(e->children[2]);const char*v=VARVAL_fn(t);if(v&&v[0])c2=v;}
                if(nargs>=3){DESCR_t t=interp_eval(e->children[3]);const char*v=VARVAL_fn(t);if(v&&v[0])c3=v;}
                const char *s; int slen, p, end;
                if(nargs>=4){
                    DESCR_t sv=interp_eval(e->children[4]);s=VARVAL_fn(sv);if(!s)s="";
                    slen=(int)strlen(s);
                    int i1=(nargs>=5)?(int)interp_eval(e->children[5]).i:1;
                    int i2=(nargs>=6)?(int)interp_eval(e->children[6]).i:slen+1;
                    if(i1<=0)i1=1; if(i2<=0)i2=slen+1;
                    p=i1-1; end=i2-1;
                } else {
                    s=scan_subj; if(!s) return FAILDESCR;
                    slen=(int)strlen(s); p=scan_pos-1; end=slen;
                }
                int depth=0;
                while(p<end&&p<slen){
                    char ch=s[p];
                    if(strchr(c2,ch)) depth++;
                    else if(strchr(c3,ch)&&depth>0) depth--;
                    else if(depth==0&&strchr(c1,ch)) return INTVAL(p+1);
                    p++;
                }
                return FAILDESCR;
            }
            if (!strcmp(fn,"find") && nargs>=2) {
                long pos1; if(icn_frame_lookup(e,&pos1)) return INTVAL(pos1);
                DESCR_t s1=interp_eval(e->children[1]),s2=interp_eval(e->children[2]);
                const char *needle=VARVAL_fn(s1),*hay=VARVAL_fn(s2);
                if(!needle||!hay) return FAILDESCR;
                char *p=strstr(hay,needle);
                return p?INTVAL((long long)(p-hay)+1):FAILDESCR;
            }
            /* coro_drive_fnc passthrough: if this E_FNC node is currently being
             * driven by coro_drive_fnc, return the suspended value directly
             * instead of re-calling the procedure (which would recurse). */
            if (e == coro_drive_node) return coro_drive_val;
            for (int i=0; i<proc_count; i++) {
                if (!strcmp(proc_table[i].name,fn)) {
                    DESCR_t args[FRAME_SLOT_MAX];
                    for (int j=0; j<nargs&&j<FRAME_SLOT_MAX; j++)
                        args[j]=interp_eval(e->children[1+j]);
                    return coro_call(proc_table[i].proc,args,nargs);
                }
            }
            /* RK-14: array builtins — arrays stored as \x01-separated strings */
            if (!strcmp(fn,"push") && nargs == 2) {
                DESCR_t arr = interp_eval(e->children[1]);
                DESCR_t val = interp_eval(e->children[2]);
                char vbuf[64]; const char *vs;
                if (IS_INT_fn(val))       { snprintf(vbuf,sizeof vbuf,"%lld",(long long)val.i); vs=vbuf; }
                else if (IS_REAL_fn(val)) { snprintf(vbuf,sizeof vbuf,"%g",val.r); vs=vbuf; }
                else vs = (val.s && *val.s) ? val.s : "";
                const char *as = (arr.v==DT_S||arr.v==DT_SNUL) ? (arr.s?arr.s:"") : "";
                size_t al=strlen(as), vl=strlen(vs);
                char *buf;
                if (al == 0) { buf=GC_malloc(vl+1); memcpy(buf,vs,vl+1); }
                else { buf=GC_malloc(al+1+vl+1); memcpy(buf,as,al); buf[al]='\x01'; memcpy(buf+al+1,vs,vl+1); }
                if (e->children[1]->kind==E_VAR && e->children[1]->ival>=0 &&
                    e->children[1]->ival<FRAME.env_n && frame_depth>0)
                    FRAME.env[e->children[1]->ival] = STRVAL(buf);
                return STRVAL(buf);
            }
            if (!strcmp(fn,"elems") && nargs == 1) {
                DESCR_t arr = interp_eval(e->children[1]);
                const char *as = (arr.v==DT_S||arr.v==DT_SNUL) ? (arr.s?arr.s:"") : "";
                if (!*as) return INTVAL(0);
                long cnt = 1;
                for (const char *p=as; *p; p++) if (*p=='\x01') cnt++;
                return INTVAL(cnt);
            }
            if (!strcmp(fn,"pop") && nargs == 1) {
                DESCR_t arr = interp_eval(e->children[1]);
                const char *as = (arr.v==DT_S||arr.v==DT_SNUL) ? (arr.s?arr.s:"") : "";
                if (!*as) return FAILDESCR;
                char *buf = GC_malloc(strlen(as)+1); strcpy(buf, as);
                char *last = strrchr(buf, '\x01');
                char *popped;
                if (last) { popped=GC_malloc(strlen(last+1)+1); strcpy(popped,last+1); *last='\0'; }
                else       { popped=GC_malloc(strlen(buf)+1);   strcpy(popped,buf);    buf[0]='\0'; }
                if (e->children[1]->kind==E_VAR && e->children[1]->ival>=0 &&
                    e->children[1]->ival<FRAME.env_n && frame_depth>0)
                    FRAME.env[e->children[1]->ival] = STRVAL(buf);
                return STRVAL(popped);
            }
            if (!strcmp(fn,"arr_get") && nargs == 2) {
                DESCR_t arr = interp_eval(e->children[1]);
                DESCR_t idx = interp_eval(e->children[2]);
                const char *as = (arr.v==DT_S||arr.v==DT_SNUL) ? (arr.s?arr.s:"") : "";
                long i = IS_INT_fn(idx) ? idx.i : 0;
                long cur = 0; const char *seg = as;
                while (cur < i) {
                    const char *nx = strchr(seg, '\x01');
                    if (!nx) return FAILDESCR;
                    seg = nx+1; cur++;
                }
                const char *end = strchr(seg, '\x01');
                size_t len = end ? (size_t)(end-seg) : strlen(seg);
                char *out = GC_malloc(len+1); memcpy(out,seg,len); out[len]='\0';
                return STRVAL(out);
            }
            if (!strcmp(fn,"arr_set") && nargs == 3) {
                DESCR_t arr = interp_eval(e->children[1]);
                DESCR_t idx = interp_eval(e->children[2]);
                DESCR_t val = interp_eval(e->children[3]);
                const char *as = (arr.v==DT_S||arr.v==DT_SNUL) ? (arr.s?arr.s:"") : "";
                long target = IS_INT_fn(idx) ? idx.i : 0;
                char vbuf[64]; const char *vs;
                if (IS_INT_fn(val)) { snprintf(vbuf,sizeof vbuf,"%lld",(long long)val.i); vs=vbuf; }
                else vs = (val.s && *val.s) ? val.s : "";
                char *out = GC_malloc(strlen(as)+strlen(vs)+64);
                out[0]='\0'; long cur=0; const char *seg=as;
                while (*seg || cur <= target) {
                    const char *end2 = strchr(seg, '\x01');
                    size_t slen = end2 ? (size_t)(end2-seg) : strlen(seg);
                    if (out[0]) strcat(out,"\x01");
                    if (cur==target) strcat(out,vs);
                    else             strncat(out,seg,slen);
                    seg = end2 ? end2+1 : seg+slen;
                    cur++;
                    if (!end2 && cur > target) break;
                }
                if (e->children[1]->kind==E_VAR && e->children[1]->ival>=0 &&
                    e->children[1]->ival<FRAME.env_n && frame_depth>0)
                    FRAME.env[e->children[1]->ival] = STRVAL(out);
                return STRVAL(out);
            }

            /* ── RK-15: Hash builtins ───────────────────────────────────────
             * Hashes stored as \x02-separated "key\x03value" pair strings.
             * hash_set(h,k,v): upsert; hash_get(h,k): lookup or NULVCL;
             * hash_exists(h,k): 1/0; hash_keys(h): \x01-sep key list;
             * hash_values(h): \x01-sep value list.                        */
#define HS '\x02'   /* pair separator */
#define HK '\x03'   /* key/value separator within a pair */
            if ((!strcmp(fn,"hash_set") && nargs == 3) ||
                (!strcmp(fn,"hash_get") && nargs == 2) ||
                (!strcmp(fn,"hash_exists") && nargs == 2) ||
                (!strcmp(fn,"hash_delete") && nargs == 2) ||
                (!strcmp(fn,"hash_keys") && nargs == 1) ||
                (!strcmp(fn,"hash_values") && nargs == 1) ||
                (!strcmp(fn,"hash_pairs") && nargs == 1)) {
                DESCR_t hd = interp_eval(e->children[1]);
                const char *hs = (hd.v==DT_S||hd.v==DT_SNUL) ? (hd.s?hd.s:"") : "";
                if (!strcmp(fn,"hash_set")) {
                    DESCR_t kd = interp_eval(e->children[2]);
                    DESCR_t vd = interp_eval(e->children[3]);
                    char kb[64], vb[64];
                    const char *ks = IS_INT_fn(kd)  ? (snprintf(kb,sizeof kb,"%lld",(long long)kd.i),kb)
                                   : IS_REAL_fn(kd) ? (snprintf(kb,sizeof kb,"%g",kd.r),kb)
                                   : (kd.s&&*kd.s?kd.s:"");
                    const char *vs = IS_INT_fn(vd)  ? (snprintf(vb,sizeof vb,"%lld",(long long)vd.i),vb)
                                   : IS_REAL_fn(vd) ? (snprintf(vb,sizeof vb,"%g",vd.r),vb)
                                   : (vd.s&&*vd.s?vd.s:"");
                    size_t kl=strlen(ks);
                    char *out = GC_malloc(strlen(hs)+kl+strlen(vs)+4); out[0]='\0';
                    const char *p = hs;
                    while (*p) {
                        const char *sep = strchr(p, HK); const char *end = strchr(p, HS);
                        if (!sep) break;
                        size_t pkl=(size_t)(sep-p);
                        if (pkl!=kl || memcmp(p,ks,kl)!=0) {
                            if (out[0]) { size_t ol=strlen(out); out[ol]=HS; out[ol+1]='\0'; }
                            size_t plen=end?(size_t)(end-p):strlen(p); strncat(out,p,plen);
                        }
                        if (!end) break; p=end+1;
                    }
                    if (out[0]) { size_t ol=strlen(out); out[ol]=HS; out[ol+1]='\0'; }
                    strcat(out,ks); { size_t ol=strlen(out); out[ol]=HK; out[ol+1]='\0'; } strcat(out,vs);
                    if (e->children[1]->kind==E_VAR && e->children[1]->ival>=0 &&
                        e->children[1]->ival<FRAME.env_n && frame_depth>0)
                        FRAME.env[e->children[1]->ival] = STRVAL(out);
                    return STRVAL(out);
                }
                if (!strcmp(fn,"hash_get") || !strcmp(fn,"hash_exists")) {
                    DESCR_t kd = interp_eval(e->children[2]);
                    char kb[64];
                    const char *ks = IS_INT_fn(kd)  ? (snprintf(kb,sizeof kb,"%lld",(long long)kd.i),kb)
                                   : IS_REAL_fn(kd) ? (snprintf(kb,sizeof kb,"%g",kd.r),kb)
                                   : (kd.s&&*kd.s?kd.s:"");
                    size_t kl=strlen(ks);
                    const char *p=hs;
                    while (*p) {
                        const char *sep=strchr(p,HK); const char *end=strchr(p,HS);
                        if (!sep) break;
                        size_t pkl=(size_t)(sep-p);
                        if (pkl==kl && memcmp(p,ks,kl)==0) {
                            if (!strcmp(fn,"hash_exists")) return INTVAL(1);
                            const char *vs=sep+1;
                            size_t vl=end?(size_t)(end-vs):strlen(vs);
                            char *out=GC_malloc(vl+1); memcpy(out,vs,vl); out[vl]='\0';
                            return STRVAL(out);
                        }
                        if (!end) break; p=end+1;
                    }
                    return !strcmp(fn,"hash_exists") ? INTVAL(0) : NULVCL;
                }
                if (!strcmp(fn,"hash_keys") || !strcmp(fn,"hash_values")) {
                    if (!*hs) { char *e2=GC_malloc(1); e2[0]='\0'; return STRVAL(e2); }
                    char *out=GC_malloc(strlen(hs)+2); out[0]='\0';
                    const char *p=hs;
                    while (*p) {
                        const char *sep=strchr(p,HK); const char *end=strchr(p,HS);
                        if (!sep) break;
                        if (out[0]) { size_t ol=strlen(out); out[ol]='\x01'; out[ol+1]='\0'; }
                        if (!strcmp(fn,"hash_keys")) {
                            strncat(out,p,(size_t)(sep-p));
                        } else {
                            const char *vs=sep+1;
                            size_t vl=end?(size_t)(end-vs):strlen(vs);
                            strncat(out,vs,vl);
                        }
                        if (!end) break; p=end+1;
                    }
                    return STRVAL(out);
                }
                if (!strcmp(fn,"hash_pairs")) {
                    if (!*hs) { char *e2=GC_malloc(1); e2[0]='\0'; return STRVAL(e2); }
                    char *out=GC_malloc(strlen(hs)*2+4); out[0]='\0';
                    const char *p=hs;
                    while (*p) {
                        const char *sep=strchr(p,HK); const char *end=strchr(p,HS);
                        if (!sep) break;
                        if (out[0]) { size_t ol=strlen(out); out[ol]='\x01'; out[ol+1]='\0'; }
                        size_t kl=(size_t)(sep-p);
                        const char *vs=sep+1;
                        size_t vl=end?(size_t)(end-vs):strlen(vs);
                        size_t ol=strlen(out);
                        memcpy(out+ol,p,kl); ol+=kl;
                        out[ol++]=':';
                        memcpy(out+ol,vs,vl); ol+=vl;
                        out[ol]='\0';
                        if (!end) break; p=end+1;
                    }
                    return STRVAL(out);
                }
                if (!strcmp(fn,"hash_delete")) {
                    DESCR_t kd = interp_eval(e->children[2]);
                    char kb[64];
                    const char *ks = IS_INT_fn(kd)  ? (snprintf(kb,sizeof kb,"%lld",(long long)kd.i),kb)
                                   : IS_REAL_fn(kd) ? (snprintf(kb,sizeof kb,"%g",kd.r),kb)
                                   : (kd.s&&*kd.s?kd.s:"");
                    size_t kl=strlen(ks);
                    char *out=GC_malloc(strlen(hs)+2); out[0]='\0';
                    const char *p=hs;
                    while (*p) {
                        const char *sep=strchr(p,HK); const char *end=strchr(p,HS);
                        if (!sep) break;
                        size_t pkl=(size_t)(sep-p);
                        if (pkl!=kl || memcmp(p,ks,kl)!=0) {
                            if (out[0]) { size_t ol=strlen(out); out[ol]=HS; out[ol+1]='\0'; }
                            size_t plen=end?(size_t)(end-p):strlen(p);
                            strncat(out,p,plen);
                        }
                        if (!end) break; p=end+1;
                    }
                    if (e->children[1]->kind==E_VAR && e->children[1]->ival>=0 &&
                        e->children[1]->ival<FRAME.env_n && frame_depth>0)
                        FRAME.env[e->children[1]->ival] = STRVAL(out);
                    return STRVAL(out);
                }
            }
#undef HS
#undef HK

            /* ── RK-22: Raku string op builtins ────────────────────────────
             * substr($s, $start [, $len])  — 0-based; maps to SNOBOL4 SUBSTR (1-based)
             * index($s, $needle [, $pos])  — 0-based pos of first match, -1 if not found
             * rindex($s, $needle [, $pos]) — 0-based pos of last match, -1 if not found
             * uc($s)   — uppercase via REPLACE(s, &lcase, &ucase)
             * lc($s)   — lowercase via REPLACE(s, &ucase, &lcase)
             * trim($s) — strip leading+trailing whitespace (Raku semantics)
             * chars($s) / length($s) — number of chars                     */
            if (!strcmp(fn,"raku_substr") || (!strcmp(fn,"substr") && nargs >= 2)) {
                DESCR_t sd = interp_eval(e->children[1]);
                DESCR_t id = interp_eval(e->children[2]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                long slen = (long)strlen(s);
                long start = IS_INT_fn(id) ? id.i : 0;
                if (start < 0) start = slen + start;
                if (start < 0) start = 0;
                if (start > slen) start = slen;
                long len = slen - start;
                if (nargs >= 3) {
                    DESCR_t ld = interp_eval(e->children[3]);
                    len = IS_INT_fn(ld) ? ld.i : len;
                    if (len < 0) len = 0;
                    if (start + len > slen) len = slen - start;
                }
                char *out = GC_malloc((size_t)len + 1);
                memcpy(out, s + start, (size_t)len); out[len] = '\0';
                return STRVAL(out);
            }
            if (!strcmp(fn,"raku_index") || (!strcmp(fn,"index") && nargs >= 2)) {
                DESCR_t sd = interp_eval(e->children[1]);
                DESCR_t nd = interp_eval(e->children[2]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                const char *needle = VARVAL_fn(nd); if (!needle) needle = "";
                long from = 0;
                if (nargs >= 3) { DESCR_t pd = interp_eval(e->children[3]); from = IS_INT_fn(pd)?pd.i:0; }
                if (from < 0) from = 0;
                if (*needle == '\0') return INTVAL(from);
                const char *found = strstr(s + from, needle);
                return found ? INTVAL((long)(found - s)) : INTVAL(-1);
            }
            if (!strcmp(fn,"raku_rindex") || (!strcmp(fn,"rindex") && nargs >= 2)) {
                DESCR_t sd = interp_eval(e->children[1]);
                DESCR_t nd = interp_eval(e->children[2]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                const char *needle = VARVAL_fn(nd); if (!needle) needle = "";
                long slen = (long)strlen(s);
                long from = slen;
                if (nargs >= 3) { DESCR_t pd = interp_eval(e->children[3]); from = IS_INT_fn(pd)?pd.i:slen; }
                size_t nlen = strlen(needle);
                if (nlen == 0) return INTVAL(from < slen ? from : slen);
                long best = -1;
                for (long i = 0; i <= from - (long)nlen; i++) {
                    if (memcmp(s + i, needle, nlen) == 0) best = i;
                }
                return INTVAL(best);
            }
            if (!strcmp(fn,"raku_match") && nargs == 2) {
                /* RK-23: $s ~~ /pattern/ — substring search (literal regex subset).
                 * Returns INTVAL(1) on match, FAILDESCR on no match.
                 * If pattern evaluates to DT_P, dispatch through match_pattern. */
                DESCR_t sd = interp_eval(e->children[1]);
                DESCR_t pd = interp_eval(e->children[2]);
                const char *subj = VARVAL_fn(sd); if (!subj) subj = "";
                if (pd.v == DT_P) {
                    extern int exec_stmt(const char *, DESCR_t *, DESCR_t, DESCR_t *, int);
                    return exec_stmt(NULL, &sd, pd, NULL, 0) ? INTVAL(1) : FAILDESCR;
                }
                const char *pat = VARVAL_fn(pd); if (!pat) pat = "";
                { Raku_nfa *nfa = raku_nfa_build(pat);
                  if (!nfa) return FAILDESCR;
                  raku_nfa_exec(nfa, subj, &g_raku_match);
                  g_raku_subject = subj;
                  raku_nfa_free(nfa);
                  return g_raku_match.matched ? INTVAL(1) : FAILDESCR;
                }
            }
            if (!strcmp(fn,"raku_match_global") && nargs == 2) {
                /* RK-37: $s ~~ m:g/pat/ -- collect all non-overlapping matches */
                /* Returns SOH-delimited list of full-match strings for for-loop */
                DESCR_t sd = interp_eval(e->children[1]);
                DESCR_t pd = interp_eval(e->children[2]);
                const char *subj = VARVAL_fn(sd); if (!subj) subj = "";
                const char *pat  = VARVAL_fn(pd); if (!pat)  pat  = "";
                Raku_nfa *nfa = raku_nfa_build(pat);
                if (!nfa) return STRVAL(GC_strdup(""));
                int slen = (int)strlen(subj);
                /* collect all matches into a SOH-delimited array string */
                char *out = GC_malloc(slen * 4 + 4); out[0] = '\0';
                int pos = 0, count = 0;
                while (pos <= slen) {
                    Raku_match m;
                    /* build a temporary subject slice via exec on offset */
                    raku_nfa_exec(nfa, subj + pos, &m);
                    if (!m.matched) break;
                    int mlen = m.full_end - m.full_start;
                    if (count > 0) { int ol=strlen(out); out[ol]='\x01'; out[ol+1]='\0'; }
                    strncat(out, subj + pos + m.full_start, (size_t)mlen);
                    /* also update g_raku_match for last match captures */
                    g_raku_match = m;
                    g_raku_match.full_start += pos;
                    g_raku_match.full_end   += pos;
                    for (int g=0;g<m.ngroups;g++) {
                        if (m.group_start[g]>=0) g_raku_match.group_start[g]+=pos;
                        if (m.group_end[g]>=0)   g_raku_match.group_end[g]+=pos;
                    }
                    g_raku_subject = subj;
                    pos += m.full_start + (mlen > 0 ? mlen : 1);
                    count++;
                }
                raku_nfa_free(nfa);
                return count > 0 ? STRVAL(out) : FAILDESCR;
            }
            if (!strcmp(fn,"raku_subst") && nargs == 2) {
                /* RK-37: $s ~~ s/pat/repl/[g] -- substitution */
                /* tok format: "pat\x01repl\x01flag" where flag=g or - */
                DESCR_t sd = interp_eval(e->children[1]);
                DESCR_t td = interp_eval(e->children[2]);
                const char *subj = VARVAL_fn(sd); if (!subj) subj = "";
                const char *tok  = VARVAL_fn(td); if (!tok)  tok  = "";
                /* split tok on \x01 */
                const char *sep1 = strchr(tok, '\x01');
                if (!sep1) return sd;
                const char *sep2 = strchr(sep1+1, '\x01');
                if (!sep2) return sd;
                int plen = (int)(sep1-tok);
                int rlen = (int)(sep2-(sep1+1));
                char *pat  = GC_malloc(plen+1); memcpy(pat, tok, plen); pat[plen]='\0';
                char *repl = GC_malloc(rlen+1); memcpy(repl, sep1+1, rlen); repl[rlen]='\0';
                int global = (*(sep2+1)=='g');
                Raku_nfa *nfa = raku_nfa_build(pat);
                if (!nfa) return sd;
                int slen=(int)strlen(subj);
                char *res = GC_malloc(slen*4+rlen*8+4); res[0]='\0';
                int pos=0, did_one=0;
                while (pos<=slen) {
                    Raku_match m; raku_nfa_exec(nfa, subj+pos, &m);
                    if (!m.matched) { strncat(res, subj+pos, (size_t)(slen-pos)); break; }
                    /* copy pre-match */
                    strncat(res, subj+pos, (size_t)m.full_start);
                    /* copy replacement (TODO: $0/$<n> expansion in repl) */
                    strcat(res, repl);
                    g_raku_match=m; g_raku_subject=subj;
                    int advance=m.full_start+(m.full_end-m.full_start>0?m.full_end-m.full_start:1);
                    pos+=advance; did_one=1;
                    if (!global) { strncat(res, subj+pos, (size_t)(slen-pos)); break; }
                }
                raku_nfa_free(nfa);
                /* update the subject variable in the frame if it was a VAR */
                if (e->children[1]->kind==E_VAR && e->children[1]->ival>=0 &&
                    e->children[1]->ival<FRAME.env_n && frame_depth>0)
                    FRAME.env[e->children[1]->ival] = STRVAL(res);
                return did_one ? STRVAL(res) : sd;
            }
            /* RK-38: file I/O builtins */
            if (!strcmp(fn,"open") && (nargs==1||nargs==2)) {
                DESCR_t pd=interp_eval(e->children[1]);
                const char *path=VARVAL_fn(pd); if(!path||!*path) return FAILDESCR;
                const char *mode="r";
                if(nargs==2){
                    DESCR_t md=interp_eval(e->children[2]);
                    const char *ms=VARVAL_fn(md); if(!ms) ms="";
                    if(strstr(ms,":w")||strstr(ms,"w")) mode="w";
                    else if(strstr(ms,":a")||strstr(ms,"a")) mode="a";
                }
                FILE *fp=fopen(path,mode);
                if(!fp) return FAILDESCR;
                int idx=raku_fh_alloc(fp);
                if(idx<0){fclose(fp);return FAILDESCR;}
                return INTVAL(idx);
            }
            if (!strcmp(fn,"close") && nargs==1) {
                DESCR_t fd=interp_eval(e->children[1]);
                int idx=(int)(IS_INT_fn(fd)?fd.i:0);
                FILE *fp=raku_fh_get(idx);
                if(fp){fclose(fp);raku_fh_free(idx);}
                return INTVAL(0);
            }
            if (!strcmp(fn,"slurp") && nargs==1) {
                DESCR_t ad=interp_eval(e->children[1]);
                FILE *fp=NULL; int need_close=0;
                if(IS_INT_fn(ad)) {
                    fp=raku_fh_get((int)ad.i);
                } else {
                    const char *path=VARVAL_fn(ad); if(!path||!*path) return STRVAL(GC_strdup(""));
                    fp=fopen(path,"r"); need_close=1;
                }
                if(!fp) return STRVAL(GC_strdup(""));
                fseek(fp,0,SEEK_END); long sz=ftell(fp); rewind(fp);
                char *buf=GC_malloc(sz+1);
                size_t nr=fread(buf,1,(size_t)sz,fp); buf[nr]='\0';
                if(need_close) fclose(fp);
                return STRVAL(buf);
            }
            if (!strcmp(fn,"lines") && nargs==1) {
                /* lines(fh|path) -> SOH-delimited line list for for-loop */
                DESCR_t ad=interp_eval(e->children[1]);
                FILE *fp=NULL; int need_close=0;
                if(IS_INT_fn(ad)) {
                    fp=raku_fh_get((int)ad.i);
                } else {
                    const char *path=VARVAL_fn(ad); if(!path||!*path) return STRVAL(GC_strdup(""));
                    fp=fopen(path,"r"); need_close=1;
                }
                if(!fp) return STRVAL(GC_strdup(""));
                char *out=GC_malloc(65536); out[0]='\0'; size_t cap=65536, used=0;
                char line[4096]; int first=1;
                while(fgets(line,sizeof line,fp)){
                    size_t ll=strlen(line);
                    while(ll>0&&(line[ll-1]=='\n'||line[ll-1]=='\r')) line[--ll]='\0';
                    size_t need=used+ll+2;
                    if(need>cap){cap=need*2;char*nb=GC_malloc(cap);memcpy(nb,out,used);out=nb;}
                    if(!first){out[used++]='\x01';}
                    memcpy(out+used,line,ll); used+=ll; out[used]='\0'; first=0;
                }
                if(need_close) fclose(fp);
                return STRVAL(out);
            }
            if ((!strcmp(fn,"raku_print_fh")||!strcmp(fn,"raku_say_fh")) && nargs==2) {
                /* RK-39: print/say to file handle */
                DESCR_t fd=interp_eval(e->children[1]);
                DESCR_t vd=interp_eval(e->children[2]);
                int idx=(int)(IS_INT_fn(fd)?fd.i:1);
                FILE *fp=raku_fh_get(idx); if(!fp) fp=stdout;
                const char *s=VARVAL_fn(vd); if(!s) s="";
                fputs(s,fp);
                if(!strcmp(fn,"raku_say_fh")) fputc('\n',fp);
                return INTVAL(0);
            }
            if (!strcmp(fn,"spurt") && nargs==2) {
                /* RK-56: spurt(path, content) -- write string to file */
                DESCR_t pd=interp_eval(e->children[1]);
                DESCR_t cd=interp_eval(e->children[2]);
                const char *path=VARVAL_fn(pd); if(!path||!*path) return FAILDESCR;
                const char *content=VARVAL_fn(cd); if(!content) content="";
                FILE *fp=fopen(path,"w"); if(!fp) return FAILDESCR;
                fputs(content,fp); fclose(fp);
                return INTVAL(0);
            }
            if (!strcmp(fn,"raku_nfa_compile") && nargs == 1) {
                /* RK-32: compile pattern string -> NFA, print state count, return 0 */
                DESCR_t pd = interp_eval(e->children[1]);
                const char *pat = VARVAL_fn(pd); if (!pat) pat = "";
                { Raku_nfa *nfa = raku_nfa_build(pat);
                  if (!nfa) { printf("NFA:%s:ERROR\n", pat); return INTVAL(0); }
                  printf("NFA:%s:states=%d\n", pat, raku_nfa_state_count(nfa));
                  raku_nfa_free(nfa);
                }
                return INTVAL(0);
            }
            if (!strcmp(fn,"raku_named_capture") && nargs == 1) {
                /* RK-35: $<n> named capture from last ~~ match */
                DESCR_t nd = interp_eval(e->children[1]);
                const char *name = VARVAL_fn(nd); if (!name) name = "";
                if (!g_raku_match.matched) return STRVAL(GC_strdup(""));
                int g = -1;
                for (int i=0;i<g_raku_match.ngroups;i++)
                    if (strcmp(g_raku_match.group_name[i],name)==0){g=i;break;}
                if (g<0||g_raku_match.group_start[g]<0) return STRVAL(GC_strdup(""));
                int gs=g_raku_match.group_start[g], ge=g_raku_match.group_end[g];
                if (ge<gs) return STRVAL(GC_strdup(""));
                int len=ge-gs; char *out=GC_malloc(len+1);
                memcpy(out,g_raku_subject+gs,(size_t)len); out[len]='\0';
                return STRVAL(out);
            }
            if (!strcmp(fn,"raku_capture") && nargs == 1) {
                /* RK-34: $N positional capture from last ~~ match */
                DESCR_t nd = interp_eval(e->children[1]);
                int n = (int)(IS_INT_fn(nd) ? nd.i : 0);
                if (!g_raku_match.matched || n < 0 || n >= g_raku_match.ngroups
                    || g_raku_match.group_start[n] < 0) return STRVAL(GC_strdup(""));
                int gs = g_raku_match.group_start[n];
                int ge = g_raku_match.group_end[n];
                if (ge < gs) return STRVAL(GC_strdup(""));
                int len = ge - gs;
                char *out = GC_malloc(len + 1);
                memcpy(out, g_raku_subject + gs, (size_t)len);
                out[len] = '\0';
                return STRVAL(out);
            }
            if (!strcmp(fn,"raku_uc") || (!strcmp(fn,"uc") && nargs == 1)) {
                DESCR_t sd = interp_eval(e->children[1]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                char *out = GC_strdup(s);
                for (char *p = out; *p; p++) *p = (char)toupper((unsigned char)*p);
                return STRVAL(out);
            }
            if (!strcmp(fn,"raku_lc") || (!strcmp(fn,"lc") && nargs == 1)) {
                DESCR_t sd = interp_eval(e->children[1]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                char *out = GC_strdup(s);
                for (char *p = out; *p; p++) *p = (char)tolower((unsigned char)*p);
                return STRVAL(out);
            }
            if (!strcmp(fn,"raku_trim")) {
                DESCR_t sd = interp_eval(e->children[1]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
                size_t len = strlen(s);
                while (len > 0 && (s[len-1]==' '||s[len-1]=='\t'||s[len-1]=='\n'||s[len-1]=='\r')) len--;
                char *out = GC_malloc(len + 1); memcpy(out, s, len); out[len] = '\0';
                return STRVAL(out);
            }
            if (!strcmp(fn,"chars") || !strcmp(fn,"length")) {
                if (nargs == 1) {
                    DESCR_t sd = interp_eval(e->children[1]);
                    const char *s = VARVAL_fn(sd); if (!s) s = "";
                    return INTVAL((long)strlen(s));
                }
            }

            /* ── RK-25: Raku try/CATCH/die exception handling ───────────────
             * raku_die(msg)         — store msg in g_raku_exception, return FAILDESCR
             * raku_try(body)        — eval body; if FAIL, clear exception, return NULVCL
             * raku_try(body, catch) — eval body; if FAIL, eval catch block, return result */
            if (!strcmp(fn,"raku_die") && nargs >= 1) {
                DESCR_t md = interp_eval(e->children[1]);
                const char *msg = VARVAL_fn(md); if (!msg) msg = "Died";
                extern char g_raku_exception[512];
                snprintf(g_raku_exception, sizeof g_raku_exception, "%s", msg);
                return FAILDESCR;
            }
            if (!strcmp(fn,"raku_try") && (nargs == 1 || nargs == 2)) {
                extern char g_raku_exception[512];
                g_raku_exception[0] = '\0';
                DESCR_t r = interp_eval(e->children[1]);   /* try body */
                int body_failed = IS_FAIL_fn(r);
                int real_die    = (g_raku_exception[0] != '\0'); /* only raku_die sets this */
                if (!body_failed) { g_raku_exception[0]='\0'; return r; } /* success */
                /* body failed */
                if (nargs == 2 && real_die) {
                    /* CATCH block: only fires on explicit die, not on fall-off-end */
                    EXPR_t *catch_blk = e->children[2];
                    int _sl2 = -1;
                    EXPR_t *_stk2[64]; int _sn2=0; _stk2[_sn2++]=catch_blk;
                    while (_sn2>0 && _sl2<0) {
                        EXPR_t *_n=_stk2[--_sn2]; if(!_n) continue;
                        if(_n->kind==E_VAR && _n->sval &&
                           (strcmp(_n->sval,"$!")==0||strcmp(_n->sval,"!")==0))
                            _sl2=(int)_n->ival;
                        for(int _ci=0;_ci<_n->nchildren&&_sn2<62;_ci++) _stk2[_sn2++]=_n->children[_ci];
                    }
                    DESCR_t exc_d = STRVAL(GC_strdup(g_raku_exception));
                    if (_sl2 >= 0 && _sl2 < FRAME.env_n) FRAME.env[_sl2] = exc_d;
                    else NV_SET_fn("$!", exc_d);
                    g_raku_exception[0] = '\0';
                    return interp_eval(catch_blk);
                }
                g_raku_exception[0] = '\0';
                return NULVCL;   /* swallow failure (no CATCH, or non-die failure) */
            }

            /* ── RK-24: Raku map/grep/sort higher-order list ops ────────────
             * raku_map(block, @arr)  — apply block to each elem, collect results
             * raku_grep(block, @arr) — collect elems where block is truthy
             * raku_sort(@arr)        — lexicographic sort
             * raku_sort(block, @arr) — sort with comparator (block uses $a/$b)
             *
             * Arrays are SOH (\x01) delimited strings.
             * Block is an E_EXPR subtree (child[1]); array is child[2] (or child[1] for sort).
             * $_ is bound into env slot via icn_scope_set for each iteration.  */

            /* helper: split SOH string → char** of elems (GC-allocated) */
#define SOH '\x01'

            if (!strcmp(fn,"raku_map") && nargs == 2) {
                EXPR_t *blk = e->children[1];          /* block EXPR_t* */
                DESCR_t arrd = interp_eval(e->children[2]);
                const char *as = VARVAL_fn(arrd); if (!as) as = "";
                /* iterate elems, eval block with $_ bound, collect */
                char *out = GC_strdup("");
                const char *seg = as;
                int first = 1;
                do {
                    const char *nx = strchr(seg, SOH);
                    size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                    char *elem = GC_malloc(elen + 1);
                    memcpy(elem, seg, elen); elem[elen] = '\0';
                    /* bind $_ — walk closure tree; $_ has sval="$_" or "_", use ival as slot */
                    { /* elem: INTVAL if numeric, else STRVAL */
                      char *_ep_ev; long _iv_ev = strtol(elem, &_ep_ev, 10);
                      DESCR_t _ev = (*_ep_ev == '\0' && _ep_ev > elem) ? INTVAL(_iv_ev) : STRVAL(elem);
                      int _sl = -1;
                      EXPR_t *_stk[64]; int _sn=0; _stk[_sn++]=blk;
                      while (_sn>0 && _sl<0) {
                          EXPR_t *_n=_stk[--_sn];
                          if (!_n) continue;
                          if (_n->kind==E_VAR && _n->sval) {
                              const char *_sv = _n->sval;
                              /* match "$_" or "_" (sigil may be stripped) */
                              if (strcmp(_sv,"$_")==0 || strcmp(_sv,"_")==0)
                                  _sl=(int)_n->ival;
                          }
                          for(int _ci=0;_ci<_n->nchildren&&_sn<62;_ci++) _stk[_sn++]=_n->children[_ci];
                      }
                      if (_sl >= 0 && _sl < FRAME.env_n) FRAME.env[_sl] = _ev;
                      else NV_SET_fn("$_", _ev); }
                    DESCR_t r = interp_eval(blk);
                    if (!IS_FAIL_fn(r)) {
                        const char *rv; char rb[64];
                        if (IS_INT_fn(r))       { snprintf(rb,sizeof rb,"%lld",(long long)r.i); rv=rb; }
                        else if (IS_REAL_fn(r)) { snprintf(rb,sizeof rb,"%g",r.r); rv=rb; }
                        else                    { rv = VARVAL_fn(r); if (!rv) rv = ""; }
                        size_t ol = strlen(out), rl = strlen(rv);
                        char *nout = GC_malloc(ol + rl + 2);
                        memcpy(nout, out, ol);
                        if (!first) { nout[ol] = SOH; memcpy(nout+ol+1, rv, rl); nout[ol+1+rl]='\0'; }
                        else        { memcpy(nout+ol, rv, rl); nout[ol+rl]='\0'; first=0; }
                        out = nout;
                    }
                    seg = nx ? nx + 1 : NULL;
                } while (seg);
                return STRVAL(out);
            }

            if (!strcmp(fn,"raku_grep") && nargs == 2) {
                EXPR_t *blk = e->children[1];
                DESCR_t arrd = interp_eval(e->children[2]);
                const char *as = VARVAL_fn(arrd); if (!as) as = "";
                char *out = GC_strdup("");
                const char *seg = as;
                int first = 1;
                do {
                    const char *nx = strchr(seg, SOH);
                    size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                    char *elem = GC_malloc(elen + 1);
                    memcpy(elem, seg, elen); elem[elen] = '\0';
                    /* bind $_ — walk closure tree; $_ has sval="$_" or "_", use ival as slot */
                    { /* elem: INTVAL if numeric, else STRVAL */
                      char *_ep_ev; long _iv_ev = strtol(elem, &_ep_ev, 10);
                      DESCR_t _ev = (*_ep_ev == '\0' && _ep_ev > elem) ? INTVAL(_iv_ev) : STRVAL(elem);
                      int _sl = -1;
                      EXPR_t *_stk[64]; int _sn=0; _stk[_sn++]=blk;
                      while (_sn>0 && _sl<0) {
                          EXPR_t *_n=_stk[--_sn];
                          if (!_n) continue;
                          if (_n->kind==E_VAR && _n->sval) {
                              const char *_sv = _n->sval;
                              /* match "$_" or "_" (sigil may be stripped) */
                              if (strcmp(_sv,"$_")==0 || strcmp(_sv,"_")==0)
                                  _sl=(int)_n->ival;
                          }
                          for(int _ci=0;_ci<_n->nchildren&&_sn<62;_ci++) _stk[_sn++]=_n->children[_ci];
                      }
                      if (_sl >= 0 && _sl < FRAME.env_n) FRAME.env[_sl] = _ev;
                      else NV_SET_fn("$_", _ev); }
                    DESCR_t r = interp_eval(blk);
                    /* RK-24: grep truthy = block did not fail (SNOBOL4 success/fail semantics).
                     * E_EQ/E_LT etc return FAILDESCR on false, non-fail on true. */
                    int truthy = !IS_FAIL_fn(r);
                    if (truthy) {
                        size_t ol = strlen(out), el = strlen(elem);
                        char *nout = GC_malloc(ol + el + 2);
                        memcpy(nout, out, ol);
                        if (!first) { nout[ol] = SOH; memcpy(nout+ol+1,elem,el); nout[ol+1+el]='\0'; }
                        else        { memcpy(nout+ol,elem,el); nout[ol+el]='\0'; first=0; }
                        out = nout;
                    }
                    seg = nx ? nx + 1 : NULL;
                } while (seg);
                return STRVAL(out);
            }

            if (!strcmp(fn,"raku_sort") && (nargs == 1 || nargs == 2)) {
                /* Simple lexicographic sort; numeric if all-integer elements.
                 * With block (nargs==2): use $a/$b comparator block. */
                DESCR_t arrd = interp_eval(e->children[nargs == 2 ? 2 : 1]);
                const char *as = VARVAL_fn(arrd); if (!as || !*as) return STRVAL(GC_strdup(""));
                EXPR_t *blk = (nargs == 2) ? e->children[1] : NULL;
                /* split into array of strings */
                int cnt = 1; for (const char *p=as;*p;p++) if(*p==SOH) cnt++;
                char **elems = GC_malloc((size_t)cnt * sizeof(char*));
                int idx = 0; const char *seg = as;
                do {
                    const char *nx = strchr(seg, SOH);
                    size_t elen = nx ? (size_t)(nx-seg) : strlen(seg);
                    char *elem = GC_malloc(elen+1); memcpy(elem,seg,elen); elem[elen]='\0';
                    elems[idx++] = elem;
                    seg = nx ? nx+1 : NULL;
                } while (seg && idx < cnt);
                /* sort: comparator block or default lexicographic */
                if (blk) {
                    /* insertion sort using block with $a/$b */
                    for (int i=1;i<cnt;i++) {
                        char *key = elems[i]; int j=i-1;
                        while (j>=0) {
                            NV_SET_fn("$a", STRVAL(elems[j]));
                            NV_SET_fn("$b", STRVAL(key));
                            DESCR_t r = interp_eval(blk);
                            long cmp = IS_INT_fn(r) ? r.i : 0;
                            if (cmp <= 0) break;
                            elems[j+1]=elems[j]; j--;
                        }
                        elems[j+1]=key;
                    }
                } else {
                    /* check if all-integer */
                    int all_int = 1;
                    for (int i=0;i<cnt&&all_int;i++) {
                        char *ep; strtol(elems[i],&ep,10);
                        if (*ep) all_int=0;
                    }
                    /* qsort */
                    if (all_int) {
                        /* numeric sort via simple insertion */
                        for (int i=1;i<cnt;i++) {
                            char *key=elems[i]; long kv=atol(key); int j=i-1;
                            while (j>=0 && atol(elems[j])>kv) { elems[j+1]=elems[j]; j--; }
                            elems[j+1]=key;
                        }
                    } else {
                        for (int i=1;i<cnt;i++) {
                            char *key=elems[i]; int j=i-1;
                            while (j>=0 && strcmp(elems[j],key)>0) { elems[j+1]=elems[j]; j--; }
                            elems[j+1]=key;
                        }
                    }
                }
                /* rejoin */
                size_t total=0; for(int i=0;i<cnt;i++) total+=strlen(elems[i])+1;
                char *out=GC_malloc(total+1); out[0]='\0';
                for (int i=0;i<cnt;i++) {
                    if (i) { size_t ol=strlen(out); out[ol]=SOH; out[ol+1]='\0'; }
                    strcat(out,elems[i]);
                }
                return STRVAL(out);
            }
#undef SOH

            /* ── RK-26: Raku OO builtins ────────────────────────────────────
             * raku_new(classname, key1, val1, key2, val2, ...)
             *   → find registered ScDatType by name, construct instance,
             *     assign named args to matching fields.
             * raku_mcall(obj, methname, arg1, arg2, ...)
             *   → look up obj's datatype name, find "TypeName__methname" proc
             *     in proc_table, call it with (obj, arg1, arg2, ...).
             * ──────────────────────────────────────────────────────────────*/
            if (!strcmp(fn,"raku_new")) {
                /* children: [fn_name_var, classname_qlit, key1, val1, ...] */
                /* e->children[0] = E_VAR("raku_new") (make_call layout)
                 * e->children[1] = E_QLIT(classname)
                 * e->children[2..] = alternating key, val */
                if (e->nchildren < 2) return NULVCL;
                DESCR_t cnameD = interp_eval(e->children[1]);
                const char *cname = VARVAL_fn(cnameD);
                if (!cname || !*cname) return FAILDESCR;
                ScDatType *t = sc_dat_find_type(cname);
                if (!t) return FAILDESCR;
                /* Build field array in order matching type definition */
                DESCR_t fvals[64];
                for (int i=0;i<t->nfields && i<64;i++) fvals[i]=NULVCL;
                /* Walk named pairs: children[2],children[3] = key,val ... */
                for (int ci=2; ci+1 < e->nchildren; ci+=2) {
                    DESCR_t kD = interp_eval(e->children[ci]);
                    DESCR_t vD = interp_eval(e->children[ci+1]);
                    const char *kname = VARVAL_fn(kD);
                    if (!kname) continue;
                    for (int fi=0;fi<t->nfields;fi++) {
                        if (strcasecmp(t->fields[fi], kname)==0) { fvals[fi]=vD; break; }
                    }
                }
                return sc_dat_construct(t, fvals, t->nfields);
            }

            if (!strcmp(fn,"raku_mcall")) {
                /* children: [fn_var, obj, methname_qlit, arg1, arg2, ...] */
                if (e->nchildren < 3) return FAILDESCR;
                DESCR_t obj    = interp_eval(e->children[1]);
                DESCR_t mnameD = interp_eval(e->children[2]);
                const char *mname = VARVAL_fn(mnameD);
                if (!mname || !*mname) return FAILDESCR;
                /* Determine class name from obj's DT_DATA type */
                const char *cname = NULL;
                if (obj.v == DT_DATA && obj.u) {
                    DATINST_t *inst = (DATINST_t *)obj.u;
                    if (inst->type) cname = inst->type->name;
                }
                if (!cname) return FAILDESCR;
                /* Build proc name: "ClassName__methname" */
                char procname[256];
                snprintf(procname, sizeof procname, "%s__%s", cname, mname);
                /* Find in proc_table */
                int pi;
                for (pi = 0; pi < proc_count; pi++)
                    if (strcmp(proc_table[pi].name, procname) == 0) break;
                if (pi >= proc_count) return FAILDESCR;
                /* Build arg array: self=obj, then extra args */
                int nextra = e->nchildren - 3;
                int total  = 1 + nextra;
                DESCR_t *callargs = GC_malloc((size_t)total * sizeof(DESCR_t));
                callargs[0] = obj;
                for (int i=0;i<nextra;i++) callargs[i+1] = interp_eval(e->children[3+i]);
                return coro_call(proc_table[pi].proc, callargs, total);
            }

            /* ── IC-3: Icon table builtins (DT_T native hash table) ────────
             * table()         → new empty table (default value = &null)
             * insert(T,k,v)   → set T[k]=v, return T
             * delete(T,k)     → remove T[k], return T
             * member(T,k)     → return T[k] if present, else fail
             * key(T)          → generator: yields each key (via every)     */
            if (!strcmp(fn,"table") && nargs <= 2) {
                /* Icon: table()      → empty table, default=&null
                 *       table(x)     → empty table, default=x
                 *       table(n,inc) → SNOBOL4 compat (ignored for Icon) */
                TBBLK_t *tbl = table_new();
                if (nargs == 1) {
                    /* Icon table(dflt) — one arg is the default value */
                    tbl->dflt = interp_eval(e->children[1]);
                } else {
                    tbl->dflt = NULVCL;
                }
                DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = tbl;
                return d;
            }
            if (!strcmp(fn,"insert") && nargs >= 2) {
                DESCR_t td = interp_eval(e->children[1]);
                if (td.v != DT_T) return FAILDESCR;
                DESCR_t kd = interp_eval(e->children[2]);
                DESCR_t vd = (nargs >= 3) ? interp_eval(e->children[3]) : NULVCL;
                char kb[64]; const char *ks;
                if (IS_INT_fn(kd))       { snprintf(kb,sizeof kb,"%lld",(long long)kd.i); ks=kb; }
                else if (IS_REAL_fn(kd)) { snprintf(kb,sizeof kb,"%g",kd.r); ks=kb; }
                else                     { ks = VARVAL_fn(kd); if (!ks) ks=""; }
                table_set_descr(td.tbl, ks, kd, vd);
                return td;
            }
            if (!strcmp(fn,"delete") && nargs >= 1) {
                /* IC-9: delete arity fix — Icon delete(T, k, …) takes one key;
                 * extra args are ignored.  delete(T) with no key uses &null
                 * as the key (Icon's null-arg padding semantics).            */
                DESCR_t td = interp_eval(e->children[1]);
                if (td.v != DT_T) return FAILDESCR;
                DESCR_t kd;
                if (nargs >= 2) kd = interp_eval(e->children[2]);
                else            kd = NULVCL;       /* delete(T) → key=&null */
                char kb[64]; const char *ks;
                if (IS_INT_fn(kd))       { snprintf(kb,sizeof kb,"%lld",(long long)kd.i); ks=kb; }
                else if (IS_REAL_fn(kd)) { snprintf(kb,sizeof kb,"%g",kd.r); ks=kb; }
                else                     { ks = VARVAL_fn(kd); if (!ks) ks=""; }
                /* walk bucket using same djb2 hash as _tbl_hash in binary:
                 * init=0x1505, hash = hash*33 ^ c, result & 0xFF */
                unsigned h = 0x1505;
                { const char *p=ks; while(*p) { h=(h<<5)+h^(unsigned char)*p++; } h&=0xFF; }
                TBPAIR_t **pp = &td.tbl->buckets[h];
                while (*pp) {
                    if (strcmp((*pp)->key, ks)==0) { TBPAIR_t *del=*pp; *pp=del->next; td.tbl->size--; break; }
                    pp = &(*pp)->next;
                }
                return td;
            }
            if (!strcmp(fn,"member") && nargs >= 1) {
                /* IC-9: Icon member(T, k, …) tests whether k is a key.
                 * 1-arg member(T) uses &null as the key (Icon null-arg padding).
                 * Extra args ignored.                                          */
                DESCR_t td = interp_eval(e->children[1]);
                if (td.v != DT_T) return FAILDESCR;
                DESCR_t kd;
                if (nargs >= 2) kd = interp_eval(e->children[2]);
                else            kd = NULVCL;       /* member(T) → key=&null */
                char kb[64]; const char *ks;
                if (IS_INT_fn(kd))       { snprintf(kb,sizeof kb,"%lld",(long long)kd.i); ks=kb; }
                else if (IS_REAL_fn(kd)) { snprintf(kb,sizeof kb,"%g",kd.r); ks=kb; }
                else                     { ks = VARVAL_fn(kd); if (!ks) ks=""; }
                if (!table_has(td.tbl, ks)) return FAILDESCR;
                return table_get(td.tbl, ks);
            }

            /* ── IC-5: key(T) — generator yielding each key of a table ───── */
            if (!strcmp(fn,"key") && nargs == 1) {
                DESCR_t td = interp_eval(e->children[1]);
                if (td.v != DT_T || !td.tbl) return FAILDESCR;
                /* oneshot: return first key; every uses coro_bb_tbl_iterate for keys */
                for (int _bi = 0; _bi < TABLE_BUCKETS; _bi++)
                    if (td.tbl->buckets[_bi])
                        return td.tbl->buckets[_bi]->key_descr;
                return FAILDESCR;
            }

            /* ── IC-5: integer(x), real(x), string(x), numeric(x) ──────────*/
            if (!strcmp(fn,"integer") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                if (IS_INT_fn(av)) return av;
                if (IS_REAL_fn(av)) return INTVAL((long long)av.r);
                const char *s = VARVAL_fn(av); if (!s) return FAILDESCR;
                /* IC-9 (2026-05-01): Icon radix prefix `BASErDIGITS` —
                 * 2..36, case-insensitive 'r'/'R', digits 0-9 + a-z (10..35). */
                {
                    const char *p = s;
                    while (*p == ' ' || *p == '\t') p++;
                    int neg = 0; if (*p == '+') p++; else if (*p == '-') { neg = 1; p++; }
                    int base = 0; const char *bstart = p;
                    while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
                    if (p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36) {
                        p++; const char *dstart = p; long long v = 0;
                        while (*p) {
                            int d = -1;
                            if (*p >= '0' && *p <= '9') d = *p - '0';
                            else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
                            else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
                            if (d < 0 || d >= base) break;
                            v = v * base + d;
                            p++;
                        }
                        while (*p == ' ' || *p == '\t') p++;
                        if (p > dstart && *p == '\0') return INTVAL(neg ? -v : v);
                    }
                }
                char *end; long long iv = strtoll(s, &end, 10);
                if (end != s && (*end=='\0'||*end==' ')) return INTVAL(iv);
                /* try real→int */
                double rv = strtod(s, &end);
                if (end != s && (*end=='\0'||*end==' ')) return INTVAL((long long)rv);
                return FAILDESCR;
            }
            if (!strcmp(fn,"real") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                if (IS_REAL_fn(av)) return av;
                if (IS_INT_fn(av)) return REALVAL((double)av.i);
                const char *s = VARVAL_fn(av); if (!s) return FAILDESCR;
                char *end; double rv = strtod(s, &end);
                if (end != s && (*end=='\0'||*end==' ')) return REALVAL(rv);
                return FAILDESCR;
            }
            if (!strcmp(fn,"string") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                if (IS_STR_fn(av)) return av;
                char *buf = GC_malloc(64);
                if (IS_INT_fn(av))       snprintf(buf,64,"%lld",(long long)av.i);
                else if (IS_REAL_fn(av)) { real_str(av.r,buf,64); }
                else return NULVCL;
                return STRVAL(buf);
            }
            if (!strcmp(fn,"numeric") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                if (IS_INT_fn(av)||IS_REAL_fn(av)) return av;
                const char *s = VARVAL_fn(av); if (!s||!*s) return FAILDESCR;
                /* IC-9: Icon radix prefix `BASErDIGITS` (numeric also accepts radix). */
                {
                    const char *p = s;
                    while (*p == ' ' || *p == '\t') p++;
                    int neg = 0; if (*p == '+') p++; else if (*p == '-') { neg = 1; p++; }
                    int base = 0; const char *bstart = p;
                    while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
                    if (p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36) {
                        p++; const char *dstart = p; long long v = 0;
                        while (*p) {
                            int d = -1;
                            if (*p >= '0' && *p <= '9') d = *p - '0';
                            else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
                            else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
                            if (d < 0 || d >= base) break;
                            v = v * base + d;
                            p++;
                        }
                        while (*p == ' ' || *p == '\t') p++;
                        if (p > dstart && *p == '\0') return INTVAL(neg ? -v : v);
                    }
                }
                char *end; long long iv = strtoll(s, &end, 10);
                if (end!=s && (*end=='\0'||*end==' ')) return INTVAL(iv);
                double rv = strtod(s, &end);
                if (end!=s && (*end=='\0'||*end==' ')) return REALVAL(rv);
                return FAILDESCR;
            }

            /* IC-9 (2026-05-01): Icon `list(n, x)` constructor.
             *   list()       — empty list
             *   list(n)      — n elements all &null
             *   list(n, x)   — n elements all = x
             * Omitted/&null `n` defaults to 0.  Negative or non-integer n fails. */
            if (!strcmp(fn,"list") && nargs >= 0) {
                int n = 0;
                DESCR_t init = NULVCL;
                if (nargs >= 1) {
                    DESCR_t nv = interp_eval(e->children[1]);
                    if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) {
                        if (IS_INT_fn(nv)) n = (int)nv.i;
                        else if (IS_REAL_fn(nv)) n = (int)nv.r;
                        else return FAILDESCR;
                        if (n < 0) return FAILDESCR;
                    }
                }
                if (nargs >= 2) {
                    DESCR_t iv = interp_eval(e->children[2]);
                    if (!IS_FAIL_fn(iv)) init = iv;
                }
                static int icnlist_reg2 = 0;
                if (!icnlist_reg2) { DEFDAT_fn("icnlist(frame_elems,frame_size,icn_type)"); icnlist_reg2 = 1; }
                DESCR_t *elems = GC_malloc((n>0?n:1)*sizeof(DESCR_t));
                for (int i = 0; i < n; i++) elems[i] = init;
                DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
                return DATCON_fn("icnlist", eptr, INTVAL(n), STRVAL("list"));
            }

            /* ── IC-5: Icon list builtins: push/pull/put/get ───────────────
             * Icon lists stored as DT_DATA with type name "icnlist" and a
             * DT_A array field "elems".  We use a simple GC array of DESCR_t. */
            if ((!strcmp(fn,"push")||!strcmp(fn,"put")||!strcmp(fn,"get")||!strcmp(fn,"pull")) && nargs >= 1) {
                DESCR_t ld = interp_eval(e->children[1]);
                /* push(L, v) — prepend */
                if (!strcmp(fn,"push") && nargs == 2) {
                    DESCR_t vd = interp_eval(e->children[2]);
                    if (ld.v != DT_DATA) return FAILDESCR;
                    int n = (int)FIELD_GET_fn(ld,"frame_size").i;
                    DESCR_t ea = FIELD_GET_fn(ld,"frame_elems");
                    DESCR_t *old = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
                    DESCR_t *nb = GC_malloc((n+1)*sizeof(DESCR_t));
                    nb[0] = vd;
                    if (old && n > 0) memcpy(nb+1,old,n*sizeof(DESCR_t));
                    FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
                    FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
                    return ld;
                }
                /* put(L, v) — append */
                if (!strcmp(fn,"put") && nargs == 2) {
                    DESCR_t vd = interp_eval(e->children[2]);
                    if (ld.v != DT_DATA) return FAILDESCR;
                    int n = (int)FIELD_GET_fn(ld,"frame_size").i;
                    DESCR_t ea = FIELD_GET_fn(ld,"frame_elems");
                    DESCR_t *old = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
                    DESCR_t *nb = GC_malloc((n+1)*sizeof(DESCR_t));
                    if (old && n > 0) memcpy(nb,old,n*sizeof(DESCR_t));
                    nb[n] = vd;
                    FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
                    FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
                    return ld;
                }
                /* get(L) — remove and return first element */
                if (!strcmp(fn,"get") && nargs == 1) {
                    if (ld.v != DT_DATA) return FAILDESCR;
                    DESCR_t ea = FIELD_GET_fn(ld,"frame_elems");
                    int n = (int)FIELD_GET_fn(ld,"frame_size").i;
                    DESCR_t *arr = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
                    if (!arr || n <= 0) return FAILDESCR;
                    DESCR_t ret = arr[0];
                    FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
                    FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
                    /* write back to var if possible */
                    if (e->children[1]->kind==E_VAR) {
                        int sl=(int)e->children[1]->ival;
                        if(sl>=0&&sl<FRAME.env_n) FRAME.env[sl]=ld;
                    }
                    return ret;
                }
                /* pull(L) — remove and return last element */
                if (!strcmp(fn,"pull") && nargs == 1) {
                    if (ld.v != DT_DATA) return FAILDESCR;
                    DESCR_t ea = FIELD_GET_fn(ld,"frame_elems");
                    int n = (int)FIELD_GET_fn(ld,"frame_size").i;
                    DESCR_t *arr = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
                    if (!arr || n <= 0) return FAILDESCR;
                    DESCR_t ret = arr[n-1];
                    FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
                    if (e->children[1]->kind==E_VAR) {
                        int sl=(int)e->children[1]->ival;
                        if(sl>=0&&sl<FRAME.env_n) FRAME.env[sl]=ld;
                    }
                    return ret;
                }
            }

            /* ── IC-5: char(n), ord(s) ──────────────────────────────────── */
            if (!strcmp(fn,"char") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                int n = (int)(IS_INT_fn(av) ? av.i : (long long)strtol(VARVAL_fn(av)?VARVAL_fn(av):"0",NULL,10));
                char *buf = GC_malloc(2); buf[0]=(char)(n&0xFF); buf[1]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"ord") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                const char *s = VARVAL_fn(av); if (!s||!*s) return FAILDESCR;
                return INTVAL((unsigned char)s[0]);
            }

            /* ── IC-5: left/right/center/repl/reverse/map/trim ─────────── */
            /* Icon spec: left(s, i, p) — i defaults to 1, p defaults to " ".
             * Omitted args (`left(s, , p)` or `left(s, &null, p)`) take the default.
             * Pad rule (verified against JCON test corpus):
             *   - LEFT-pads (right's left, center's left): fill[i % fl] from start of pad.
             *   - RIGHT-pads (left's right, center's right): pad ends at fill[fl-1] —
             *     pad[i] = fill[(i + fl - padlen) % fl] so the last pad char is fill[fl-1].
             */
            if (!strcmp(fn,"left") && nargs >= 1) {
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                int sl=(int)strlen(s);
                int n = 1;
                if (nargs >= 2) {
                    DESCR_t nv = interp_eval(e->children[2]);
                    if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
                }
                if (n < 0) n = 0;
                const char *fill=" "; int fl=1;
                if (nargs >= 3) {
                    DESCR_t fd = interp_eval(e->children[3]);
                    if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                        const char *fs = VARVAL_fn(fd);
                        if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
                    }
                }
                char *buf=GC_malloc(n+1);
                int copy = sl < n ? sl : n;
                for (int i = 0; i < copy; i++) buf[i] = s[i];
                int rpad = n - copy;
                /* Right-pad: pad ends at fill[fl-1].  Padlen rpad, pad index k=0..rpad-1,
                 * pad char = fill[(k + fl - rpad) % fl] (clamped non-negative). */
                for (int k = 0; k < rpad; k++) {
                    int idx = ((k + fl - rpad) % fl + fl) % fl;
                    buf[copy + k] = fill[idx];
                }
                buf[n]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"right") && nargs >= 1) {
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                int sl=(int)strlen(s);
                int n = 1;
                if (nargs >= 2) {
                    DESCR_t nv = interp_eval(e->children[2]);
                    if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
                }
                if (n < 0) n = 0;
                const char *fill=" "; int fl=1;
                if (nargs >= 3) {
                    DESCR_t fd = interp_eval(e->children[3]);
                    if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                        const char *fs = VARVAL_fn(fd);
                        if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
                    }
                }
                char *buf=GC_malloc(n+1);
                int pad = n - sl; if (pad < 0) pad = 0;
                /* Left-pad: fill cycles starting at fill[0]. */
                for (int i = 0; i < pad; i++) buf[i] = fill[i % fl];
                int srcoff = (sl > n) ? (sl - n) : 0;
                int copy = sl - srcoff; if (pad + copy > n) copy = n - pad;
                for (int i = 0; i < copy; i++) buf[pad + i] = s[srcoff + i];
                buf[n]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"center") && nargs >= 1) {
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                int sl=(int)strlen(s);
                int n = 1;
                if (nargs >= 2) {
                    DESCR_t nv = interp_eval(e->children[2]);
                    if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
                }
                if (n < 0) n = 0;
                const char *fill=" "; int fl=1;
                if (nargs >= 3) {
                    DESCR_t fd = interp_eval(e->children[3]);
                    if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                        const char *fs = VARVAL_fn(fd);
                        if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
                    }
                }
                char *buf=GC_malloc(n+1);
                int lpad = (n - sl) / 2; if (lpad < 0) lpad = 0;
                /* When source longer than n, take middle n chars; offset rounds UP
                 * (right-bias) per Icon spec.  Verified against JCON center test. */
                int srcoff = (sl > n) ? (sl - n + 1) / 2 : 0;
                int copy = sl - srcoff; if (lpad + copy > n) copy = n - lpad;
                int rpad = n - lpad - copy;
                /* lpad — left-aligned cycle starting at fill[0] */
                for (int i = 0; i < lpad; i++) buf[i] = fill[i % fl];
                /* source */
                for (int i = 0; i < copy; i++) buf[lpad + i] = s[srcoff + i];
                /* rpad — pad ends at fill[fl-1] */
                for (int k = 0; k < rpad; k++) {
                    int idx = ((k + fl - rpad) % fl + fl) % fl;
                    buf[lpad + copy + k] = fill[idx];
                }
                buf[n]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"repl") && nargs == 2) {
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                int n=(int)to_int(interp_eval(e->children[2])); if(n<0)n=0;
                int sl=(int)strlen(s); char *buf=GC_malloc(sl*n+1); buf[0]='\0';
                for(int i=0;i<n;i++) memcpy(buf+i*sl,s,sl); buf[sl*n]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"reverse") && nargs == 1) {
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                int sl=(int)strlen(s); char *buf=GC_malloc(sl+1);
                for(int i=0;i<sl;i++) buf[i]=s[sl-1-i]; buf[sl]='\0';
                return STRVAL(buf);
            }
            if (!strcmp(fn,"map") && nargs >= 1 && nargs <= 3) {
                /* Icon map(s, c1, c2): c1 defaults &ucase, c2 defaults &lcase.
                 * Each char of s — if it appears in c1 at index k, replaced by c2[k];
                 * else passed through. c1 and c2 must be same length (truncated).
                 * &null arg (omitted) → use the default. */
                static const char *UCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                static const char *LCASE = "abcdefghijklmnopqrstuvwxyz";
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                const char *from = UCASE, *to = LCASE;
                if (nargs >= 2) {
                    DESCR_t fv=interp_eval(e->children[2]);
                    if (fv.v != DT_SNUL) {
                        const char *fs = VARVAL_fn(fv);
                        if (fs) from = fs;
                    }
                }
                if (nargs >= 3) {
                    DESCR_t tv=interp_eval(e->children[3]);
                    if (tv.v != DT_SNUL) {
                        const char *ts = VARVAL_fn(tv);
                        if (ts) to = ts;
                    }
                }
                int sl=(int)strlen(s); char *buf=GC_malloc(sl+1);
                int fl=(int)strlen(from), tl=(int)strlen(to);
                /* Icon spec: c1 and c2 same length — fail otherwise.
                 * Walk c1 RIGHT-TO-LEFT so duplicate keys: last definition wins
                 * (Icon: map("abcdef","aa","bc") → "cbcdef" — second 'a'→'c'). */
                for (int i=0;i<sl;i++) {
                    char c=s[i]; int hit=0;
                    for (int j=fl-1;j>=0;j--) {
                        if (from[j]==c) { buf[i] = (j<tl) ? to[j] : c; hit=1; break; }
                    }
                    if (!hit) buf[i]=c;
                }
                buf[sl]='\0'; return STRVAL(buf);
            }
            if (!strcmp(fn,"trim") && (nargs == 1 || nargs == 2)) {
                DESCR_t sv=interp_eval(e->children[1]); const char *s=VARVAL_fn(sv); if(!s)s="";
                /* Determine cset of trim chars. Default: space.
                 * 2-arg: c arg may be string or cset (we treat both as char list).
                 * &null arg → use default. */
                const char *cset = " ";
                if (nargs == 2) {
                    DESCR_t cv = interp_eval(e->children[2]);
                    if (cv.v != DT_SNUL) {
                        const char *cs = VARVAL_fn(cv);
                        if (cs) cset = cs;
                    }
                }
                if (g_lang == 1 || nargs == 2) {
                    /* Icon: trim trailing chars in cset */
                    int sl=(int)strlen(s);
                    while (sl > 0 && strchr(cset, s[sl-1])) sl--;
                    char *buf=GC_malloc(sl+1); memcpy(buf,s,sl); buf[sl]='\0';
                    return STRVAL(buf);
                } else {
                    /* Raku trim — both ends, whitespace only */
                    while(*s==' '||*s=='\t'||*s=='\n'||*s=='\r') s++;
                    size_t len=strlen(s);
                    while(len>0&&(s[len-1]==' '||s[len-1]=='\t'||s[len-1]=='\n'||s[len-1]=='\r')) len--;
                    char *buf=GC_malloc(len+1); memcpy(buf,s,len); buf[len]='\0';
                    return STRVAL(buf);
                }
            }
            /* ── IC-5: type(x), image(x), copy(x) ──────────────────────── */
            if (!strcmp(fn,"type") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                const char *t;
                if (IS_INT_fn(av))       t="integer";
                else if (IS_REAL_fn(av)) t="real";
                else if (av.v==DT_T)     t="table";
                else if (av.v==DT_A)     t="list";
                else if (av.v==DT_DATA)  {
                    /* check if icnlist tag */
                    DESCR_t tag = FIELD_GET_fn(av,"icn_type");
                    t = (tag.v==DT_S && tag.s) ? tag.s : "record";
                }
                else t="string";
                return STRVAL(t);
            }
            if (!strcmp(fn,"image") && nargs == 0) {
                /* IC-9: image() with no args — Icon spec: missing arg defaults to &null */
                return STRVAL("&null");
            }
            if (!strcmp(fn,"image") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                if (IS_FAIL_fn(av)) return FAILDESCR;  /* IC-9: image(?T_empty) etc must fail */
                char *buf = GC_malloc(128);
                if (av.v == DT_SNUL)     return STRVAL("&null");
                if (IS_INT_fn(av))       { snprintf(buf,128,"%lld",(long long)av.i); return STRVAL(buf); }
                if (IS_REAL_fn(av))      { real_str(av.r,buf,128); return STRVAL(buf); }
                if (av.v==DT_T)          { snprintf(buf,128,"table(%d)",av.tbl?av.tbl->size:0); return STRVAL(buf); }
                if (av.v==DT_DATA)       { snprintf(buf,128,"record"); return STRVAL(buf); }
                /* String: produce "abc" with C-style escapes for control chars and " and \. */
                const char *s=VARVAL_fn(av); if (!s) s = "";
                int sl = (int)strlen(s);
                /* Worst-case expansion: each byte → \xNN (4 chars) + 2 quotes + NUL */
                char *out = GC_malloc(sl*4 + 3);
                int o = 0;
                out[o++] = '"';
                for (int i = 0; i < sl; i++) {
                    unsigned char c = (unsigned char)s[i];
                    switch (c) {
                        case '"':  out[o++]='\\'; out[o++]='"';  break;
                        case '\\': out[o++]='\\'; out[o++]='\\'; break;
                        case '\n': out[o++]='\\'; out[o++]='n';  break;
                        case '\t': out[o++]='\\'; out[o++]='t';  break;
                        case '\r': out[o++]='\\'; out[o++]='r';  break;
                        default:
                            if (c < 0x20 || c == 0x7f) {
                                o += snprintf(out+o, 5, "\\x%02x", c);
                            } else {
                                out[o++] = (char)c;
                            }
                    }
                }
                out[o++] = '"';
                out[o] = '\0';
                return STRVAL(out);
            }
            if (!strcmp(fn,"copy") && nargs == 1) {
                /* IC-9: shallow copy — Icon `copy(X)` returns a new container
                 * whose elements are the same descriptors (no deep copy of
                 * referenced values).  Previously a no-op (returned the same
                 * DESCR_t), which aliased the original — every !y +:= V
                 * mutated x as well.  Now allocates a fresh TBBLK_t / icnlist
                 * and copies entries.                                       */
                DESCR_t src = interp_eval(e->children[1]);
                if (src.v == DT_T && src.tbl) {
                    TBBLK_t *nt = table_new();
                    nt->dflt = src.tbl->dflt;
                    nt->init = src.tbl->init;
                    nt->inc  = src.tbl->inc;
                    for (int b = 0; b < TABLE_BUCKETS; b++)
                        for (TBPAIR_t *p = src.tbl->buckets[b]; p; p = p->next)
                            table_set_descr(nt, p->key, p->key_descr, p->val);
                    DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = nt;
                    return d;
                }
                if (src.v == DT_DATA) {
                    DESCR_t tag = FIELD_GET_fn(src, "icn_type");
                    if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                        DESCR_t ea = FIELD_GET_fn(src, "frame_elems");
                        int n = (int)FIELD_GET_fn(src, "frame_size").i;
                        DESCR_t *src_elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                        /* Build a fresh icnlist mirroring the E_MAKELIST shape. */
                        DESCR_t *new_elems = (DESCR_t *)GC_malloc((size_t)(n > 0 ? n : 1) * sizeof(DESCR_t));
                        if (src_elems && n > 0) memcpy(new_elems, src_elems, (size_t)n * sizeof(DESCR_t));
                        DESCR_t eptr; eptr.v = DT_DATA; eptr.slen = 0; eptr.ptr = (void *)new_elems;
                        return DATCON_fn("icnlist", eptr, INTVAL(n), STRVAL("list"));
                    }
                }
                /* For strings, integers, reals, sets — value semantics already
                 * give a fresh descriptor; return src directly.              */
                return src;
            }

            /* ── IC-5: swap(L, k) is actually handled as E_SWAP op ─────── */
            /* ── IC-5: size *L for DT_DATA lists ──────────────────────────
             * E_SIZE is handled below; nothing to add in E_FNC.           */

            /* ── IC-7: math builtins: abs, max, min, sqrt ──────────────── */
            if (!strcmp(fn,"abs") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                if (IS_REAL_fn(av)) return REALVAL(fabs(av.r));
                return INTVAL(av.i < 0 ? -av.i : av.i);
            }
            if (!strcmp(fn,"max") && nargs >= 2) {
                DESCR_t best = interp_eval(e->children[1]);
                for (int _j = 2; _j <= nargs; _j++) {
                    DESCR_t cv = interp_eval(e->children[_j]);
                    int gt = (IS_REAL_fn(best)||IS_REAL_fn(cv))
                        ? ((IS_REAL_fn(best)?best.r:(double)best.i) < (IS_REAL_fn(cv)?cv.r:(double)cv.i))
                        : (best.i < cv.i);
                    if (gt) best = cv;
                }
                return best;
            }
            if (!strcmp(fn,"min") && nargs >= 2) {
                DESCR_t best = interp_eval(e->children[1]);
                for (int _j = 2; _j <= nargs; _j++) {
                    DESCR_t cv = interp_eval(e->children[_j]);
                    int lt = (IS_REAL_fn(best)||IS_REAL_fn(cv))
                        ? ((IS_REAL_fn(best)?best.r:(double)best.i) > (IS_REAL_fn(cv)?cv.r:(double)cv.i))
                        : (best.i > cv.i);
                    if (lt) best = cv;
                }
                return best;
            }
            if (!strcmp(fn,"sqrt") && nargs == 1) {
                DESCR_t av = interp_eval(e->children[1]);
                double v = IS_REAL_fn(av) ? av.r : (double)av.i;
                return REALVAL(sqrt(v));
            }
            /* seq(i) / seq(i,j) — generator: i, i+1, i+2, ... (up to j if given).
             * Returns first value here; coro_eval handles E_FNC "seq" as a box. */
            if (!strcmp(fn,"seq") && nargs >= 1) {
                DESCR_t start = interp_eval(e->children[1]);
                return IS_INT_fn(start) ? start : INTVAL(1);
            }

            /* ── IC-7: sort(L) / sortf(L, n) ───────────────────────────── */
            if ((!strcmp(fn,"sort") && nargs == 1) || (!strcmp(fn,"sortf") && nargs == 2)) {
                DESCR_t ld = interp_eval(e->children[1]);
                if (ld.v != DT_DATA) return FAILDESCR;
                DESCR_t ea = FIELD_GET_fn(ld,"frame_elems");
                int n = (int)FIELD_GET_fn(ld,"frame_size").i;
                if (n <= 0) return ld;
                DESCR_t *arr = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
                if (!arr) return ld;
                /* copy into new array for sort */
                DESCR_t *sorted = GC_malloc(n * sizeof(DESCR_t));
                memcpy(sorted, arr, n * sizeof(DESCR_t));
                int field_idx = (!strcmp(fn,"sortf") && nargs == 2)
                    ? (int)to_int(interp_eval(e->children[2])) - 1 : -1;
                /* insertion sort — small lists only; correct semantics */
                for (int _i = 1; _i < n; _i++) {
                    DESCR_t key = sorted[_i]; int _j = _i - 1;
                    while (_j >= 0) {
                        DESCR_t a = sorted[_j], b = key;
                        if (field_idx >= 0) {
                            /* sortf: compare field field_idx of record via DATINST_t */
                            if (a.v==DT_DATA && a.u) { DATINST_t *_ia=(DATINST_t*)a.u; if(_ia->type&&field_idx<_ia->type->nfields) a=_ia->fields[field_idx]; }
                            if (b.v==DT_DATA && b.u) { DATINST_t *_ib=(DATINST_t*)b.u; if(_ib->type&&field_idx<_ib->type->nfields) b=_ib->fields[field_idx]; }
                        }
                        int cmp;
                        if (IS_INT_fn(a) && IS_INT_fn(b)) cmp = (a.i > b.i) ? 1 : (a.i < b.i) ? -1 : 0;
                        else { const char *sa=VARVAL_fn(a),*sb=VARVAL_fn(b); cmp=strcmp(sa?sa:"",sb?sb:""); }
                        if (cmp <= 0) break;
                        sorted[_j+1] = sorted[_j]; _j--;
                    }
                    sorted[_j+1] = key;
                }
                /* build new icnlist with sorted elements */
                DESCR_t res = ld; /* same type tag */
                FIELD_SET_fn(res,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=sorted});
                FIELD_SET_fn(res,"frame_size",INTVAL(n));
                return res;
            }

            /* ── IC-5: record constructor — Icon puts name in children[0]->sval,
             * not in e->sval, so the shared E_FNC handler misses it.
             * Look up fn in sc_dat registry; if found, construct instance. */
            {
                ScDatType *_dt = sc_dat_find_type(fn);
                if (_dt) {
                    DESCR_t _args[FRAME_SLOT_MAX];
                    for (int _j = 0; _j < nargs && _j < FRAME_SLOT_MAX; _j++)
                        _args[_j] = interp_eval(e->children[1+_j]);
                    return sc_dat_construct(_dt, _args, nargs);
                }
            }

            return NULVCL;
        }
        case E_ALT:
        case E_ALTERNATE: {
            /* Icon value alternation: expr1 | expr2 | ... — return leftmost non-fail */
            if (e->nchildren < 1) return FAILDESCR;
            for (int i = 0; i < e->nchildren; i++) {
                DESCR_t v = interp_eval(e->children[i]);
                if (!IS_FAIL_fn(v)) return v;
            }
            return FAILDESCR;
        }
        case E_EVERY: {
            if (e->nchildren < 1) return NULVCL;
            EXPR_t *gen  = e->children[0];
            EXPR_t *body = (e->nchildren > 1) ? e->children[1] : NULL;
            /* IC-2a: coro_eval + BB_PUMP — all goal-directed ops through Byrd boxes.
             * Special case: E_ASSIGN with generative RHS — drive the leaf generator
             * and re-evaluate the full assignment each tick so frame locals are read fresh.
             * e.g.: every total := total + (1 to n) inside a proc body.
             * NOTE: E_AUGOP is NOT special-cased here — it has its own is_suspendable path
             * in the E_AUGOP handler that correctly drives the generator per-tick. */
            if (gen->kind == E_ASSIGN &&
                gen->nchildren >= 2 && is_suspendable(gen->children[1])) {
                EXPR_t *leaf = find_leaf_suspendable(gen->children[1]);
                if (!leaf) leaf = gen->children[1];
                bb_node_t rbox = coro_eval(leaf);
                DESCR_t tick = rbox.fn(rbox.ζ, α);
                while (!IS_FAIL_fn(tick) && !FRAME.returning && !FRAME.loop_break) {
                    FRAME.loop_next = 0;
                    coro_drive_node = leaf;
                    coro_drive_val  = tick;
                    interp_eval(gen);
                    coro_drive_node = NULL;
                    if (body) interp_eval(body);
                    if (FRAME.returning || FRAME.loop_break) break;
                    tick = rbox.fn(rbox.ζ, β);
                }
                FRAME.loop_break = 0;
                FRAME.loop_next = 0;
                return NULVCL;
            }
            /* IC-6: E_SEQ conjunction — every (gen_expr & body_expr).
             * E_SEQ is Icon's & operator. Drive gen (children[0]) as generator;
             * evaluate remaining children as body per successful tick.
             * e.g.: every (x := (1|2|3|4|5)) > 2 & write(x)
             *   gen  = E_SEQ(E_GT(E_ASSIGN(x,alt), 2), E_FNC(write,x))
             * We split: filter_gen = children[0], seq_body = children[1..n-1]. */
            if (gen->kind == E_SEQ && gen->nchildren >= 2 && is_suspendable(gen->children[0])) {
                EXPR_t *filter = gen->children[0];
                bb_node_t fbox = coro_eval(filter);
                DESCR_t tick = fbox.fn(fbox.ζ, α);
                while (!IS_FAIL_fn(tick) && !FRAME.returning && !FRAME.loop_break) {
                    FRAME.loop_next = 0;
                    /* Execute remaining seq children as body */
                    for (int _si = 1; _si < gen->nchildren; _si++)
                        interp_eval(gen->children[_si]);
                    if (body) interp_eval(body);
                    if (FRAME.returning || FRAME.loop_break) break;
                    tick = fbox.fn(fbox.ζ, β);
                }
                FRAME.loop_break = 0;
                FRAME.loop_next = 0;
                return NULVCL;
            }
            /* When body==NULL, the box's own side-effects ARE the work (e.g. every write(1 to 5)).
             * When body!=NULL, box produces a value; body runs separately each tick. */
            bb_node_t box = coro_eval(gen);
            /* RK-21: save caller frame depth before pumping — coro_bb_suspend (gather coroutine)
             * leaves its frame on the stack during suspend, so frame_depth increases by 1
             * after box.fn(α). Binding must target the CALLER's frame, not FRAME. */
            int caller_depth = frame_depth;
            DESCR_t val = box.fn(box.ζ, α);
            while (!IS_FAIL_fn(val) && !FRAME.returning && !FRAME.loop_break) {
                FRAME.loop_next = 0;
                /* RK-21: bind loop variable into the CALLER's frame (depth saved before pump). */
                if (gen->sval && *gen->sval && caller_depth >= 1) {
                    IcnFrame *cf = &frame_stack[caller_depth - 1];
                    int slot = scope_get(&cf->sc, gen->sval);
                    if (slot >= 0 && slot < cf->env_n)
                        cf->env[slot] = val;
                    else
                        NV_SET_fn(gen->sval, val);
                }
                if (body) {
                    frame_push(gen, val.v == DT_I ? val.i : 0, val.v == DT_I ? NULL : val.s);
                    /* RK-21: if a coroutine (gather) frame is suspended on the stack,
                     * step frame_depth back to caller_depth so FRAME is the caller's
                     * frame during body execution. The coroutine frame is preserved at
                     * frame_stack[caller_depth] and restored after body runs. */
                    int saved_depth = frame_depth;
                    frame_depth = caller_depth;
                    interp_eval(body);
                    frame_depth = saved_depth;
                    frame_pop();
                }
                if (FRAME.returning || FRAME.loop_break) break;
                val = box.fn(box.ζ, β);
            }
            FRAME.loop_break = 0;
            FRAME.loop_next = 0;
            return NULVCL;
        }
        case E_WHILE: {
            int saved_brk = FRAME.loop_break; FRAME.loop_break = 0;
            int saved_nxt = FRAME.loop_next;  FRAME.loop_next  = 0;
            while (!FRAME.returning && !FRAME.loop_break && !FRAME.suspending &&
                   !IS_FAIL_fn(interp_eval(e->children[0]))) {
                FRAME.loop_next = 0;
                if (e->nchildren > 1) interp_eval(e->children[1]);
                if (FRAME.suspending) break;   /* suspend yield — exit loop, return to coro_call */
            }
            FRAME.loop_break = saved_brk;
            FRAME.loop_next  = saved_nxt;
            return NULVCL;
        }
        case E_UNTIL: {
            int saved_brk = FRAME.loop_break; FRAME.loop_break = 0;
            int saved_nxt = FRAME.loop_next;  FRAME.loop_next  = 0;
            while (!FRAME.returning && !FRAME.loop_break && !FRAME.suspending) {
                DESCR_t cv = (e->nchildren > 0) ? interp_eval(e->children[0]) : FAILDESCR;
                if (!IS_FAIL_fn(cv)) break;
                FRAME.loop_next = 0;
                if (e->nchildren > 1) interp_eval(e->children[1]);
                if (FRAME.suspending) break;
            }
            FRAME.loop_break = saved_brk;
            FRAME.loop_next  = saved_nxt;
            return NULVCL;
        }
        case E_REPEAT: {
            int saved_brk = FRAME.loop_break; FRAME.loop_break = 0;
            int saved_nxt = FRAME.loop_next;  FRAME.loop_next  = 0;
            while (!FRAME.returning && !FRAME.loop_break && !FRAME.suspending) {
                FRAME.loop_next = 0;
                if (e->nchildren > 0) { interp_eval(e->children[0]); if (FRAME.suspending) break; }
            }
            FRAME.loop_break = saved_brk;
            FRAME.loop_next  = saved_nxt;
            return NULVCL;
        }
        case E_SUSPEND: {
            /* Icon suspend: yield a value to coro_drive_fnc loop. */
            DESCR_t val = (e->nchildren > 0) ? interp_eval(e->children[0]) : NULVCL;
            if (!IS_FAIL_fn(val)) {
                FRAME.suspending  = 1;
                FRAME.suspend_val = val;
                FRAME.suspend_do  = (e->nchildren > 1) ? e->children[1] : NULL;
            }
            return NULVCL;
        }
        case E_SEQ: {
            /* IC-9: Icon & conjunction inside a proc frame.  Evaluate children
             * left-to-right; return FAILDESCR on the first failure; return last
             * child's value on full success.  Must live in the icon-frame switch
             * (not only in the shared switch) so that all children are evaluated
             * with frame_depth > 0, keeping E_VAR/E_ASSIGN routed to the
             * frame-local env slots rather than to NV. */
            if (e->nchildren == 0) return NULVCL;
            DESCR_t _seq_last = NULVCL;
            for (int _si = 0; _si < e->nchildren; _si++) {
                _seq_last = interp_eval(e->children[_si]);
                if (IS_FAIL_fn(_seq_last)) return FAILDESCR;
                if (FRAME.returning || FRAME.loop_break || FRAME.loop_next) break;
            }
            return _seq_last;
        }
        case E_SEQ_EXPR: {
            DESCR_t v = NULVCL;
            for (int i = 0; i < e->nchildren && !FRAME.returning && !FRAME.loop_next; i++)
                v = interp_eval(e->children[i]);
            return v;
        }
        case E_IF: {
            if (e->nchildren < 1) return NULVCL;
            EXPR_t *test = e->children[0];
            /* IC-8: goal-directed test.  Icon if-conditions consult their
             * test expression as a generator: if the generator yields ANY
             * non-fail value, the then-branch fires; only if the generator
             * is exhausted without producing a non-fail value does the
             * else-branch fire.  Example (rung36_jcon_primes):
             *     if i % (2 to i-1) = 0 then next
             * succeeds for any divisor `d` in `2..i-1` for which
             * `i % d = 0`.  Pre-IC-8 the test was called via interp_eval
             * once and saw only the first generated value, so the then-
             * branch never fired for composite i.  Now we pump test via
             * coro_eval α/β, stopping on the first success (then-branch)
             * or on exhaustion (else-branch).  Mirrors the E_EVERY pump
             * pattern below. */
            if (is_suspendable(test)) {
                bb_node_t box = coro_eval(test);
                DESCR_t v = box.fn(box.ζ, α);
                if (!IS_FAIL_fn(v) && !FRAME.returning && !FRAME.loop_break)
                    return (e->nchildren > 1) ? interp_eval(e->children[1]) : v;
                return (e->nchildren > 2) ? interp_eval(e->children[2]) : FAILDESCR;
            }
            DESCR_t cv = interp_eval(test);
            if (!IS_FAIL_fn(cv)) { if (e->nchildren > 1) return interp_eval(e->children[1]); }
            else                  { if (e->nchildren > 2) return interp_eval(e->children[2]); }
            return NULVCL;
        }
        case E_LOOP_NEXT: {
            /* `next` — abort the rest of the current loop body, ask the
             * enclosing loop to advance to its next iteration.  Loop
             * handlers (E_EVERY, E_WHILE, E_UNTIL, E_REPEAT) inspect and
             * clear loop_next at iteration boundaries. */
            FRAME.loop_next = 1;
            return NULVCL;
        }
        case E_RETURN: {
            DESCR_t rv = (e->nchildren > 0) ? interp_eval(e->children[0]) : NULVCL;
            FRAME.returning  = 1;
            FRAME.return_val = rv;
            return rv;
        }
        case E_PROC_FAIL: {
            FRAME.returning  = 1;
            FRAME.return_val = FAILDESCR;
            return FAILDESCR;
        }
        default: break;
        }
    }

    switch (e->kind) {
    case E_ILIT:   return INTVAL(e->ival);
    case E_FLIT:   return REALVAL(e->dval);
    case E_QLIT:   return e->sval ? STRVAL(e->sval) : NULVCL;
    case E_NUL:    return NULVCL;

    case E_VAR:
        if (e->sval && *e->sval) {
            /* IC-9: Icon scan-state keywords read in scan body outside any Icon proc frame.
             * When scan_depth > 0 and frame_depth == 0 (e.g. "str" ? body in main()),
             * the icon-frame switch above is skipped, so &pos and &subject must be handled here.
             * Only fires for Icon scan context; SNOBOL4 mode uses NV_GET_fn for &pos etc. */
            if (scan_depth > 0 && !frame_depth && e->sval[0] == '&') {
                const char *kw = e->sval + 1;
                if (!strcmp(kw,"pos"))     return INTVAL(scan_pos);
                if (!strcmp(kw,"subject")) return scan_subj ? STRVAL(scan_subj) : NULVCL;
            }
            /* SN-3: shadow table takes priority — param/local named after a pattern
             * primitive (LEN, ANY, SPAN, …) is invisible to NV_GET_fn. */
            { DESCR_t _sv; if (shadow_get(e->sval, &_sv)) return _sv; }
            DESCR_t _vr = NV_GET_fn(e->sval);
            if (!IS_NULL(_vr)) return _vr;
            /* Zero-arg builtin (ARB, REM, FAIL, SUCCEED, etc.) stored as
               function, not variable — only try if name is a registered fn.
               Guard prevents unset ordinary variables from spuriously calling
               APPLY_fn and triggering Error 5.
               SN-26-binmon-3way: if APPLY_fn returns FAIL (e.g. caller is
               a >0-arity builtin like VALUE invoked with 0 args), don't
               propagate the FAIL up — fall back to the unset-var value
               (NULVCL).  This matches CSNOBOL4 / SPITBOL semantics where
               an unbound identifier passed as an argument is the empty
               string, not a failure that aborts the enclosing call. */
            if (FNCEX_fn(e->sval)) {
                DESCR_t _fr = APPLY_fn(e->sval, NULL, 0);
                if (!IS_FAIL_fn(_fr) && !IS_NULL(_fr)) return _fr;
            }
            return _vr; /* unset variable */
        }
        return NULVCL;

    case E_KEYWORD: {
        if (!e->sval || !*e->sval) return NULVCL;
        /* Keywords are case-insensitive; NV stores them uppercase (e.g. "LCASE","UCASE").
         * Lexer strips '&' and preserves original case — uppercase before lookup. */
        char uc[64]; int _ki;
        for (_ki = 0; e->sval[_ki] && _ki < 63; _ki++)
            uc[_ki] = toupper((unsigned char)e->sval[_ki]);
        uc[_ki] = '\0';
        return NV_GET_fn(uc);
    }

    case E_INTERROGATE: {
        /* ?X — o$int: null string if X succeeds; fail if X fails */
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        return NULVCL;
    }

    case E_NAME: {
        /* .X — dot operator: delegate to NAME_fn (snobol4.c export).
         * NAME_fn returns NAMEVAL for keywords/IO vars (not addressable by ptr)
         * and NAMEPTR (interior ptr) for ordinary NV cells.
         * BP-1: .field(x) — E_FNC child with one arg — must return NAMEPTR into
         * the DATA struct field cell, not a name-table lookup. */
        if (e->nchildren < 1) return FAILDESCR;
        EXPR_t *child = e->children[0];
        if (child->kind == E_FNC && child->sval && child->nchildren == 1) {
            DESCR_t inst = interp_eval(child->children[0]);
            DESCR_t *cell = data_field_ptr(child->sval, inst);
            if (cell) return NAMEPTR(cell);
        }
        if ((child->kind == E_VAR || child->kind == E_KEYWORD)
                && child->sval)
            return NAME_fn(child->sval);
        DESCR_t *cell = interp_eval_ref(child);
        if (cell) return NAMEPTR(cell);
        return FAILDESCR;
    }

    case E_MNS:
        if (e->nchildren < 1) return FAILDESCR;
        return neg(interp_eval(e->children[0]));

    /* OE-5: E_RETURN for Icon/Raku return statements */
    case E_RETURN: {
        if (frame_depth > 0) {
            FRAME.return_val = (e->nchildren > 0)
                ? interp_eval(e->children[0]) : NULVCL;
            FRAME.returning = 1;
            return FRAME.return_val;
        }
        return (e->nchildren > 0) ? interp_eval(e->children[0]) : NULVCL;
    }

    /* Icon/Raku fail-return — distinct from E_FAIL (SNOBOL4 FAIL pattern). */
    case E_PROC_FAIL: {
        if (frame_depth > 0) {
            FRAME.return_val = FAILDESCR;
            FRAME.returning  = 1;
        }
        return FAILDESCR;
    }

    case E_PLS: {
        /* Unary + coerces operand to numeric (int or real) */
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (IS_INT(v) || IS_REAL(v)) return v;
        /* String → try integer, then real */
        const char *s = VARVAL_fn(v);
        if (!s || !*s) return INTVAL(0);
        char *end = NULL;
        long long iv = strtoll(s, &end, 10);
        if (end && *end == '\0') return INTVAL(iv);
        double dv = strtod(s, &end);
        if (end && *end == '\0') return REALVAL(dv);
        return INTVAL(0);
    }

    case E_OPSYN: {
        /* OPSYN operator: sval holds the operator symbol ("@", "&").
         * Dispatch via APPLY_fn — OPSYN registration aliased the symbol
         * to the target function via register_fn_alias. */
        if (!e->sval) return FAILDESCR;
        if (e->nchildren == 2) {
            DESCR_t l = interp_eval(e->children[0]);
            DESCR_t r = interp_eval(e->children[1]);
            if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
            DESCR_t args[2] = { l, r };
            return APPLY_fn(e->sval, args, 2);
        } else if (e->nchildren == 1) {
            DESCR_t v = interp_eval(e->children[0]);
            if (IS_FAIL_fn(v)) return FAILDESCR;
            DESCR_t args[1] = { v };
            return APPLY_fn(e->sval, args, 1);
        }
        return FAILDESCR;
    }

    case E_ADD: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        return add(l, r);
    }
    case E_SUB: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        return sub(l, r);
    }
    case E_MUL: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        return mul(l, r);
    }
    case E_DIV: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        return DIVIDE_fn(l, r);
    }
    case E_MOD: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        long li = IS_INT_fn(l) ? l.i : (long)l.r;
        long ri = IS_INT_fn(r) ? r.i : (long)r.r;
        return ri ? INTVAL(li % ri) : FAILDESCR;
    }
    case E_POW: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        /* SNOBOL4: int**int with non-negative exponent → integer; any real operand → real.
         * Icon (g_lang==1): `^` always produces real, even with integer operands.    */
        if (g_lang != 1 && IS_INT_fn(l) && IS_INT_fn(r) && r.i >= 0) {
            long base = l.i, result = 1; int exp = (int)r.i;
            for (int k = 0; k < exp; k++) result *= base;
            return INTVAL(result);
        }
        double base = IS_REAL_fn(l) ? l.r : (double)l.i;
        double exp  = IS_REAL_fn(r) ? r.r : (double)r.i;
        return (DESCR_t){ .v = DT_R, .r = pow(base, exp) };
    }

    case E_CAT:
    case E_SEQ: {
        if (e->nchildren == 0) return NULVCL;
        /* IC-9 (2026-05-02): In Icon mode, E_SEQ is the & (conjunction) operator —
         * evaluate children left to right; if any fails, return FAILDESCR; return
         * the last child's value.  This is completely different from SNOBOL4's
         * E_SEQ which is string/pattern concatenation.  Gate on g_lang==1 AND
         * e->kind==E_SEQ so the E_CAT fall-through (||) is unaffected. */
        if (g_lang == 1 && e->kind == E_SEQ) {
            DESCR_t last = NULVCL;
            for (int ci = 0; ci < e->nchildren; ci++) {
                last = interp_eval(e->children[ci]);
                if (IS_FAIL_fn(last)) return FAILDESCR;
            }
            return last;
        }
        /* DYN-59: interp_eval is STRING context by default; pattern context
         * uses interp_eval_pat() which calls pat_cat unconditionally.
         * DYN-68: mixed-mode: if the accumulated value is DT_P (pattern),
         * switch to pat_cat so that pattern-building expressions like
         * "icase = icase (upr(c) | lwr(c))" work correctly in value context.
         * SNOBOL4 rule: concatenation of pattern with anything yields pattern.
         * RT-112: once we detect a pattern operand, re-evaluate ALL remaining
         * children via interp_eval_pat so *var/*func become XDSAR/XATP nodes
         * rather than frozen DT_E (which pat_cat cannot handle).
         * SN-26c-parseerr-g: pre-scan children for E_DEFER (i.e. *X).  In
         * value context, interp_eval(E_DEFER) returns DT_E (frozen
         * expression), which is NOT detected by IS_PAT(acc).  If E_DEFER is
         * the FIRST child, the in_pat_mode promotion from the loop below
         * never fires, and CONCAT_fn(DT_E, "B") produces garbage.  Beauty's
         *   pat = *snoFunction $'(' *snoExprList $')'
         * lowered to E_SEQ(E_DEFER, E_VAR, E_DEFER, E_VAR) is the canonical
         * victim.  Fix: if any child is E_DEFER, we are building a pattern;
         * use interp_eval_pat for child[0] and pat_cat for the rest. */
        int has_defer = 0;
        for (int j = 0; j < e->nchildren; j++) {
            EXPR_t *cj = e->children[j];
            if (cj && cj->kind == E_DEFER) { has_defer = 1; break; }
        }
        DESCR_t acc = has_defer ? interp_eval_pat(e->children[0])
                                : interp_eval(e->children[0]);
        if (IS_FAIL_fn(acc)) return FAILDESCR;
        int in_pat_mode = has_defer ? 1 : IS_PAT(acc);
        for (int i = 1; i < e->nchildren; i++) {
            DESCR_t nxt;
            if (in_pat_mode) {
                nxt = interp_eval_pat(e->children[i]);
            } else {
                nxt = interp_eval(e->children[i]);
            }
            if (IS_FAIL_fn(nxt)) return FAILDESCR;
            if (in_pat_mode || IS_PAT(nxt)) {
                if (!in_pat_mode) {
                    /* SN-26-bridge-coverage-u: First pattern seen mid-concat.
                     * Do NOT re-evaluate `nxt` — interp_eval on pattern-shaped
                     * children (E_ALT etc.) already returns DT_P with all inner
                     * function calls fired exactly once.  Re-evaluating would
                     * call those functions a second time (e.g. the canonical
                     * `leader (upr(letter) | lwr(letter))` would call upr/lwr
                     * twice each).  Keep `nxt` as-is and pat_cat will coerce
                     * any non-pattern operands via pat_to_patnd.
                     *
                     * Earlier children (0..i-1) likewise don't need re-eval:
                     * any E_DEFER among them is already caught by the has_defer
                     * pre-scan above (which would have made in_pat_mode=1 from
                     * the start), so all prior `acc` values are valid scalars
                     * that pat_to_patnd will coerce to pat_lit.  pat_cat with
                     * a string LHS and pattern RHS produces an XCAT correctly. */
                    in_pat_mode = 1;
                }
                acc = pat_cat(acc, nxt);
            } else {
                /* SPITBOL rule: if either operand is null string, return the
                 * other operand UNCHANGED (no type coercion). Spec §concat:
                 * "if either operand is the null string, the other operand is
                 *  returned unchanged. It is not coerced into the string type."
                 * IC-9 (2026-05-01): Icon mode is different — `""` is a real
                 * (empty) string, not `&null`. `"" || ""` must yield `""`
                 * (DT_S, slen=0), not `&null` (DT_SNUL). Force string concat
                 * via CONCAT_fn unconditionally in Icon mode, AND post-coerce
                 * a DT_SNUL/null result back to a real empty DT_S so image()
                 * etc. distinguish empty-string from &null. */
                if (g_lang != 1 && acc.v == DT_SNUL)
                    acc = nxt;
                else if (g_lang != 1 && nxt.v == DT_SNUL)
                    { /* acc unchanged */ }
                else
                    acc = CONCAT_fn(acc, nxt);
                if (g_lang == 1 && (acc.v == DT_SNUL || (acc.v == DT_S && !acc.s))) {
                    DESCR_t empty; empty.v = DT_S; empty.s = GC_strdup(""); empty.slen = 0;
                    acc = empty;
                }
            }
            if (IS_FAIL_fn(acc)) return FAILDESCR;
        }
        return acc;
    }

    case E_ASSIGN: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t val = interp_eval(e->children[1]);
        if (IS_FAIL_fn(val)) return FAILDESCR;
        EXPR_t *lv = e->children[0];
        /* INFRA-5b: E_SCAN as lvalue — `if (str ? PAT = repl)` form.
         * The Snocone frontend emits E_ASSIGN(E_SCAN(subj, pat), repl) when a
         * pattern-match-with-replacement is used in expression (if/while head)
         * position.  The old path fell through every lvalue branch silently,
         * so the replacement was never applied and the capture side-effects were
         * lost (exec_stmt inside E_SCAN's own handler was called with repl=NULL).
         * Fix: detect E_SCAN as lv, extract subject name and pattern child,
         * then call exec_stmt with the already-evaluated replacement value. */
        if (lv && lv->kind == E_SCAN && !frame_depth && !g_pl_active) {
            if (lv->nchildren < 1) return FAILDESCR;
            EXPR_t *scan_subj_expr = lv->children[0];
            const char *sname = (scan_subj_expr->kind == E_VAR) ? scan_subj_expr->sval : NULL;
            DESCR_t subj_d = interp_eval(scan_subj_expr);
            if (IS_FAIL_fn(subj_d)) return FAILDESCR;
            DESCR_t pat_d = (lv->nchildren >= 2) ? interp_eval_pat(lv->children[1]) : pat_epsilon();
            if (IS_FAIL_fn(pat_d)) return FAILDESCR;
            /* val is the replacement (already evaluated above as e->children[1]) */
            extern int exec_stmt(const char *, DESCR_t *, DESCR_t, DESCR_t *, int);
            int ok = exec_stmt(sname, sname ? NULL : &subj_d, pat_d, &val, 1);
            return ok ? val : FAILDESCR;
        }
        /* IC-9: string-section / string-index lvalue. */
        if (lv && (lv->kind == E_SECTION || lv->kind == E_SECTION_PLUS ||
                   lv->kind == E_SECTION_MINUS)) {
            if (icn_string_section_assign(lv, val)) return val;
            return FAILDESCR;
        }
        if (lv && lv->kind == E_IDX && lv->nchildren == 2) {
            /* Try string-index first; if base is non-string, fall through to subscript_set. */
            if (icn_string_section_assign(lv, val)) return val;
            DESCR_t _b = interp_eval(lv->children[0]);
            if (_b.v == DT_S || _b.v == DT_SNUL) return FAILDESCR;
        }
        if (lv && lv->kind == E_VAR && lv->sval && lv->sval[0] == '&' &&
                scan_depth > 0 && !frame_depth) {
            /* IC-9: Icon scan-state keyword write in scan body outside any Icon proc frame. */
            if (!kw_assign(lv->sval + 1, val)) return FAILDESCR;
        } else if (lv && lv->kind == E_VAR && lv->sval)
            NV_SET_fn(lv->sval, val);  /* inner expr assign: no trace (stmt-level already traced) */
        else if (lv && lv->kind == E_IDX && lv->nchildren >= 2) {
            /* arr<i> = val  or  arr<i,j> = val */
            DESCR_t base = interp_eval(lv->children[0]);
            if (!IS_FAIL_fn(base)) {
                DESCR_t idx = interp_eval(lv->children[1]);
                if (!IS_FAIL_fn(idx)) {
                    if (lv->nchildren >= 3) {
                        DESCR_t idx2 = interp_eval(lv->children[2]);
                        if (!IS_FAIL_fn(idx2))
                            subscript_set2(base, idx, idx2, val);
                    } else {
                        subscript_set(base, idx, val);
                    }
                }
            }
        }
        else if (lv && lv->kind == E_FNC && lv->sval && lv->nchildren >= 1) {
            if (strcmp(lv->sval, "ITEM") == 0 && lv->nchildren >= 2) {  /* SN-19 */
                /* ITEM(arr, i [,j]) = val — programmatic subscript setter */
                DESCR_t base = interp_eval(lv->children[0]);
                if (!IS_FAIL_fn(base)) {
                    DESCR_t idx = interp_eval(lv->children[1]);
                    if (!IS_FAIL_fn(idx)) {
                        if (lv->nchildren >= 3) {
                            DESCR_t idx2 = interp_eval(lv->children[2]);
                            if (!IS_FAIL_fn(idx2))
                                subscript_set2(base, idx, idx2, val);
                        } else {
                            subscript_set(base, idx, val);
                        }
                    }
                }
            } else {
                /* DATA field setter: fname(obj) = val
                 * Evaluate the first argument; if it's a DT_DATA instance,
                 * dispatch through FIELD_SET_fn using the function name as field name. */
                DESCR_t obj = interp_eval(lv->children[0]);
                if (!IS_FAIL_fn(obj))
                    FIELD_SET_fn(obj, lv->sval, val);
            }
        }
        else if (lv && lv->kind == E_INDIRECT && lv->nchildren > 0) {
            EXPR_t *ichild = lv->children[0];
            const char *nm = NULL;
            if (ichild->kind == E_CAPT_COND_ASGN && ichild->nchildren == 1
                    && ichild->children[0]->kind == E_VAR && ichild->children[0]->sval)
                nm = ichild->children[0]->sval;
            else { DESCR_t nd = interp_eval(ichild); nm = VARVAL_fn(nd); }
            if (nm && *nm) {
                char *fn = GC_strdup(nm); sno_fold_name(fn);  /* SN-19 */
                NV_SET_fn(fn, val);  /* inner expr: no trace */
            }
        }
        return val;
    }

    case E_INDIRECT: {
        if (e->nchildren < 1) return FAILDESCR;
        EXPR_t *child = e->children[0];
        /* $.var parses as E_INDIRECT(E_NAME(E_CAPT_COND_ASGN(E_VAR)))
         * $.var<idx> parses as E_INDIRECT(E_NAME(E_CAPT_COND_ASGN(E_VAR, idx)))
         * The E_NAME wrapper (from the dot-prefix parse) is unwrapped first.
         * Semantics: the identifier name is used literally (not its value).
         *   $.var      => NV_GET_fn("var")
         *   $.var<idx> => subscript( NV_GET_fn("var"), idx ) */
        /* $.var<idx> parses as E_INDIRECT(E_NAME(E_CAPT_COND_ASGN(E_VAR,idx)))
         * — the E_NAME wrapper is from the dot-prefix parse. Unwrap it.
         * $X (no dot) parses as E_INDIRECT(E_VAR("X")) — no E_NAME wrapper.
         * Track whether we unwrapped an E_NAME to distinguish:
         *   $.var = literal name lookup (return var's value directly)
         *   $X    = runtime indirect (evaluate X, use its value as lookup name) */
        int had_name_wrap = 0;
        if (child->kind == E_NAME && child->nchildren == 1) {
            child = child->children[0];
            had_name_wrap = 1;
        }

        /* E_VAR child: $.var (had_name_wrap=1) vs $X (had_name_wrap=0) */
        if (child->kind == E_VAR && child->sval) {
            if (had_name_wrap)
                return NV_GET_fn(child->sval);          /* $.var — literal name */
            /* $X — evaluate X's runtime value, use it as the variable name.
             * IS_NAMEPTR (slen=1, .ptr = live DESCR_t*) vs IS_NAMEVAL (slen=0, .s = name).
             * Do NOT use ptr!=NULL — for NAMEVAL .s and .ptr alias the same union. */
            DESCR_t _xv = NV_GET_fn(child->sval);
            if (IS_NAMEPTR(_xv)) return NAME_DEREF_PTR(_xv);
            if (IS_NAMEVAL(_xv)) return NV_GET_fn(_xv.s);
            const char *_xnm0 = VARVAL_fn(_xv);
            if (!_xnm0 || !*_xnm0) return NULVCL;
            char *_xnm = GC_strdup(_xnm0); sno_fold_name(_xnm);  /* SN-19 */
            DESCR_t _xnamed = NV_GET_fn(_xnm);
            if (IS_NAMEPTR(_xnamed)) return NAME_DEREF_PTR(_xnamed);
            if (IS_NAMEVAL(_xnamed)) return NV_GET_fn(_xnamed.s);
            return _xnamed;
        }

        /* E_IDX after E_NAME unwrap: $.var<idx> subscript form
         * children[0]=E_VAR "name", children[1]=index expr */
        if (child->kind == E_IDX && child->nchildren >= 2
                && child->children[0]->kind == E_VAR && child->children[0]->sval) {
            const char *nm = child->children[0]->sval;
            DESCR_t base = NV_GET_fn(nm);
            if (IS_FAIL_fn(base)) return FAILDESCR;
            if (child->nchildren == 2) {
                DESCR_t idx = interp_eval(child->children[1]);
                if (IS_FAIL_fn(idx)) return FAILDESCR;
                return subscript_get(base, idx);
            }
            DESCR_t i1 = interp_eval(child->children[1]);
            DESCR_t i2 = interp_eval(child->children[2]);
            if (IS_FAIL_fn(i1) || IS_FAIL_fn(i2)) return FAILDESCR;
            return subscript_get2(base, i1, i2);
        }

        if (child->kind == E_CAPT_COND_ASGN && child->nchildren == 1) {
            EXPR_t *inner = child->children[0];
            /* $.var<idx> case: dot child is E_IDX whose base is E_VAR */
            if (inner->kind == E_IDX && inner->nchildren >= 2
                    && inner->children[0]->kind == E_VAR
                    && inner->children[0]->sval) {
                const char *nm = inner->children[0]->sval;
                DESCR_t base = NV_GET_fn(nm);
                if (IS_FAIL_fn(base)) return FAILDESCR;
                if (inner->nchildren == 2) {
                    DESCR_t idx = interp_eval(inner->children[1]);
                    if (IS_FAIL_fn(idx)) return FAILDESCR;
                    return subscript_get(base, idx);
                }
                DESCR_t i1 = interp_eval(inner->children[1]);
                DESCR_t i2 = interp_eval(inner->children[2]);
                if (IS_FAIL_fn(i1) || IS_FAIL_fn(i2)) return FAILDESCR;
                return subscript_get2(base, i1, i2);
            }
            /* $.var case: dot child is plain E_VAR — but only if this is a
             * literal $.var (direct name lookup). For $X (runtime indirect),
             * evaluate X's value and use THAT as the variable name.
             * Distinction: $.var uses the identifier literally; $X uses X's value.
             * Since parser wraps both as E_CAPT_COND_ASGN(E_VAR), we must
             * evaluate the inner var and use its string value as the lookup key. */
            if (inner->kind == E_VAR && inner->sval) {
                DESCR_t xval = NV_GET_fn(inner->sval);
                if (IS_NAMEPTR(xval)) return NAME_DEREF_PTR(xval);
                if (IS_NAMEVAL(xval)) return NV_GET_fn(xval.s);
                const char *nm2_0 = VARVAL_fn(xval);
                if (!nm2_0 || !*nm2_0) return NULVCL;
                char *nm2 = GC_strdup(nm2_0); sno_fold_name(nm2);  /* SN-19 */
                DESCR_t named = NV_GET_fn(nm2);
                if (IS_NAMEPTR(named)) return NAME_DEREF_PTR(named);
                if (IS_NAMEVAL(named)) return NV_GET_fn(named.s);
                return named;
            }
            /* fallback: evaluate inner directly */
            DESCR_t nd = interp_eval(inner);
            const char *nm2_0 = VARVAL_fn(nd);
            if (!nm2_0 || !*nm2_0) return NULVCL;
            char *nm2 = GC_strdup(nm2_0); sno_fold_name(nm2);  /* SN-19 */
            DESCR_t named2 = NV_GET_fn(nm2);
            if (IS_NAMEPTR(named2)) return NAME_DEREF_PTR(named2);
            if (IS_NAMEVAL(named2)) return NV_GET_fn(named2.s);
            return named2;
        }
        /* $expr — indirect through runtime string/name value */
        DESCR_t nd = interp_eval(child);
        if (IS_NAMEPTR(nd)) return NAME_DEREF_PTR(nd);
        if (IS_NAMEVAL(nd)) return NV_GET_fn(nd.s);
        const char *nm0 = VARVAL_fn(nd);
        if (!nm0 || !*nm0) return NULVCL;
        char *nm = GC_strdup(nm0); sno_fold_name(nm);  /* SN-19 */
        /* The named variable might also be a DT_N — dereference one more level */
        DESCR_t named = NV_GET_fn(nm);
        if (IS_NAMEPTR(named)) return NAME_DEREF_PTR(named);
        if (IS_NAMEVAL(named)) return NV_GET_fn(named.s);
        return named;
    }

    case E_FNC: {
        if (!e->sval || !*e->sval) return FAILDESCR;

        /* SB-6.X: ARBNO(P)/FENCE(P) take a *pattern* arg.  When evaluated in
         * value context (e.g. RHS of  Pat = ARBNO(*Cmd) ), the default
         * arg-eval path below uses interp_eval (value context) on each child,
         * which turns E_DEFER(E_VAR) into DT_E (frozen value-form expr).
         * APPLY_fn(ARBNO, DT_E, ...) then materialises the DT_E by thawing
         * Cmd's *current* value as a literal — losing deferred-ref semantics
         * (XDSAR), so subsequent re-binds of Cmd are not seen and the
         * pattern even fails to match its own initial value.
         *
         * Mirror the pat-context fix at interp_eval_pat E_FNC (above): when
         * the function name is ARBNO or FENCE, evaluate the first child in
         * pattern context so E_DEFER(E_VAR) becomes a proper XDSAR ref node,
         * then call pat_arbno/pat_fence_p directly.  All other E_FNC names
         * fall through to the value-context arg eval as before. */
        if (e->nchildren > 0) {
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

        /* DEFINE('spec'[,'entry']) — register user function.
         * SIL DEFIFN returns the function name string on success (DIFFER-able).
         * Extract name = everything before '(' or ',' in spec. */
        if (strcmp(e->sval, "DEFINE") == 0) {  /* SN-19: AST token, canonical */
            const char *spec = define_spec_from_expr(e);
            if (spec && *spec) {
                const char *entry = define_entry_from_expr(e);
                if (entry) DEFINE_fn_entry(spec, NULL, entry);
                else       DEFINE_fn(spec, NULL);
                /* SNOBOL4 spec: DEFINE returns null string on success */
                return NULVCL;
            }
            return FAILDESCR;   /* malformed spec → FAIL per SIL */
        }

        int nargs = e->nchildren;
        DESCR_t *args = nargs > 0
            ? (DESCR_t *)alloca((size_t)nargs * sizeof(DESCR_t))
            : NULL;
        for (int i = 0; i < nargs; i++) {
            args[i] = interp_eval(e->children[i]);
            if (IS_FAIL_fn(args[i])) return FAILDESCR;
        }

        /* DYN-70 fix: check for user-defined body label BEFORE calling APPLY_fn.
         * APPLY_fn internally dispatches user functions via call_user_function,
         * so calling APPLY_fn then call_user_function again causes double execution.
         * Rule: if a body label exists in the program → user-defined → skip APPLY_fn.
         * Builtins never have a body label; user functions always do (prescan_defines). */
        {
            /* Resolve body: try as-is, uppercase, then entry_label (OPSYN aliases) */
            STMT_t *body = label_lookup(e->sval);
            if (!body) {
                char ufn[128];
                size_t fl = strlen(e->sval);
                if (fl >= sizeof(ufn)) fl = sizeof(ufn)-1;
                for (size_t i = 0; i <= fl; i++) ufn[i] = (char)toupper((unsigned char)e->sval[i]);
                body = label_lookup(ufn);
            }
            if (!body) {
                const char *el = FUNC_ENTRY_fn(e->sval);
                if (el) body = label_lookup(el);
            }
            if (body) {
                /* User-defined function — call interpreter directly, never via APPLY_fn */
                DESCR_t r = call_user_function(e->sval, args, nargs);
                if (IS_NAME(r)) return NAME_DEREF(r);
                return r;
            }
        }
        /* ── U-22: cross-language fallback in value-context E_FNC ────────────
         * SNO body lookup above found nothing.  Try Icon proc table, then
         * Prolog pred table, before falling through to builtins/APPLY_fn. */
        {
            /* Try Icon proc table (case-sensitive) */
            for (int _ci = 0; _ci < proc_count; _ci++) {
                if (strcmp(proc_table[_ci].name, e->sval) == 0)
                    return coro_call(proc_table[_ci].proc, args, nargs);
            }
            /* Try Prolog pred table: "name/arity" key */
            if (g_pl_active) {
                char _pk[256];
                snprintf(_pk, sizeof _pk, "%s/%d", e->sval, nargs);
                EXPR_t *_choice = pl_pred_table_lookup(&g_pl_pred_table, _pk);
                if (_choice) {
                    Term **_pl_args = (nargs > 0) ? pl_env_new(nargs) : NULL;
                    Term **_saved   = g_pl_env;
                    g_pl_env = _pl_args;
                    bb_node_t _root = pl_box_choice(_choice, g_pl_env, nargs);
                    int _ok = bb_broker(_root, BB_ONCE, NULL, NULL);
                    g_pl_env = _saved;
                    return _ok ? INTVAL(1) : FAILDESCR;
                }
            }
        }
        /* IDENT/DIFFER: per SPITBOL spec, arguments must have SAME data type AND value.
         * IDENT(3, '3') FAILS (integer vs string). IDENT(S) succeeds iff S is null string.
         * DIFFER(S,T) succeeds iff they differ in type OR value. DIFFER(S) succeeds iff S != ''.
         * Both return NULVCL on success. */
        /* EVAL/CODE: binary _EVAL_/_CODE_ are stubs; route through our full impl. */
        if (strcmp(e->sval, "EVAL") == 0) {  /* SN-19 */
            if (nargs < 1) return FAILDESCR;
            extern DESCR_t EVAL_fn(DESCR_t);
            DESCR_t _er = EVAL_fn(args[0]);
            return _er;
        }
        if (strcmp(e->sval, "CODE") == 0) {  /* SN-19 */
            if (nargs < 1) return FAILDESCR;
            const char *_cs = VARVAL_fn(args[0]);
            extern DESCR_t code(const char *);
            return (_cs && *_cs) ? code(_cs) : FAILDESCR;
        }
        if (strcmp(e->sval, "IDENT") == 0) {  /* SN-19 */
            if (nargs == 1) {
                /* IDENT(S) — succeed if S is null string */
                return IS_NULL_fn(args[0]) ? NULVCL : FAILDESCR;
            }
            if (nargs >= 2) {
                /* Normalize: treat DT_SNUL and DT_S("") as same null type for comparison */
                int a_null = IS_NULL_fn(args[0]), b_null = IS_NULL_fn(args[1]);
                if (a_null && b_null) return NULVCL;   /* both null → identical */
                if (a_null || b_null) return FAILDESCR; /* one null, one not → differ */
                /* Same non-null type AND same string value */
                if (args[0].v != args[1].v) return FAILDESCR;
                const char *sa = VARVAL_fn(args[0]);
                const char *sb = VARVAL_fn(args[1]);
                if (!sa) sa = ""; if (!sb) sb = "";
                return strcmp(sa, sb) == 0 ? NULVCL : FAILDESCR;
            }
        }
        if (strcmp(e->sval, "DIFFER") == 0) {  /* SN-19 */
            if (nargs == 1) {
                return IS_NULL_fn(args[0]) ? FAILDESCR : NULVCL;
            }
            if (nargs >= 2) {
                int a_null = IS_NULL_fn(args[0]), b_null = IS_NULL_fn(args[1]);
                if (a_null && b_null) return FAILDESCR;  /* both null → identical → DIFFER fails */
                if (a_null || b_null) return NULVCL;     /* one null, one not → differ */
                if (args[0].v != args[1].v) return NULVCL;
                const char *sa = VARVAL_fn(args[0]);
                const char *sb = VARVAL_fn(args[1]);
                if (!sa) sa = ""; if (!sb) sb = "";
                return strcmp(sa, sb) != 0 ? NULVCL : FAILDESCR;
            }
        }

        /* SC-1: DATA constructor/field-accessor dispatch via our registry */
        {
            ScDatType *_dt = sc_dat_find_type(e->sval);
            if (_dt) return sc_dat_construct(_dt, args, nargs);
            int _fi = 0;
            ScDatType *_ft = sc_dat_find_field(e->sval, &_fi);
            if (_ft && nargs >= 1) return sc_dat_field_get(e->sval, args[0]);
        }
        /* No body label → builtin or unknown. APPLY_fn handles both. */
        if (FNCEX_fn(e->sval)) {
            DESCR_t bres = APPLY_fn(e->sval, args, nargs);
            return bres;
        }
        return APPLY_fn(e->sval, args, nargs);
    }

    case E_IDX: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t base = interp_eval(e->children[0]);
        if (IS_FAIL_fn(base)) return FAILDESCR;
        if (e->nchildren == 2) {
            DESCR_t idx = interp_eval(e->children[1]);
            if (IS_FAIL_fn(idx)) return FAILDESCR;
            return subscript_get(base, idx);
        }
        DESCR_t i1 = interp_eval(e->children[1]);
        DESCR_t i2 = interp_eval(e->children[2]);
        if (IS_FAIL_fn(i1) || IS_FAIL_fn(i2)) return FAILDESCR;
        return subscript_get2(base, i1, i2);
    }

    case E_DEFER: {
        /* *expr — SIL *X operator. Three sub-cases:
         *
         * *var  (E_VAR): deferred variable reference — fetch value NOW.
         *   "term = *factor" stores factor's current pattern.
         *
         * *func(args) (E_FNC): always builds a deferred T_FUNC/XATP pattern
         *   node via pat_user_call — fires the function at match time.
         *   This applies in ALL contexts (RHS assignment, pattern expr, etc.).
         *   "addop = ANY('+-') . *Push()" — *Push() must be a pattern node.
         *
         * *complex_expr: freeze as DT_E for EVAL() to thaw later. */
        if (e->nchildren < 1) return NULVCL;
        EXPR_t *child = e->children[0];
        /* RUNTIME-6: *X in value context ALWAYS produces DT_E (EXPRESSION).
         * The child EXPR_t* is frozen; EVAL() thaws and executes it.
         * interp_eval_pat handles the pattern-context path (*var, *func). */
        DESCR_t d;
        d.v    = DT_E;
        d.slen = 0;
        d.s    = NULL;    /* clear union first... */
        d.ptr  = child;   /* ...then store ptr last (ptr and s share union) */
        return d;
    }

    case E_NOT: {
        /* \X — o$nta/b/c: succeed (null) iff X fails; fail if X succeeds.
         * Expression-context version; pattern-context uses bb_not. */
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return NULVCL;
        return FAILDESCR;
    }

    case E_SIZE: {
        /* *E — size of string, list, or table.
         * String: number of characters.  List/table (SOH-delimited): element count.
         * DT_T: native table → tbl->size. */
        if (e->nchildren < 1) return INTVAL(0);
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (v.v == DT_T) return INTVAL(v.tbl ? v.tbl->size : 0);
        /* IC-5: DT_DATA icnlist */
        if (v.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(v,"icn_type");
            if (tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)
                return INTVAL((int)FIELD_GET_fn(v,"frame_size").i);
        }
        if (IS_INT_fn(v)) return INTVAL(0);   /* integer has no size */
        if (IS_REAL_fn(v)) return INTVAL(0);
        /* String: count chars, or SOH-delimited elements for arrays */
        const char *s = VARVAL_fn(v);
        if (!s) return INTVAL(0);
        /* If string contains SOH (\x01) it is a Raku/Icon array — count elements */
        if (strchr(s, '\x01')) {
            long n = 1;
            for (const char *p = s; *p; p++) if (*p == '\x01') n++;
            return INTVAL(n);
        }
        long len = v.slen > 0 ? v.slen : (long)strlen(s);
        return INTVAL(len);
    }

    case E_ALT: {
        /* child[0] | child[1] | ... — build pat_alt chain left-to-right.
         * Pattern alternation is inherently a pattern operation, so every
         * child should be evaluated in pattern context so that *var/*func
         * children become XDSAR/XATP nodes rather than frozen DT_E (which
         * silently coerces to pat_lit of the NAME string — wrong and
         * confusing). */
        if (e->nchildren == 0) return NULVCL;
        DESCR_t acc = interp_eval_pat(e->children[0]);
        for (int i = 1; i < e->nchildren; i++)
            acc = pat_alt(acc, interp_eval_pat(e->children[i]));
        return acc;
    }
    case E_VLIST: {
        /* Goal-directed value-context disjunction.  Try children
         * left-to-right; return first non-failing value; fail if all fail.
         * SPITBOL `(a, b, c)` paren-list and Snocone `||`.
         * Distinct from E_ALT (pattern alt is lazy/backtracking).
         * Side effects of arm k happen iff arms 0..k-1 all failed. */
        if (e->nchildren == 0) return FAILDESCR;
        for (int i = 0; i < e->nchildren; i++) {
            DESCR_t v = interp_eval(e->children[i]);
            if (!IS_FAIL_fn(v)) return v;
        }
        return FAILDESCR;
    }
    case E_CAPT_COND_ASGN: {
        /* pat . target — conditional assignment on match success.
         * target may be:
         *   E_VAR "name"         → XNME node, assign to named var at flush
         *   E_DEFER(E_FNC(...))  → XCALLCAP node: call func at match time to
         *                          get DT_N lvalue, write matched text at flush.
         *                          Function must NOT be called at build time. */
        if (e->nchildren < 2) return NULVCL;
        DESCR_t pat = interp_eval_pat(e->children[0]);
        EXPR_t *tgt = e->children[1];
        if (tgt->kind == E_DEFER && tgt->nchildren == 1
                && tgt->children[0]->kind == E_FNC && tgt->children[0]->sval) {
            /* Deferred-function target — build XCALLCAP, don't call now */
            EXPR_t *fnc = tgt->children[0];
            int na = fnc->nchildren;
            /* TL-2: when every arg is a plain E_VAR, store *names* and defer
             * lookup to flush time (NAME_commit).  This matches oracle semantics
             * where args are resolved AFTER earlier . captures in the same
             * pattern have written their variables. */
            int all_vars = (na > 0);
            for (int i = 0; i < na; i++) {
                EXPR_t *c = fnc->children[i];
                if (!c || c->kind != E_VAR || !c->sval) { all_vars = 0; break; }
            }
            if (all_vars) {
                char **names = (char **)GC_malloc(na * sizeof(char *));
                for (int i = 0; i < na; i++) names[i] = (char *)fnc->children[i]->sval;
                return pat_assign_callcap_named(pat, fnc->sval, NULL, 0, names, na);
            }
            /* SN-26c-parseerr-c: defer E_FNC sub-args via DT_E wrapping;
             * thaw at match time in name_commit_value(NM_CALL).
             * SN-26c-parseerr-d: also defer E_VAR — when args are mixed
             * (e.g. E_QLIT + E_VAR) the all_vars fast path above doesn't
             * fire, and an E_VAR set by an earlier capture in the same
             * pattern would otherwise be eagerly read here at build time.
             * SN-26-bridge-coverage-t: also defer compound expressions —
             * e.g. `*Reduce('[]', nTop() + 1)` — where the arg's top kind
             * is E_ADD/E_SUB/etc but contains an E_FNC inside.  Eager
             * interp_eval at build time would call nTop() before the
             * pattern matches, violating SPITBOL's deferred-pattern
             * semantics.  Wrap all args as DT_E; thaw at match time
             * via EVAL_fn → EXPVAL_fn (name_t.c:97). */
            DESCR_t *av = na > 0 ? GC_malloc(na * sizeof(DESCR_t)) : NULL;
            for (int i = 0; i < na; i++) {
                EXPR_t *arg = fnc->children[i];
                if (arg && arg->kind == E_QLIT) {
                    /* Pure string literal — safe to evaluate eagerly,
                     * idempotent under EVAL.  Avoids needless DT_E wrap. */
                    av[i] = interp_eval(arg);
                } else if (arg) {
                    /* Any other expression kind — defer.  EXPVAL_fn at
                     * thaw time handles all EXPR_t shapes. */
                    av[i].v = DT_E;
                    av[i].ptr = arg;
                    av[i].slen = 0;
                } else {
                    av[i] = NULVCL;
                }
            }
            return pat_assign_callcap(pat, fnc->sval, av, na);
        }
        /* Snocone *fn() lowers as E_INDIRECT(E_FNC(...)) — same semantics as
         * E_DEFER(E_FNC(...)): call fn at flush time to get lvalue, assign then.
         * SC-26: route this through pat_assign_callcap so it joins the unified
         * NAM list and fires in left-to-right order after preceding captures. */
        if (tgt->kind == E_INDIRECT && tgt->nchildren == 1
                && tgt->children[0]->kind == E_FNC && tgt->children[0]->sval) {
            EXPR_t *fnc = tgt->children[0];
            int na = fnc->nchildren;
            int all_vars = (na > 0);
            for (int i = 0; i < na; i++) {
                EXPR_t *c = fnc->children[i];
                if (!c || c->kind != E_VAR || !c->sval) { all_vars = 0; break; }
            }
            if (all_vars) {
                char **names = (char **)GC_malloc(na * sizeof(char *));
                for (int i = 0; i < na; i++) names[i] = (char *)fnc->children[i]->sval;
                return pat_assign_callcap_named(pat, fnc->sval, NULL, 0, names, na);
            }
            /* SN-26c-parseerr-c: defer E_FNC sub-args.
             * SN-26c-parseerr-d: also defer E_VAR (see twin site above).
             * SN-26-bridge-coverage-t: defer compound expressions too —
             * see twin site above for full rationale. */
            DESCR_t *av = na > 0 ? GC_malloc(na * sizeof(DESCR_t)) : NULL;
            for (int i = 0; i < na; i++) {
                EXPR_t *arg = fnc->children[i];
                if (arg && arg->kind == E_QLIT) {
                    av[i] = interp_eval(arg);
                } else if (arg) {
                    av[i].v = DT_E;
                    av[i].ptr = arg;
                    av[i].slen = 0;
                } else {
                    av[i] = NULVCL;
                }
            }
            return pat_assign_callcap(pat, fnc->sval, av, na);
        }
        const char *nm = tgt->sval;
        if (!nm && tgt->kind == E_INDIRECT && tgt->nchildren > 0) {
            /* REM . $'$B' — target is E_INDIRECT(E_QLIT "$B").
             * We need the *variable name* ("$B"), not the value of $'$B'.
             * The name is the evaluated string of the child expression. */
            EXPR_t *ichild = tgt->children[0];
            if (ichild->kind == E_QLIT || ichild->kind == E_VAR)
                nm = ichild->sval;                     /* literal name: $'$B' or $.X */
            else {
                DESCR_t nd = interp_eval(ichild);      /* runtime indirect: $X */
                nm = VARVAL_fn(nd);
            }
        }
        return nm ? pat_assign_cond(pat, STRVAL((char *)nm)) : pat;
    }
    case E_CAPT_IMMED_ASGN: {
        /* pat $ target — immediate assignment during match */
        if (e->nchildren < 2) return NULVCL;
        DESCR_t pat = interp_eval_pat(e->children[0]);
        EXPR_t *tgt = e->children[1];
        if (tgt->kind == E_DEFER && tgt->nchildren == 1
                && tgt->children[0]->kind == E_FNC && tgt->children[0]->sval) {
            /* SN-26c-parseerr-f: "pat $ *fn(args)" — deferred-call immediate capture.
             * Mirror E_CAPT_COND_ASGN logic: build XCALLCAP with imm=1 so the
             * function is called at match time (not build time) with current arg
             * values.  This is critical when args are set by prior $ captures in
             * the same pattern (e.g. beauty's "SPAN $ tx $ *match(list, pattern)").
             * Calling fn eagerly at build time reads empty vars → fn returns fail
             * → match guard silently vanishes. */
            EXPR_t *fnc = tgt->children[0];
            int na = fnc->nchildren;
            int all_vars = (na > 0);
            for (int i = 0; i < na; i++) {
                EXPR_t *c = fnc->children[i];
                if (!c || c->kind != E_VAR || !c->sval) { all_vars = 0; break; }
            }
            if (all_vars) {
                char **names = (char **)GC_malloc(na * sizeof(char *));
                for (int i = 0; i < na; i++) names[i] = (char *)fnc->children[i]->sval;
                return pat_assign_callcap_named_imm(pat, fnc->sval, NULL, 0, names, na);
            }
            /* Mixed args: defer E_VAR and E_FNC; evaluate literals eagerly. */
            DESCR_t *av = na > 0 ? GC_malloc(na * sizeof(DESCR_t)) : NULL;
            for (int i = 0; i < na; i++) {
                EXPR_t *arg = fnc->children[i];
                if (arg && (arg->kind == E_FNC || arg->kind == E_VAR)) {
                    av[i].v = DT_E;
                    av[i].ptr = arg;
                    av[i].slen = 0;
                } else {
                    av[i] = interp_eval(arg);
                }
            }
            return pat_assign_callcap_named_imm(pat, fnc->sval, av, na, NULL, 0);
        }
        EXPR_t *tgt2 = e->children[1];
        const char *nm = tgt2->sval;
        if (!nm && tgt2->kind == E_INDIRECT && tgt2->nchildren > 0) {
            EXPR_t *ichild = tgt2->children[0];
            if (ichild->kind == E_QLIT || ichild->kind == E_VAR)
                nm = ichild->sval;
            else { DESCR_t nd = interp_eval(ichild); nm = VARVAL_fn(nd); }
        }
        return nm ? pat_assign_imm(pat, STRVAL((char *)nm)) : pat;
    }
    case E_CAPT_CURSOR: {
        /* Two forms:
         *   unary:  @var         — E_CAPT_CURSOR(E_VAR)         nchildren==1
         *   binary: pat @ var    — E_CAPT_CURSOR(pat, E_VAR)    nchildren==2
         * Both write the cursor position into var as DT_I at match time. */
        if (e->nchildren == 1) {
            /* unary @var: epsilon left, cursor capture into var */
            const char *nm = e->children[0]->sval;
            if (!nm) return NULVCL;
            return pat_at_cursor(nm);
        }
        if (e->nchildren < 2) return NULVCL;
        DESCR_t left_pat = interp_eval_pat(e->children[0]);
        const char *nm   = e->children[1]->sval;
        if (!nm) return left_pat;
        DESCR_t atp = pat_at_cursor(nm);
        return pat_cat(left_pat, atp);
    }

    /* ── Numeric relational operators ─────────────────────────────────────
     * Each compares two numeric operands; succeeds (returns rhs) or fails.
     * Icon relops return the RIGHT operand on success — this lets them act
     * as filters in generator chains: every write(2 < (1 to 4)) → 3, 4.
     * (SNOBOL4 uses a separate path; these cases are Icon-only.) */
#define NUMREL(op) do { \
        if (e->nchildren < 2) return FAILDESCR; \
        DESCR_t l = interp_eval(e->children[0]); \
        DESCR_t r = interp_eval(e->children[1]); \
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR; \
        double lv = (l.v==DT_R) ? l.r : (double)(l.v==DT_I ? l.i : 0); \
        double rv = (r.v==DT_R) ? r.r : (double)(r.v==DT_I ? r.i : 0); \
        if (!(lv op rv)) return FAILDESCR; \
        return r; \
    } while(0)
    case E_LT: NUMREL(<);
    case E_LE: NUMREL(<=);
    case E_GT: NUMREL(>);
    case E_GE: NUMREL(>=);
    case E_EQ: NUMREL(==);
    case E_NE: NUMREL(!=);
#undef NUMREL

    /* ── Lexicographic (string) relational operators ───────────────────────
     * Each compares two string operands; succeeds (returns rhs) or fails.
     * Icon string relops return the RIGHT operand on success — oracle:
     * ocomp.r StrComp macro: "Return y as the result of the comparison." */
#define STRREL(cmpop) do { \
        if (e->nchildren < 2) return FAILDESCR; \
        DESCR_t l = interp_eval(e->children[0]); \
        DESCR_t r = interp_eval(e->children[1]); \
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR; \
        const char *ls = VARVAL_fn(l); if (!ls) ls = ""; \
        const char *rs = VARVAL_fn(r); if (!rs) rs = ""; \
        int cmp = strcmp(ls, rs); \
        if (!(cmp cmpop 0)) return FAILDESCR; \
        return r; \
    } while(0)
    case E_LLT: STRREL(<);
    case E_LLE: STRREL(<=);
    case E_LGT: STRREL(>);
    case E_LGE: STRREL(>=);
    case E_LEQ: STRREL(==);
    case E_LNE: STRREL(!=);
#undef STRREL

    /* ── IC-8: Icon `===` deep-identity (non-generator path) ─────────────────
     * The icon-frame E_IF at L2607 routes to coro_eval when is_suspendable(test)
     * is true (e.g. `if x === key(T)` where key(T) is a generator). The path
     * here handles plain `a === b` and `&null === x` evaluation in any context.
     * Returns rhs on identity (Icon goal-directed convention), FAILDESCR else. */
    case E_IDENTICAL: {
        if (e->nchildren < 2) return FAILDESCR;
        DESCR_t l = interp_eval(e->children[0]);
        DESCR_t r = interp_eval(e->children[1]);
        if (IS_FAIL_fn(l) || IS_FAIL_fn(r)) return FAILDESCR;
        return icn_descr_identical(l, r) ? r : FAILDESCR;
    }

    /* ── Prolog IR nodes — S-1C-2/3 ──────────────────────────────────────────
     * Only reached when g_pl_active is set (Prolog program running).
     * E_CHOICE drives clause selection via the Byrd box broker (pl_broker.c).
     * E_UNIFY/E_CUT/E_TRAIL_* are leaf goal nodes evaluated inline. */
    case E_UNIFY: {
        if (!g_pl_active) return NULVCL;
        Term *t1 = pl_unified_term_from_expr(e->children[0], g_pl_env);
        Term *t2 = pl_unified_term_from_expr(e->children[1], g_pl_env);
        int mark = trail_mark(&g_pl_trail);
        if (!unify(t1, t2, &g_pl_trail)) { trail_unwind(&g_pl_trail, mark); return FAILDESCR; }
        return INTVAL(1);
    }
    case E_CUT:
        if (g_pl_active) g_pl_cut_flag = 1;
        return INTVAL(1);
    case E_TRAIL_MARK:
    case E_TRAIL_UNWIND:
        return NULVCL;
    case E_CLAUSE:
        /* Clauses are iterated by E_CHOICE — never dispatched standalone. */
        return NULVCL;
    case E_CHOICE: {
        /* Drive clause selection via the Byrd box broker.
         * pl_box_choice builds the full OR/CAT/head-unify box tree.
         * bb_broker drives α (BB_ONCE) — OR-box retries β internally per clause.
         * g_pl_env holds the caller's arg Term** array (arity slots).
         * Returns INTVAL(1) on first solution (γ), FAILDESCR on ω. */
        if (!g_pl_active) return NULVCL;
        int arity = 0;
        if (e->sval) { const char *sl = strrchr(e->sval, '/'); if (sl) arity = atoi(sl+1); }
        bb_node_t root = pl_box_choice(e, g_pl_env, arity);
        int ok = bb_broker(root, BB_ONCE, NULL, NULL);
        return ok ? INTVAL(1) : FAILDESCR;
    }

    /* ── Icon EKinds — OE-3 ─── */

    case E_CSET: return e->sval ? STRVAL(e->sval) : NULVCL;

    case E_TO: case E_TO_BY: {
        long cur;
        if (icn_frame_lookup(e, &cur)) return INTVAL(cur);
        if (e->nchildren < 1) return NULVCL;
        return interp_eval(e->children[0]);
    }

    case E_EVERY: {
        if (e->nchildren < 1) return NULVCL;
        EXPR_t *gen  = e->children[0];
        EXPR_t *body = (e->nchildren > 1) ? e->children[1] : NULL;
        /* IC-2a: coro_eval + BB_PUMP — all goal-directed ops through Byrd boxes.
         * Special case: if gen is E_ASSIGN or E_AUGOP with a generative RHS,
         * drive the LEAF generator inside the RHS and re-evaluate gen each tick so
         * that mutable frame locals (e.g. `total`) are read fresh.  e.g.:
         *   every total := total + (1 to n)   -- E_ASSIGN(E_VAR(total), E_ADD(E_VAR(total), E_TO(1,n)))
         *   every total +:= (1 to n)           -- E_AUGOP with E_TO rhs
         * We find the leaf generator node (e.g. E_TO), drive only that via coro_eval,
         * and inject each raw tick value via coro_drive_node passthrough.  interp_eval(gen)
         * then re-reads the current value of `total` from the frame slot each iteration. */
        if ((gen->kind == E_ASSIGN) &&
            gen->nchildren >= 2 && is_suspendable(gen->children[1])) {
            EXPR_t *leaf = find_leaf_suspendable(gen->children[1]);
            if (!leaf) leaf = gen->children[1];   /* fallback: treat whole RHS as gen */
            bb_node_t rbox = coro_eval(leaf);
            DESCR_t tick = rbox.fn(rbox.ζ, α);
            while (!IS_FAIL_fn(tick) && !FRAME.returning && !FRAME.loop_break) {
                /* Inject the raw generator tick value via drive passthrough,
                 * then re-evaluate gen (the assign/augop) — interp_eval re-reads
                 * the current frame value of any E_VAR in the expression. */
                coro_drive_node = leaf;
                coro_drive_val  = tick;
                interp_eval(gen);
                coro_drive_node = NULL;
                if (body) interp_eval(body);
                if (FRAME.returning || FRAME.loop_break) break;
                tick = rbox.fn(rbox.ζ, β);
            }
            FRAME.loop_break = 0;
            return NULVCL;
        }
        EXPR_t *do_expr = body ? body : gen;
        bb_node_t box = coro_eval(gen);
        DESCR_t val = box.fn(box.ζ, α);
        while (!IS_FAIL_fn(val) && !FRAME.returning && !FRAME.loop_break) {
            frame_push(gen, val.v == DT_I ? val.i : 0, val.v == DT_I ? NULL : val.s);
            interp_eval(do_expr);
            frame_pop();
            if (FRAME.returning || FRAME.loop_break) break;
            val = box.fn(box.ζ, β);
        }
        FRAME.loop_break = 0;
        return NULVCL;
    }

    case E_WHILE: {
        int saved_brk = FRAME.loop_break; FRAME.loop_break = 0;
        while (!FRAME.returning && !FRAME.loop_break && !FRAME.suspending &&
               !IS_FAIL_fn(interp_eval(e->children[0]))) {
            if (e->nchildren > 1) interp_eval(e->children[1]);
            if (FRAME.suspending) break;
        }
        FRAME.loop_break = saved_brk;
        return NULVCL;
    }

    case E_UNTIL: {
        int saved_brk = FRAME.loop_break; FRAME.loop_break = 0;
        while (!FRAME.returning && !FRAME.loop_break && !FRAME.suspending) {
            DESCR_t cv = (e->nchildren > 0) ? interp_eval(e->children[0]) : FAILDESCR;
            if (!IS_FAIL_fn(cv)) break;
            if (e->nchildren > 1) interp_eval(e->children[1]);
            if (FRAME.suspending) break;
        }
        FRAME.loop_break = saved_brk;
        return NULVCL;
    }

    case E_REPEAT: {
        int saved_brk = FRAME.loop_break; FRAME.loop_break = 0;
        while (!FRAME.returning && !FRAME.loop_break && !FRAME.suspending)
            if (e->nchildren > 0) { interp_eval(e->children[0]); if (FRAME.suspending) break; }
        FRAME.loop_break = saved_brk;
        return NULVCL;
    }

    case E_SEQ_EXPR: {
        DESCR_t v = NULVCL;
        for (int i = 0; i < e->nchildren && !FRAME.returning; i++)
            v = interp_eval(e->children[i]);
        return v;
    }

    case E_IF: {
        if (e->nchildren < 1) return NULVCL;
        DESCR_t cv = interp_eval(e->children[0]);
        if (!IS_FAIL_fn(cv))
            return (e->nchildren > 1) ? interp_eval(e->children[1]) : cv;
        return (e->nchildren > 2) ? interp_eval(e->children[2]) : FAILDESCR;
    }

    case E_CASE: {
        /* Two layouts:
         *   Icon:  child[0]=topic, then pairs [val, body]..., optional trailing default_body
         *   Raku:  child[0]=topic, then triples [cmpnode(E_ILIT|E_NUL), val, body]
         * Detect by child[1]: if it's E_ILIT or E_NUL it's a Raku cmpnode (triples).
         * Icon case values are always full expressions — never bare E_ILIT/E_NUL. */
        if (e->nchildren < 1) return NULVCL;
        DESCR_t topic = interp_eval(e->children[0]);
        /* Detect layout by child count:
         *   Raku triples: nchildren = 1 + 3*N  (topic + N×[cmpnode,val,body])  → (nchildren-1) % 3 == 0
         *   Icon pairs:   nchildren = 1 + 2*N  or  1 + 2*N + 1 (with default)  → (nchildren-1) % 2 == 0 or 1
         * Additionally verify child[1] is E_ILIT or E_NUL for Raku (cmpnode marker). */
        int is_raku_layout = (e->nchildren >= 4 && (e->nchildren - 1) % 3 == 0 &&
            e->children[1] && (e->children[1]->kind == E_ILIT || e->children[1]->kind == E_NUL));
        if (is_raku_layout) {
            /* Raku: triples [cmpnode(E_ILIT), val, body] */
            int i = 1;
            while (i + 2 < e->nchildren) {
                EXPR_t *cmpnode = e->children[i];
                EXPR_t *val     = e->children[i+1];
                EXPR_t *body    = e->children[i+2];
                i += 3;
                if (cmpnode->kind == E_NUL) return interp_eval(body);
                EXPR_e cmp = (EXPR_e)(cmpnode->ival);
                DESCR_t wval = interp_eval(val);
                int match = 0;
                if (cmp == E_LEQ) {
                    const char *ts = IS_STR_fn(topic)?topic.s:VARVAL_fn(topic);
                    const char *ws = IS_STR_fn(wval)?wval.s:VARVAL_fn(wval);
                    match = (ts && ws && strcmp(ts,ws)==0);
                } else {
                    if (IS_INT_fn(topic) && IS_INT_fn(wval)) match = (topic.i == wval.i);
                    else { const char *ts=VARVAL_fn(topic),*ws=VARVAL_fn(wval); match=(ts&&ws&&strcmp(ts,ws)==0); }
                }
                if (match) return interp_eval(body);
            }
            if (i+1 < e->nchildren && e->children[i]->kind==E_NUL)
                return interp_eval(e->children[i+1]);
            return NULVCL;
        }
        /* Icon: pairs [val, body] then optional trailing default body */
        int nc = e->nchildren;
        int i = 1;
        while (i + 1 < nc) {
            DESCR_t wval = interp_eval(e->children[i]);
            EXPR_t *body = e->children[i+1];
            i += 2;
            int match;
            if (IS_INT_fn(topic) && IS_INT_fn(wval)) match = (topic.i == wval.i);
            else {
                const char *ts = VARVAL_fn(topic), *ws = VARVAL_fn(wval);
                match = (ts && ws && strcmp(ts, ws) == 0);
            }
            if (match) return interp_eval(body);
        }
        /* trailing default body (odd child count) */
        if (i < nc) return interp_eval(e->children[i]);
        return NULVCL;
    }

    case E_NULL: {
        /* /E — succeeds (yields &null) iff E succeeds with the null value;
         * fails if E fails OR if E yields any non-null value.
         * Pre-IC-9 this was wrong: it returned NULVCL iff IS_FAIL_fn(v),
         * conflating "E failed" with "E was null".  /x[k] for a missing
         * table key (returns NULVCL, which is success-with-null) reported
         * fail; /x[k] for a present non-null value also reported fail; the
         * two contradictory bugs cancelled in some tests, but rung36_jcon_*
         * exposed them via probes like `/x[1] | write("/1")`. */
        if (e->nchildren < 1) return NULVCL;
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (v.v == DT_SNUL) return NULVCL;
        if (v.v == DT_S && (!v.s || v.s[0] == '\0')) return NULVCL;
        return FAILDESCR;
    }

    case E_NONNULL: {
        /* \E — succeeds (yields E) iff E succeeds with a non-null value;
         * fails if E fails OR if E yields the null value.  Mirror of
         * E_NULL above.  Pre-IC-9 this missed the DT_SNUL case entirely
         * — \&null returned &null (success) instead of failing. */
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        if (v.v == DT_SNUL) return FAILDESCR;
        if (v.v == DT_S && (!v.s || v.s[0] == '\0')) return FAILDESCR;
        return v;
    }

    case E_RANDOM: {
        /* ?E — Icon random selector.  IC-9 (session 2026-04-30 #20):
         *   ?n  (integer)  → random integer in [1,n]; fails if n ≤ 0
         *   ?s  (string)   → random character of s; fails if s is empty
         *   ?&null         → fails (treated as empty string)
         *   ?T  (table)    → random entry value; fails if T is empty
         *   ?L  (list)     → random element; fails if L is empty
         * Pre-fix this was unhandled (default→NULVCL), so ?T_empty returned
         * &null rather than failing — surfaced in rung36_jcon_table line
         * `should fail &null` and rung36_jcon_evalx `?table() ----> &null`.
         *
         * RNG: local static LCG (Knuth MMIX constants), self-contained.
         * No cross-file linkage; matches frontend's icn_random shape. */
        if (e->nchildren < 1) return FAILDESCR;
        DESCR_t v = interp_eval(e->children[0]);
        if (IS_FAIL_fn(v)) return FAILDESCR;
        static unsigned long _rnd_seed = 12345UL;
        _rnd_seed = _rnd_seed * 6364136223846793005UL + 1442695040888963407UL;
        unsigned long _rnd = _rnd_seed >> 33;

        /* DT_T table — pick random entry value, fail if empty */
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
            return FAILDESCR;  /* unreachable when size matches reality */
        }
        /* DT_DATA icnlist — pick random element, fail if empty */
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
            /* IC-9 (2026-05-01): DT_DATA record — pick random field value, fail if no fields */
            if (v.u && v.u->type && v.u->type->nfields > 0 && v.u->fields) {
                int n = v.u->type->nfields;
                return v.u->fields[_rnd % (unsigned long)n];
            }
            return FAILDESCR;
        }
        /* Integer — random in [1,n]; fail if n ≤ 0 */
        if (IS_INT_fn(v)) {
            long long n = v.i;
            if (n <= 0) return FAILDESCR;
            return INTVAL((long long)((_rnd % (unsigned long)n) + 1));
        }
        /* &null — fail */
        if (v.v == DT_SNUL) return FAILDESCR;
        /* String — random character, fail if empty */
        const char *s = VARVAL_fn(v);
        if (!s || !*s) return FAILDESCR;
        long slen = v.slen > 0 ? v.slen : (long)strlen(s);
        if (slen <= 0) return FAILDESCR;
        char *out = (char *)GC_malloc(2);
        out[0] = s[_rnd % (unsigned long)slen];
        out[1] = '\0';
        return STRVAL(out);
    }

    case E_AUGOP: {
        if (e->nchildren < 2) return NULVCL;
        EXPR_t *lhs = e->children[0];
        EXPR_t *rhs = e->children[1];
        /* Helper lambda: apply augop to (lv, rv), write back to lhs slot, return result */
        #define AUGOP_APPLY(lv_, rv_) do { \
            DESCR_t _lv = (lv_), _rv = (rv_); \
            if (IS_FAIL_fn(_lv)||IS_FAIL_fn(_rv)) break; \
            long _li = IS_INT_fn(_lv)?_lv.i:(long)_lv.r; \
            long _ri = IS_INT_fn(_rv)?_rv.i:(long)_rv.r; \
            DESCR_t _res = NULVCL; \
            switch((IcnTkKind)e->ival){ \
                case TK_AUGPLUS:   _res=INTVAL(_li+_ri); break; \
                case TK_AUGMINUS:  _res=INTVAL(_li-_ri); break; \
                case TK_AUGSTAR:   _res=INTVAL(_li*_ri); break; \
                case TK_AUGSLASH:  _res=_ri?INTVAL(_li/_ri):FAILDESCR; break; \
                case TK_AUGMOD:    _res=_ri?INTVAL(_li%_ri):FAILDESCR; break; \
                case TK_AUGCONCAT: { \
                    const char *_ls=VARVAL_fn(_lv),*_rs=VARVAL_fn(_rv); \
                    if(!_ls)_ls="";if(!_rs)_rs=""; \
                    size_t _ll=strlen(_ls),_rl=strlen(_rs); \
                    char *_buf=GC_malloc(_ll+_rl+1); \
                    memcpy(_buf,_ls,_ll);memcpy(_buf+_ll,_rs,_rl);_buf[_ll+_rl]='\0'; \
                    _res=STRVAL(_buf); break; \
                } \
                /* IC-9: comparison-augops — `lv OP:= rv` evaluates `lv OP rv`; \
                 * on success the result is rv (which is then written back to lhs), \
                 * on failure the augop fails (alternation `| "none"` then runs). */ \
                case TK_AUGEQ:   _res = (_li == _ri) ? _rv : FAILDESCR; break; \
                case TK_AUGNE:   _res = (_li != _ri) ? _rv : FAILDESCR; break; \
                case TK_AUGLT:   _res = (_li <  _ri) ? _rv : FAILDESCR; break; \
                case TK_AUGLE:   _res = (_li <= _ri) ? _rv : FAILDESCR; break; \
                case TK_AUGGT:   _res = (_li >  _ri) ? _rv : FAILDESCR; break; \
                case TK_AUGGE:   _res = (_li >= _ri) ? _rv : FAILDESCR; break; \
                case TK_AUGSEQ: case TK_AUGSNE: \
                case TK_AUGSLT: case TK_AUGSLE: case TK_AUGSGT: case TK_AUGSGE: { \
                    const char *_lcs=VARVAL_fn(_lv),*_rcs=VARVAL_fn(_rv); \
                    if(!_lcs)_lcs="";if(!_rcs)_rcs=""; \
                    int _cmp = strcmp(_lcs, _rcs); int _ok = 0; \
                    switch ((IcnTkKind)e->ival) { \
                        case TK_AUGSEQ: _ok = (_cmp == 0); break; \
                        case TK_AUGSNE: _ok = (_cmp != 0); break; \
                        case TK_AUGSLT: _ok = (_cmp <  0); break; \
                        case TK_AUGSLE: _ok = (_cmp <= 0); break; \
                        case TK_AUGSGT: _ok = (_cmp >  0); break; \
                        case TK_AUGSGE: _ok = (_cmp >= 0); break; \
                        default: break; \
                    } \
                    _res = _ok ? _rv : FAILDESCR; break; \
                } \
                default: _res=INTVAL(_li+_ri); break; \
            } \
            if (!IS_FAIL_fn(_res) && lhs->kind == E_VAR) { \
                int _slot=(int)lhs->ival; \
                if (frame_depth > 0 && _slot>=0 && _slot<FRAME.env_n) \
                    FRAME.env[_slot]=_res; \
                else if (_slot < 0 && lhs->sval && lhs->sval[0] != '&') \
                    set_and_trace(lhs->sval, _res); \
            } else if (!IS_FAIL_fn(_res) && lhs->kind == E_IDX && lhs->nchildren >= 2) { \
                DESCR_t _base = interp_eval(lhs->children[0]); \
                DESCR_t _idx  = interp_eval(lhs->children[1]); \
                if (!IS_FAIL_fn(_base) && !IS_FAIL_fn(_idx)) subscript_set(_base, _idx, _res); \
            } else if (!IS_FAIL_fn(_res) && lhs->kind == E_FIELD && lhs->sval && lhs->nchildren >= 1) { \
                DESCR_t _obj = interp_eval(lhs->children[0]); \
                if (!IS_FAIL_fn(_obj)) { DESCR_t *_cell = data_field_ptr(lhs->sval, _obj); if (_cell) *_cell = _res; } \
            } \
            _augop_result = _res; \
        } while(0)

        /* If RHS is a generator: apply augop once per tick, re-reading lhs each time.
         * This implements  every sum +:= (1 to 5)  →  sum=1,3,6,10,15
         * and  every result ||:= !s  →  result="x","xy"  */
        DESCR_t _augop_result = NULVCL;
        /* IC-9: !container OP:= rhs  — bang-iterate as augmented-assign lvalue.
         * Walks every cell of the container, applies the augop in place, in one pass.
         * Mirrors the E_ITERATE LHS branch in E_ASSIGN.  rhs evaluated once.
         *   !T +:= v  (table) — buckets walk, modify TBPAIR_t::val
         *   !L +:= v  (list)  — array walk, modify frame_elems[i]
         */
        if (lhs && lhs->kind == E_ITERATE && lhs->nchildren >= 1) {
            DESCR_t cv = interp_eval(lhs->children[0]);
            DESCR_t rv = interp_eval(rhs);
            if (!IS_FAIL_fn(cv) && !IS_FAIL_fn(rv)) {
                #define AUGOP_CELL(cell_) do { \
                    DESCR_t _lv = (cell_); \
                    if (IS_FAIL_fn(_lv)) break; \
                    long _li = IS_INT_fn(_lv)?_lv.i:(long)_lv.r; \
                    long _ri = IS_INT_fn(rv)?rv.i:(long)rv.r; \
                    DESCR_t _res = NULVCL; \
                    switch((IcnTkKind)e->ival){ \
                        case TK_AUGPLUS:   _res=INTVAL(_li+_ri); break; \
                        case TK_AUGMINUS:  _res=INTVAL(_li-_ri); break; \
                        case TK_AUGSTAR:   _res=INTVAL(_li*_ri); break; \
                        case TK_AUGSLASH:  _res=_ri?INTVAL(_li/_ri):FAILDESCR; break; \
                        case TK_AUGMOD:    _res=_ri?INTVAL(_li%_ri):FAILDESCR; break; \
                        case TK_AUGCONCAT: { \
                            const char *_ls=VARVAL_fn(_lv),*_rs=VARVAL_fn(rv); \
                            if(!_ls)_ls="";if(!_rs)_rs=""; \
                            size_t _ll=strlen(_ls),_rl=strlen(_rs); \
                            char *_buf=GC_malloc(_ll+_rl+1); \
                            memcpy(_buf,_ls,_ll);memcpy(_buf+_ll,_rs,_rl);_buf[_ll+_rl]='\0'; \
                            _res=STRVAL(_buf); break; \
                        } \
                        default: _res=INTVAL(_li+_ri); break; \
                    } \
                    if (!IS_FAIL_fn(_res)) { (cell_) = _res; _augop_result = _res; } \
                } while(0)
                if (cv.v == DT_T && cv.tbl) {
                    for (int b = 0; b < TABLE_BUCKETS; b++)
                        for (TBPAIR_t *p = cv.tbl->buckets[b]; p; p = p->next)
                            AUGOP_CELL(p->val);
                } else if (cv.v == DT_DATA) {
                    DESCR_t tag = FIELD_GET_fn(cv, "icn_type");
                    if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                        DESCR_t ea = FIELD_GET_fn(cv, "frame_elems");
                        int n = (int)FIELD_GET_fn(cv, "frame_size").i;
                        DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                        if (elems && n > 0) for (int i = 0; i < n; i++) AUGOP_CELL(elems[i]);
                    } else if (cv.u && cv.u->type && cv.u->type->nfields > 0 && cv.u->fields) {
                        /* IC-9 (2026-05-01): every !record OP:= V — apply OP to each field cell.
                         * Mirrors the table/list branches above; same AUGOP_CELL macro semantics. */
                        for (int i = 0; i < cv.u->type->nfields; i++) AUGOP_CELL(cv.u->fields[i]);
                    }
                }
                #undef AUGOP_CELL
            }
        } else if (rhs && is_suspendable(rhs)) {
            bb_node_t rbox = coro_eval(rhs);
            DESCR_t tick = rbox.fn(rbox.ζ, α);
            while (!IS_FAIL_fn(tick) && !FRAME.loop_break && !FRAME.returning) {
                DESCR_t cur_lv = interp_eval(lhs);   /* re-read lhs each tick */
                AUGOP_APPLY(cur_lv, tick);
                tick = rbox.fn(rbox.ζ, β);
            }
        } else {
            DESCR_t lv = interp_eval(lhs);
            DESCR_t rv = interp_eval(rhs);
            AUGOP_APPLY(lv, rv);
        }
        #undef AUGOP_APPLY
        return _augop_result;
    }

    case E_LOOP_BREAK: {
        FRAME.loop_break = 1;
        return (e->nchildren > 0) ? interp_eval(e->children[0]) : NULVCL;
    }

    case E_SCAN: {
        if (e->nchildren < 1) return FAILDESCR;
        /* ── SNOBOL4 context: `subj ? pat` as expression — run exec_stmt ──
         * When used inside ~(), EVAL(), or another expression in SNOBOL4 mode,
         * E_SCAN must perform the actual pattern match (not just build a pattern
         * descriptor as the Icon path does).  Without this, ~(s ? pat) always
         * fails because interp_eval_pat(pat) returns a DT_P descriptor which is
         * non-fail, so E_NOT sees success and returns FAILDESCR unconditionally.
         * Fix: evaluate subject as string, evaluate pattern in pat context,
         * run exec_stmt, return NULVCL on success or FAILDESCR on failure. */
        if (!frame_depth && !g_pl_active) {
            /* SNOBOL4 mode — perform the match */
            DESCR_t subj_d = interp_eval(e->children[0]);
            if (IS_FAIL_fn(subj_d)) return FAILDESCR;
            /* subject name for write-back */
            const char *sname = (e->children[0]->kind == E_VAR) ? e->children[0]->sval : NULL;
            DESCR_t pat_d = (e->nchildren >= 2) ? interp_eval_pat(e->children[1]) : pat_epsilon();
            if (IS_FAIL_fn(pat_d)) return FAILDESCR;
            int ok = exec_stmt(sname, sname ? NULL : &subj_d, pat_d, NULL, 0);
            return ok ? NULVCL : FAILDESCR;
        }
        /* ── Icon / Prolog context: generator scan ── */
        DESCR_t subj_d = interp_eval(e->children[0]);
        if (IS_FAIL_fn(subj_d)) return FAILDESCR;
        const char *subj_s = VARVAL_fn(subj_d); if (!subj_s) subj_s = "";
        if (scan_depth < SCAN_STACK_MAX) {
            scan_stack[scan_depth].subj = scan_subj;
            scan_stack[scan_depth].pos  = scan_pos;
            scan_depth++;
        }
        scan_subj = subj_s; scan_pos = 1;
        DESCR_t r = (e->nchildren >= 2) ? interp_eval(e->children[1]) : NULVCL;
        if (scan_depth > 0) {
            scan_depth--;
            scan_subj = scan_stack[scan_depth].subj;
            scan_pos  = scan_stack[scan_depth].pos;
        }
        return r;
    }

    case E_ITERATE: {
        /* IC-3: DT_T table — return first value (oneshot; every uses coro_bb_tbl_iterate) */
        if (e->nchildren >= 1) {
            DESCR_t sv = interp_eval(e->children[0]);
            if (sv.v == DT_T && sv.tbl) {
                for (int bi = 0; bi < TABLE_BUCKETS; bi++) {
                    if (sv.tbl->buckets[bi]) return sv.tbl->buckets[bi]->val;
                }
                return FAILDESCR;
            }
            /* IC-5: DT_DATA icnlist — !L returns first element (every drives the rest) */
            if (sv.v == DT_DATA) {
                DESCR_t tag = FIELD_GET_fn(sv,"icn_type");
                if (tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0) {
                    int n=(int)FIELD_GET_fn(sv,"frame_size").i;
                    DESCR_t ea=FIELD_GET_fn(sv,"frame_elems");
                    DESCR_t *elems=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
                    if(!elems||n<=0) return FAILDESCR;
                    return elems[0];
                }
                /* IC-9 (2026-05-01): DT_DATA record — !R returns first field value.
                 * Mirrors the icnlist branch above: scalar context yields child 0,
                 * every-context drives the rest via coro_bb_record_iterate.  See
                 * coro_eval E_ITERATE for the box. */
                if (sv.u && sv.u->type && sv.u->type->nfields > 0 && sv.u->fields) {
                    return sv.u->fields[0];
                }
            }
        }
        long cur; const char *str;
        if (icn_frame_lookup_sv(e, &cur, &str) && str) {
            char *ch = GC_malloc(2); ch[0] = str[cur]; ch[1] = '\0';
            return STRVAL(ch);
        }
        return FAILDESCR;
    }

    case E_SUSPEND: {
        /* Icon suspend: yield a value to the driving coro_drive E_FNC loop.
         * Signal by setting FRAME.suspending=1; coro_drive sees the flag,
         * runs the every-body, executes the do-clause, clears the flag, and
         * re-enters the procedure body loop.  For non-generator (bare call)
         * contexts there is no driver, so we just return the value. */
        DESCR_t val = (e->nchildren > 0) ? interp_eval(e->children[0]) : NULVCL;
        if (frame_depth > 0) {
            FRAME.suspending  = 1;
            FRAME.suspend_val = val;
            FRAME.suspend_do  = (e->nchildren > 1) ? e->children[1] : NULL;
        }
        return val;
    }

    /* ── IC-5: E_SWAP — x :=: y  (swap two lvalues) ─────────────────────
     * IC-9 (session #26): left-to-right halt-on-keyword-OOB semantics.
     * Swap writes rv → lhs first, then lv → rhs.  If a keyword side
     * OOB-fails, stop immediately — don't perform later writes.
     * Pre-fix: both writes always attempted, leaving state half-swapped
     * in shapes the JCON corpus's subjpos test specifically exercises.
     * (My first attempt at "atomic all-or-nothing" was wrong; tracing
     * subjpos.expected lines 57/58 shows x :=: &pos with `x:=9, &pos:=3`
     * → x=3 (lhs write committed) &pos=3 (rhs write OOB-aborted).)        */
    case E_SWAP: {
        if (e->nchildren < 2 || frame_depth <= 0) return NULVCL;
        EXPR_t *lhs = e->children[0], *rhs = e->children[1];
        DESCR_t lv = interp_eval(lhs), rv = interp_eval(rhs);
        if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
        /* Step 1: write rv → lhs.  Halt on keyword-OOB. */
        if (lhs && lhs->kind == E_VAR) {
            if (lhs->sval && lhs->sval[0] == '&') {
                if (!kw_assign(lhs->sval + 1, rv)) return FAILDESCR;
            } else {
                int sl=(int)lhs->ival;
                if (sl>=0&&sl<FRAME.env_n) FRAME.env[sl]=rv;
                else if (sl<0&&lhs->sval) NV_SET_fn(lhs->sval,rv);
            }
        }
        /* Step 2: write lv → rhs. */
        if (rhs && rhs->kind == E_VAR) {
            if (rhs->sval && rhs->sval[0] == '&') {
                if (!kw_assign(rhs->sval + 1, lv)) return FAILDESCR;
            } else {
                int sl=(int)rhs->ival;
                if (sl>=0&&sl<FRAME.env_n) FRAME.env[sl]=lv;
                else if (sl<0&&rhs->sval) NV_SET_fn(rhs->sval,lv);
            }
        }
        return rv;
    }

    /* ── IC-9 session #26: E_REVSWAP — x <-> y  reversible value swap ────
     * Outside `every`, no driver backtracks; behaves identically to
     * left-to-right halt-on-fail E_SWAP.  Inside `every`, coro_eval
     * routes to coro_bb_revswap for snapshot + revert on β.                */
    case E_REVSWAP: {
        if (e->nchildren < 2 || frame_depth <= 0) return NULVCL;
        EXPR_t *lhs = e->children[0], *rhs = e->children[1];
        DESCR_t lv = interp_eval(lhs), rv = interp_eval(rhs);
        if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
        if (lhs && lhs->kind == E_VAR) {
            if (lhs->sval && lhs->sval[0] == '&') {
                if (!kw_assign(lhs->sval + 1, rv)) return FAILDESCR;
            } else {
                int sl=(int)lhs->ival;
                if (sl>=0&&sl<FRAME.env_n) FRAME.env[sl]=rv;
                else if (sl<0&&lhs->sval) NV_SET_fn(lhs->sval,rv);
            }
        }
        if (rhs && rhs->kind == E_VAR) {
            if (rhs->sval && rhs->sval[0] == '&') {
                if (!kw_assign(rhs->sval + 1, lv)) return FAILDESCR;
            } else {
                int sl=(int)rhs->ival;
                if (sl>=0&&sl<FRAME.env_n) FRAME.env[sl]=lv;
                else if (sl<0&&rhs->sval) NV_SET_fn(rhs->sval,lv);
            }
        }
        return rv;
    }

    /* ── IC-5: E_LCONCAT — s1 ||| s2  (list concatenation = string concat alias) */
    case E_LCONCAT: {
        if (e->nchildren < 2) return NULVCL;
        DESCR_t a = interp_eval(e->children[0]);
        DESCR_t b = interp_eval(e->children[1]);
        /* For string operands, behave like string concat */
        char ab[64], bb[64];
        const char *as = IS_INT_fn(a)?(snprintf(ab,64,"%lld",(long long)a.i),ab):IS_REAL_fn(a)?(real_str(a.r,ab,64),ab):VARVAL_fn(a);
        const char *bs = IS_INT_fn(b)?(snprintf(bb,64,"%lld",(long long)b.i),bb):IS_REAL_fn(b)?(real_str(b.r,bb,64),bb):VARVAL_fn(b);
        if (!as) as=""; if (!bs) bs="";
        size_t al=strlen(as),bl=strlen(bs);
        char *buf=GC_malloc(al+bl+1); memcpy(buf,as,al); memcpy(buf+al,bs,bl); buf[al+bl]='\0';
        return STRVAL(buf);
    }

    /* ── IC-5: E_MAKELIST — [e1,e2,...] list constructor ───────────────── */
    case E_MAKELIST: {
        int n = e->nchildren;
        /* Register icnlist type once if needed, using a global flag */
        static int icnlist_registered = 0;
        if (!icnlist_registered) { DEFDAT_fn("icnlist(frame_elems,frame_size,icn_type)"); icnlist_registered=1; }
        DESCR_t *elems = GC_malloc((n>0?n:1)*sizeof(DESCR_t));
        for (int i = 0; i < n; i++) elems[i] = interp_eval(e->children[i]);
        DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
        DESCR_t ld = DATCON_fn("icnlist", eptr, INTVAL(n), STRVAL("list"));
        return ld;
    }

    /* ── IC-5: E_SECTION — s[i:j] string section ─────────────────────────
     * Icon position rules:
     *   p ≥ 1     → position p (1-based; position 1 is before first char)
     *   p == 0    → position past last char (= slen+1)
     *   p < 0     → position slen+1+p   (-1 → slen, -2 → slen-1, …)
     * Out-of-bounds (after normalization, p < 1 or p > slen+1) → fail. */
    case E_SECTION: {
        if (e->nchildren < 3) return NULVCL;
        DESCR_t sd = interp_eval(e->children[0]);
        const char *s = VARVAL_fn(sd); if (!s) s="";
        int slen = (int)strlen(s);
        int i = (int)to_int(interp_eval(e->children[1]));
        int j = (int)to_int(interp_eval(e->children[2]));
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (j == 0) j = slen + 1; else if (j < 0) j = slen + 1 + j;
        if (i < 1 || i > slen+1 || j < 1 || j > slen+1) return FAILDESCR;
        int lo = i < j ? i : j, hi = i < j ? j : i;
        int len = hi - lo;
        char *buf = GC_malloc(len+1); memcpy(buf, s+lo-1, len); buf[len]='\0';
        return STRVAL(buf);
    }

    /* IC-9 (2026-05-01): E_SECTION_PLUS — s[i+:n] read.  Position i, length n.
     * Equivalent to s[i : i+n] with negative-position normalization on i. */
    case E_SECTION_PLUS: {
        if (e->nchildren < 3) return NULVCL;
        DESCR_t sd = interp_eval(e->children[0]);
        const char *s = VARVAL_fn(sd); if (!s) s = "";
        int slen = (int)strlen(s);
        int i = (int)to_int(interp_eval(e->children[1]));
        int n = (int)to_int(interp_eval(e->children[2]));
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen+1) return FAILDESCR;
        /* n may be negative — equivalent to extending leftward. */
        int lo, hi;
        if (n >= 0) { lo = i; hi = i + n; }
        else        { lo = i + n; hi = i; }
        if (lo < 1 || hi > slen+1) return FAILDESCR;
        int len = hi - lo;
        char *buf = GC_malloc(len+1); memcpy(buf, s+lo-1, len); buf[len]='\0';
        return STRVAL(buf);
    }

    /* IC-9 (2026-05-01): E_SECTION_MINUS — s[i-:n] read.  Position i, span n
     * to the LEFT of i.  Equivalent to s[i-n : i] with negative-n flipping. */
    case E_SECTION_MINUS: {
        if (e->nchildren < 3) return NULVCL;
        DESCR_t sd = interp_eval(e->children[0]);
        const char *s = VARVAL_fn(sd); if (!s) s = "";
        int slen = (int)strlen(s);
        int i = (int)to_int(interp_eval(e->children[1]));
        int n = (int)to_int(interp_eval(e->children[2]));
        if (i == 0) i = slen + 1; else if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen+1) return FAILDESCR;
        int lo, hi;
        if (n >= 0) { lo = i - n; hi = i; }
        else        { lo = i;     hi = i - n; }
        if (lo < 1 || hi > slen+1) return FAILDESCR;
        int len = hi - lo;
        char *buf = GC_malloc(len+1); memcpy(buf, s+lo-1, len); buf[len]='\0';
        return STRVAL(buf);
    }

    /* ── IC-5: E_INITIAL — once-only block; persists local values across calls ─ */
    case E_INITIAL: {
        /* Uses file-scope init_tab[] keyed on e->id.
         * First call: run block, snapshot assigned locals.
         * Subsequent calls: restore snapshot (updated at call exit by icn_init_update_snapshot). */
        IcnInitEnt *ent = NULL;
        for (int _i = 0; _i < icn_init_n; _i++)
            if (init_tab[_i].id == e->id) { ent = &init_tab[_i]; break; }

        if (!ent) {
            /* ── First call: run the block ── */
            for (int i = 0; i < e->nchildren; i++) interp_eval(e->children[i]);
            if (icn_init_n < ICN_INIT_MAX) {
                ent = &init_tab[icn_init_n++];
                ent->id = e->id; ent->ns = 0;
                for (int i = 0; i < e->nchildren && ent->ns < ICN_INIT_SLOTS; i++) {
                    EXPR_t *ch = e->children[i];
                    if (!ch || ch->kind != E_ASSIGN || ch->nchildren < 1) continue;
                    EXPR_t *lhs = ch->children[0];
                    if (!lhs || lhs->kind != E_VAR || !lhs->sval) continue;
                    IcnInitSlot *sl = &ent->s[ent->ns++];
                    strncpy(sl->nm, lhs->sval, 63); sl->nm[63] = '\0';
                    if (frame_depth > 0 && lhs->ival >= 0 && lhs->ival < FRAME.env_n)
                        sl->val = FRAME.env[lhs->ival];
                    else
                        sl->val = NV_GET_fn(lhs->sval);
                }
            }
            e->ival = 1;
        } else {
            /* ── Subsequent calls: restore snapshot into frame/NV ── */
            for (int si = 0; si < ent->ns; si++) {
                int restored = 0;
                if (frame_depth > 0) {
                    for (int i = 0; i < e->nchildren && !restored; i++) {
                        EXPR_t *ch = e->children[i];
                        if (!ch || ch->kind != E_ASSIGN || ch->nchildren < 1) continue;
                        EXPR_t *lhs = ch->children[0];
                        if (!lhs || lhs->kind != E_VAR || !lhs->sval) continue;
                        if (strcasecmp(lhs->sval, ent->s[si].nm) == 0
                            && lhs->ival >= 0 && lhs->ival < FRAME.env_n) {
                            FRAME.env[lhs->ival] = ent->s[si].val;
                            restored = 1;
                        }
                    }
                }
                if (!restored) NV_SET_fn(ent->s[si].nm, ent->s[si].val);
            }
        }
        return NULVCL;
    }

    /* ── IC-5: E_RECORD — register record type ──────────────────────────── */
    case E_RECORD: {
        /* e->sval = type name; children = field name E_VAR nodes.
         * Build spec string "typename(f1,f2,...)" and call DEFDAT_fn + sc_dat_register. */
        if (!e->sval) return NULVCL;
        /* Only register once (EXPR_t node persists across calls) */
        if (e->ival != 0) return NULVCL;
        e->ival = 1;
        char spec[256]; int pos=0;
        pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", e->sval);
        for (int i = 0; i < e->nchildren && pos < (int)sizeof(spec)-2; i++) {
            if (i > 0) spec[pos++]=',';
            const char *fn2 = (e->children[i] && e->children[i]->sval) ? e->children[i]->sval : "";
            pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
        }
        if (pos < (int)sizeof(spec)-1) spec[pos++]=')';
        spec[pos]='\0';
        DEFDAT_fn(spec);
        sc_dat_register(spec);   /* IC-5: also register in our dispatch table */
        return NULVCL;
    }

    /* ── IC-5: E_FIELD — field access: obj.fieldname ────────────────────── */
    case E_FIELD: {
        /* e->sval = field name; child[0] = object expression */
        if (!e->sval || e->nchildren < 1) return NULVCL;
        DESCR_t obj = interp_eval(e->children[0]);
        if (IS_FAIL_fn(obj)) return FAILDESCR;
        DESCR_t *cell = data_field_ptr(e->sval, obj);
        if (!cell) return FAILDESCR;
        return *cell;
    }

    /* ── IC-5: E_GLOBAL (declaration, skip at eval time) ───────────────── */
    case E_GLOBAL:
        return NULVCL;

    default:
        return NULVCL;
    }
}


/* -- interp_eval_ref -- lvalue evaluator → DESCR_t* (SIL NAME semantics) -----
 * Returns a pointer to the live descriptor cell for the given lvalue expression.
 * Mirrors SIL: ARYA10 (array), ASSCR (table), FIELD (DATA), GNVARS (variable).
 * Returns NULL for non-addressable positions (OOB, I/O vars, etc.).
 * The caller wraps the result as NAMEPTR(ptr) to create a DT_N descriptor.
 * --------------------------------------------------------------------- */
