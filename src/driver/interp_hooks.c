/*
 * interp_hooks.c — hook functions and IR print utilities
 *
 * Split from interp.c by RS-3 (GOAL-REWRITE-SCRIP).
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * DATE:    2026-05-02
 */

#include "interp_private.h"

/* ══════════════════════════════════════════════════════════════════════════
 * main
 * ══════════════════════════════════════════════════════════════════════════ */

/* _eval_str_impl_fn — EVAL(string) hook for pattern-context strings.
 * Uses bison parse_expr_pat_from_str (snobol4.tab.c) which produces
 * EXPR_t with correct EXPR_e values directly — no CMPILE/CMPND_t bridge. */
DESCR_t _eval_str_impl_fn(const char *s) {
    EXPR_t *tree = parse_expr_pat_from_str(s);
    if (!tree) return FAILDESCR;
    return interp_eval_pat(tree);
}

DESCR_t _eval_pat_impl_fn(DESCR_t pat) {
    /* Run DT_P pattern against empty subject — used by EVAL_fn for *func() patterns.
     * If function fails at match time, EVAL fails. */
    extern int exec_stmt(const char *, DESCR_t *, DESCR_t, DESCR_t *, int);
    DESCR_t subj = STRVAL("");
    int ok = exec_stmt("", &subj, pat, NULL, 0);
    return ok ? NULVCL : FAILDESCR;
}


/* label_exists — called by LABEL() builtin via sno_set_label_exists_hook */
int _label_exists_fn(const char *name) {
    return label_lookup(name) != NULL;
}

/* S-10: forward declarations so _usercall_hook can call them */
DESCR_t _builtin_IDENT(DESCR_t *args, int nargs);
DESCR_t _builtin_DIFFER(DESCR_t *args, int nargs);
DESCR_t _builtin_DATA(DESCR_t *args, int nargs);

/* _usercall_hook: calls user functions via call_user_function;
 * for pure builtins (FNCEX_fn && no body label) uses APPLY_fn directly
 * so FAILDESCR propagates correctly (DYN-74: fixes *ident(1,2) in EVAL).
 * U-22: cross-call extension — if name not found in SNO label table,
 * try proc_table (BB_PUMP one-shot) then g_pl_pred_table (BB_ONCE). */
