/*
 * scrip_rt.h — public ABI for libscrip_rt.so (M-JITEM-X64 / EM-1..EM-3)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * libscrip_rt.so is the runtime support library that mode-4-emitted
 * binaries link against. It carries language-level semantics — pattern
 * matcher, NV table, builtins, GC, generator BB pump (post-Step 19),
 * Prolog backtracking machinery (post-Step 19) — so that emitted
 * executables contain only compiled SM chunks plus calls into a stable
 * C ABI defined here.
 *
 * EM-1 surface:
 *   scrip_rt_init      — process startup; receives argc/argv
 *   scrip_rt_finalize  — process shutdown; returns the program rc
 *
 * EM-2 surface:
 *   scrip_rt_push_int  — push a 64-bit int onto the SM value stack
 *   scrip_rt_pop_int   — pop the top of the SM value stack as int
 *   scrip_rt_halt      — record the program's exit code
 *   scrip_rt_unhandled_op — runtime trap for unimplemented opcodes
 *
 * EM-3 surface (this commit):
 *   scrip_rt_push_str  — push a string (DT_S) value onto the SM stack
 *   scrip_rt_pop_descr — pop a ScripRtVal (typed descriptor) from the stack
 *   scrip_rt_arith     — binary integer arithmetic (ADD/SUB/MUL/DIV/MOD)
 *   scrip_rt_nv_get    — load a named variable onto the stack  [stub]
 *   scrip_rt_nv_set    — store TOS into a named variable       [stub]
 *   scrip_rt_pop_void  — pop and discard TOS (SM_POP)
 *
 * EM-5 surface:
 *   scrip_rt_push_chunk_descr — push a DT_E chunk descriptor (entry_pc,
 *                               arity).  SM_CALL_CHUNK and SM_RETURN
 *                               are baked direct in the emitter (call
 *                               .LpcN / ret) and do not need ABI symbols.
 *
 * SM_DUP / SM_SWAP — not in the sm_opcode_t enum (the rung text was
 * aspirational).  EM-3 covers the opcodes that actually exist.
 *
 * Stability: every symbol exported here is part of the mode-4 ABI.
 * Once an emitted binary references a symbol, the signature must not
 * change. Additions are backward-compatible by definition.
 *
 * EM-2 deviation note: scrip_rt_pop_int was originally planned for
 * EM-3.  It was pulled forward because SM_HALT's codegen needs it.
 * See closed-rung entry in GOAL-MODE4-EMIT.md.
 *
 * EM-3 deviation note: scrip_rt_nv_get / scrip_rt_nv_set are stubs
 * that abort at runtime.  Full NV table integration requires the
 * snobol4 runtime (GC, name folding, keyword dispatch) which cannot
 * be linked into libscrip_rt.so without significant layering work.
 * The EM-3 gate (integer arithmetic only) does not exercise these
 * symbols.  They are declared here to make the ABI stable; the stubs
 * will be replaced in a later rung without changing the signature.
 */

#ifndef SCRIP_RT_H
#define SCRIP_RT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── ScripRtVal — typed value descriptor ─────────────────────────────
 *
 * Mirrors the layout of DESCR_t (descr.h) so that the SM value stack
 * inside libscrip_rt.so carries typed values.  The tag values are kept
 * identical to DTYPE_t so the two types are bit-compatible.
 *
 * Only the types used through EM-3 are named here.  The full DTYPE_t
 * enumeration (DT_DATA, DT_NAME, etc.) is in snobol4.h / descr.h; the
 * correspondence is maintained by construction.
 */
typedef enum {
    SCRIP_RT_SNUL = 0,   /* null / unset              */
    SCRIP_RT_STR  = 1,   /* string — char* in .s      */
    SCRIP_RT_INT  = 6,   /* integer — int64_t in .i   */
    SCRIP_RT_CHUNK = 11, /* chunk descriptor (DT_E)   */
    SCRIP_RT_FAIL = 99   /* failure sentinel           */
} ScripRtTag;

typedef struct {
    int32_t  tag;    /* ScripRtTag (int32 to match DESCR_t padding)  */
    uint32_t slen;   /* string byte length (0 for non-string)         */
    union {
        char    *s;  /* SCRIP_RT_STR  */
        int64_t  i;  /* SCRIP_RT_INT  */
        double   f;  /* future float  */
    };
} ScripRtVal;

