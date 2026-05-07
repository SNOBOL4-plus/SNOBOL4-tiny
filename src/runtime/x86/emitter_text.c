/*
 * emitter_text.c — GAS/text-mode implementation of emitter_v
 *
 * Writes GNU-as directives to a FILE*.  Output is a .s file suitable
 * for `gcc -c` with .intel_syntax noprefix.
 *
 * Labels are symbolic strings; jumps emit "jmp <name>" and the
 * assembler resolves them.  No patch list needed.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Sprint:  EM-7b' / GOAL-MODE4-EMIT
 */

#include "emitter_v.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* ── context ──────────────────────────────────────────────────────────────── */

typedef struct {
    FILE *out;
    int   pos;   /* informational byte counter */
} text_ctx_t;

#define CTX(e) ((text_ctx_t *)((e)->ctx))

/* ── helpers ──────────────────────────────────────────────────────────────── */

static FILE *out_of(emitter_v *e) {
    FILE *f = CTX(e)->out;
    return f ? f : stdout;
}

/* ── vtable implementations ───────────────────────────────────────────────── */

static void text_emit_bytes(emitter_v *e, const uint8_t *bs, int n,
                            const char *anno)
{
    FILE *f = out_of(e);
    /* Emit all bytes as a single .byte directive line */
    fprintf(f, "    .byte ");
    for (int i = 0; i < n; i++) {
        if (i) fprintf(f, ", ");
        fprintf(f, "0x%02x", (unsigned)bs[i]);
    }
    if (anno) fprintf(f, "    # %s", anno);
    fprintf(f, "\n");
    CTX(e)->pos += n;
}

static void text_label_define(emitter_v *e, bb_label_t *lbl)
{
    fprintf(out_of(e), "%s:\n", lbl->name);
}

static void text_emit_jmp(emitter_v *e, bb_label_t *target, jmp_kind_t kind)
{
    FILE *f = out_of(e);
    const char *mn;
    switch (kind) {
    case JMP_JMP: mn = "jmp";  break;
    case JMP_JE:  mn = "je";   break;
    case JMP_JNE: mn = "jne";  break;
    case JMP_JL:  mn = "jl";   break;
    case JMP_JGE: mn = "jge";  break;
    case JMP_JG:  mn = "jg";   break;
    default:      mn = "jmp";  break;
    }
    fprintf(f, "    %-8s%s\n", mn, target->name);
    /* Approximate byte count for a near jump: 2 bytes (rel8) or 6 (rel32).
     * We use 6 conservatively so TEXT pos stays ≥ BINARY pos. */
    CTX(e)->pos += 6;
}

static void text_call_rax(emitter_v *e)
{
    fprintf(out_of(e), "    call    rax\n");
    CTX(e)->pos += 2;
}

static void text_call_imm64(emitter_v *e, uint64_t addr, const char *anno)
{
    FILE *f = out_of(e);
    /* Intel syntax: mov rax, imm64; call rax */
    fprintf(f, "    mov     rax, 0x%llx\n", (unsigned long long)addr);
    fprintf(f, "    call    rax");
    if (anno) fprintf(f, "    # %s", anno);
    fprintf(f, "\n");
    CTX(e)->pos += 12;   /* REX.W B8 <8> + FF D0 = 12 bytes */
}

static void text_section_text(emitter_v *e)
{
    fprintf(out_of(e), ".text\n");
}

static void text_global_sym(emitter_v *e, const char *name)
{
    fprintf(out_of(e), ".global %s\n", name);
}

static void text_intel_syntax(emitter_v *e)
{
    fprintf(out_of(e), ".intel_syntax noprefix\n");
}

static int text_pos(emitter_v *e)
{
    return CTX(e)->pos;
}

static void text_fprintf_raw(emitter_v *e, const char *fmt, ...)
{
    FILE *f = out_of(e);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}

/* ── constructor ──────────────────────────────────────────────────────────── */

static const emitter_v text_vtable_template = {
    .emit_bytes    = text_emit_bytes,
    .label_define  = text_label_define,
    .emit_jmp      = text_emit_jmp,
    .emit_call_rax = text_call_rax,
    .emit_call_imm64 = text_call_imm64,
    .section_text  = text_section_text,
    .global_sym    = text_global_sym,
    .intel_syntax  = text_intel_syntax,
    .pos           = text_pos,
    .fprintf_raw   = text_fprintf_raw,
    .ctx           = NULL,
};

emitter_v *emitter_text_new(FILE *out)
{
    emitter_v *e = malloc(sizeof(emitter_v));
    if (!e) return NULL;
    *e = text_vtable_template;
    text_ctx_t *ctx = calloc(1, sizeof(text_ctx_t));
    if (!ctx) { free(e); return NULL; }
    ctx->out = out;
    ctx->pos = 0;
    e->ctx = ctx;
    return e;
}

/* ── emitter_end (TEXT mode) ──────────────────────────────────────────────── */
/* TEXT mode has no patch list; just return the informational byte count. */
static int text_end(text_ctx_t *ctx) { return ctx->pos; }

/* ── shared emitter_end / emitter_free ────────────────────────────────────── */
/* Defined here because both modes need them; binary.c provides its own
 * binary_end() helper called by emitter_end() via the emitter kind flag. */

void emitter_free(emitter_v *e)
{
    if (!e) return;
    free(e->ctx);
    free(e);
}

int emitter_end(emitter_v *e)
{
    if (!e) return 0;
    /* Distinguish TEXT vs BINARY by the emit_bytes vtable slot —
     * text_emit_bytes is unique to text mode. */
    if (e->emit_bytes == text_emit_bytes) {
        return text_end((text_ctx_t *)e->ctx);
    }
    /* Binary mode: delegate to bb_emit_end() (which resolves patches). */
    return bb_emit_end();
}