DESCR_t _usercall_hook(const char *name, DESCR_t *args, int nargs) {
    /* S-10 fix: handle scrip.c-only predicates directly so *IDENT(x)/*DIFFER(x)
     * in pattern context correctly fail/succeed via bb_usercall -> g_user_call_hook. */
    if (strcmp(name, "IDENT") == 0)  return _builtin_IDENT(args, nargs);  /* SN-19 */
    if (strcmp(name, "DIFFER") == 0) return _builtin_DIFFER(args, nargs); /* SN-19 */
    if (strcmp(name, "DATA") == 0)   return _builtin_DATA(args, nargs);   /* SN-19 */
    /* ITEM(arr,i) read and ITEM_SET(rhs,arr,i) write — SM emits these for ITEM() syntax.
     * ITEM read: args[0]=arr, args[1]=i, [args[2]=j for 2D].
     * ITEM_SET write: args[0]=rhs, args[1]=arr, args[2]=i, [args[3]=j for 2D]. */
    if (strcmp(name, "ITEM") == 0 && nargs >= 2) {        /* SN-19 */
        if (nargs >= 3) return subscript_get2(args[0], args[1], args[2]);
        return subscript_get(args[0], args[1]);
    }
    if (strcmp(name, "ITEM_SET") == 0 && nargs >= 3) {    /* SN-19 */
        DESCR_t rhs = args[0], arr = args[1], idx = args[2];
        if (nargs >= 4) { subscript_set2(arr, idx, args[3], rhs); }
        else            { subscript_set(arr, idx, rhs); }
        return rhs;
    }
    /* SC-1: DATA constructor/field-accessor/field-mutator dispatch via sc_dat registry.
     * Must precede label lookup so struct names shadow any same-named labels. */
    {
        ScDatType *_dt = sc_dat_find_type(name);
        if (_dt) return sc_dat_construct(_dt, args, nargs);
        int _fi = 0;
        ScDatType *_ft = sc_dat_find_field(name, &_fi);
        if (_ft && nargs >= 1) return sc_dat_field_get(name, args[0]);
        /* Field mutator: fname_SET(rhs, obj) — sm_lower emits this for fname(obj)=rhs.
         * Strip the _SET suffix, look up the field, write through data_field_ptr.
         * SN-19 stage-2: name arrives canonical, suffix emitted canonical, strcmp correct */
        size_t _nlen = strlen(name);
        if (_nlen > 4 && strcmp(name + _nlen - 4, "_SET") == 0 && nargs >= 2) {
            char _fname[128];
            size_t _flen = _nlen - 4;
            if (_flen >= sizeof(_fname)) _flen = sizeof(_fname) - 1;
            memcpy(_fname, name, _flen); _fname[_flen] = '\0';
            DESCR_t *_cell = data_field_ptr(_fname, args[1]);
            if (_cell) { *_cell = args[0]; return args[0]; }
        }
    }
    /* Check for a body label (user-defined function) */
    const char *_entry = FUNC_ENTRY_fn(name);
    STMT_t *_body = _entry ? label_lookup(_entry) : NULL;
    if (!_body) _body = label_lookup(name);
    if (!_body) {
        char _uf[128]; size_t _fl = strlen(name);
        if (_fl >= sizeof(_uf)) _fl = sizeof(_uf) - 1;
        for (size_t _i = 0; _i <= _fl; _i++)
            _uf[_i] = (char)toupper((unsigned char)name[_i]);
        _body = label_lookup(_uf);
    }
    /* Pure builtin (no body) AND registered as builtin: use APPLY_fn for correct failure */
    if (!_body && FNCEX_fn(name)) return APPLY_fn(name, args, nargs);

    /* ── U-22: cross-language fallback ────────────────────────────────
     * Name not found in SNO label/builtin tables.  Try Icon, then Prolog.
     * This lets SNOBOL4 source call Icon procedures and Prolog predicates
     * by name, the same way the linker resolves an undefined symbol. */
    if (!_body) {
        /* Try Icon proc table (case-sensitive — Icon is case-sensitive) */
        for (int _i = 0; _i < proc_count; _i++) {
            if (strcmp(proc_table[_i].name, name) == 0) {
                /* Call as one-shot: drive the Icon proc and return its value.
                 * coro_call returns FAILDESCR if the procedure fails. */
                return coro_call(proc_table[_i].proc, args, nargs);
            }
        }
        /* Try Prolog pred table: name/arity key, e.g. "color/1" */
        if (g_pl_active) {
            char pl_key[256];
            snprintf(pl_key, sizeof pl_key, "%s/%d", name, nargs);
            EXPR_t *choice = pl_pred_table_lookup(&g_pl_pred_table, pl_key);
            if (choice) {
                /* Set up Prolog arg Term** from DESCR_t args, drive BB_ONCE */
                Term **pl_args = (nargs > 0) ? pl_env_new(nargs) : NULL;
                for (int _i = 0; _i < nargs; _i++)
                    pl_args[_i] = pl_unified_term_from_expr(
                        /* wrap DESCR_t as a literal EXPR_t leaf */
                        (args[_i].v == DT_S)
                            ? &(EXPR_t){ .kind = E_QLIT, .sval = (char*)args[_i].s }
                            : &(EXPR_t){ .kind = E_ILIT, .ival = (long)args[_i].s },
                        NULL);
                Term **saved_env = g_pl_env;
                g_pl_env = pl_args;
                bb_node_t root = pl_box_choice(choice, g_pl_env, nargs);
                int ok = bb_broker(root, BB_ONCE, NULL, NULL);
                g_pl_env = saved_env;
                return ok ? INTVAL(1) : FAILDESCR;
            }
        }
    }

    /* User-defined (has body) OR unknown: call_user_function handles both */
    return call_user_function(name, args, nargs);
}


/* ── ir_print_stmt — print one STMT_t as IR sexp for comparison sweep ──────
 * Emits: (STMT [:lbl L] [:subj EXPR] [:pat EXPR] [:repl EXPR] [:go*])
 * Used by --dump-ir and --dump-ir-bison.
 * ir_print_node() is from src/ir/ir_print.c — linked via Makefile.
 * ----------------------------------------------------------------------- */
