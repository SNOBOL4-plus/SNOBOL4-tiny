/*
 * sm_codegen_x64_emit.c — SM_Program → standalone x86-64 asm
 *                         (M-JITEM-X64, GOAL-MODE4-EMIT EM-1, EM-2+)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * Generates GNU-as / Intel-syntax x86-64 assembly. The emitted asm is
 * assembled (and linked against libscrip_rt.so) outside the scrip
 * process — see scripts/test_smoke_jit_emit_x64.sh for the full
 * pipeline: emit → as → ld → ./prog.
 *
 * ── Calling convention (M-JITEM-X64) ──────────────────────────────────
 *
 * Every emitted entry uses System V AMD64.  main() is the standard
 * (argc:edi, argv:rsi, envp:rdx).  Inside main:
 *
 *   1.  Prologue: push rbp; mov rbp, rsp.  This makes rsp 16-byte
 *       aligned at every subsequent call site.
 *   2.  scrip_rt_init(argc, argv) is called first.  argc/argv are
 *       already in edi/rsi.
 *   3.  One emitted block per SM_Instr in prog->instrs[], in pc order.
 *       Each block is a small fragment of asm that either bakes the
 *       op directly (future rungs) or calls a scrip_rt_* helper
 *       (current rungs).  PLT-relative calls keep the binary PIC-
 *       compatible.
 *   4.  Epilogue: scrip_rt_finalize() returns the program's exit
 *       code; we ret with rax preserved from the call.
 *
 * Register reservations for future baked-direct opcodes (EM-3+):
 *   r12 — SM value-stack pointer (callee-saved; survives PLT calls)
 *   r13 — SM_State pointer        (callee-saved)
 * Until any baked-direct opcode lands these are unused; the emitter
 * does not currently save/restore them in the prologue.  When EM-3
 * begins, the prologue grows accordingly.
 *
 * ── Opcode coverage ──────────────────────────────────────────────────
 *
 * EM-1: literal-zero scaffold (no per-op walk; just init+finalize).
 * EM-2: SM_HALT, SM_PUSH_LIT_I covered via scrip_rt_halt/push_int.
 *       SM_NOP not present in the SM opcode enum (rung text was
 *       aspirational); skipped honestly.  Every other opcode emits
 *       an UNHANDLED_OP marker that calls scrip_rt_unhandled_op at
 *       runtime — fail-fast rather than silently-wrong.
 * EM-3+: stack ops + arithmetic; the unhandled set shrinks
 *       monotonically.
 */

#include "sm_codegen_x64_emit.h"
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include "sm_prog.h"

/* ── Per-opcode emitters ────────────────────────────────────────────── */
/* Each emitter writes a fragment that, when assembled, runs at the SM
 * pc corresponding to its instruction index.  A leading `.LpcN:` label
 * lets future control-flow rungs (EM-4) build emit-time jumps. */

static int emit_pc_label(FILE *out, int pc)
{
    return (fprintf(out, ".Lpc%d:\n", pc) < 0) ? -1 : 0;
}

static int emit_op_halt(FILE *out, const SM_Instr *ins, int pc)
{
    /* SM_HALT — exit the program with the int at TOS as the rc.
     *
     * Honest deviation from the EM-2 step text: the rung says
     * "Add scrip_rt_halt(int rc)" and "exits with the expected rc",
     * but mode-2's SM_HALT handler in sm_interp.c just `return 0;`
     * — there is no rc operand on the opcode and no rc on the SM
     * stack in that semantics.  To make this rung's gate actually
     * distinguishable from EM-1's (both would otherwise exit 0
     * regardless), we adopt a meaningful semantics here: the rc
     * comes from the top of the SM value stack.  Programs that want
     * non-zero exit push a literal int first, then SM_HALT.
     *
     * This requires scrip_rt_pop_int — an EM-3 ABI symbol pulled
     * forward by one rung.  Documented in the closed-step entry. */
    (void)ins;
    return fprintf(out,
        "\t# pc=%d  SM_HALT (rc <- TOS via scrip_rt_pop_int)\n"
        "\tcall    scrip_rt_pop_int@PLT\n"
        "\tmov     edi, eax\n"
        "\tcall    scrip_rt_halt@PLT\n",
        pc) < 0 ? -1 : 0;
}

