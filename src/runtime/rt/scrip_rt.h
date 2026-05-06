/*
 * scrip_rt.h — public ABI for libscrip_rt.so (M-JITEM-X64 / EM-1..EM-6)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * libscrip_rt.so is the runtime support library that mode-4-emitted
 * binaries link against.  It carries language-level semantics — pattern
 * matcher, NV table, builtins, GC, generator BB pump (post-EM-10),
 * Prolog backtracking machinery (post-EM-14) — so that emitted
 * executables contain only compiled SM chunks plus calls into this ABI.
 *
 * Value type: DESCR_t (from descr.h / snobol4.h).  The separate
 * ScripRtVal/ScripRtTag types that existed through EM-5 are gone.
 * DESCR_t is the one true descriptor; every stack slot is a DESCR_t.
 * DT_* tag constants (DT_SNUL=0, DT_S=1, DT_I=6, DT_E=11, DT_FAIL=99)
 * come from descr.h which emitted binaries include via snobol4.h.
 *
 * EM-1 surface:
 *   scrip_rt_init      — process startup; receives argc/argv
 *   scrip_rt_finalize  — process shutdown; returns the program rc
 *
 * EM-2 surface:
 *   scrip_rt_push_int  — push a DT_I integer onto the SM value stack
 *   scrip_rt_pop_int   — pop TOS as int64_t
 *   scrip_rt_halt      — record the program's exit code
 *   scrip_rt_unhandled_op — runtime trap for unimplemented opcodes
 *
 * EM-3 surface:
 *   scrip_rt_push_str  — push a DT_S string onto the SM stack
 *   scrip_rt_pop_descr — pop TOS into a DESCR_t (SM_STORE_VAR codegen)
 *   scrip_rt_arith     — binary arithmetic (ADD/SUB/MUL/DIV/MOD)
 *   scrip_rt_nv_get    — load named variable onto stack (real in EM-6)
 *   scrip_rt_nv_set    — store TOS into named variable  (real in EM-6)
 *   scrip_rt_pop_void  — pop and discard TOS (SM_POP)
 *
 * EM-4 surface:
 *   scrip_rt_last_ok     — read success flag (SM_JUMP_S / SM_JUMP_F)
 *   scrip_rt_set_last_ok — write success flag
 *
 * EM-5 surface:
 *   scrip_rt_push_chunk_descr — push DT_E chunk descriptor (SM_PUSH_CHUNK)
 *   SM_CALL_CHUNK / SM_RETURN are baked direct (call .LpcN / ret).
 *
 * EM-6 surface:
 *   scrip_rt_pat_lit        — SM_PAT_LIT    (a[0].s = literal)
 *   scrip_rt_pat_span       — SM_PAT_SPAN   (pops charset from vstack)
 *   scrip_rt_pat_break      — SM_PAT_BREAK  (pops charset from vstack)
 *   scrip_rt_pat_any        — SM_PAT_ANY    (pops charset from vstack)
 *   scrip_rt_pat_notany     — SM_PAT_NOTANY (pops charset from vstack)
 *   scrip_rt_pat_len        — SM_PAT_LEN    (pops int from vstack)
 *   scrip_rt_pat_pos        — SM_PAT_POS    (pops int from vstack)
 *   scrip_rt_pat_rpos       — SM_PAT_RPOS   (pops int from vstack)
 *   scrip_rt_pat_tab        — SM_PAT_TAB    (pops int from vstack)
 *   scrip_rt_pat_rtab       — SM_PAT_RTAB   (pops int from vstack)
 *   scrip_rt_pat_arb        — SM_PAT_ARB    (no arg)
 *   scrip_rt_pat_arbno      — SM_PAT_ARBNO  (pops inner from patstack)
 *   scrip_rt_pat_rem        — SM_PAT_REM
 *   scrip_rt_pat_fence      — SM_PAT_FENCE
 *   scrip_rt_pat_fence1     — SM_PAT_FENCE1 (pops child from patstack)
 *   scrip_rt_pat_fail       — SM_PAT_FAIL
 *   scrip_rt_pat_abort      — SM_PAT_ABORT
 *   scrip_rt_pat_succeed    — SM_PAT_SUCCEED
 *   scrip_rt_pat_bal        — SM_PAT_BAL
 *   scrip_rt_pat_eps        — SM_PAT_EPS
 *   scrip_rt_pat_cat        — SM_PAT_CAT    (pops right then left)
 *   scrip_rt_pat_alt        — SM_PAT_ALT    (pops right then left)
 *   scrip_rt_pat_deref      — SM_PAT_DEREF  (pops DESCR_t from vstack)
 *   scrip_rt_pat_refname    — SM_PAT_REFNAME (a[0].s = var name)
 *   scrip_rt_pat_capture    — SM_PAT_CAPTURE (a[0].s=var, a[1].i=kind)
 *   scrip_rt_pat_boxval     — SM_PAT_BOXVAL (pat-stack top -> vstack)
 *   scrip_rt_exec_stmt      — SM_EXEC_STMT  (a[0].s=subj, a[1].i=has_repl)
 *
 * Stability: every symbol exported here is part of the mode-4 ABI.
 * Additions are backward-compatible; signatures must not change.
 */

#ifndef SCRIP_RT_H
#define SCRIP_RT_H

