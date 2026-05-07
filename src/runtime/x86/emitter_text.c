/*
 * emitter_text.c — GAS/text-mode implementation of emitter_v
 *
 * emit_insn renders each bb_insn_desc_t as a single readable GAS line
 * using Intel syntax (matching .intel_syntax noprefix in the .s preamble).
 * No .byte walls — every instruction is a real mnemonic.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Sprint:  EM-7b'' / GOAL-MODE4-EMIT
 */

#include "emitter_v.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct { FILE *out; int pos; } text_ctx_t;
#define CTX(e) ((text_ctx_t *)((e)->ctx))
static FILE *outf(emitter_v *e) { FILE *f = CTX(e)->out; return f ? f : stdout; }

/* ── emit_insn: one GAS line per instruction ─────────────────────────────── */

static void text_emit_insn(emitter_v *e, const bb_insn_desc_t *d)
{
    FILE *f = outf(e);
    uint64_t a0 = d->a0;
    uint32_t a1 = d->a1;
    uint8_t  a2 = d->a2;

    switch (d->kind) {
    /* 64-bit reg ← imm64 */
    case BB_INSN_MOV_R10_IMM64: fprintf(f,"    mov     r10, 0x%llx\n",(unsigned long long)a0); CTX(e)->pos+=10; break;
    case BB_INSN_MOV_RAX_IMM64: fprintf(f,"    mov     rax, 0x%llx\n",(unsigned long long)a0); CTX(e)->pos+=10; break;
    case BB_INSN_MOV_RDI_IMM64: fprintf(f,"    mov     rdi, 0x%llx\n",(unsigned long long)a0); CTX(e)->pos+=10; break;
    case BB_INSN_MOV_RSI_IMM64: fprintf(f,"    mov     rsi, 0x%llx\n",(unsigned long long)a0); CTX(e)->pos+=10; break;
    case BB_INSN_MOV_RDX_IMM64: fprintf(f,"    mov     rdx, 0x%llx\n",(unsigned long long)a0); CTX(e)->pos+=10; break;
    case BB_INSN_MOV_RCX_IMM64: fprintf(f,"    mov     rcx, 0x%llx\n",(unsigned long long)a0); CTX(e)->pos+=10; break;
    /* 32-bit reg ← imm32 */
    case BB_INSN_MOV_ESI_IMM32: fprintf(f,"    mov     esi, %u\n",  a1); CTX(e)->pos+=5; break;
    case BB_INSN_MOV_EAX_IMM32: fprintf(f,"    mov     eax, %u\n",  a1); CTX(e)->pos+=5; break;
    case BB_INSN_ADD_EAX_IMM32: fprintf(f,"    add     eax, %u\n",  a1); CTX(e)->pos+=5; break;
    case BB_INSN_SUB_EAX_IMM32: fprintf(f,"    sub     eax, %u\n",  a1); CTX(e)->pos+=5; break;
    case BB_INSN_CMP_EAX_IMM32: fprintf(f,"    cmp     eax, %u\n",  a1); CTX(e)->pos+=5; break;
    case BB_INSN_CMP_ESI_IMM8:  fprintf(f,"    cmp     esi, %u\n", (unsigned)a2); CTX(e)->pos+=3; break;
    /* memory loads */
    case BB_INSN_MOV_EAX_RCXMEM:   fprintf(f,"    mov     eax, [rcx]\n"); CTX(e)->pos+=2; break;
    case BB_INSN_MOV_RAX_RCXMEM:   fprintf(f,"    mov     rax, [rcx]\n"); CTX(e)->pos+=3; break;
    case BB_INSN_CMP_EAX_RCXMEM:   fprintf(f,"    cmp     eax, [rcx]\n"); CTX(e)->pos+=2; break;
    case BB_INSN_MOV_EAX_R10MEM:   fprintf(f,"    mov     eax, [r10]\n"); CTX(e)->pos+=3; break;
    case BB_INSN_MOV_R10MEM_EAX:   fprintf(f,"    mov     [r10], eax\n"); CTX(e)->pos+=3; break;
    /* reg-reg */
    case BB_INSN_MOV_ECX_EAX:      fprintf(f,"    mov     ecx, eax\n"); CTX(e)->pos+=2; break;
    case BB_INSN_MOV_RDI_RAX:      fprintf(f,"    mov     rdi, rax\n"); CTX(e)->pos+=3; break;
    case BB_INSN_MOV_RDX_RAX:      fprintf(f,"    mov     rdx, rax\n"); CTX(e)->pos+=3; break;
    case BB_INSN_CMP_EAX_ECX:      fprintf(f,"    cmp     eax, ecx\n"); CTX(e)->pos+=2; break;
    case BB_INSN_TEST_EAX_EAX:     fprintf(f,"    test    eax, eax\n"); CTX(e)->pos+=2; break;
    case BB_INSN_TEST_RAX_RAX:     fprintf(f,"    test    rax, rax\n"); CTX(e)->pos+=3; break;
    case BB_INSN_XOR_EDX_EDX:      fprintf(f,"    xor     edx, edx\n"); CTX(e)->pos+=2; break;
    case BB_INSN_MOVSXD_RCX_R10MEM:fprintf(f,"    movsxd  rcx, dword ptr [r10]\n"); CTX(e)->pos+=3; break;
    case BB_INSN_LEA_RAX_RAXRCX:   fprintf(f,"    lea     rax, [rax+rcx]\n"); CTX(e)->pos+=4; break;
    /* control */
    case BB_INSN_RET:      fprintf(f,"    ret\n"); CTX(e)->pos+=1; break;
    case BB_INSN_CALL_RAX: fprintf(f,"    call    rax\n"); CTX(e)->pos+=2; break;
    }
}

