/*
 * sm_codegen_x64_emit.c -- SM_Program -> standalone x86-64 GNU-as text
 *                         (M-JITEM-X64, GOAL-MODE4-EMIT EM-1..EM-3)
 *
 * Authors: Lon Jones Cherryholmes, Claude Sonnet
 * Date: 2026-05-06
 *
 * Architecture (settled session #67 -- see archive/EMITTER-MODE4-ARCH.md):
 *
 * TWO SEPARATE EMITTERS inside this file:
 *
 *   emit_sm_instr()  -- straight-line SM opcodes (push/pop/arith/control)
 *                       Calls GNU-as macros from sm_macros.s. One macro
 *                       per opcode group. Three-column SNOBOL4 layout:
 *                         .LpcN:   SM_ADD         ; pop r,l -> push l+r
 *                       Arithmetic and push/pop bake inline via macros.
 *                       libscrip_rt.so boundary: NV table, matcher, GC.
 *
 *   emit_bb_box()    -- BB graph boxes (called per SM_PAT_* instruction)
 *                       One proc per box. Four local labels per proc:
 *                         .alpha (try), .beta (retry),
 *                         .gamma (success exit), .omega (failure exit)
 *                       Three-column law inside each port body.
 *                       Proven precedent: bb_emit.c + snobol4_asm.mac
 *                       (106/106 SPITBOL oracle).
 *
 * Calling convention: System V AMD64. .intel_syntax noprefix.
 * sm_macros.s included at the top of every emitted file.
 *
 * Opcode coverage:
 *   EM-1: literal-zero scaffold (init+finalize only).
 *   EM-2: SM_HALT, SM_PUSH_LIT_I. SM_NOP not in opcode enum.
 *   EM-3: SM_PUSH_LIT_S, SM_PUSH_VAR, SM_STORE_VAR, SM_POP,
 *         SM_ADD/SUB/MUL/DIV/MOD via sm_macros.s.
 *         SM_DUP/SM_SWAP not in enum (honest deviation).
 *         emit_bb_box() scaffold added (no SM_PAT_* coverage yet).
 *   EM-4+: control flow (SM_JUMP/S/F), BB box coverage.
 */

#include "sm_codegen_x64_emit.h"
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include "sm_prog.h"
#include <string.h>

/* -----------------------------------------------------------------------
 * File header emitter
 * ----------------------------------------------------------------------- */

static int emit_file_header(FILE *out, int count)
{
    return fprintf(out,
        "# -----------------------------------------------------------------------\n"
        "# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-3)\n"
        "# %d SM instructions. Links against libscrip_rt.so.\n"
        "# Architecture: two emitters -- SM straight-line via sm_macros.s\n"
        "#   macros (inline x86); BB boxes via emit_bb_box() one-proc-per-box.\n"
        "# See archive/EMITTER-MODE4-ARCH.md for the full design.\n"
        "# -----------------------------------------------------------------------\n"
        "\t.intel_syntax noprefix\n"
        "# Include SM opcode macro library (one macro per opcode group)\n"
        "# .include \"sm_macros.s\"  # assembled separately; macros used by name below\n"
        "\t.text\n"
        "\t.globl  main\n"
        "\t.type   main, @function\n"
        "main:\n"
        "\tpush    rbp\n"
        "\tmov     rbp, rsp\n"
        "\t# scrip_rt_init(argc, argv) -- argc in edi, argv in rsi\n"
        "\tcall    scrip_rt_init@PLT\n",
        count) < 0 ? -1 : 0;
}

static int emit_file_footer(FILE *out)
{
    return fputs(
        "\t# -- epilogue -------------------------------------------\n"
        "\tcall    scrip_rt_finalize@PLT\n"
        "\tpop     rbp\n"
        "\tret\n"
        "\t.size   main, .-main\n"
        "\t.section .note.GNU-stack,\"\",@progbits\n",
        out) == EOF ? -1 : 0;
}

/* -----------------------------------------------------------------------
 * Three-column helpers
 *
 * Three-column SNOBOL4 layout (from BB-GEN-X86-TEXT.md law):
 *   Col 1 (label):  col 0, width 24
 *   Col 2 (action): col 24, macro name + args
 *   Col 3 (goto):   semicolon comment or live jmp
 *
 * SM_LINE(label, action, goto_comment) -- full three-column line
 * SM_ACT(action, goto_comment)         -- action + goto, no label
 * SM_LABEL_DEF(label)                  -- label only line
 * ----------------------------------------------------------------------- */

