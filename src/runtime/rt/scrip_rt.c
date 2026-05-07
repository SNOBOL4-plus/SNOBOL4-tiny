/*
 * scrip_rt.c — libscrip_rt.so implementation (M-JITEM-X64 / EM-1..EM-7-pre)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06; EM-7-revert 2026-05-07
 *
 * EM-1: init / finalize skeleton.
 * EM-2: push_int / pop_int / halt / unhandled_op.
 * EM-3: push_str / pop_descr / arith / nv_get(stub) / nv_set(stub) / pop_void.
 * EM-4: last_ok flag.
 * EM-5: push_chunk_descr.
 * EM-6: [REVERTED in EM-7-revert, session #72] The full pattern-builder
 *   ABI (scrip_rt_pat_*, scrip_rt_exec_stmt) and the runtime pat-stack
 *   (g_pat_stack[], g_pat_sp) are removed.  Lon's correction: this
 *   brokered descriptor-tree-then-broker model was the wrong architecture
 *   for emitted code.  See GOAL-MODE4-EMIT.md "Design Discoveries"
 *   section.  EM-7a/b/c will reintroduce pattern emit using the proven
 *   dual-mode bb_emit infrastructure (bb_flat in EMIT_TEXT mode for
 *   invariant sub-trees baked as flat .text chunks; bb_emit BINARY mode
 *   for variant nodes built into bb_pool RX memory at runtime).
 *   The full SNOBOL4 runtime stays linked in (bb_pool, snobol4_pattern,
 *   stmt_exec, etc.) — those objects will be called by EM-7c via the
 *   bb_build_flat / bb_build_binary entries already proven in mode-3.
 * EM-7-pre keepers: scrip_rt_concat / scrip_rt_push_null /
 *   scrip_rt_coerce_num / scrip_rt_call / scrip_rt_do_return.  These are
 *   Phase 1/4/5 concerns, orthogonal to BB / pattern matching.
 *
 * Value type throughout: DESCR_t (snobol4.h / descr.h).
 *
 * State:
 *   g_vstack[]   — DESCR_t value stack (cap = VSTACK_CAP).
 *   g_vtop       — next free slot (0 = empty).
 *   g_halt_rc    — rc from most recent scrip_rt_halt().
 *   g_halt_set   — nonzero once halt has been called.
 *   g_last_ok    — success flag for SM_JUMP_S / SM_JUMP_F.
 */

#include "scrip_rt.h"

/* Full SNOBOL4 runtime headers — .so links all runtime objects -fPIC. */
#include "../../runtime/x86/snobol4.h"
#include "../../runtime/x86/descr.h"
#include "../../runtime/x86/sil_macros.h"   /* EM-7: IS_NAMEPTR / IS_NAMEVAL / NAME_DEREF_PTR */
#include "../../runtime/x86/bb_pool.h"
#include "../../runtime/x86/bb_box.h"       /* EM-7c: bb_box_fn + exec_stmt_blob */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==============================================================================
 * Forward declarations — symbols in snobol4.c / stmt_exec.c / snobol4_pattern.c
 *============================================================================*/

extern void    SNO_INIT_fn(void);
extern int     exec_stmt(const char *subj_name, DESCR_t *subj_var,
                         DESCR_t pat, DESCR_t *repl, int has_repl);
extern DESCR_t NV_GET_fn(const char *name);
extern DESCR_t NV_SET_fn(const char *name, DESCR_t val);
extern DESCR_t NAME_fn(const char *varname);
extern char   *VARVAL_fn(DESCR_t v);

extern void    register_fn(const char *name,
                           DESCR_t (*fn)(DESCR_t *, int),
                           int min_args, int max_args);

