/*
 * sm_prog.h — SM_Instr / SM_Program types (M-SCRIP-U2)
 *
 * The flat instruction array that SM-LOWER produces from IR.
 * Both the interpreter dispatch loop and the in-memory code generator
 * walk this same array — one dispatches in C, the other emits x86 blobs.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Date: 2026-04-06
 */

#ifndef SM_PROG_H
#define SM_PROG_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* ── Opcode enumeration ─────────────────────────────────────────────── */

typedef enum {
    /* Control */
    SM_LABEL = 0,
    SM_JUMP,
    SM_JUMP_S,
    SM_JUMP_F,
    SM_HALT,
    SM_STNO,       /* increment &STCOUNT/&STNO at each statement boundary */

    /* Values */
    SM_PUSH_LIT_S,
    SM_PUSH_LIT_I,
    SM_PUSH_LIT_F,
    SM_PUSH_NULL,
    SM_PUSH_NULL_NOFLIP, /* push null but do NOT clobber last_ok — for E_SCAN value-balance */
    SM_PUSH_VAR,
    SM_PUSH_EXPR,    /* push DT_E frozen expression; a[0].ptr = EXPR_t* */
    SM_PUSH_CHUNK,   /* push DT_E chunk descriptor; a[0].i=entry_pc, a[1].i=arity */
    SM_CALL_CHUNK,   /* pop chunk descriptor, push return frame, jump to entry_pc */
    SM_STORE_VAR,
    SM_POP,

    /* Arithmetic / String */
    SM_ADD,
    SM_SUB,
    SM_MUL,
    SM_DIV,
    SM_EXP,
    SM_MOD,   /* OC-1 RS-6: integer/real modulo — all languages use this */
    SM_CONCAT,
    SM_COERCE_NUM, /* unary +: coerce top of stack to int or real */
    SM_NEG,

    /* Pattern construction */
    SM_PAT_LIT,
    SM_PAT_ANY,
    SM_PAT_NOTANY,
    SM_PAT_SPAN,
    SM_PAT_BREAK,
    SM_PAT_LEN,
    SM_PAT_POS,
    SM_PAT_RPOS,
    SM_PAT_TAB,
    SM_PAT_RTAB,
    SM_PAT_ARB,
    SM_PAT_ARBNO,    /* pop inner pat, push ARBNO(inner) */
    SM_PAT_REM,
    SM_PAT_BAL,
    SM_PAT_FENCE,
    SM_PAT_FENCE1,  /* pop child pat, push pat_fence_p(child) — FENCE(P) */
    SM_PAT_ABORT,
    SM_PAT_FAIL,
    SM_PAT_SUCCEED,
    SM_PAT_EPS,         /* push epsilon pattern onto pat-stack */
    SM_PAT_ALT,
    SM_PAT_CAT,
    SM_PAT_DEREF,
    SM_PAT_REFNAME,     /* *var in pattern context — a[0].s = var name; push pat_ref(name)
                         * onto pat-stack.  Unlike SM_PUSH_VAR + SM_PAT_DEREF which fetches
                         * the variable's CURRENT value at pattern-build time (wrong for
                         * self-recursive patterns like primary = ... | '(' *primary ')'),
                         * this opcode preserves the name and defers lookup to match time
                         * via XDSAR / bb_deferred_var.  Mirrors the --ir-run pat_ref(name)
                         * path in interp_eval_pat's E_DEFER(E_VAR) case.  SN-6 fix. */
    SM_PAT_CAPTURE,
    SM_PAT_CAPTURE_FN,  /* . *func() — a[0].s=funcname; calls func(matched_text) at match time */
    SM_PAT_CAPTURE_FN_ARGS, /* . *func(args) / $ *func(args) — args-are-values form.
                         * a[0].s = funcname, a[1].i = kind (0=cond, 1=imm), a[2].i = nargs.
                         * The nargs values were pushed onto the value stack by preceding
                         * lower_expr calls; handler pops them (last-pushed = last arg) and
                         * calls pat_assign_callcap(child, fname, values, nargs).  SN-8a.
                         * Emitted when any arg is not a plain E_VAR (E_QLIT literal, nested
                         * expression, etc.) — the TL-2 name-stash path handles the all-E_VAR
                         * case in SM_PAT_CAPTURE_FN. */
    SM_PAT_USERCALL,    /* bare *func() — a[0].s=funcname; a[2].s = '\t'-separated arg names (or NULL)
                         * Builds XATP deferred-usercall pattern via pat_user_call; at match time
                         * the engine invokes func() per position and the call's FAIL propagates
                         * as pattern FAIL.  SN-17a. */
    SM_PAT_USERCALL_ARGS, /* bare *func(args) — args-are-values form.
                         * a[0].s = funcname, a[1].i = nargs.  The nargs values were pushed
                         * onto the value stack; handler pops them and calls
                         * pat_user_call(fname, values, nargs).  SN-8a.
                         * Emitted when any arg is not a plain E_VAR. */

    /* Statement execution */
    SM_EXEC_STMT,

    /* Byrd box broker modes — U-16
     * SM_BB_PUMP: pops bb_node_t* from value stack; calls bb_broker(root,BB_PUMP,body_fn,arg);
     *             pushes tick count as DT_I.  For Icon 'every' / generator loops.
     * SM_BB_ONCE: pops bb_node_t* from value stack; calls bb_broker(root,BB_ONCE,NULL,NULL);
     *             sets st->last_ok.  For Prolog goal dispatch.
     * Note: BB_SCAN is already wired via SM_EXEC_STMT → exec_stmt → bb_broker(BB_SCAN). */
    SM_BB_PUMP,
    SM_BB_ONCE,
    /* CHUNKS-step12: BB pump for an Icon user-proc identified by name + nargs.
     * Replaces the synthesised E_FNC + emit_push_expr + SM_BB_PUMP wrapper
     * that sm_lower used to emit for the top-level call_main(). a[0].s = proc
     * name, a[1].i = nargs. Args (when nargs>0) are popped from the value
     * stack in caller order and passed straight to the coroutine staging
     * machinery — no EXPR_t* is constructed at lowering time and none is
     * walked by this opcode. The IR walk that remains lives entirely inside
     * coro_call(proc_table[i].proc, ...) and is Step 17's territory. */
    SM_BB_PUMP_PROC,

    /* Functions */
    SM_CALL,
    SM_RETURN,
    SM_FRETURN,
    SM_NRETURN,
    SM_RETURN_S,    /* :S(RETURN)  — return normal value only if last_ok */
    SM_RETURN_F,    /* :F(RETURN)  — return normal value only if !last_ok */
    SM_FRETURN_S,   /* :S(FRETURN) — return FAIL only if last_ok */
    SM_FRETURN_F,   /* :F(FRETURN) — return FAIL only if !last_ok */
    SM_NRETURN_S,   /* :S(NRETURN) — return NAME only if last_ok */
    SM_NRETURN_F,   /* :F(NRETURN) — return NAME only if !last_ok */
    SM_DEFINE,

    /* Type dispatch / indirect */
    SM_JUMP_INDIR,
    SM_SELBRA,

    /* State save/restore */
    SM_STATE_PUSH,
    SM_STATE_POP,

    /* Integer arithmetic */
    SM_INCR,
    SM_DECR,
    SM_LCOMP,
    SM_RCOMP,
    SM_TRIM,
    SM_ACOMP,
    SM_SPCINT,
    SM_SPREAL,

    SM_PAT_BOXVAL,  /* pop pat-stack top → push as DT_P onto value-stack */

    SM_OPCODE_COUNT
} sm_opcode_t;

