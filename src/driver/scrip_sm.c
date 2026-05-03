/*
 * scrip_sm.c — shared SM-mode preamble + run-with-recovery helpers (RS-14).
 *
 * Implementations of sm_preamble() and sm_run_with_recovery() — see
 * scrip_sm.h for design rationale.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.7
 * Date: 2026-05-03
 */

#include <stdio.h>
#include <setjmp.h>
#include "scrip_sm.h"
#include "../runtime/x86/sm_lower.h"
#include "../runtime/common/ir_clone.h"
#include "interp_private.h"   /* label_table_build, prescan_defines, label_table_clear_stmts */

extern int g_sno_err_active;
extern jmp_buf g_sno_err_jmp;

SM_Program *sm_preamble(void *prog_void)
{
    CODE_t *prog = (CODE_t *)prog_void;
    label_table_build(prog);
    prescan_defines(prog);
    g_sno_err_active = 1;   /* arm so sno_runtime_error longjmps safely */

    SM_Program *sm = sm_lower(prog);
    if (!sm) {
        fprintf(stderr, "scrip: sm_lower failed\n");
        return NULL;
    }

    /* RS-9b: IR tree no longer needed — SM_Program is self-contained.
     * SM_PUSH_EXPR pointers were cloned to GC memory by emit_push_expr.
     * RS-9c: g_current_sm_prog must be set so _usercall_hook detects SM bodies.
     * Clear label_table stmt pointers so any residual label_lookup
     * calls return NULL (no dangling STMT_t* dereference). */
    g_current_sm_prog = sm;
    code_free(prog);
    label_table_clear_stmts();
    return sm;
}

void sm_run_with_recovery(SM_Program *sm, sm_runner_fn runner)
{
    SM_State st;
    sm_state_init(&st);

    /* Arm g_sno_err_jmp: sno_runtime_error longjmps here on error.
     * We treat each error as statement failure: mark last_ok=0, advance pc,
     * and re-enter the runner.  Mirrors execute_program's per-stmt setjmp
     * pattern and prevents longjmp into an uninitialized jmp_buf. */
    int hybrid_err;
    while (1) {
        hybrid_err = setjmp(g_sno_err_jmp);
        if (hybrid_err != 0) {
            /* runtime error fired mid-statement: mark fail, advance past
             * the current instruction and continue */
            st.last_ok = 0;
            st.sp = 0;  /* reset value stack — state is undefined after error */
            if (st.pc < sm->count) st.pc++;  /* skip offending instruction */
            /* drain to next SM_STNO boundary so we resume cleanly */
            while (st.pc < sm->count &&
                   sm->instrs[st.pc].op != SM_STNO &&
                   sm->instrs[st.pc].op != SM_HALT)
                st.pc++;
        }
        int rc = runner(sm, &st);
        if (rc == 0 || rc < -1) break;  /* halted or fatal */
        if (st.pc >= sm->count) break;
    }
}
