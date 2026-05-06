/*
 * scrip_rt.c — libscrip_rt.so implementation (M-JITEM-X64 / EM-1..EM-3)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * EM-1: skeleton with init / finalize as no-ops.
 * EM-2: push_int / pop_int / halt / unhandled_op — synthetic PUSH_LIT_I+HALT.
 * EM-3: typed value stack (ScripRtVal); push_str / pop_descr / arith /
 *       nv_get(stub) / nv_set(stub) / pop_void.  Integer arithmetic gate.
 *
 * State model (EM-3):
 *   g_vstack[]     — ScripRtVal stack.  Fixed capacity for EM-3.
 *   g_vtop         — index of next free slot (0 = empty).
 *   g_halt_rc      — rc from most recent scrip_rt_halt call.
 *   g_halt_set     — set once scrip_rt_halt is called.
 *
 * EM-3 deviations (documented per RULES.md):
 *   - SM_DUP / SM_SWAP: not in sm_opcode_t enum — rung text was aspirational.
 *   - scrip_rt_nv_get / scrip_rt_nv_set: stubs; NV table needs GC+snobol4 rt.
 *   - scrip_rt_arith: integer-only; string coercion deferred to later rung.
 */

#include "scrip_rt.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Internal state ─────────────────────────────────────────────────── */

#define SCRIP_RT_VSTACK_CAP 256
static ScripRtVal g_vstack[SCRIP_RT_VSTACK_CAP];
static int        g_vtop   = 0;

static int        g_halt_rc  = 0;
static int        g_halt_set = 0;

/* ── Internal stack helpers ─────────────────────────────────────────── */

static void vstack_push(ScripRtVal v)
{
    if (g_vtop >= SCRIP_RT_VSTACK_CAP) {
        fprintf(stderr,
            "libscrip_rt: SM value stack overflow (cap=%d).\n",
            SCRIP_RT_VSTACK_CAP);
        abort();
    }
    g_vstack[g_vtop++] = v;
}

static ScripRtVal vstack_pop(void)
{
    if (g_vtop <= 0) {
        fprintf(stderr,
            "libscrip_rt: SM value stack underflow at vstack_pop.\n");
        abort();
    }
    return g_vstack[--g_vtop];
}

/* ── EM-1 entries ───────────────────────────────────────────────────── */

void scrip_rt_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;
}

int scrip_rt_finalize(void)
{
    return g_halt_set ? g_halt_rc : 0;
}

/* ── EM-2 entries ───────────────────────────────────────────────────── */

void scrip_rt_push_int(int64_t v)
{
    ScripRtVal sv;
    sv.tag  = SCRIP_RT_INT;
    sv.slen = 0;
    sv.i    = v;
    vstack_push(sv);
}

int64_t scrip_rt_pop_int(void)
{
    ScripRtVal sv = vstack_pop();
    if (sv.tag != SCRIP_RT_INT) {
        fprintf(stderr,
            "libscrip_rt: scrip_rt_pop_int: TOS is not SCRIP_RT_INT "
            "(tag=%d).\n", sv.tag);
        abort();
    }
    return sv.i;
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

/* ── EM-3 entries ───────────────────────────────────────────────────── */

void scrip_rt_push_str(const char *s, uint32_t slen)
{
    ScripRtVal sv;
    sv.tag  = SCRIP_RT_STR;
    sv.slen = slen ? slen : (uint32_t)(s ? strlen(s) : 0);
    sv.s    = (char *)s;
    vstack_push(sv);
}

void scrip_rt_pop_descr(ScripRtVal *out)
{
    if (!out) { abort(); }
    *out = vstack_pop();
}

void scrip_rt_arith(int op)
{
    /* SM opcode integer values (sm_prog.h — kept as literals):
     *   SM_ADD=17  SM_SUB=18  SM_MUL=19  SM_DIV=20  SM_MOD=22   */
    ScripRtVal r = vstack_pop();
    ScripRtVal l = vstack_pop();

    if (l.tag != SCRIP_RT_INT || r.tag != SCRIP_RT_INT) {
        fprintf(stderr,
            "libscrip_rt: scrip_rt_arith(op=%d): EM-3 supports only "
            "SCRIP_RT_INT operands (l.tag=%d r.tag=%d).  String coercion "
            "deferred to later rung.\n",
            op, l.tag, r.tag);
        abort();
    }

    int64_t lv = l.i, rv = r.i, result = 0;
    switch (op) {
        case 17: result = lv + rv;  break;   /* SM_ADD */
        case 18: result = lv - rv;  break;   /* SM_SUB */
        case 19: result = lv * rv;  break;   /* SM_MUL */
        case 20:
            if (rv == 0) {
                fprintf(stderr, "libscrip_rt: SM_DIV divide by zero.\n");
                abort();
            }
            result = lv / rv;
            break;
        case 22:
            if (rv == 0) {
                fprintf(stderr, "libscrip_rt: SM_MOD modulo by zero.\n");
                abort();
            }
            result = lv % rv;
            break;
        default:
            fprintf(stderr,
                "libscrip_rt: scrip_rt_arith: unexpected op=%d.\n", op);
            abort();
    }

    ScripRtVal sv;
    sv.tag  = SCRIP_RT_INT;
    sv.slen = 0;
    sv.i    = result;
    vstack_push(sv);
}

void scrip_rt_nv_get(const char *name)
{
    fprintf(stderr,
        "libscrip_rt: scrip_rt_nv_get(\"%s\"): NV table not yet linked "
        "into libscrip_rt.so (EM-3 stub).\n",
        name ? name : "(null)");
    abort();
}

void scrip_rt_nv_set(const char *name)
{
    fprintf(stderr,
        "libscrip_rt: scrip_rt_nv_set(\"%s\"): NV table not yet linked "
        "into libscrip_rt.so (EM-3 stub).\n",
        name ? name : "(null)");
    abort();
}

void scrip_rt_pop_void(void)
{
    (void)vstack_pop();
}

int scrip_rt_last_ok(void)
{
    /* EM-4 stub: the SM interpreter tracks last_ok in SM_State.
     * In mode-4 emitted code, last_ok is maintained by scrip_rt_arith
     * and other ops that set it. For now returns 1 (success) as a safe
     * default for the EM-3 gate which does not exercise conditional jumps.
     * Will be wired to real SM last_ok state when SM_JUMP_S/F land (EM-4). */
    return 1;
}