/* pat_*() constructors — snobol4_pattern.c */
extern DESCR_t pat_lit(const char *s);
extern DESCR_t pat_span(const char *chars);
extern DESCR_t pat_break_(const char *chars);
extern DESCR_t pat_any_cs(const char *chars);
extern DESCR_t pat_notany(const char *chars);
extern DESCR_t pat_len(int64_t n);
extern DESCR_t pat_pos(int64_t n);
extern DESCR_t pat_rpos(int64_t n);
extern DESCR_t pat_tab(int64_t n);
extern DESCR_t pat_rtab(int64_t n);
extern DESCR_t pat_arb(void);
extern DESCR_t pat_arbno(DESCR_t inner);
extern DESCR_t pat_rem(void);
extern DESCR_t pat_fence(void);
extern DESCR_t pat_fence_p(DESCR_t inner);
extern DESCR_t pat_fail(void);
extern DESCR_t pat_abort(void);
extern DESCR_t pat_succeed(void);
extern DESCR_t pat_bal(void);
extern DESCR_t pat_epsilon(void);
extern DESCR_t pat_cat(DESCR_t left, DESCR_t right);
extern DESCR_t pat_alt(DESCR_t left, DESCR_t right);
extern DESCR_t pat_ref(const char *name);
extern DESCR_t pat_assign_imm(DESCR_t child, DESCR_t var);
extern DESCR_t pat_assign_cond(DESCR_t child, DESCR_t var);
extern DESCR_t pat_at_cursor(const char *varname);
extern DESCR_t pat_user_call(const char *name, DESCR_t *args, int nargs);

/* User-call hook for *func() in pattern position (snobol4.c extern) */
extern DESCR_t (*g_user_call_hook)(const char *, DESCR_t *, int);

/*==============================================================================
 * Internal state
 *============================================================================*/

#define VSTACK_CAP   65536  /* EM-7: beauty.sno self-host needs deep stack (256 was insufficient) */

static DESCR_t g_vstack[VSTACK_CAP];
static int     g_vtop    = 0;

/* g_pat_stack[] / g_pat_sp removed in EM-7-revert (session #72) — the
 * brokered runtime descriptor-tree state had no place in the corrected
 * five-phase mode-4 model. */

static int     g_halt_rc  = 0;
static int     g_halt_set = 0;
static int     g_last_ok  = 1;  /* default success at process start */

/*==============================================================================
 * Internal stack helpers
 *============================================================================*/

static void vstack_push(DESCR_t d)
{
    if (g_vtop >= VSTACK_CAP) {
        fprintf(stderr, "libscrip_rt: SM value stack overflow (cap=%d).\n", VSTACK_CAP);
        abort();
    }
    g_vstack[g_vtop++] = d;
}

static DESCR_t vstack_pop(void)
{
    if (g_vtop <= 0) {
        fprintf(stderr, "libscrip_rt: SM value stack underflow.\n");
        abort();
    }
    return g_vstack[--g_vtop];
}

/* pat_push / pat_pop_internal removed in EM-7-revert (session #72) along
 * with the rest of the brokered Phase-3 path. */

/* vstack_pop_str / vstack_pop_int64 helpers were used only by the
 * EM-6 pattern-builder ABI, removed in EM-7-revert (session #72). */

/*==============================================================================
 * EM-6 builtin shims — registered with register_fn so exec_stmt can
 * dispatch *IDENT / *DIFFER in pattern position.
 *============================================================================*/

static DESCR_t _rt_IDENT(DESCR_t *a, int n)
{
    /* IDENT(x[,y]): succeed if x == y (or x is null-string when n==1). */
    const char *s1 = (n >= 1 && a[0].v == DT_S) ? (a[0].s ? a[0].s : "") : "";
    const char *s2 = (n >= 2 && a[1].v == DT_S) ? (a[1].s ? a[1].s : "") : "";
    if (n == 1) return (s1[0] == '\0') ? NULVCL : FAILDESCR;
    return (strcmp(s1, s2) == 0) ? a[0] : FAILDESCR;
}