/* ── EM-1 surface ────────────────────────────────────────────────────── */

void scrip_rt_init(int argc, char **argv);
int  scrip_rt_finalize(void);

/* ── EM-2 surface ────────────────────────────────────────────────────── */

/*
 * scrip_rt_push_int — push a DT_I integer onto the SM value stack.
 * Backward-compatible with EM-2; the stack now holds ScripRtVal entries
 * but the int ABI is unchanged.
 */
void    scrip_rt_push_int(int64_t v);

/*
 * scrip_rt_pop_int — pop TOS as a signed 64-bit integer.
 * Backward-compatible with EM-2.  If TOS is not DT_I, aborts.
 */
int64_t scrip_rt_pop_int(void);

void scrip_rt_halt(int rc);
void scrip_rt_unhandled_op(int op);

/* ── EM-3 surface ────────────────────────────────────────────────────── */

/*
 * scrip_rt_push_str — push a DT_S string onto the SM value stack.
 * `s` is a C string pointer (lifetime managed by the caller / GC in
 * later rungs).  `slen` is the byte length; pass 0 to use strlen.
 */
void scrip_rt_push_str(const char *s, uint32_t slen);

/*
 * scrip_rt_pop_descr — pop TOS into the caller-supplied ScripRtVal.
 * Used by SM_STORE_VAR codegen to inspect the type before storing.
 */
void scrip_rt_pop_descr(ScripRtVal *out);

/*
 * scrip_rt_arith — binary arithmetic: pops two ScripRtVals (r then l),
 * computes l <op> r, pushes the result.
 *
 * `op` is the sm_opcode_t integer value:
 *   SM_ADD=17, SM_SUB=18, SM_MUL=19, SM_DIV=20, SM_MOD=22
 * (values from sm_prog.h — kept as ints to avoid the header dependency).
 *
 * EM-3 scope: integer operands only.  Both operands must be
 * SCRIP_RT_INT; any other type combination aborts with a diagnostic
 * (the gate program uses only literals).  Full string→number coercion
 * is deferred to the rung that lands SM_PUSH_VAR/SM_STORE_VAR with a
 * live NV table.
 *
 * Divide-by-zero aborts via abort(3).
 */
void scrip_rt_arith(int op);

/*
 * scrip_rt_nv_get — push the value of the named variable onto the
 * SM value stack.  STUB in EM-3: aborts with a clear message.
 * Signature is stable; the stub will be replaced when the NV table
 * is linked into libscrip_rt.so.
 */
void scrip_rt_nv_get(const char *name);

/*
 * scrip_rt_nv_set — pop TOS and store it in the named variable.
 * STUB in EM-3: aborts with a clear message.
 */
void scrip_rt_nv_set(const char *name);

/*
 * scrip_rt_pop_void — pop and discard TOS (SM_POP codegen).
 */
void scrip_rt_pop_void(void);

/*
 * scrip_rt_last_ok -- return 1 if the most recent SM operation succeeded,
 * 0 if it failed. Used by SM_JUMP_S and SM_JUMP_F codegen (EM-4).
 * Declared here for ABI stability; exercised starting EM-4.
 */
int scrip_rt_last_ok(void);

/*
 * scrip_rt_set_last_ok -- set the last_ok flag (1 = success, 0 = failure).
 * Used by emitted code and by future runtime ops (pattern matcher, etc.)
 * that have a notion of success/failure.  EM-4 ABI addition.
 */
void scrip_rt_set_last_ok(int ok);

/* ── EM-5 surface ────────────────────────────────────────────────────── */

/*
 * scrip_rt_push_chunk_descr -- push a DT_E chunk descriptor onto the
 * SM value stack.  Used by SM_PUSH_CHUNK codegen.  The descriptor
 * carries entry_pc (in .i) and arity (in slen) so that downstream
 * code (sm_call_chunk via dispatch, EVAL builtins) can resolve it.
 *
 * For SM_CALL_CHUNK with a compile-time-known entry_pc, the emitter
 * generates a baked direct `call .LpcN` and does NOT push a descriptor
 * first -- the descriptor path is for cases where the chunk identity
 * is computed at runtime (e.g., a pattern's *expr cursor stored in NV).
 */
void scrip_rt_push_chunk_descr(int64_t entry_pc, int64_t arity);

#ifdef __cplusplus
}
#endif

#endif /* SCRIP_RT_H */