#include <stdint.h>

/* DESCR_t is the value type for all stack operations.  Emitted binaries
 * that include snobol4.h / descr.h get the full struct definition; this
 * forward declaration lets the ABI prototypes compile without snobol4.h. */
#ifndef DESCR_T_DEFINED
#define DESCR_T_DEFINED
typedef struct DESCR_t DESCR_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ── EM-1 surface ────────────────────────────────────────────────────── */

void scrip_rt_init(int argc, char **argv);
int  scrip_rt_finalize(void);

/* ── EM-2 surface ────────────────────────────────────────────────────── */

void    scrip_rt_push_int(int64_t v);        /* push DT_I integer */
int64_t scrip_rt_pop_int(void);              /* pop TOS as int64_t (aborts if not DT_I) */
void    scrip_rt_halt(int rc);
void    scrip_rt_halt_tos(void);  /* safe-pop TOS as rc (DT_I) else 0 */
void    scrip_rt_unhandled_op(int op);

/* ── EM-3 surface ────────────────────────────────────────────────────── */

void scrip_rt_push_str(const char *s, uint32_t slen); /* push DT_S; slen=0 -> strlen */
void scrip_rt_pop_descr(DESCR_t *out);       /* pop TOS into *out (SM_STORE_VAR) */
void scrip_rt_arith(int op);                 /* SM_ADD=17 SUB=18 MUL=19 DIV=20 MOD=22 */
void scrip_rt_nv_get(const char *name);      /* push value of named variable */
void scrip_rt_nv_set(const char *name);      /* pop TOS -> named variable */
void scrip_rt_pop_void(void);                /* pop and discard TOS (SM_POP) */

/* ── EM-4 surface ────────────────────────────────────────────────────── */

int  scrip_rt_last_ok(void);
void scrip_rt_set_last_ok(int ok);

/* ── EM-5 surface ────────────────────────────────────────────────────── */

/* Push DT_E chunk descriptor {entry_pc, arity} (SM_PUSH_CHUNK).
 * SM_CALL_CHUNK with known entry_pc bakes direct `call .LpcN`. */
void scrip_rt_push_chunk_descr(int64_t entry_pc, int64_t arity);

/* ── EM-6 surface — pattern builder ─────────────────────────────────── */

/* Baked-literal primitives. */
void scrip_rt_pat_lit(const char *s);        /* SM_PAT_LIT   */
void scrip_rt_pat_refname(const char *name); /* SM_PAT_REFNAME */

/* Primitives that pop a charset or integer from the SM value stack. */
void scrip_rt_pat_span(void);                /* SM_PAT_SPAN    */
void scrip_rt_pat_break(void);               /* SM_PAT_BREAK   */
void scrip_rt_pat_any(void);                 /* SM_PAT_ANY     */
void scrip_rt_pat_notany(void);              /* SM_PAT_NOTANY  */
void scrip_rt_pat_len(void);                 /* SM_PAT_LEN     */
void scrip_rt_pat_pos(void);                 /* SM_PAT_POS     */
void scrip_rt_pat_rpos(void);                /* SM_PAT_RPOS    */
void scrip_rt_pat_tab(void);                 /* SM_PAT_TAB     */
void scrip_rt_pat_rtab(void);                /* SM_PAT_RTAB    */

/* Nullary pattern constructors. */
void scrip_rt_pat_arb(void);
void scrip_rt_pat_rem(void);
void scrip_rt_pat_fence(void);
void scrip_rt_pat_fail(void);
void scrip_rt_pat_abort(void);
void scrip_rt_pat_succeed(void);
void scrip_rt_pat_bal(void);
void scrip_rt_pat_eps(void);

/* Combinators (pop from pat-stack). */
void scrip_rt_pat_arbno(void);               /* SM_PAT_ARBNO  (pops inner)  */
void scrip_rt_pat_fence1(void);              /* SM_PAT_FENCE1 (pops child)  */
void scrip_rt_pat_cat(void);                 /* SM_PAT_CAT    (pops r, l)   */
void scrip_rt_pat_alt(void);                 /* SM_PAT_ALT    (pops r, l)   */

/* Deref: pop DESCR_t from vstack, interpret as pattern, push to patstack. */
void scrip_rt_pat_deref(void);               /* SM_PAT_DEREF  */

/* Capture: pops child from patstack.
 * kind: 0=conditional(.), 1=immediate($), 2=cursor(@). */
void scrip_rt_pat_capture(const char *varname, int kind); /* SM_PAT_CAPTURE */

/* Boxval: pop patstack top, push as DT_P onto vstack. */
void scrip_rt_pat_boxval(void);              /* SM_PAT_BOXVAL */

/* Execute one SNOBOL4 statement.
 * subj_name: variable name for write-back (NULL = anonymous).
 * has_repl:  1 if a replacement DESCR_t was pushed onto the SM vstack.
 * Stack on entry (TOS last): [subject_descr] [replacement_or_zero].
 * Pat-stack holds the assembled pattern.
 * Sets last_ok (1=:S, 0=:F); resets pat-stack to empty. */
void scrip_rt_exec_stmt(const char *subj_name, int has_repl);

#ifdef __cplusplus
}
#endif

#endif /* SCRIP_RT_H */