static DESCR_t _rt_DIFFER(DESCR_t *a, int n)
{
    const char *s1 = (n >= 1 && a[0].v == DT_S) ? (a[0].s ? a[0].s : "") : "";
    const char *s2 = (n >= 2 && a[1].v == DT_S) ? (a[1].s ? a[1].s : "") : "";
    if (n == 1) return (s1[0] != '\0') ? a[0] : FAILDESCR;
    return (strcmp(s1, s2) != 0) ? a[0] : FAILDESCR;
}

static DESCR_t _rt_usercall(const char *name, DESCR_t *args, int nargs)
{
    /* Dispatch *func() in pattern position: look up name in NV table
     * (it should be a DT_E chunk or a registered C function).
     * For EM-6 scope the NV table holds only C builtins; chunk dispatch
     * via scrip_rt_call_chunk is added when EM-6 exercises USERCALL. */
    DESCR_t fn = NV_GET_fn(name ? name : "");
    if (IS_FAIL_fn(fn)) return FAILDESCR;
    /* Fall through to FAILDESCR for unresolved names (safe for EM-6 gate). */
    (void)args; (void)nargs;
    return FAILDESCR;
}

/*==============================================================================
 * EM-1 entries
 *============================================================================*/

void scrip_rt_init(int argc, char **argv)
{
    (void)argc; (void)argv;

    /* EM-6: bring up the full SNOBOL4 runtime.
     * Mirrors the init sequence in scrip.c's SM-run mode block. */
    bb_pool_init();
    SNO_INIT_fn();

    register_fn("IDENT",  _rt_IDENT,  1, 2);
    register_fn("DIFFER", _rt_DIFFER, 1, 2);

    g_user_call_hook = _rt_usercall;
}

int scrip_rt_finalize(void)
{
    return g_halt_set ? g_halt_rc : 0;
}

/*==============================================================================
 * EM-2 entries
 *============================================================================*/

void scrip_rt_push_int(int64_t v)
{
    vstack_push(INTVAL(v));
}

int64_t scrip_rt_pop_int(void)
{
    DESCR_t d = vstack_pop();
    if (d.v != DT_I) {
        fprintf(stderr,
            "libscrip_rt: scrip_rt_pop_int: TOS is not DT_I (tag=%d).\n", d.v);
        abort();
    }
    return d.i;
}

void scrip_rt_halt(int rc)
{
    g_halt_rc  = rc;
    g_halt_set = 1;
}

void scrip_rt_halt_tos(void)
{
    /* Safe-pop TOS as DT_I exit code; use 0 if TOS is missing or not DT_I.
     * This lets SM_HALT work for both synthetic tests (PUSH_LIT_I N + HALT)
     * and real programs (HALT with non-integer TOS → normal exit rc=0). */
    int rc = 0;
    if (g_vtop > 0) {
        DESCR_t d = g_vstack[g_vtop - 1];
        if (d.v == DT_I) { rc = (int)d.i; g_vtop--; }
        /* Non-DT_I TOS: leave stack intact, rc stays 0 */
    }
    g_halt_rc  = rc;
    g_halt_set = 1;
}

void scrip_rt_unhandled_op(int op)
{
    fprintf(stderr,
        "libscrip_rt: unhandled SM opcode %d reached in emitted code.\n"
        "  (scrip --dump-sm to identify; subsequent EM-N rungs shrink the set)\n",
        op);
    abort();
}

/*==============================================================================
 * EM-3 entries
 *============================================================================*/

void scrip_rt_push_str(const char *s, uint32_t slen)
{
    DESCR_t d;
    d.v    = DT_S;
    d.slen = slen ? slen : (uint32_t)(s ? strlen(s) : 0);
    d.s    = (char *)s;
    vstack_push(d);
}

void scrip_rt_pop_descr(DESCR_t *out)
{
    if (!out) { fprintf(stderr, "libscrip_rt: pop_descr: NULL out ptr.\n"); abort(); }
    *out = vstack_pop();
}

