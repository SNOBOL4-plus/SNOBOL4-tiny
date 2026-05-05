/*
 * sm_interp.h — SM_Program C interpreter dispatch loop (M-SCRIP-U2)
 *
 * Interprets a SM_Program directly in C — no x86 emission.
 * This is the Mode I execution engine and the correctness reference
 * for Mode G (in-memory codegen).
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Date: 2026-04-06
 */

#ifndef SM_INTERP_H
#define SM_INTERP_H

#include <stdlib.h>
#include <setjmp.h>
#include "sm_prog.h"
#include "snobol4.h"

/* ── RS-9a: SM call frame ────────────────────────────────────────────────
 * One frame per active user-defined function call in --sm-run mode.
 * Mirrors CallFrame in interp_call.c but references SM PCs not IR nodes. */
#define SM_CALL_STACK_MAX 256
#define SM_SAVED_NV_MAX    64

typedef struct {
    int         ret_pc;               /* PC to resume after RETURN */
    int         ret_ok;               /* 1=S branch on return, 0=F branch (FRETURN) */
    char       *retval_name;          /* NV slot the body writes its return value into */
    int         nsaved;               /* number of NV vars saved */
    char       *saved_names[SM_SAVED_NV_MAX];
    DESCR_t     saved_vals [SM_SAVED_NV_MAX];
    int         ret_jump_s_pc;        /* caller's :S label PC (-1 = fall through) */
    int         ret_jump_f_pc;        /* caller's :F label PC (-1 = fall through) */
    /* RS-9c: caller value-stack snapshot (survives SM_STNO resets inside callee) */
    int         caller_sp;            /* sp before nargs were popped */
    DESCR_t    *caller_stack;         /* GC'd copy of caller's stack[0..caller_sp-1] */
} SmCallFrame;

/* Interpreter state */
typedef struct {
    DESCR_t  *stack;       /* dynamic value stack (realloc-grown) */
    int       sp;          /* stack pointer: stack[0..sp-1] are live */
    int       stack_cap;   /* current allocated capacity */
    int       last_ok;     /* 1 = last operation succeeded, 0 = failed */
    int       pc;          /* program counter: index into SM_Program */
    jmp_buf   err_jmp;     /* per-statement error recovery (SM_STNO arms it) */
    int       err_fail_pc; /* pc to jump to on runtime error (-1 = halt) */
    int       err_armed;   /* 1 if err_jmp is live */
    /* RS-9a: SM-native call stack */
    SmCallFrame call_stack[SM_CALL_STACK_MAX];
    int         call_depth;
} SM_State;

/*
 * Execute prog from instruction 0 until SM_HALT or end.
 * Returns 0 on normal halt, -1 on error.
 * Uses the live SNOBOL4 runtime (NV_GET_fn / NV_SET_fn etc.) —
 * call only after SNO_INIT_fn() has run.
 */
int sm_interp_run(SM_Program *prog, SM_State *st);
int sm_interp_run_steps(SM_Program *prog, SM_State *st, int n);  /* IM-4 */

/* IM-4: SM step-limit globals */
extern int     g_sm_step_limit;
extern int     g_sm_steps_done;
extern jmp_buf g_sm_step_jmp;

/* Initialise a fresh SM_State (stack empty, pc=0, last_ok=1) */
void sm_state_init(SM_State *st);

/* Push / pop helpers (used by sm_interp_run and tests) */
void sm_push(SM_State *st, DESCR_t d);
DESCR_t sm_pop(SM_State *st);
DESCR_t sm_peek(SM_State *st);

/* CHUNKS-step02: run a compiled chunk thunk, return its result */
DESCR_t sm_call_chunk(int entry_pc);

#endif /* SM_INTERP_H */