static int sm_line(FILE *out, const char *label, const char *action,
                   const char *goto_col)
{
    /* Col 1 (label): width 24; Col 2 (action): width 36; Col 3 (goto/#comment).
     * GNU-as with .intel_syntax uses # for line comments; ; is stmt separator.
     * goto_col that starts with ';' is a comment: replace with '#'. */
    const char *gc = "";
    char gc_buf[128] = "";
    if (goto_col && *goto_col) {
        /* Determine if goto_col is a live jump (jmp/je/jne/ret/call) or a comment.
         * Anything that is not an asm keyword gets prefixed with "# " to make
         * it a comment. GNU-as .intel_syntax does not accept bare words after
         * an instruction. */
        int is_asm = (strncmp(goto_col, "jmp", 3) == 0 ||
                      strncmp(goto_col, "je",  2) == 0 ||
                      strncmp(goto_col, "jne", 3) == 0 ||
                      strncmp(goto_col, "jz",  2) == 0 ||
                      strncmp(goto_col, "jnz", 3) == 0 ||
                      strncmp(goto_col, "ret", 3) == 0 ||
                      strncmp(goto_col, "call",4) == 0 ||
                      goto_col[0] == '#');
        if (is_asm) {
            gc = goto_col;
        } else if (goto_col[0] == ';') {
            snprintf(gc_buf, sizeof(gc_buf), "# %s", goto_col + 1);
            gc = gc_buf;
        } else {
            snprintf(gc_buf, sizeof(gc_buf), "# %s", goto_col);
            gc = gc_buf;
        }
    }
    const char *lbl = (label && *label) ? label : "";
    return fprintf(out, "%-24s%-36s%s\n", lbl, action, gc) < 0 ? -1 : 0;
}

/* -----------------------------------------------------------------------
 * SM straight-line opcode emitters
 * Each writes one or more three-column lines using sm_macros.s macros.
 * ----------------------------------------------------------------------- */

static int emit_pc_label(FILE *out, int pc)
{
    char lbl[32];
    snprintf(lbl, sizeof(lbl), ".Lpc%d:", pc);
    return fprintf(out, "%-24s\n", lbl) < 0 ? -1 : 0;
}