void scrip_rt_arith(int op)
{
    /* op: SM_ADD=17 SM_SUB=18 SM_MUL=19 SM_DIV=20 SM_MOD=22 */
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();

    int64_t lv = (l.v == DT_I) ? l.i : to_int(l);
    int64_t rv = (r.v == DT_I) ? r.i : to_int(r);
    int64_t result;

    switch (op) {
        case 17: result = lv + rv; break;
        case 18: result = lv - rv; break;
        case 19: result = lv * rv; break;
        case 20:
            if (!rv) { fprintf(stderr, "libscrip_rt: SM_DIV by zero.\n"); abort(); }
            result = lv / rv; break;
        case 22:
            if (!rv) { fprintf(stderr, "libscrip_rt: SM_MOD by zero.\n"); abort(); }
            result = lv % rv; break;
        default:
            fprintf(stderr, "libscrip_rt: scrip_rt_arith: bad op=%d.\n", op);
            abort();
    }
    vstack_push(INTVAL(result));
}

void scrip_rt_nv_get(const char *name)
{
    vstack_push(NV_GET_fn(name ? name : ""));
}

void scrip_rt_nv_set(const char *name)
{
    DESCR_t val = vstack_pop();
    NV_SET_fn(name ? name : "", val);
}

void scrip_rt_pop_void(void)
{
    (void)vstack_pop();
}

/*==============================================================================
 * EM-4 entries
 *============================================================================*/

int scrip_rt_last_ok(void)  { return g_last_ok; }
void scrip_rt_set_last_ok(int ok) { g_last_ok = ok ? 1 : 0; }

/*==============================================================================
 * EM-5 entries
 *============================================================================*/

void scrip_rt_push_chunk_descr(int64_t entry_pc, int64_t arity)
{
    /* DT_E chunk descriptor: .i = entry_pc, .slen = arity.
     * Mirrors sm_interp.c's DT_E handling for SM_PUSH_CHUNK. */
    DESCR_t d;
    d.v    = DT_E;
    d.slen = (uint32_t)arity;
    d.i    = entry_pc;
    vstack_push(d);
}

/*==============================================================================
 * EM-7c — pattern match for pre-built BB blobs (mode-4 emit path)
 *
 * The mode-4 emitter bakes invariant pattern sub-trees as flat .text
 * chunks via bb_build_flat_text(), with externally-visible entry
 * symbols `_pat_inv_<id>_alpha` etc.  At runtime, the emitted binary
 * pushes the subject and replacement on the SM value stack and calls
 * scrip_rt_match_blob(blob_alpha, sname, has_repl).
 *
 * Stack contract (top-of-stack first, top last popped):
 *   [repl_or_zero]    ← top
 *   [subj_descr]
 *
 * Parameters:
 *   blob_alpha   — address of `_pat_inv_<id>_alpha`
 *   subj_name    — subject variable name for write-back, or NULL
 *   has_repl     — 1 if a replacement is present
 *
 * Calls exec_stmt_blob() (declared in bb_box.h, defined in stmt_exec.c)
 * with the popped subject + replacement.  Stores the :S/:F result on
 * the libscrip_rt last-ok flag (so SM_JUMP_S / SM_JUMP_F see it).
 *============================================================================*/

extern void scrip_rt_set_last_ok(int v);   /* defined below */

void scrip_rt_match_blob(void *blob_alpha,
                         const char *subj_name,
                         int has_repl)
{
    /* Pop replacement (always present — sm_lower emits SM_PUSH_LIT_I 0
     * when has_repl=0 to keep the value-stack shape uniform). */
    DESCR_t repl = vstack_pop();
    DESCR_t subj = vstack_pop();

    bb_box_fn root_fn = (bb_box_fn)blob_alpha;
    int ok = exec_stmt_blob(subj_name,
                            &subj,
                            root_fn,
                            has_repl ? &repl : NULL,
                            has_repl);
    scrip_rt_set_last_ok(ok);
}

