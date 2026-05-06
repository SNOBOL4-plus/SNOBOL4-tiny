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
 *   EM-4: SM_LABEL (no-op; .LpcN label suffices), SM_JUMP (direct jmp),
 *         SM_JUMP_S/F (call last_ok + test + conditional jmp).
 *   EM-5+: SM_PUSH_CHUNK / SM_CALL_CHUNK / SM_RETURN, BB box coverage.
 */

#include "sm_codegen_x64_emit.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "sm_prog.h"
#include <string.h>

/* -----------------------------------------------------------------------
 * Source-line cache (EM-4-readability)
 *
 * When the caller passes a non-NULL src_path, we slurp the file once,
 * split it into lines (1-based index), and use it to print a verbatim
 * source banner above each statement's asm block.
 *
 * Goal: the emitted .s file reads top-to-bottom like an annotated
 * disassembly -- each statement's source text is right above the
 * x86 it produces.  Nobody else's compiler does this.  Ours does.
 * ----------------------------------------------------------------------- */

typedef struct {
    char       *buf;       /* whole-file backing buffer (NUL-terminated)  */
    char      **lines;     /* lines[i] = pointer into buf, 1-based;
                              lines[0] is unused so lookups are direct.  */
    int         count;     /* highest 1-based line index that exists     */
    const char *path;      /* original src_path (borrowed; may be NULL)  */
} SrcLines;

/* Slurp src_path into sl.  Returns 0 on success, -1 on error.
 * On error sl is left zeroed; the emitter falls back to "no source"
 * mode and emits structural banners only. */
static int srclines_load(SrcLines *sl, const char *src_path)
{
    memset(sl, 0, sizeof(*sl));
    if (!src_path) return -1;
    FILE *f = fopen(src_path, "rb");
    if (!f) return -1;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return -1; }
    long n = ftell(f);
    if (n < 0)                       { fclose(f); return -1; }
    rewind(f);
    char *buf = malloc((size_t)n + 1);
    if (!buf)                        { fclose(f); return -1; }
    size_t got = fread(buf, 1, (size_t)n, f);
    fclose(f);
    buf[got] = '\0';

    /* First pass: count lines.  Last line w/o trailing \n still counts. */
    int count = 0;
    for (size_t i = 0; i < got; i++) if (buf[i] == '\n') count++;
    if (got > 0 && buf[got-1] != '\n') count++;

    char **lines = calloc((size_t)count + 2, sizeof(char *));
    if (!lines) { free(buf); return -1; }

    /* Second pass: split.  NUL-terminate each line (overwrite '\n').
     * lines[0] left NULL so 1-based lookups are direct. */
    int    li = 1;
    char  *p  = buf;
    char  *line_start = buf;
    for (size_t i = 0; i < got; i++) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
            lines[li++] = line_start;
            line_start = &buf[i+1];
        }
    }
    if (line_start < buf + got) lines[li++] = line_start;

    sl->buf   = buf;
    sl->lines = lines;
    sl->count = li - 1;
    sl->path  = src_path;
    (void)p;
    return 0;
}

static void srclines_free(SrcLines *sl)
{
    free(sl->lines);
    free(sl->buf);
    memset(sl, 0, sizeof(*sl));
}

/* Lookup line text by 1-based index.  Returns NULL if out of range. */
static const char *srclines_get(const SrcLines *sl, int lineno)
{
    if (!sl || !sl->lines || lineno < 1 || lineno > sl->count) return NULL;
    return sl->lines[lineno];
}

/* Strip a trailing '\r' (CRLF source files) for clean banner output. */
static void srcline_strip_cr(char *s)
{
    if (!s) return;
    size_t n = strlen(s);
    if (n > 0 && s[n-1] == '\r') s[n-1] = '\0';
}

/* -----------------------------------------------------------------------
 * File header emitter
 * ----------------------------------------------------------------------- */