/* ── label_define ──────────────────────────────────────────────────────────── */
static void text_label_define(emitter_v *e, bb_label_t *lbl)
{ fprintf(outf(e), "%s:\n", lbl->name); }

/* ── emit_jmp ──────────────────────────────────────────────────────────────── */
static void text_emit_jmp(emitter_v *e, bb_label_t *target, jmp_kind_t kind)
{
    FILE *f = outf(e);
    const char *mn[] = {"jmp","je","jne","jl","jge","jg"};
    fprintf(f, "    %-8s%s\n", mn[(int)kind < 6 ? (int)kind : 0], target->name);
    CTX(e)->pos += 6;
}

/* ── global_sym ────────────────────────────────────────────────────────────── */
static void text_global_sym(emitter_v *e, const char *name)
{ fprintf(outf(e), ".global %s\n", name); }

/* ── fprintf_raw ───────────────────────────────────────────────────────────── */
static void text_fprintf_raw(emitter_v *e, const char *fmt, ...)
{ va_list ap; va_start(ap,fmt); vfprintf(outf(e),fmt,ap); va_end(ap); }

/* ── pos ───────────────────────────────────────────────────────────────────── */
static int text_pos(emitter_v *e) { return CTX(e)->pos; }

/* ── constructor ───────────────────────────────────────────────────────────── */
static const emitter_v text_tmpl = {
    .emit_insn    = text_emit_insn,
    .label_define = text_label_define,
    .emit_jmp     = text_emit_jmp,
    .global_sym   = text_global_sym,
    .fprintf_raw  = text_fprintf_raw,
    .pos          = text_pos,
    .ctx          = NULL,
};

emitter_v *emitter_text_new(FILE *out)
{
    emitter_v *e = malloc(sizeof(emitter_v));
    if (!e) return NULL;
    *e = text_tmpl;
    text_ctx_t *ctx = calloc(1, sizeof(text_ctx_t));
    if (!ctx) { free(e); return NULL; }
    ctx->out = out; ctx->pos = 0;
    e->ctx = ctx;
    return e;
}

void emitter_free(emitter_v *e) { if (!e) return; free(e->ctx); free(e); }

int emitter_end(emitter_v *e)
{
    if (!e) return 0;
    if (e->emit_insn == text_emit_insn)
        return CTX(e)->pos;
    return bb_emit_end();
}
