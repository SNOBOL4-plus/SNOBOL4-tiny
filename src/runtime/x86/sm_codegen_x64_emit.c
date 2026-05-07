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
 *   EM-5: SM_PUSH_CHUNK (push DT_E descriptor), SM_CALL_CHUNK (baked
 *         direct call to .Lpc<entry_pc>), SM_RETURN (native ret).
 *         Conditional return variants (SM_RETURN_S/F, SM_FRETURN[_S/_F],
 *         SM_NRETURN[_S/_F]) still trap via emit_sm_unhandled.
 *   EM-6: SM_PAT_* opcodes emit PLT calls to scrip_rt_pat_*().
 *         SM_EXEC_STMT emits PLT call to scrip_rt_exec_stmt().
 *         SM_PAT_REFNAME, SM_PAT_BOXVAL, SM_PAT_CAPTURE,
 *         SM_PAT_DEREF, SM_PAT_FENCE1 baked.
 *         emit_bb_box() upgraded: now emits real PLT calls for all
 *         EM-6 covered opcodes; falls through to unhandled for rest.
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
 * String table — EM-6
 *
 * The emitter cannot use raw in-process pointers for string literals —
 * the emitted binary runs in a different process.  Instead, we collect
 * all unique C strings referenced by the SM_Program into a string table,
 * emit them in .section .rodata as .Lstr_N labels, and reference those
 * labels by name in the instruction emitters.
 *
 * Two-pass protocol:
 *   1. strtab_collect(prog) — walk the SM_Program, intern all a[].s strings.
 *   2. strtab_emit(out)     — write .section .rodata with all .string data.
 *   3. Instruction emitters call strtab_label(s) to get the label name.
 * ----------------------------------------------------------------------- */

#define STRTAB_CAP 8192  /* EM-7: beauty.sno has ~2500 unique strings; 8192 gives headroom */

typedef struct {
    const char *s;       /* original pointer from SM_Instr.a[].s */
    int         idx;     /* assigned label index (.Lstr_<idx>) */
} StrEntry;

static StrEntry g_strtab[STRTAB_CAP];
static int      g_strtab_n = 0;

static void strtab_reset(void)
{
    g_strtab_n = 0;
}

/* Intern a string; return its label index (0-based). */
static int strtab_intern(const char *s)
{
    if (!s) s = "";
    /* Linear scan — small tables; fast enough for realistic programs. */
    for (int i = 0; i < g_strtab_n; i++)
        if (g_strtab[i].s == s || strcmp(g_strtab[i].s, s) == 0)
            return g_strtab[i].idx;
    if (g_strtab_n >= STRTAB_CAP) {
        fprintf(stderr, "sm_codegen_x64_emit: string table overflow\n");
        abort();
    }
    int idx = g_strtab_n;
    g_strtab[g_strtab_n].s   = s;
    g_strtab[g_strtab_n].idx = idx;
    g_strtab_n++;
    return idx;
}

/* Return the .Lstr_N label name for string s (must be interned first). */
static void strtab_label(char *buf, size_t bufsz, const char *s)
{
    if (!s) s = "";
    for (int i = 0; i < g_strtab_n; i++)
        if (g_strtab[i].s == s || strcmp(g_strtab[i].s, s) == 0) {
            snprintf(buf, bufsz, ".Lstr_%d", g_strtab[i].idx);
            return;
        }
    /* Should not happen if caller always interns first. */
    snprintf(buf, bufsz, ".Lstr_ERR");
}

/* Escape a C string for GNU-as .string directive.
 * Only escapes \, ", and control chars that matter. */
static void strtab_escape(char *out, size_t outsz, const char *s)
{
    size_t j = 0;
    out[j++] = '"';
    for (const char *p = s; *p && j + 6 < outsz; p++) {
        unsigned char c = (unsigned char)*p;
        if      (c == '\\') { out[j++] = '\\'; out[j++] = '\\'; }
        else if (c == '"')  { out[j++] = '\\'; out[j++] = '"';  }
        else if (c == '\n') { out[j++] = '\\'; out[j++] = 'n';  }
        else if (c == '\t') { out[j++] = '\\'; out[j++] = 't';  }
        else if (c < 0x20 || c == 0x7f) {
            j += (size_t)snprintf(out + j, outsz - j, "\\%03o", c);
        } else {
            out[j++] = (char)c;
        }
    }
    out[j++] = '"';
    if (j < outsz) out[j] = '\0';
}

