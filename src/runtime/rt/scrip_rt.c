/*
 * scrip_rt.c — libscrip_rt.so implementation (M-JITEM-X64 / EM-1..EM-6)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * EM-1: init / finalize skeleton.
 * EM-2: push_int / pop_int / halt / unhandled_op.
 * EM-3: push_str / pop_descr / arith / nv_get(stub) / nv_set(stub) / pop_void.
 * EM-4: last_ok flag.
 * EM-5: push_chunk_descr.
 * EM-6: full SNOBOL4 runtime linked in.
 *   - scrip_rt_init calls bb_pool_init() + SNO_INIT_fn() + registers builtins.
 *   - scrip_rt_nv_get / scrip_rt_nv_set: real (NV_GET_fn / NV_SET_fn).
 *   - scrip_rt_arith: string coercion via to_int() / to_real().
 *   - scrip_rt_pat_*: pat-stack mirror of sm_interp.c SM_PAT_* dispatch.
 *   - scrip_rt_exec_stmt: delegates to exec_stmt() from stmt_exec.c.
 *
 * Value type throughout: DESCR_t (snobol4.h / descr.h).  ScripRtVal /
 * ScripRtTag are gone.  Every stack slot is a DESCR_t; DT_* tag constants
 * come from descr.h.
 *
 * State:
 *   g_vstack[]   — DESCR_t value stack (fixed cap for EM-6).
 *   g_vtop       — next free slot (0 = empty).
 *   g_pat_stack[]— DESCR_t pat-stack (mirrors sm_interp.c g_pat_stack).
 *   g_pat_sp     — next free pat-stack slot.
 *   g_halt_rc    — rc from most recent scrip_rt_halt().
 *   g_halt_set   — nonzero once halt has been called.
 *   g_last_ok    — success flag for SM_JUMP_S / SM_JUMP_F.
 */

#include "scrip_rt.h"

/* Full SNOBOL4 runtime headers — .so links all runtime objects -fPIC. */
#include "../../runtime/x86/snobol4.h"
#include "../../runtime/x86/descr.h"
#include "../../runtime/x86/bb_pool.h"

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

#define VSTACK_CAP   256
#define PATSTACK_CAP  64

static DESCR_t g_vstack[VSTACK_CAP];
static int     g_vtop    = 0;

static DESCR_t g_pat_stack[PATSTACK_CAP];
static int     g_pat_sp   = 0;

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

static void pat_push(DESCR_t d)
{
    if (g_pat_sp >= PATSTACK_CAP) {
        fprintf(stderr, "libscrip_rt: pat-stack overflow (cap=%d).\n", PATSTACK_CAP);
        abort();
    }
    g_pat_stack[g_pat_sp++] = d;
}

static DESCR_t pat_pop_internal(void)
{
    if (g_pat_sp <= 0) {
        fprintf(stderr, "libscrip_rt: pat-stack underflow.\n");
        abort();
    }
    return g_pat_stack[--g_pat_sp];
}

/* Pop a charset string from vstack; return pointer valid for this call.
 * The string is owned by the DESCR_t; lifetime matches the GC heap. */
static const char *vstack_pop_str(void)
{
    DESCR_t d = vstack_pop();
    if (d.v == DT_S) return d.s ? d.s : "";
    /* Coerce via VARVAL_fn (handles DT_I -> decimal string etc.) */
    char *s = VARVAL_fn(d);
    return s ? s : "";
}

static int64_t vstack_pop_int64(void)
{
    DESCR_t d = vstack_pop();
    if (d.v == DT_I) return d.i;
    return to_int(d);
}

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
 * EM-6 entries — pattern builder
 *============================================================================*/

void scrip_rt_pat_lit(const char *s)
{
    pat_push(pat_lit(s ? s : ""));
}

void scrip_rt_pat_refname(const char *name)
{
    pat_push(pat_ref(name ? name : ""));
}

void scrip_rt_pat_span(void)
{
    const char *cs = vstack_pop_str();
    pat_push(pat_span(cs));
}

void scrip_rt_pat_break(void)
{
    const char *cs = vstack_pop_str();
    pat_push(pat_break_(cs));
}