/*==============================================================================
 * EM-6 entries — REMOVED in EM-7-revert (session #72)
 *
 * The full pattern-builder ABI (scrip_rt_pat_lit, scrip_rt_pat_span, ...,
 * scrip_rt_pat_capture, scrip_rt_pat_boxval) and the broker entry
 * scrip_rt_exec_stmt have been removed.  See the file-top comment block
 * for rationale.  The proven pattern-construction primitives in
 * snobol4_pattern.c (pat_lit, pat_span, pat_arb, pat_assign_*, ...) are
 * still linked into libscrip_rt.so and remain available — EM-7c will
 * call them from the corrected emit-time path (bb_flat in EMIT_TEXT
 * mode) and the runtime emitter (bb_emit BINARY mode) without the
 * descriptor-tree pat-stack intermediary.
 *============================================================================*/

/*==============================================================================
 * EM-7 entries
 *============================================================================*/

void scrip_rt_concat(void)
{
    /* SM_CONCAT: pop right then left; push CONCAT_fn(left, right). */
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();
    DESCR_t result = CONCAT_fn(l, r);
    vstack_push(result);
    g_last_ok = (result.v != DT_FAIL);
}

void scrip_rt_push_null(void)
{
    /* SM_PUSH_NULL: push null (empty-string) descriptor; null is non-fail. */
    vstack_push(NULVCL);
    g_last_ok = 1;
}

void scrip_rt_coerce_num(void)
{
    /* SM_COERCE_NUM: unary +; coerce string to int (or real if not integer). */
    DESCR_t v = vstack_pop();
    if (v.v == DT_S) {
        int64_t iv = to_int(v);
        if (iv != 0 || (v.s && v.s[0] == '0')) {
            vstack_push(INTVAL(iv));
        } else {
            double rv = to_real(v);
            vstack_push(REALVAL(rv));
        }
    } else {
        vstack_push(v);
    }
    g_last_ok = 1;
}

/* SM_CALL helpers — pseudo-calls inlined here to avoid a full INVOKE_fn round trip.
 * The full pseudo-call vocabulary mirrors sm_interp.c's SM_CALL handler. */
extern DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
extern DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
extern int     subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
extern int     subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
extern void    sno_fold_name(char *name);

/* Local fold-get / fold-set helpers (mirror nv_fold_get/set in sm_interp.c).
 * Note: scrip is case-sensitive by default (RULES.md SN-31), so fold is a no-op
 * in practice — but the SIL-level NV layer may still want canonical names for
 * cases where the source explicitly uppercases.  Cheap; safe; matches interp. */
static DESCR_t _rt_nv_fold_get(const char *raw)
{
    if (!raw || !*raw) return NULVCL;
    char *n = GC_strdup(raw); sno_fold_name(n);
    return NV_GET_fn(n);
}
static void _rt_nv_fold_set(const char *raw, DESCR_t val)
{
    if (!raw || !*raw) return;
    char *n = GC_strdup(raw); sno_fold_name(n);
    NV_SET_fn(n, val);
}