/* Collect all strings from the SM_Program into the table.
 * Only opcodes that actually carry string payloads in a[0].s / a[2].s
 * are interned — never blindly walk all instructions (other opcodes
 * carry integer / pointer payloads in the same union fields). */
static void strtab_collect(const SM_Program *prog)
{
    strtab_reset();
    for (int i = 0; i < prog->count; i++) {
        const SM_Instr *ins = &prog->instrs[i];
        switch (ins->op) {
        /* Opcodes that carry string data in a[0].s */
        case SM_PUSH_LIT_S:
        case SM_PUSH_VAR:
        case SM_STORE_VAR:
        case SM_PAT_LIT:
        case SM_PAT_REFNAME:
        case SM_PAT_CAPTURE:
        case SM_PAT_CAPTURE_FN:
        case SM_PAT_USERCALL:
        case SM_PAT_CAPTURE_FN_ARGS:  /* EM-7: a[0].s = fname */
        case SM_PAT_USERCALL_ARGS:    /* EM-7: a[0].s = fname */
        case SM_EXEC_STMT:
        case SM_CALL:
            if (ins->a[0].s) strtab_intern(ins->a[0].s);
            break;
        default:
            break;
        }
        /* a[2].s: only SM_PAT_CAPTURE_FN / SM_PAT_USERCALL carry name lists */
        switch (ins->op) {
        case SM_PAT_CAPTURE_FN:
        case SM_PAT_USERCALL:
            if (ins->a[2].s) strtab_intern(ins->a[2].s);
            break;
        default:
            break;
        }
    }
}

/* Emit .section .rodata with all interned strings as .Lstr_N: .string "..." */
static int strtab_emit_rodata(FILE *out)
{
    if (g_strtab_n == 0) return 0;
    if (fputs("\t.section .rodata\n", out) == EOF) return -1;
    char esc[1024];
    for (int i = 0; i < g_strtab_n; i++) {
        strtab_escape(esc, sizeof(esc), g_strtab[i].s);
        if (fprintf(out, ".Lstr_%d:\n\t.string %s\n", i, esc) < 0) return -1;
    }
    if (fputs("\t.text\n", out) == EOF) return -1;
    return 0;
}



