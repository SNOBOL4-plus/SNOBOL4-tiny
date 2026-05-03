/*============================================================================================================================
 * coro_stmt.c — RS-17b: pure-BB statement-context evaluator for Icon Byrd boxes.
 *
 * See coro_stmt.h for the contract and migration plan.
 *
 * Today this is a thin trampoline that delegates every kind to `interp_eval`,
 * exactly mirroring the RS-17a coro_value.c starting point.  The 13
 * statement-context call sites in coro_runtime.c that previously called
 * `interp_eval(...)` directly now route through `bb_exec_stmt(...)`.  This
 * gives us a single chokepoint where Icon statement-level kinds arrive,
 * enabling their incremental migration into a real switch in subsequent
 * sub-rungs (RS-17b-cont).  When that switch handles every kind that
 * actually reaches this entry point, the fallthrough goes away, the extern
 * declaration with it, and (paired with RS-18 for pl_runtime.c) RS-19 can
 * promote coro_runtime.c into the isolation grep gate.
 *
 * Note on return value: `interp_eval` returns DESCR_t, but every one of the
 * 13 call sites in coro_runtime.c either discarded the return value or
 * reassigned to a result variable that was overwritten before use.  The
 * statement-context contract is therefore void.  Discarding the IR-walker
 * return value here is correct.
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
 * SPRINT:  RS-17b (2026-05-03)
 *==========================================================================================================================*/

#include "coro_stmt.h"
#include "snobol4.h"

/* interp_eval is the IR-mode tree-walker; bb_exec_stmt falls through here for
 * all kinds today.  RS-17b-cont rungs replace each fallthrough kind with an
 * in-this-file branch until this declaration goes away (and the isolation
 * gate can promote coro_runtime.c via RS-19). */
extern DESCR_t interp_eval(EXPR_t *e);

/*------------------------------------------------------------------------------------------------------------------------------
 * bb_exec_stmt — execute e in statement context.
 *
 * Statement context means: side effects only.  No caller-visible return
 * value — the 13 sites in coro_runtime.c either discard the DESCR_t result
 * or overwrite it before the function returns.  Control flow propagates
 * through IcnFrame state which is observed by the caller after this
 * returns.
 *
 * Today: pure passthrough to interp_eval.  RS-17b-cont sub-rungs will lift
 * Icon statement-level kinds (E_BLOCK, E_WHILE, E_REPEAT, E_UNTIL, E_IF,
 * E_EVERY, E_RETURN, E_PROC_FAIL, E_LOOP_BREAK, E_LOOP_NEXT, E_ASSIGN,
 * E_FNC builtins called for side effect, etc.) into an explicit dispatch.
 *----------------------------------------------------------------------------------------------------------------------------*/
void bb_exec_stmt(EXPR_t *e)
{
    if (!e) return;

    /* Fallthrough: RS-17b-cont rungs migrate kinds into a switch above this
     * call, replacing site after site. */
    (void)interp_eval(e);
}