static void ir_print_stmt(STMT_t *st, FILE *f) {
    fprintf(f, "(STMT");
    if (st->label)       fprintf(f, " :lbl %s", st->label);
    if (st->has_eq)      fprintf(f, " :eq");
    if (st->is_end)      fprintf(f, " :end");
    if (st->subject)   { fprintf(f, " :subj ");  ir_print_node(st->subject,     f); }
    if (st->pattern)   { fprintf(f, " :pat ");   ir_print_node(st->pattern,     f); }
    if (st->replacement){fprintf(f, " :repl ");  ir_print_node(st->replacement, f); }
    if (st->goto_u || st->goto_u_expr || st->goto_s || st->goto_s_expr || st->goto_f || st->goto_f_expr) {
        /* RS-1: goto fields now flat in STMT_t */
        if (st->goto_u)     fprintf(f, " :go %s",  st->goto_u);
        if (st->goto_s)     fprintf(f, " :goS %s", st->goto_s);
        if (st->goto_f)     fprintf(f, " :goF %s", st->goto_f);
        if (st->goto_u_expr){ fprintf(f, " :go $(");  ir_print_node(st->goto_u_expr, f); fprintf(f, ")"); }
        if (st->goto_s_expr){ fprintf(f, " :goS $("); ir_print_node(st->goto_s_expr, f); fprintf(f, ")"); }
        if (st->goto_f_expr){ fprintf(f, " :goF $("); ir_print_node(st->goto_f_expr, f); fprintf(f, ")"); }
    }
    fprintf(f, ")\n");
}

/* Dump a full CODE_t* as IR sexp — one line per statement. */
void ir_dump_program(CODE_t *prog, FILE *f) {
    if (!prog) { fprintf(f, "(NULL-PROGRAM)\n"); return; }
    for (STMT_t *st = prog->head; st; st = st->next)
        ir_print_stmt(st, f);
}

/* ── S-10 fix: IDENT/DIFFER wrappers for register_fn ──────────────────────
 * IDENT and DIFFER are scrip.c-only builtins not in the binary runtime's
 * APPLY_fn table.  When *IDENT(x) fires at match time via deferred_call_fn,
 * APPLY_fn("IDENT",...) returns non-FAILDESCR for unknown names → T_FUNC
 * always succeeds.  Registering these wrappers makes APPLY_fn dispatch them
 * correctly so *IDENT(n(x)) fails when n(x) is not the null string. */
DESCR_t _builtin_IDENT(DESCR_t *args, int nargs) {
    if (nargs == 1) return IS_NULL_fn(args[0]) ? NULVCL : FAILDESCR;
    if (nargs >= 2) {
        int a_null = IS_NULL_fn(args[0]), b_null = IS_NULL_fn(args[1]);
        if (a_null && b_null) return NULVCL;
        if (a_null || b_null) return FAILDESCR;
        if (args[0].v != args[1].v) return FAILDESCR;
        const char *sa = VARVAL_fn(args[0]), *sb = VARVAL_fn(args[1]);
        if (!sa) sa = ""; if (!sb) sb = "";
        return strcmp(sa, sb) == 0 ? NULVCL : FAILDESCR;
    }
    return FAILDESCR;
}
DESCR_t _builtin_DIFFER(DESCR_t *args, int nargs) {
    if (nargs == 1) return IS_NULL_fn(args[0]) ? FAILDESCR : NULVCL;
    if (nargs >= 2) {
        int a_null = IS_NULL_fn(args[0]), b_null = IS_NULL_fn(args[1]);
        if (a_null && b_null) return FAILDESCR;
        if (a_null || b_null) return NULVCL;
        if (args[0].v != args[1].v) return NULVCL;
        const char *sa = VARVAL_fn(args[0]), *sb = VARVAL_fn(args[1]);
        if (!sa) sa = ""; if (!sb) sb = "";
        return strcmp(sa, sb) != 0 ? NULVCL : FAILDESCR;
    }
    return FAILDESCR;
}

/* ── EVAL/CODE wrappers ─────────────────────────────────────────────────────
 * EVAL and CODE are not registered in the binary APPLY_fn table.
 * FNCEX_fn("EVAL")=0 so _usercall_hook falls through to call_user_function
 * which finds no body → returns NULVCL (STRING) instead of DT_P.
 * Fix: register wrappers so FNCEX_fn("EVAL")=1 → APPLY_fn dispatches here
 * → EVAL_fn → CONVE_fn → EXPVAL_fn → correct DT_P for pattern expressions. */
extern DESCR_t EVAL_fn(DESCR_t);
extern DESCR_t code(const char *);
DESCR_t _builtin_EVAL(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    return EVAL_fn(args[0]);
}
DESCR_t _builtin_CODE(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    const char *s = VARVAL_fn(args[0]);
    if (!s || !*s) return FAILDESCR;
    return code(s);
}

/* SC-1: print(v...) — Snocone print builtin: outputs each arg on its own line */