static int emit_file_header(FILE *out, int count)
{
    /* The .rodata section (string literals) was already emitted by strtab_emit_rodata
     * before this call.  We resume with .text + main prologue. */
    return fprintf(out,
        "# -----------------------------------------------------------------------\n"
        "# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-6)\n"
        "# %d SM instructions. Links against libscrip_rt.so.\n"
        "# Architecture: two emitters -- SM straight-line via sm_macros.s\n"
        "#   macros (inline x86); BB boxes via emit_bb_box() one-proc-per-box.\n"
        "# See archive/EMITTER-MODE4-ARCH.md for the full design.\n"
        "# -----------------------------------------------------------------------\n"
        "\t.intel_syntax noprefix\n"
        "# Include SM opcode macro library (one macro per opcode group)\n"
        "# .include \"sm_macros.s\"  # assembled separately; macros used by name below\n"
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
    /* SM_HALT: call scrip_rt_halt_tos() which safe-pops TOS as rc
     * if it's DT_I, else uses 0.  This serves both the synthetic
     * EM-2 test (PUSH_LIT_I 42 + HALT → rc=42) and real programs
     * (HALT with non-int TOS → rc=0, matching the interpreter). */
    return sm_line(out, "", "call    scrip_rt_halt_tos@PLT", "# SM_HALT");
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
    const char *s    = ins->a[0].s ? ins->a[0].s : "";
    int64_t     slen = ins->a[1].i;
    char lbl[32], act[80], preview[STR_PREVIEW_MAX + 8], goto_col[STR_PREVIEW_MAX + 16];
    strtab_label(lbl, sizeof(lbl), s);
    render_str_preview(preview, sizeof(preview), s, (int)slen);
    snprintf(goto_col, sizeof(goto_col), "# str=%s", preview);
    /* RIP-relative LEA: PIC-correct, works in -no-pie and PIC binaries. */
    snprintf(act, sizeof(act), "lea     rdi, [rip + %s]", lbl);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    snprintf(act, sizeof(act), "mov     esi, %d", (int)slen);
    if (sm_line(out, "", act, "# slen") < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_push_str@PLT", "");
}

static int emit_sm_push_var(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    char lbl[32], act[80], goto_col[80];
    strtab_label(lbl, sizeof(lbl), name);
    snprintf(act, sizeof(act), "lea     rdi, [rip + %s]", lbl);
    snprintf(goto_col, sizeof(goto_col), "# var=%s", name);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_nv_get@PLT", "# SM_PUSH_VAR -> TOS");
}

static int emit_sm_store_var(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    char lbl[32], act[80], goto_col[80];
    strtab_label(lbl, sizeof(lbl), name);
    snprintf(act, sizeof(act), "lea     rdi, [rip + %s]", lbl);
    snprintf(goto_col, sizeof(goto_col), "# store -> %s", name);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_nv_set@PLT", "# SM_STORE_VAR pop TOS");
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
 * EM-5: SM_PUSH_CHUNK / SM_CALL_CHUNK / SM_RETURN
 *
 * SM_PUSH_CHUNK pushes a DT_E chunk descriptor onto the SM value stack.
 *   a[0].i = entry_pc   a[1].i = arity
 * Codegen: scrip_rt_push_chunk_descr(entry_pc, arity).  The runtime
 * stores it as a DESCR_t { v=DT_E, slen=arity, i=entry_pc }
 * so a downstream SM_CALL "EVAL" / sm_call_chunk path can find it.
 *
 * SM_CALL_CHUNK is a baked direct call.  a[0].i = entry_pc resolves at
 * emit-time to the .Lpc<entry_pc> label that emit_pc_label has already
 * planted at every PC.  The native CALL pushes the return address on
 * the host stack; SM_RETURN's RET pops it.  The SM value stack lives
 * inside libscrip_rt.so and is shared across the call -- the chunk
 * pushes its result onto the same stack the caller will read from.
 *
 * Honest deviation from the interpreter:  sm_interp.c's SM_CALL_CHUNK
 * snapshots the caller's value stack to a heap buffer, runs the chunk
 * on an empty stack, then restores + appends the result.  The mode-4
 * emitter does NOT do this.  Rationale:
 *   (1) For EM-5's gate program (single chunk pushing a single int and
 *       returning), shared-stack call/ret is byte-correct.
 *   (2) Stack-discipline violations in chunk bodies are bugs in the
 *       lowerer, not the emitter; if the lowerer gets it right we
 *       don't need to defensively snapshot.
 *   (3) When SM_SUSPEND/SM_RESUME land in EM-10 we'll need a full
 *       coexpression record anyway -- the snapshot machinery moves
 *       there, not here.
 * If a future rung surfaces a real test case that needs the
 * snapshot-and-restore semantics, we revisit.
 *
 * SM_RETURN_S / SM_RETURN_F / SM_FRETURN[_S/_F] / SM_NRETURN[_S/_F]
 * are NOT yet handled by the emitter.  They fall through to
 * emit_sm_unhandled and produce a runtime trap if executed.  The
 * tracked .s files for the demo programs assemble cleanly (the
 * unhandled stub is a real call instruction); they will not RUN
 * correctly until a near-future rung adds the conditional-return
 * shapes.  EM-5's gate doesn't exercise them.
 * ----------------------------------------------------------------------- */

static int emit_sm_push_chunk(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    int64_t entry_pc = ins->a[0].i;
    int64_t arity    = ins->a[1].i;
    char act[80];
    char goto_col[80];
    snprintf(act, sizeof(act), "movabs  rdi, %" PRId64, entry_pc);
    snprintf(goto_col, sizeof(goto_col), "; chunk entry_pc=%" PRId64, entry_pc);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    snprintf(act, sizeof(act), "mov     esi, %d", (int)arity);
    snprintf(goto_col, sizeof(goto_col), "; arity=%d", (int)arity);
    if (sm_line(out, "", act, goto_col) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_push_chunk_descr@PLT",
                   "; SM_PUSH_CHUNK -> DT_E on TOS");
}

static int emit_sm_call_chunk(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    int target = (int)ins->a[0].i;
    char act[80];
    char goto_col[80];
    /* Baked direct call.  No runtime dispatch -- emit-time pc IS runtime pc. */
    snprintf(act, sizeof(act), "call    .Lpc%d", target);
    snprintf(goto_col, sizeof(goto_col), "; SM_CALL_CHUNK -> pc=%d", target);
    return sm_line(out, "", act, goto_col);
}

static int emit_sm_return(FILE *out, int pc)
{
    (void)pc;
    /* SM_RETURN: native return.  The chunk's last push left the result
     * on the SM value stack inside libscrip_rt.so; the caller reads it
     * after the call returns. */
    return sm_line(out, "", "ret", "; SM_RETURN");
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

    /* SM_STNO is a source-statement boundary marker only.
     * No runtime call needed — &STNO / &STCOUNT support deferred to a
     * later rung.  The major banner above is the complete emission. */
    return 0;
}

/* -----------------------------------------------------------------------
 * EM-6 helper: lea rdi, [rip + .Lstr_N] + PLT call (const char* arg).
 * String must have been interned in the global StrTable by strtab_collect.
 * ----------------------------------------------------------------------- */
static int emit_pat_call_str(FILE *out, const char *fn_plt,
                             const char *ptr_str, const char *annotation)
{
    char lbl[32], act[80], call[64];
    strtab_label(lbl, sizeof(lbl), ptr_str ? ptr_str : "");
    snprintf(act, sizeof(act), "lea     rdi, [rip + %s]", lbl);
    if (sm_line(out, "", act, annotation) < 0) return -1;
    snprintf(call, sizeof(call), "call    %s@PLT", fn_plt);
    return sm_line(out, "", call, "");
}

/* EM-6 helper: lea rdi + mov esi + PLT call (const char*, int). */
static int emit_pat_call_str_int(FILE *out, const char *fn_plt,
                                 const char *ptr_str, int ival,
                                 const char *annotation)
{
    char lbl[32], act[80], call[64];
    strtab_label(lbl, sizeof(lbl), ptr_str ? ptr_str : "");
    snprintf(act, sizeof(act), "lea     rdi, [rip + %s]", lbl);
    if (sm_line(out, "", act, annotation) < 0) return -1;
    snprintf(act, sizeof(act), "mov     esi, %d", ival);
    if (sm_line(out, "", act, "") < 0) return -1;
    snprintf(call, sizeof(call), "call    %s@PLT", fn_plt);
    return sm_line(out, "", call, "");
}

/* EM-6 helper: bare PLT call (nullary pat constructors / vstack-arg ops). */
static int emit_pat_call_0(FILE *out, const char *fn_plt, const char *annot)
{
    char call[64];
    snprintf(call, sizeof(call), "call    %s@PLT", fn_plt);
    return sm_line(out, "", call, annot);
}

/* -----------------------------------------------------------------------
 * BB box emitter (emit_bb_box) -- EM-6
 *
 * Each SM_PAT_* opcode emits a PLT call into libscrip_rt.so that builds
 * the runtime pat-stack (mirrors sm_interp.c SM_PAT_* dispatch).
 * SM_EXEC_STMT is in the main dispatch switch (not here).
 *
 * Architecture note (settled session #67): the four-port alpha/beta/
 * gamma/omega proc layout is for the JIT-run path.  Mode-4 emit builds
 * the pat-stack via sequential scrip_rt_pat_* calls then fires
 * scrip_rt_exec_stmt -- no per-box proc needed for EM-6.
 * ----------------------------------------------------------------------- */

static int emit_bb_box(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    char annot[64];

    switch (ins->op) {

    /* ── Literal-arg primitives ──────────────────────────────────── */
    case SM_PAT_LIT: {
        const char *s = ins->a[0].s ? ins->a[0].s : "";
        snprintf(annot, sizeof(annot), "# PAT_LIT str=\"%.30s\"", s);
        return emit_pat_call_str(out, "scrip_rt_pat_lit", s, annot);
    }
    case SM_PAT_REFNAME: {
        const char *s = ins->a[0].s ? ins->a[0].s : "";
        snprintf(annot, sizeof(annot), "# PAT_REFNAME var=%s", s);
        return emit_pat_call_str(out, "scrip_rt_pat_refname", s, annot);
    }

    /* ── Charset / integer-from-vstack primitives ───────────────── */
    case SM_PAT_SPAN:    return emit_pat_call_0(out, "scrip_rt_pat_span",    "# PAT_SPAN");
    case SM_PAT_BREAK:   return emit_pat_call_0(out, "scrip_rt_pat_break",   "# PAT_BREAK");
    case SM_PAT_ANY:     return emit_pat_call_0(out, "scrip_rt_pat_any",     "# PAT_ANY");
    case SM_PAT_NOTANY:  return emit_pat_call_0(out, "scrip_rt_pat_notany",  "# PAT_NOTANY");
    case SM_PAT_LEN:     return emit_pat_call_0(out, "scrip_rt_pat_len",     "# PAT_LEN");
    case SM_PAT_POS:     return emit_pat_call_0(out, "scrip_rt_pat_pos",     "# PAT_POS");
    case SM_PAT_RPOS:    return emit_pat_call_0(out, "scrip_rt_pat_rpos",    "# PAT_RPOS");
    case SM_PAT_TAB:     return emit_pat_call_0(out, "scrip_rt_pat_tab",     "# PAT_TAB");
    case SM_PAT_RTAB:    return emit_pat_call_0(out, "scrip_rt_pat_rtab",    "# PAT_RTAB");

    /* ── Nullary constructors ────────────────────────────────────── */
    case SM_PAT_ARB:     return emit_pat_call_0(out, "scrip_rt_pat_arb",     "# PAT_ARB");
    case SM_PAT_REM:     return emit_pat_call_0(out, "scrip_rt_pat_rem",     "# PAT_REM");
    case SM_PAT_FENCE:   return emit_pat_call_0(out, "scrip_rt_pat_fence",   "# PAT_FENCE");
    case SM_PAT_FAIL:    return emit_pat_call_0(out, "scrip_rt_pat_fail",    "# PAT_FAIL");
    case SM_PAT_ABORT:   return emit_pat_call_0(out, "scrip_rt_pat_abort",   "# PAT_ABORT");
    case SM_PAT_SUCCEED: return emit_pat_call_0(out, "scrip_rt_pat_succeed", "# PAT_SUCCEED");
    case SM_PAT_BAL:     return emit_pat_call_0(out, "scrip_rt_pat_bal",     "# PAT_BAL");
    case SM_PAT_EPS:     return emit_pat_call_0(out, "scrip_rt_pat_eps",     "# PAT_EPS");

    /* ── Pat-stack combinators ───────────────────────────────────── */
    case SM_PAT_ARBNO:   return emit_pat_call_0(out, "scrip_rt_pat_arbno",   "# PAT_ARBNO");
    case SM_PAT_FENCE1:  return emit_pat_call_0(out, "scrip_rt_pat_fence1",  "# PAT_FENCE1");
    case SM_PAT_CAT:     return emit_pat_call_0(out, "scrip_rt_pat_cat",     "# PAT_CAT");
    case SM_PAT_ALT:     return emit_pat_call_0(out, "scrip_rt_pat_alt",     "# PAT_ALT");

    /* ── Deref / boxval ─────────────────────────────────────────── */
    case SM_PAT_DEREF:   return emit_pat_call_0(out, "scrip_rt_pat_deref",   "# PAT_DEREF");
    case SM_PAT_BOXVAL:  return emit_pat_call_0(out, "scrip_rt_pat_boxval",  "# PAT_BOXVAL");

    /* ── Capture ─────────────────────────────────────────────────── */
    case SM_PAT_CAPTURE: {
        const char *vname = ins->a[0].s ? ins->a[0].s : "";
        int kind = (int)ins->a[1].i;
        snprintf(annot, sizeof(annot), "# PAT_CAPTURE %s kind=%d", vname, kind);
        return emit_pat_call_str_int(out, "scrip_rt_pat_capture",
                                     vname, kind, annot);
    }

    default: {
        /* SM_PAT_CAPTURE_FN, SM_PAT_USERCALL, etc. -- unhandled in EM-6. */
        char act[80];
        snprintf(act, sizeof(act), "mov     edi, %d", (int)ins->op);
        if (sm_line(out, "", act, "# UNHANDLED SM_PAT_* (not baked EM-6)") < 0)
            return -1;
        return sm_line(out, "", "call    scrip_rt_unhandled_op@PLT", "");
    }
    }
}

/* -----------------------------------------------------------------------
 * EM-7 emitters: SM_CALL, SM_CONCAT, SM_PUSH_NULL, SM_COERCE_NUM,
 *                SM_RETURN_S/F, SM_FRETURN[_S/_F], SM_NRETURN[_S/_F],
 *                SM_PAT_CAPTURE_FN_ARGS, SM_PAT_USERCALL_ARGS.
 * ----------------------------------------------------------------------- */

/* SM_CONCAT: pop right then left; push CONCAT result.  All in libscrip_rt. */
static int emit_sm_concat(FILE *out, int pc)
{
    (void)pc;
    return sm_line(out, "", "call    scrip_rt_concat@PLT", "# SM_CONCAT");
}

/* SM_PUSH_NULL: push null (empty-string) descriptor; sets last_ok=1. */
static int emit_sm_push_null(FILE *out, int pc)
{
    (void)pc;
    return sm_line(out, "", "call    scrip_rt_push_null@PLT", "# SM_PUSH_NULL");
}

/* SM_COERCE_NUM: unary +; coerce string→int/real if needed. */
static int emit_sm_coerce_num(FILE *out, int pc)
{
    (void)pc;
    return sm_line(out, "", "call    scrip_rt_coerce_num@PLT", "# SM_COERCE_NUM");
}

/* SM_CALL: general function call.  All dispatch (pseudo-calls, builtins,
 * user-defined) lives in scrip_rt_call(name, nargs).
 *   a[0].s = function name (interned in strtab)
 *   a[1].i = nargs                                                       */
static int emit_sm_call(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    const char *name = ins->a[0].s ? ins->a[0].s : "";
    int         nargs = (int)ins->a[1].i;
    char lbl[32], act[96], annot[80];
    strtab_label(lbl, sizeof(lbl), name);
    snprintf(act,   sizeof(act),   "lea     rdi, [rip + %s]", lbl);
    snprintf(annot, sizeof(annot), "# fname=\"%s\"", name);
    if (sm_line(out, "", act, annot) < 0) return -1;
    snprintf(act,   sizeof(act),   "mov     esi, %d", nargs);
    snprintf(annot, sizeof(annot), "# nargs=%d", nargs);
    if (sm_line(out, "", act, annot) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_call@PLT", "# SM_CALL");
}

/* SM_RETURN_S / SM_RETURN_F / SM_FRETURN[_S/_F] / SM_NRETURN[_S/_F].
 *
 * Idiom:  call scrip_rt_do_return(kind, cond)  → eax = 1 if return fires.
 *         test eax, eax  → jz .skip<pc>  (if 0, fall through)
 *         ret
 *      .skip<pc>:        (next instruction)
 *
 * For unconditional plain SM_RETURN, emit_sm_return already handles it
 * (pure native ret).  This handler covers the 8 conditional variants. */
static int emit_sm_return_variant(FILE *out, sm_opcode_t op, int pc)
{
    int kind = 0;  /* RETURN */
    if (op == SM_FRETURN || op == SM_FRETURN_S || op == SM_FRETURN_F) kind = 1;
    if (op == SM_NRETURN || op == SM_NRETURN_S || op == SM_NRETURN_F) kind = 2;

    int cond = 0;  /* unconditional */
    if (op == SM_RETURN_S || op == SM_FRETURN_S || op == SM_NRETURN_S) cond = 1;
    if (op == SM_RETURN_F || op == SM_FRETURN_F || op == SM_NRETURN_F) cond = 2;

    char act[96], annot[64];
    snprintf(act,   sizeof(act),   "mov     edi, %d", kind);
    snprintf(annot, sizeof(annot), "# kind=%d (0=RET 1=FRET 2=NRET)", kind);
    if (sm_line(out, "", act, annot) < 0) return -1;
    snprintf(act,   sizeof(act),   "mov     esi, %d", cond);
    snprintf(annot, sizeof(annot), "# cond=%d (0=uncon 1=:S 2=:F)", cond);
    if (sm_line(out, "", act, annot) < 0) return -1;
    if (sm_line(out, "", "call    scrip_rt_do_return@PLT", sm_opcode_name(op)) < 0) return -1;
    if (sm_line(out, "", "test    eax, eax",                "# fire?") < 0) return -1;
    char skip_act[64];
    snprintf(skip_act, sizeof(skip_act), "jz      .Lretskip_%d", pc);
    if (sm_line(out, "", skip_act,        "# no-fire: fall through") < 0) return -1;
    if (sm_line(out, "", "ret",                              "# fire: native return") < 0) return -1;
    char lbl[64];
    snprintf(lbl, sizeof(lbl), ".Lretskip_%d:", pc);
    if (fprintf(out, "%s\n", lbl) < 0) return -1;
    return 0;
}

/* SM_PAT_CAPTURE_FN_ARGS:  . *fn(args) / $ *fn(args)  — pops nargs+child.
 *   a[0].s = fname; a[1].i = is_imm; a[2].i = nargs.  */
static int emit_sm_pat_capture_fn_args(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    const char *fname  = ins->a[0].s ? ins->a[0].s : "";
    int         is_imm = (int)ins->a[1].i;
    int         nargs  = (int)ins->a[2].i;
    char lbl[32], act[96], annot[80];
    strtab_label(lbl, sizeof(lbl), fname);
    snprintf(act,   sizeof(act),   "lea     rdi, [rip + %s]", lbl);
    snprintf(annot, sizeof(annot), "# fname=\"%s\"", fname);
    if (sm_line(out, "", act, annot) < 0) return -1;
    snprintf(act,   sizeof(act),   "mov     esi, %d", is_imm);
    snprintf(annot, sizeof(annot), "# is_imm=%d", is_imm);
    if (sm_line(out, "", act, annot) < 0) return -1;
    snprintf(act,   sizeof(act),   "mov     edx, %d", nargs);
    snprintf(annot, sizeof(annot), "# nargs=%d", nargs);
    if (sm_line(out, "", act, annot) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_pat_capture_fn_args@PLT",
                   "# SM_PAT_CAPTURE_FN_ARGS");
}

/* SM_PAT_USERCALL_ARGS:  *fn(args) — pops nargs (no child).
 *   a[0].s = fname; a[1].i = nargs.  */
static int emit_sm_pat_usercall_args(FILE *out, const SM_Instr *ins, int pc)
{
    (void)pc;
    const char *fname = ins->a[0].s ? ins->a[0].s : "";
    int         nargs = (int)ins->a[1].i;
    char lbl[32], act[96], annot[80];
    strtab_label(lbl, sizeof(lbl), fname);
    snprintf(act,   sizeof(act),   "lea     rdi, [rip + %s]", lbl);
    snprintf(annot, sizeof(annot), "# fname=\"%s\"", fname);
    if (sm_line(out, "", act, annot) < 0) return -1;
    snprintf(act,   sizeof(act),   "mov     esi, %d", nargs);
    snprintf(annot, sizeof(annot), "# nargs=%d", nargs);
    if (sm_line(out, "", act, annot) < 0) return -1;
    return sm_line(out, "", "call    scrip_rt_pat_usercall_args@PLT",
                   "# SM_PAT_USERCALL_ARGS");
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

    /* EM-6: collect all string literals and variable names into the string
     * table, then emit them in .section .rodata before .text.  This makes
     * every string pointer in the emitted binary a RIP-relative reference
     * to a .Lstr_N label rather than an in-process pointer from the emitter. */
    strtab_collect(prog);
    if (strtab_emit_rodata(out) != 0) return -1;

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

            /* EM-5: chunk descriptor push, baked-direct chunk call, return. */
            case SM_PUSH_CHUNK:   rc = emit_sm_push_chunk(out, ins, pc); break;
            case SM_CALL_CHUNK:   rc = emit_sm_call_chunk(out, ins, pc); break;
            case SM_RETURN:       rc = emit_sm_return(out, pc);          break;

            /* EM-7: SM_CALL (general) + SM_CONCAT + SM_PUSH_NULL + SM_COERCE_NUM
             * + conditional return variants + SM_PAT_*_ARGS. */
            case SM_CALL:         rc = emit_sm_call(out, ins, pc);       break;
            case SM_CONCAT:       rc = emit_sm_concat(out, pc);          break;
            case SM_PUSH_NULL:    rc = emit_sm_push_null(out, pc);       break;
            case SM_COERCE_NUM:   rc = emit_sm_coerce_num(out, pc);      break;
            case SM_FRETURN:
            case SM_NRETURN:
            case SM_RETURN_S:
            case SM_RETURN_F:
            case SM_FRETURN_S:
            case SM_FRETURN_F:
            case SM_NRETURN_S:
            case SM_NRETURN_F:    rc = emit_sm_return_variant(out, ins->op, pc); break;
            case SM_PAT_CAPTURE_FN_ARGS:
                                  rc = emit_sm_pat_capture_fn_args(out, ins, pc); break;
            case SM_PAT_USERCALL_ARGS:
                                  rc = emit_sm_pat_usercall_args(out, ins, pc);   break;

            /* SM_STNO -- statement boundary; emits major page break w/ source. */
            case SM_STNO:         rc = emit_sm_stno(out, ins, pc,
                                                    sl_loaded ? &sl : NULL); break;

            /* EM-6: SM_PAT_* opcodes — all route through emit_bb_box()
             * which dispatches per-opcode to scrip_rt_pat_*@PLT.
             * SM_EXEC_STMT fires scrip_rt_exec_stmt with subj_name + has_repl. */
            case SM_PAT_LIT:
            case SM_PAT_ANY:
            case SM_PAT_NOTANY:
            case SM_PAT_SPAN:
            case SM_PAT_BREAK:
            case SM_PAT_LEN:
            case SM_PAT_POS:
            case SM_PAT_RPOS:
            case SM_PAT_TAB:
            case SM_PAT_RTAB:
            case SM_PAT_ARB:
            case SM_PAT_ARBNO:
            case SM_PAT_ALT:
            case SM_PAT_CAT:
            case SM_PAT_EPS:
            case SM_PAT_REM:
            case SM_PAT_BAL:
            case SM_PAT_FENCE:
            case SM_PAT_FENCE1:
            case SM_PAT_ABORT:
            case SM_PAT_FAIL:
            case SM_PAT_SUCCEED:
            case SM_PAT_CAPTURE:
            case SM_PAT_DEREF:
            case SM_PAT_REFNAME:
            case SM_PAT_BOXVAL:
                                  rc = emit_bb_box(out, ins, pc);         break;

            case SM_EXEC_STMT: {
                /* scrip_rt_exec_stmt(const char *subj_name, int has_repl)
                 * a[0].s = subject variable name for write-back (NULL = anonymous)
                 * a[1].i = has_repl flag */
                const char *sname    = ins->a[0].s;
                int         has_repl = (int)ins->a[1].i;
                char lbl[32], act[80], annot[64];
                if (sname) {
                    strtab_label(lbl, sizeof(lbl), sname);
                    snprintf(act, sizeof(act), "lea     rdi, [rip + %s]", lbl);
                    snprintf(annot, sizeof(annot), "# subj=%s", sname);
                } else {
                    snprintf(act, sizeof(act), "xor     edi, edi");
                    snprintf(annot, sizeof(annot), "# subj=NULL (anonymous)");
                }
                if (sm_line(out, "", act, annot) < 0) { rc = -1; break; }
                snprintf(act, sizeof(act), "mov     esi, %d", has_repl);
                if (sm_line(out, "", act, has_repl ? "# has_repl=1" : "# has_repl=0") < 0) { rc = -1; break; }
                rc = sm_line(out, "", "call    scrip_rt_exec_stmt@PLT", "# SM_EXEC_STMT");
                break;
            }

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
