/*
 * sm_codegen_x64_emit.c — SM_Program → standalone x86-64 asm
 *                         (M-JITEM-X64, GOAL-MODE4-EMIT EM-1+)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * EM-1 implementation: literal-zero stub.
 *
 * The emitted asm is a minimal System V AMD64 main() that:
 *   1. calls scrip_rt_init(argc, argv)
 *   2. calls scrip_rt_finalize() and returns its rc
 *
 * Both runtime entries are no-ops in libscrip_rt.so v0.1, so the
 * resulting binary loads, links, and exits 0. This rung is wiring only;
 * the SM_Program parameter is intentionally unused (asserted but
 * otherwise ignored). Each subsequent EM-N rung adds real codegen for
 * a slice of the opcode set, replacing the literal-zero scaffold.
 *
 * Output dialect: GNU-as / x86_64 System V (compatible with `as` and
 * `gcc`-driven assembly). PIC-friendly relocations (PLT calls, no
 * absolute addressing of externs).
 */

#include "sm_codegen_x64_emit.h"
#include <assert.h>
#include <stdio.h>

int sm_codegen_x64_emit(SM_Program *prog, FILE *out)
{
    /* prog is required (caller invariant) but unused in EM-1.
     * Casting to void silences -Wunused without removing the assert. */
    assert(prog != NULL);
    assert(out  != NULL);
    (void)prog;

    /* Header — file-level metadata for traceability. */
    if (fputs(
        "# ──────────────────────────────────────────────────────────────────\n"
        "# scrip --jit-emit --x64  output  (M-JITEM-X64 / EM-1 stub)\n"
        "# Emitted by sm_codegen_x64_emit() — literal-zero scaffold.\n"
        "# Links against libscrip_rt.so for scrip_rt_init / scrip_rt_finalize.\n"
        "# ──────────────────────────────────────────────────────────────────\n"
        "\t.intel_syntax noprefix\n"
        "\t.text\n"
        "\t.globl  main\n"
        "\t.type   main, @function\n"
        "main:\n"
        "\tpush    rbp\n"
        "\tmov     rbp, rsp\n"
        "\t# scrip_rt_init(argc, argv)  — argc in edi, argv in rsi already.\n"
        "\tcall    scrip_rt_init@PLT\n"
        "\t# scrip_rt_finalize() returns int rc; tail through to ret.\n"
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
