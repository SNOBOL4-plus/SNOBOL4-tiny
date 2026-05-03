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
#include "polyglot.h"         /* RS-26: polyglot_lang_mask, polyglot_init, LANG_SNO */

extern int g_sno_err_active;
extern jmp_buf g_sno_err_jmp;

SM_Program *sm_preamble(void *prog_void)
{
    CODE_t *prog = (CODE_t *)prog_void;
    label_table_build(prog);
    prescan_defines(prog);
    g_sno_err_active = 1;   /* arm so sno_runtime_error longjmps safely */

    /* RS-26: symmetric preamble — populate proc_table (Icon/Raku) and
     * g_pl_pred_table (Prolog) from the live IR.  For pure-SNO programs the
     * lang_mask is just (1<<LANG_SNO) and the per-language init branches
     * inside polyglot_init are guarded — adds no observable behaviour for
     * SNOBOL4.  For Icon/Prolog this is what makes coro_call /
     * pl_pred_table_lookup find their targets when SM_BB_PUMP / SM_BB_ONCE
     * fires inside sm_interp_run. */
    uint32_t lang_mask = polyglot_lang_mask(prog);
    polyglot_init(prog, lang_mask);

    SM_Program *sm = sm_lower(prog);
    if (!sm) {
        fprintf(stderr, "scrip: sm_lower failed\n");
        return NULL;
    }

    /* RS-9b: SM_Program is self-contained for SNOBOL4 — emit_push_expr
     * GC-clones the EXPR_t subtrees so SM owns them.  Free the IR.
     *
     * RS-26: but for non-SNO frontends, BB drives the live IR — the proc/
     * pred tables populated above hold EXPR_t* into prog that survive only
     * if the IR survives.  Gate code_free on lang_mask: pure-SNO programs
     * still get the RS-9b behaviour; mixed or non-SNO programs keep the IR
     * alive for the duration of execution.
     *
     * RS-9c: g_current_sm_prog must be set so _usercall_hook detects SM
     * bodies, regardless of whether IR is freed. */
    g_current_sm_prog = sm;
    if (lang_mask == (1u << LANG_SNO)) {
        code_free(prog);
        label_table_clear_stmts();
    }
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