/* ── Instruction operand union ──────────────────────────────────────── */

typedef union {
    int64_t     i;          /* integer literal / label index / nargs */
    double      f;          /* float literal */
    const char *s;          /* string literal / variable name / label name */
    int         b;          /* boolean (has_repl etc.) */
    void       *ptr;        /* frozen pointer (EXPR_t* for SM_PUSH_EXPR, etc.) */
} sm_operand_t;

/* ── Compiled chunk descriptor ──────────────────────────────────────── */
/* Replaces raw EXPR_t* in DT_E descriptors once a site is migrated away
 * from SM_PUSH_EXPR.  entry_pc indexes SM_Program::instrs[]; arity is the
 * number of args expected on the SM value stack at entry (0 for a thunk).
 * SM_PUSH_CHUNK encodes these as a[0].i and a[1].i respectively. */
typedef struct {
    int entry_pc;   /* index into SM_Program::instrs[] where chunk starts */
    int arity;      /* args on SM value stack at entry; 0 = thunk */
} SmChunk_t;



#define SM_MAX_OPERANDS 3

typedef struct {
    sm_opcode_t   op;
    sm_operand_t  a[SM_MAX_OPERANDS];   /* a[0], a[1], a[2] */
} SM_Instr;

/* ── CODE_t (flat array of instructions) ───────────────────────────── */

typedef struct {
    SM_Instr    *instrs;
    int          count;
    int          cap;
    /* IM-9: per-statement source label (1-based; stno_labels[0] unused).
     * stno_labels[n] = source label of statement n, or NULL if unlabelled.
     * Populated by sm_lower(); strings are interned (not owned). */
    const char **stno_labels;
    int          stno_labels_cap;   /* allocated slots (indices 0..cap-1) */
    int          stno_count;        /* number of statements lowered */
} SM_Program;

/* ── Builder helpers ────────────────────────────────────────────────── */

SM_Program *sm_prog_new(void);
void        sm_prog_free(SM_Program *p);

/* Append one instruction; returns its index */
int sm_emit(SM_Program *p, sm_opcode_t op);
int sm_emit_s(SM_Program *p, sm_opcode_t op, const char *s);
int sm_emit_i(SM_Program *p, sm_opcode_t op, int64_t i);
int sm_emit_f(SM_Program *p, sm_opcode_t op, double f);
int sm_emit_ptr(SM_Program *p, sm_opcode_t op, void *ptr);
int sm_emit_si(SM_Program *p, sm_opcode_t op, const char *s, int64_t i);
int sm_emit_ii(SM_Program *p, sm_opcode_t op, int64_t i0, int64_t i1);

/* Label: emit SM_LABEL with index=next_instr; return the label index */
int sm_label(SM_Program *p);

/* Label with name: emit SM_LABEL and store label name in a[0].s for runtime lookup.
 * Returns the label index (same as sm_label). RS-9a: needed for SM call frames. */
int sm_label_named(SM_Program *p, const char *name);

/* RS-9b: global pointer to the active SM_Program, set by scrip.c after sm_lower().
 * Allows _usercall_hook to detect SM-bodied functions without the IR tree. */
extern SM_Program *g_current_sm_prog;

/* Look up the PC (SM_LABEL instr index) for a named label. Returns -1 if not found.
 * RS-9a: used by SM_CALL to jump to user-defined function bodies. */
int sm_label_pc_lookup(const SM_Program *p, const char *name);

/* Patch a jump target: set a[0].i of instr at `jump_idx` to `target_label` */
void sm_patch_jump(SM_Program *p, int jump_idx, int target_label);

/* IM-9: record source label string for statement stno (1-based); NULL = unlabelled */
void sm_stno_label_record(SM_Program *p, int stno, const char *label);
void sm_prog_print(const SM_Program *p, FILE *out);

/* Opcode name for diagnostics */
const char *sm_opcode_name(sm_opcode_t op);

#endif /* SM_PROG_H */