void scrip_rt_call(const char *name, int nargs)
{
    /* Pop nargs from value stack into args[] in original order. */
    DESCR_t args[32];
    if (nargs > 32) nargs = 32;
    for (int k = nargs - 1; k >= 0; k--) args[k] = vstack_pop();

    /* ── Pseudo-calls (handled inline, mirror sm_interp.c) ────────────── */
    if (name && strcmp(name, "INDIR_GET") == 0) {
        /* $expr: name on stack, push value */
        DESCR_t name_d = args[0];
        DESCR_t val;
        if (IS_NAMEPTR(name_d))      val = NAME_DEREF_PTR(name_d);
        else if (IS_NAMEVAL(name_d)) val = _rt_nv_fold_get(name_d.s);
        else                         val = _rt_nv_fold_get(VARVAL_fn(name_d));
        vstack_push(val);
        g_last_ok = 1;
        return;
    }
    if (name && strcmp(name, "NAME_PUSH") == 0) {
        /* .X: pop name string, push DT_N NAMEVAL */
        DESCR_t name_d = args[0];
        const char *vname0 = VARVAL_fn(name_d);
        char *vname = GC_strdup(vname0 ? vname0 : ""); sno_fold_name(vname);
        vstack_push(NAMEVAL(vname));
        g_last_ok = 1;
        return;
    }
    if (name && strcmp(name, "ASGN_INDIR") == 0) {
        DESCR_t name_d = args[1];
        DESCR_t val    = args[0];
        int ok = 0;
        if (IS_NAMEPTR(name_d)) { *(DESCR_t*)name_d.ptr = val; ok = 1; }
        else if (IS_NAMEVAL(name_d)) { _rt_nv_fold_set(name_d.s, val); ok = 1; }
        else {
            const char *vname0 = VARVAL_fn(name_d);
            if (vname0 && *vname0) { _rt_nv_fold_set(vname0, val); ok = 1; }
        }
        vstack_push(val);
        g_last_ok = ok;
        return;
    }
    if (name && strcmp(name, "IDX") == 0) {
        if (nargs == 2) {
            DESCR_t r = subscript_get(args[0], args[1]);
            vstack_push(r);
            g_last_ok = (r.v != DT_FAIL);
        } else if (nargs == 3) {
            DESCR_t r = subscript_get2(args[0], args[1], args[2]);
            vstack_push(r);
            g_last_ok = (r.v != DT_FAIL);
        } else {
            /* N-dim via ITEM builtin */
            DESCR_t r = INVOKE_fn("ITEM", args, nargs);
            vstack_push(r);
            g_last_ok = (r.v != DT_FAIL);
        }
        return;
    }
    if (name && strcmp(name, "IDX_SET") == 0) {
        if (nargs == 3) {        /* val, base, idx */
            DESCR_t val = args[0]; DESCR_t base = args[1]; DESCR_t idx = args[2];
            g_last_ok = subscript_set(base, idx, val);
            vstack_push(val);
        } else if (nargs == 4) {
            DESCR_t val = args[0]; DESCR_t base = args[1];
            DESCR_t i = args[2]; DESCR_t j = args[3];
            g_last_ok = subscript_set2(base, i, j, val);
            vstack_push(val);
        } else {
            DESCR_t r = INVOKE_fn("ITEM_SET", args, nargs);
            g_last_ok = (r.v != DT_FAIL);
            vstack_push(args[0]);  /* val */
        }
        return;
    }

    /* SN-6: SNOBOL4 semantics — if any argument is FAIL, the call fails
     * without invoking the function. */
    for (int k = 0; k < nargs; k++) {
        if (args[k].v == DT_FAIL) {
            vstack_push(FAILDESCR);
            g_last_ok = 0;
            return;
        }
    }

    /* Default dispatch: INVOKE_fn handles builtins and (via g_user_call_hook)
     * user-defined chunks.  In mode-4 today, user-defined SNOBOL4 functions
     * cannot be dispatched yet (no name→native-PC table emitted) — those will
     * fall through to FAILDESCR.  Tracked as honest deviation for EM-7. */
    DESCR_t result = INVOKE_fn(name ? name : "", args, nargs);
    vstack_push(result);
    g_last_ok = (result.v != DT_FAIL);
}

/* SM_RETURN / SM_FRETURN / SM_NRETURN and conditional variants.
 *
 * In mode-4 with native call/ret, the chunk simply executes `ret` for plain
 * RETURN — the return value sits on the value stack already (value chunk's
 * body pushed it).  But FRETURN must replace TOS with FAILDESCR; NRETURN
 * must pop the value and push the function's name as a NAMEVAL descriptor.
 * Conditional variants check g_last_ok before doing anything.
 *
 * Returns 1 if the return should fire (caller emits `ret`), 0 if not (caller
 * falls through).  Note: when condition not met, this function does NOT
 * touch the value stack — the chunk continues normally.
 *
 * In the unconditional+plain RETURN case, the emitter does NOT call this
 * function — it emits `ret` directly.  This function exists for FRETURN /
 * NRETURN and conditional variants. */
