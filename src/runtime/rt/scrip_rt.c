/*
 * scrip_rt.c — libscrip_rt.so implementation (M-JITEM-X64 / EM-1+EM-2)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * EM-1: skeleton with init / finalize as no-ops.
 * EM-2: push_int / pop_int / halt / unhandled_op land — enough to
 *       run a synthetic SM_PUSH_LIT_I + SM_HALT program and observe
 *       its rc surface through the OS exit code.
 *
 * State model:
 *   g_value_stack[] — fixed-capacity int64 stack.  Capacity tuned for
 *     EM-2 synthetic programs; EM-3 will replace with a growing buffer
 *     and add typed entries (DESCR_t-equivalents).
 *   g_value_top    — index of the next free slot (0 = empty).
 *   g_halt_rc      — the rc most recently recorded by scrip_rt_halt;
 *                    surfaced by scrip_rt_finalize.
 *   g_halt_set     — whether scrip_rt_halt was ever called this run;
 *                    finalize returns 0 if not.
 *
 * This file is compiled with -fPIC and linked into libscrip_rt.so.
 * Only the symbols declared in scrip_rt.h are part of the public ABI;
 * everything else inside the .so is implementation detail.
 */

#include "scrip_rt.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ── Internal state ─────────────────────────────────────────────────── */

#define SCRIP_RT_VSTACK_CAP 256
static int64_t g_value_stack[SCRIP_RT_VSTACK_CAP];
static int     g_value_top = 0;

static int     g_halt_rc   = 0;
static int     g_halt_set  = 0;

/* ── EM-1 entries ───────────────────────────────────────────────────── */

void scrip_rt_init(int argc, char **argv)
{
    /* EM-1: no-op. Receiver of argc/argv to keep the ABI stable
     * across rungs that need them (later: &INPUT, &OUTPUT, args). */
    (void)argc;
    (void)argv;
}

int scrip_rt_finalize(void)
{
    /* EM-2: return the halt-recorded rc, else 0. */
    return g_halt_set ? g_halt_rc : 0;
}

/* ── EM-2 entries ───────────────────────────────────────────────────── */

void scrip_rt_push_int(int64_t v)
{
    if (g_value_top >= SCRIP_RT_VSTACK_CAP) {
        fprintf(stderr,
            "libscrip_rt: SM value stack overflow at scrip_rt_push_int "
            "(cap=%d).  EM-2 fixed capacity; EM-3 may grow.\n",
            SCRIP_RT_VSTACK_CAP);
        abort();
    }
    g_value_stack[g_value_top++] = v;
}

int64_t scrip_rt_pop_int(void)
{
    if (g_value_top <= 0) {
        fprintf(stderr,
            "libscrip_rt: SM value stack underflow at scrip_rt_pop_int.\n");
        abort();
    }
    return g_value_stack[--g_value_top];
}

void scrip_rt_halt(int rc)
{
    g_halt_rc  = rc;
    g_halt_set = 1;
}

void scrip_rt_unhandled_op(int op)
{
    fprintf(stderr,
        "libscrip_rt: emitted code reached an opcode the EM-2 emitter "
        "does not yet bake (sm_opcode=%d).  Cross-reference scrip "
        "--dump-sm output for the SM_Program; subsequent EM-N rungs "
        "shrink the unhandled set monotonically.\n",
        op);
    abort();
}