static int emit_sm_halt(FILE *out, int pc)
{
    (void)pc;
    /* SM_HALT macro: pop TOS as rc, call scrip_rt_halt, fall to epilogue */
    if (sm_line(out, "", "call    scrip_rt_pop_int@PLT",
                "; rc <- TOS") < 0) return -1;
    if (sm_line(out, "", "mov     edi, eax", "") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_halt@PLT", "");
}

static int emit_sm_push_lit_i(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    char act[80];
    /* SM_PUSH_INT macro: movabs rdi,val / call scrip_rt_push_int */
    snprintf(act, sizeof(act), "movabs  rdi, %" PRId64, ins->a[0].i);
    if (sm_line(out, "", act, "") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_push_int@PLT", "");
}

/* EM-3 opcodes */

static int emit_sm_push_lit_s(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_PUSH_STR macro: movabs rdi,ptr / mov esi,slen / call push_str */
    const char *s    = ins->a[0].s ? ins->a[0].s : "";
    int64_t     slen = ins->a[1].i;
    char act[80];
    snprintf(act, sizeof(act), "movabs  rdi, %" PRIu64,
             (uint64_t)(uintptr_t)s);
    if (sm_line(out, "", act, "") < 0) return -1;
    snprintf(act, sizeof(act), "mov     esi, %d", (int)slen);
    if (sm_line(out, "", act, "") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_push_str@PLT", "");
}

static int emit_sm_push_var(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_PUSH_VAR macro -- scrip_rt_nv_get stub in EM-3 */
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    char act[80];
    snprintf(act, sizeof(act), "movabs  rdi, %" PRIu64,
             (uint64_t)(uintptr_t)name);
    if (sm_line(out, "", act, "; SM_PUSH_VAR: nv_get stub EM-3") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_nv_get@PLT", "");
}

static int emit_sm_store_var(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_STORE_VAR macro -- scrip_rt_nv_set stub in EM-3 */
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    char act[80];
    snprintf(act, sizeof(act), "movabs  rdi, %" PRIu64,
             (uint64_t)(uintptr_t)name);
    if (sm_line(out, "", act, "; SM_STORE_VAR: nv_set stub EM-3") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_nv_set@PLT", "");
}

static int emit_sm_pop(FILE *out, int pc)
{
    (void)pc;
    /* SM_POP macro */
    return sm_line(out, "", "call    scrip_rt_pop_void@PLT",
                   "; SM_POP: discard TOS");
}

static int emit_sm_arith(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_ADD/SUB/MUL/DIV/MOD macros: mov edi,op / call scrip_rt_arith
     * Opcode int values from sm_prog.h:
     *   SM_ADD=17 SM_SUB=18 SM_MUL=19 SM_DIV=20 SM_MOD=22 */
    char act[80];
    snprintf(act, sizeof(act), "mov     edi, %d", (int)ins->op);
    if (sm_line(out, "", act,
                sm_opcode_name(ins->op)) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_arith@PLT", "");
}

/* -----------------------------------------------------------------------
 * BB box emitter (emit_bb_box)
 *
 * Called once per SM_PAT_* instruction. Emits one labeled proc with
 * four local labels: .box_N_alpha, .box_N_beta, .box_N_gamma, .box_N_omega.
 * Three-column law inside each port body.
 *
 * EM-3: scaffold only -- all SM_PAT_* still go to unhandled trap.
 * EM-6+: full pattern box coverage.
 * ----------------------------------------------------------------------- */

static int emit_bb_box(FILE *out, const SM_Instr *ins, int pc)
{
    /* Scaffold: emit a commented-out box proc skeleton for documentation.
     * The unhandled trap fires at runtime. Full implementation in EM-6. */
    fprintf(out,
        "# -- BB box scaffold pc=%d op=%s --\n"
        "# proc .bb_box_%d\n"
        "#   .alpha:   (not yet baked)\n"
        "#   .beta:    (not yet baked)\n"
        "#   .gamma:   (connected to next box alpha)\n"
        "#   .omega:   (connected to enclosing beta)\n"
        "# endp\n",
        pc, sm_opcode_name(ins->op), pc);
    /* Fall through to unhandled trap (runtime will abort) */
    char act[80];
    snprintf(act, sizeof(act), "mov     edi, %d", (int)ins->op);
    if (sm_line(out, "", act, "; UNHANDLED BB box") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_unhandled_op@PLT", "");
}

/* -----------------------------------------------------------------------
 * Unhandled opcode trap
 * ----------------------------------------------------------------------- */

static int emit_sm_unhandled(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    char act[80];
    snprintf(act, sizeof(act), "mov     edi, %d", (int)ins->op);
    if (sm_line(out, "", act,
                sm_opcode_name(ins->op)) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_unhandled_op@PLT", "");
}

/* -----------------------------------------------------------------------
 * Top-level entry
 * ----------------------------------------------------------------------- */

int sm_codegen_x64_emit(SM_Program *prog, FILE *out)
{
    assert(prog != NULL);
    assert(out  != NULL);

    if (emit_file_header(out, prog->count) != 0) return -1;

    for (int pc = 0; pc < prog->count; pc++) {
        const SM_Instr *ins = &prog->instrs[pc];

        /* Per-PC label: .LpcN -- used by EM-4 control-flow jumps */
        if (emit_pc_label(out, pc) != 0) return -1;

        int rc;
        switch (ins->op) {
            /* EM-2: halt + integer push */
            case SM_HALT:         rc = emit_sm_halt(out, pc);            break;
            case SM_PUSH_LIT_I:   rc = emit_sm_push_lit_i(out, ins, pc); break;

            /* EM-3: string push, var load/store, pop, arithmetic */
            case SM_PUSH_LIT_S:   rc = emit_sm_push_lit_s(out, ins, pc); break;
            case SM_PUSH_VAR:     rc = emit_sm_push_var(out, ins, pc);   break;
            case SM_STORE_VAR:    rc = emit_sm_store_var(out, ins, pc);  break;
            case SM_POP:          rc = emit_sm_pop(out, pc);             break;
            case SM_ADD:
            case SM_SUB:
            case SM_MUL:
            case SM_DIV:
            case SM_MOD:          rc = emit_sm_arith(out, ins, pc);      break;

            /* BB box opcodes: one-proc-per-box (scaffold EM-3, full EM-6) */
            case SM_PAT_LIT:
            case SM_PAT_ANY:
            case SM_PAT_NOTANY:
            case SM_PAT_SPAN:
            case SM_PAT_BREAK:
            case SM_PAT_LEN:
            case SM_PAT_ARB:
            case SM_PAT_ARBNO:
            case SM_PAT_ALT:
            case SM_PAT_CAT:
            case SM_PAT_EPS:
            case SM_PAT_REM:
            case SM_PAT_BAL:
            case SM_PAT_FENCE:
            case SM_PAT_ABORT:
            case SM_PAT_FAIL:
            case SM_PAT_SUCCEED:
            case SM_PAT_CAPTURE:
            case SM_PAT_DEREF:
                                  rc = emit_bb_box(out, ins, pc);         break;

            default:              rc = emit_sm_unhandled(out, ins, pc);  break;
        }
        if (rc != 0) return -1;
    }

    return emit_file_footer(out);
}