int scrip_rt_do_return(int kind, int cond)
{
    /* cond: 0=unconditional, 1=only if last_ok, 2=only if !last_ok */
    if (cond == 1 && !g_last_ok) return 0;
    if (cond == 2 &&  g_last_ok) return 0;

    /* kind: 0=RETURN, 1=FRETURN, 2=NRETURN */
    if (kind == 1) {
        /* FRETURN: discard whatever the body produced, push FAILDESCR.
         * Body already pushed its retval (if any) — pop it; push FAIL. */
        if (g_vtop > 0) (void)vstack_pop();
        vstack_push(FAILDESCR);
        g_last_ok = 0;
    } else if (kind == 2) {
        /* NRETURN: pop body's retval; push a NAMEVAL.  Mode-4 deviation:
         * we don't track the function's retval-slot name at this layer, so
         * we use the raw value as a NAME if it's a string, else FAILDESCR.
         * This is enough for the dot-star NRETURN idiom (push_list = .dummy)
         * because the body explicitly assigned a NAMEVAL via NAME_PUSH. */
        DESCR_t v = (g_vtop > 0) ? vstack_pop() : FAILDESCR;
        if (v.v == DT_N) {
            vstack_push(v);          /* already a NAME */
        } else if (v.v == DT_S && v.s) {
            char *n = GC_strdup(v.s); sno_fold_name(n);
            vstack_push(NAMEVAL(n));
        } else {
            vstack_push(FAILDESCR);
        }
        g_last_ok = 1;
    } else {
        /* RETURN: leave TOS alone; just `ret`. */
        g_last_ok = (g_vtop > 0 && g_vstack[g_vtop - 1].v != DT_FAIL);
    }
    return 1;
}

/* SM_PAT_CAPTURE_FN_ARGS / SM_PAT_USERCALL_ARGS runtime helpers and
 * their pat_assign_callcap[_named_imm] externs were REMOVED in
 * EM-7-revert (session #72) along with the rest of the brokered
 * Phase-3 path.  The underlying pat_assign_callcap* primitives in
 * snobol4_pattern.c remain available for EM-7c. */

/*==============================================================================
 * EM-6 stubs — symbols pulled in transitively by eval_code.c / eval_pat.c
 * that belong to the polyglot driver (sm_interp.c, interp_eval.c, etc.).
 * These are not exercised by the EM-6 SNOBOL4 pattern gate; they will be
 * replaced by real implementations in later rungs (EM-10+).
 *============================================================================*/

#include "../../runtime/x86/sm_interp.h"  /* DESCR_t sm_call_chunk(int) */
#include "../../runtime/x86/sm_prog.h"    /* sm_opcode_name */

/* sm_call_chunk: used by eval_code.c when a DT_E chunk is EVAL'd.
 * Not exercised in EM-6 SNOBOL4 pattern gate (no chunk-via-EVAL paths). */
DESCR_t sm_call_chunk(int entry_pc)
{
    fprintf(stderr,
        "libscrip_rt: sm_call_chunk(%d) called — DT_E EVAL dispatch "
        "not yet wired in EM-6.  Add to EM-10 scope.\n", entry_pc);
    abort();
}

/* sm_opcode_name: used by sm_interp diagnostics pulled in via sm_interp.h.
 * Provide a minimal implementation for the .so. */
const char *sm_opcode_name(sm_opcode_t op)
{
    (void)op;
    return "?";
}

/* _is_pat_fnc_name / _expr_is_pat: used by eval_pat.c.
 * These decide at eval time whether a function call is pattern-returning.
 * Not exercised in EM-6 gate; safe stubs. */
#include "../../driver/interp_private.h"
int _is_pat_fnc_name(const char *s)  { (void)s; return 0; }
int _expr_is_pat(EXPR_t *e)          { (void)e; return 0; }