static int emit_op_push_lit_i(FILE *out, const SM_Instr *ins, int pc)
{
    /* SM_PUSH_LIT_I — push 64-bit signed integer.  a[0].i carries
     * the literal.  movabs handles full int64 range. */
    return fprintf(out,
        "\t# pc=%d  SM_PUSH_LIT_I  %" PRId64 "\n"
        "\tmovabs  rdi, %" PRId64 "\n"
        "\tcall    scrip_rt_push_int@PLT\n",
        pc, ins->a[0].i, ins->a[0].i) < 0 ? -1 : 0;
}

static int emit_op_unhandled(FILE *out, const SM_Instr *ins, int pc)
{
    /* Any opcode the emitter does not yet recognize routes here.
     * Runtime trap is loud and clear — see scrip_rt_unhandled_op
     * in libscrip_rt.so.  Argument is the opcode integer for
     * post-mortem diagnosis. */
    return fprintf(out,
        "\t# pc=%d  UNHANDLED_OP  %s (opcode=%d)\n"
        "\tmov     edi, %d\n"
        "\tcall    scrip_rt_unhandled_op@PLT\n",
        pc, sm_opcode_name(ins->op), (int)ins->op, (int)ins->op) < 0 ? -1 : 0;
}

/* ── Top-level entry ────────────────────────────────────────────────── */

int sm_codegen_x64_emit(SM_Program *prog, FILE *out)
{
    assert(prog != NULL);
    assert(out  != NULL);

    /* File header. */
    if (fprintf(out,
        "# ──────────────────────────────────────────────────────────────────\n"
        "# scrip --jit-emit --x64  output  (M-JITEM-X64 / EM-1+EM-2)\n"
        "# Emitted by sm_codegen_x64_emit() — %d SM instructions.\n"
        "# Links against libscrip_rt.so for the scrip_rt_* ABI.\n"
        "# ──────────────────────────────────────────────────────────────────\n"
        "\t.intel_syntax noprefix\n"
        "\t.text\n"
        "\t.globl  main\n"
        "\t.type   main, @function\n"
        "main:\n"
        "\tpush    rbp\n"
        "\tmov     rbp, rsp\n"
        "\t# scrip_rt_init(argc, argv)  — argc in edi, argv in rsi already.\n"
        "\tcall    scrip_rt_init@PLT\n",
        prog->count) < 0) {
        return -1;
    }

    /* Per-instruction dispatch.  EM-2: SM_HALT and SM_PUSH_LIT_I are
     * baked; everything else goes to the unhandled trap.  Each block
     * begins with a .Lpc<N> label so EM-4's jumps have somewhere to
     * land.  The label costs nothing at runtime — the assembler
     * resolves it locally and emits no relocation. */
    for (int pc = 0; pc < prog->count; pc++) {
        const SM_Instr *ins = &prog->instrs[pc];
        if (emit_pc_label(out, pc) != 0) return -1;
        int rc;
        switch (ins->op) {
            case SM_HALT:
                rc = emit_op_halt(out, ins, pc);
                break;
            case SM_PUSH_LIT_I:
                rc = emit_op_push_lit_i(out, ins, pc);
                break;
            default:
                rc = emit_op_unhandled(out, ins, pc);
                break;
        }
        if (rc != 0) return -1;
    }

    /* Epilogue. scrip_rt_finalize() returns int — the program's rc.
     * If a SM_HALT executed it has already recorded the rc inside
     * libscrip_rt; finalize then surfaces it.  If the SM_Program
     * ran off the end without halting, finalize returns 0. */
    if (fputs(
        "\t# ── epilogue ─────────────────────────────────────\n"
        "\tcall    scrip_rt_finalize@PLT\n"
        "\tpop     rbp\n"
        "\tret\n"
        "\t.size   main, .-main\n"
        "\t.section .note.GNU-stack,\"\",@progbits\n",
        out) == EOF) {
        return -1;
    }
    return 0;
}