static int emit_file_header(FILE *out, int count)
{
    return fprintf(out,
        "# -----------------------------------------------------------------------\n"
        "# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-4)\n"
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
 * Page breaks (EM-4-readability)
 *
 * Major (==): printed at every statement boundary -- carries the verbatim
 *             source text of the statement (when available) plus stno/lineno.
 * Minor (--): printed between conceptual blocks within a statement, used
 *             sparingly to avoid clutter.
 * ----------------------------------------------------------------------- */

static int emit_major_break(FILE *out, int stno, int lineno,
                            const char *src_text)
{
    /* Bracket bar at column 0, full width 78 (matches GNU-as
     * convention; '#' is the line-comment introducer). */
    if (fputs(
        "\n# ============================================================================\n",
        out) == EOF) return -1;
    if (src_text && *src_text) {
        if (fprintf(out, "# stmt %d  (line %d):  %s\n",
                    stno, lineno, src_text) < 0) return -1;
    } else if (lineno > 0) {
        if (fprintf(out, "# stmt %d  (line %d)\n", stno, lineno) < 0) return -1;
    } else {
        if (fprintf(out, "# stmt %d\n", stno) < 0) return -1;
    }
    if (fputs(
        "# ============================================================================\n",
        out) == EOF) return -1;
    return 0;
}

static int emit_minor_break(FILE *out, const char *caption)
{
    if (fputs("# ----------------------------------------------------------------------------\n",
              out) == EOF) return -1;
    if (caption && *caption) {
        if (fprintf(out, "# %s\n", caption) < 0) return -1;
        if (fputs("# ----------------------------------------------------------------------------\n",
                  out) == EOF) return -1;
    }
    return 0;
}

/* Render a printable, single-line preview of a string literal for use in
 * inline annotations (e.g. movabs rdi,<ptr>  # str="hi").  Truncates at
 * MAX_PREVIEW chars and replaces non-printable bytes with '.'. */
#define STR_PREVIEW_MAX  40
static void render_str_preview(char *dst, size_t cap,
                               const char *s, int slen)
{
    if (cap == 0) return;
    size_t  n = (slen > 0) ? (size_t)slen : (s ? strlen(s) : 0);
    size_t  o = 0;
    if (o < cap) dst[o++] = '"';
    for (size_t i = 0; i < n && o + 2 < cap && i < STR_PREVIEW_MAX; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == '"' || c == '\\') {
            if (o + 1 < cap) dst[o++] = '\\';
            dst[o++] = (char)c;
        } else if (c < 0x20 || c == 0x7f) {
            dst[o++] = '.';
        } else {
            dst[o++] = (char)c;
        }
    }
    if (n > STR_PREVIEW_MAX && o + 4 < cap) {
        dst[o++] = '.'; dst[o++] = '.'; dst[o++] = '.';
    }
    if (o + 1 < cap) dst[o++] = '"';
    dst[o] = '\0';
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
    /* Annotate the opaque ptr-as-immediate with the actual literal. */
    char preview[STR_PREVIEW_MAX + 8];
    render_str_preview(preview, sizeof(preview), s, (int)slen);
    char goto_col[STR_PREVIEW_MAX + 16];
    snprintf(goto_col, sizeof(goto_col), "; str=%s", preview);
    snprintf(act, sizeof(act), "movabs  rdi, %" PRIu64,
             (uint64_t)(uintptr_t)s);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    snprintf(act, sizeof(act), "mov     esi, %d", (int)slen);
    if (sm_line(out, "", act, "; slen") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_push_str@PLT", "");
}

static int emit_sm_push_var(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_PUSH_VAR macro -- scrip_rt_nv_get stub in EM-3 */
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    char act[80];
    char goto_col[80];
    snprintf(act, sizeof(act), "movabs  rdi, %" PRIu64,
             (uint64_t)(uintptr_t)name);
    snprintf(goto_col, sizeof(goto_col), "; var=%s", name);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_nv_get@PLT",
                   "; SM_PUSH_VAR -> TOS");
}

static int emit_sm_store_var(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_STORE_VAR macro -- scrip_rt_nv_set stub in EM-3 */
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    char act[80];
    char goto_col[80];
    snprintf(act, sizeof(act), "movabs  rdi, %" PRIu64,
             (uint64_t)(uintptr_t)name);
    snprintf(goto_col, sizeof(goto_col), "; store -> %s", name);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_nv_set@PLT",
                   "; SM_STORE_VAR pop TOS");
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

/* EM-4 opcodes: SM_LABEL + SM_JUMP / SM_JUMP_S / SM_JUMP_F */

static int emit_sm_label(FILE *out, const SM_Instr *ins, int pc)
{
    /* SM_LABEL is a no-op marker.  The .LpcN label emitted at every PC
     * already serves as the jump target.  Nothing else to do.  Kept as
     * a documented case so the switch never falls into emit_sm_unhandled. */
    (void)out; (void)ins; (void)pc;
    return 0;
}

static int emit_sm_jump(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    /* SM_JUMP: unconditional direct jump to .Lpc<target>.  No runtime
     * call -- pure inline x86.  Three-column: empty label / jmp action /
     * target as comment for readability. */
    char act[80];
    char goto_col[64];
    int  target = (int)ins->a[0].i;
    snprintf(act,      sizeof(act),      "jmp     .Lpc%d", target);
    snprintf(goto_col, sizeof(goto_col), "; SM_JUMP -> pc=%d", target);
    return sm_line(out, "", act, goto_col);
}

static int emit_sm_jump_cond(FILE *out, const SM_Instr *ins, int pc,
                             int take_when_ok)
{
    /* Shared core for SM_JUMP_S (take_when_ok=1) and SM_JUMP_F (=0).
     * Idiom:
     *   call scrip_rt_last_ok@PLT  ; eax = last_ok (0 or 1)
     *   test eax, eax
     *   <jnz | jz> .Lpc<target>
     */
    (void)pc;
    int  target = (int)ins->a[0].i;
    char act[80];
    char goto_col[80];
    if (sm_line(out, "", "call    scrip_rt_last_ok@PLT",
                "; EM-4 conditional jump") < 0) return -1;
    if (sm_line(out, "", "test    eax, eax", "") < 0) return -1;
    snprintf(act, sizeof(act), "%s     .Lpc%d",
             take_when_ok ? "jnz" : "jz", target);
    snprintf(goto_col, sizeof(goto_col), "; %s -> pc=%d",
             take_when_ok ? "SM_JUMP_S" : "SM_JUMP_F", target);
    return sm_line(out, "", act, goto_col);
}

static int emit_sm_jump_s(FILE *out, const SM_Instr *ins, int pc)
{
    return emit_sm_jump_cond(out, ins, pc, /*take_when_ok=*/1);
}

static int emit_sm_jump_f(FILE *out, const SM_Instr *ins, int pc)
{
    return emit_sm_jump_cond(out, ins, pc, /*take_when_ok=*/0);
}

/* -----------------------------------------------------------------------
 * SM_STNO -- statement boundary
 *
 * EM-4-readability:  the SM_STNO opcode marks a source-statement
 * boundary.  We use it to emit a major page-break banner showing the
 * verbatim source line, plus the runtime tick into &STNO/&STCOUNT.
 *
 * Operand layout (from sm_lower.c, EM-4):
 *   a[0].i  = source statement number (1-based)
 *   a[1].i  = source line number (added EM-4-readability; safe for
 *             interp because sm_interp.c reads only a[0].i)
 * ----------------------------------------------------------------------- */

static int emit_sm_stno(FILE *out, const SM_Instr *ins, int pc,
                        const SrcLines *sl)
{
    (void)pc;
    int stno   = (int)ins->a[0].i;
    int lineno = (int)ins->a[1].i;

    /* Lineno fallback: the parser only records lineno for *labeled*
     * statements (s->lineno = lbl.lineno).  For unlabeled statements
     * lineno comes through as 0.  In typical SNOBOL4 source one
     * statement occupies one line, so stno is an accurate estimator.
     * Any lookup that produces a non-blank source line is then a win;
     * mismatches produce blank-source banners (graceful degradation).
     * Future rung: have the parser record lineno on every statement
     * (one-line .y change) and remove this fallback. */
    int try_lineno = lineno;
    if (try_lineno <= 0 || (sl && try_lineno > sl->count))
        try_lineno = stno;

    char line_copy[1024];
    const char *src = NULL;
    if (sl) {
        const char *raw = srclines_get(sl, try_lineno);
        if (raw && *raw) {
            /* Copy and strip trailing CR if present (CRLF-friendly). */
            strncpy(line_copy, raw, sizeof(line_copy) - 1);
            line_copy[sizeof(line_copy) - 1] = '\0';
            srcline_strip_cr(line_copy);
            src = line_copy;
        }
    }

    /* If lineno was authoritatively recorded but out of range, suppress
     * the misleading "(line N)" suffix in the banner.  If we used the
     * stno-based fallback successfully, present try_lineno as the line. */
    int banner_lineno;
    if (lineno > 0 && (!sl || lineno <= sl->count)) {
        banner_lineno = lineno;
    } else if (src) {
        banner_lineno = try_lineno;   /* fallback hit something printable */
    } else {
        banner_lineno = 0;            /* truly unknown */
    }

    if (emit_major_break(out, stno, banner_lineno, src) != 0) return -1;

    /* Per-PC label still emitted (already done by caller before us). */
    /* Runtime tick: scrip_rt_stno_tick(stno) in a future rung; for EM-4
     * we keep the SM_STNO call as a no-op stub annotated for clarity.
     * Currently the EM-3+ runtime does not yet implement &STNO/&STCOUNT
     * in libscrip_rt.so; the emitter falls through to unhandled trap if
     * the program actually references those keywords.  Marker comment
     * documents the intent. */
    char act[80];
    char goto_col[80];
    snprintf(act,      sizeof(act),      "mov     edi, %d", stno);
    snprintf(goto_col, sizeof(goto_col), "; SM_STNO stno=%d (no-op stub)",
             stno);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_unhandled_op@PLT",
                   "; runtime &STNO support: future rung");
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

int sm_codegen_x64_emit(SM_Program *prog, FILE *out, const char *src_path)
{
    assert(prog != NULL);
    assert(out  != NULL);

    /* EM-4-readability: load the source file once if a path was given. */
    SrcLines sl;
    int sl_loaded = (srclines_load(&sl, src_path) == 0);

    if (emit_file_header(out, prog->count) != 0) {
        if (sl_loaded) srclines_free(&sl);
        return -1;
    }

    /* If we have source, print a header banner naming the file and stmt
     * count -- a one-line "table of contents" for the human reader. */
    if (sl_loaded) {
        fprintf(out,
            "# source-file: %s  (%d lines)\n"
            "# Each statement appears below as a major banner ('====') above\n"
            "# the asm it produced.  Inline annotations on the right column\n"
            "# show the source-level object referenced by each macro call.\n",
            sl.path, sl.count);
    }

    for (int pc = 0; pc < prog->count; pc++) {
        const SM_Instr *ins = &prog->instrs[pc];

        /* Per-PC label: .LpcN -- used by EM-4 control-flow jumps */
        if (emit_pc_label(out, pc) != 0) {
            if (sl_loaded) srclines_free(&sl);
            return -1;
        }

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

            /* EM-4: control flow.  SM_LABEL is a no-op (the .LpcN label at
             * every PC already serves as the target); SM_JUMP/S/F resolve
             * targets to baked-at-emit-time .Lpc<a[0].i>. */
            case SM_LABEL:        rc = emit_sm_label(out, ins, pc);      break;
            case SM_JUMP:         rc = emit_sm_jump(out, ins, pc);       break;
            case SM_JUMP_S:       rc = emit_sm_jump_s(out, ins, pc);     break;
            case SM_JUMP_F:       rc = emit_sm_jump_f(out, ins, pc);     break;

            /* SM_STNO -- statement boundary; emits major page break w/ source. */
            case SM_STNO:         rc = emit_sm_stno(out, ins, pc,
                                                    sl_loaded ? &sl : NULL); break;

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
        if (rc != 0) {
            if (sl_loaded) srclines_free(&sl);
            return -1;
        }
    }

    int frc = emit_file_footer(out);
    if (sl_loaded) srclines_free(&sl);
    return frc;
}