void scrip_rt_pat_any(void)
{
    const char *cs = vstack_pop_str();
    pat_push(pat_any_cs(cs));
}

void scrip_rt_pat_notany(void)
{
    const char *cs = vstack_pop_str();
    pat_push(pat_notany(cs));
}

void scrip_rt_pat_len(void)   { pat_push(pat_len(vstack_pop_int64())); }
void scrip_rt_pat_pos(void)   { pat_push(pat_pos(vstack_pop_int64())); }
void scrip_rt_pat_rpos(void)  { pat_push(pat_rpos(vstack_pop_int64())); }
void scrip_rt_pat_tab(void)   { pat_push(pat_tab(vstack_pop_int64())); }
void scrip_rt_pat_rtab(void)  { pat_push(pat_rtab(vstack_pop_int64())); }

void scrip_rt_pat_arb(void)     { pat_push(pat_arb()); }
void scrip_rt_pat_rem(void)     { pat_push(pat_rem()); }
void scrip_rt_pat_fence(void)   { pat_push(pat_fence()); }
void scrip_rt_pat_fail(void)    { pat_push(pat_fail()); }
void scrip_rt_pat_abort(void)   { pat_push(pat_abort()); }
void scrip_rt_pat_succeed(void) { pat_push(pat_succeed()); }
void scrip_rt_pat_bal(void)     { pat_push(pat_bal()); }
void scrip_rt_pat_eps(void)     { pat_push(pat_epsilon()); }

void scrip_rt_pat_arbno(void)
{
    DESCR_t inner = pat_pop_internal();
    pat_push(pat_arbno(inner));
}

void scrip_rt_pat_fence1(void)
{
    DESCR_t child = pat_pop_internal();
    pat_push(pat_fence_p(child));
}

void scrip_rt_pat_cat(void)
{
    DESCR_t right = pat_pop_internal();
    DESCR_t left  = pat_pop_internal();
    pat_push(pat_cat(left, right));
}

void scrip_rt_pat_alt(void)
{
    DESCR_t right = pat_pop_internal();
    DESCR_t left  = pat_pop_internal();
    pat_push(pat_alt(left, right));
}

void scrip_rt_pat_deref(void)
{
    DESCR_t v = vstack_pop();
    if (v.v == DT_P) {
        pat_push(v);                        /* already a pattern */
    } else if (v.v == DT_S && v.s) {
        pat_push(pat_lit(v.s));             /* string -> literal */
    } else {
        char *name = VARVAL_fn(v);
        pat_push(pat_ref(name ? name : ""));
    }
}

void scrip_rt_pat_capture(const char *varname, int kind)
{
    DESCR_t child = pat_pop_internal();
    DESCR_t var   = NAME_fn(varname ? varname : "");
    if (kind == 1)
        pat_push(pat_assign_imm(child, var));
    else if (kind == 2)
        pat_push(pat_cat(child, pat_at_cursor(varname ? varname : "")));
    else
        pat_push(pat_assign_cond(child, var));
}

void scrip_rt_pat_boxval(void)
{
    /* Pop top of pat-stack, push as DT_P onto vstack. */
    vstack_push(pat_pop_internal());
}

void scrip_rt_exec_stmt(const char *subj_name, int has_repl)
{
    /* Stack layout (TOS last, matches sm_interp.c SM_EXEC_STMT):
     *   [subject_descr]  [replacement_or_zero]  <- TOS
     * The replacement slot is ALWAYS pushed (as INTVAL(0) when has_repl=0).
     * We must ALWAYS pop it, regardless of has_repl. */
    DESCR_t repl   = vstack_pop();   /* always pop: repl or INTVAL(0) placeholder */
    DESCR_t subj_d = vstack_pop();   /* subject descriptor */
    DESCR_t pat_d  = (g_pat_sp > 0) ? pat_pop_internal() : pat_epsilon();

    int ok = exec_stmt(subj_name, &subj_d, pat_d,
                       has_repl ? &repl : NULL, has_repl);
    g_last_ok = ok;
    g_pat_sp  = 0;  /* reset pat-stack after each statement */
}

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
