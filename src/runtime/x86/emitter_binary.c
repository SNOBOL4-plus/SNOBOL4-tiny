/*
 * emitter_binary.c — binary-mode implementation of emitter_v
 *
 * Writes raw x86-64 bytes into a bb_pool buffer.  All existing
 * bb_emit_* global-state functions are used under the hood — the vtable
 * is a thin wrapper that routes through the proven bb_emit.c machinery.
 * The global state (bb_emit_buf, bb_emit_pos, bb_emit_size, bb_patch_list)
 * remains for now; the vtable boundary prevents callers from caring.
 *
 * A future cleanup rung can move that state into a per-emitter struct and
 * delete the globals; for now the single-emitter-at-a-time discipline
 * (which bb_flat.c and bb_build.c have always relied on) is preserved.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Sprint:  EM-7b' / GOAL-MODE4-EMIT
 */

#include "emitter_v.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

/* ── vtable implementations ───────────────────────────────────────────────── */

static void binary_emit_bytes(emitter_v *e, const uint8_t *bs, int n,
                              const char *anno)
{
    (void)e; (void)anno;
    for (int i = 0; i < n; i++)
        bb_emit_byte(bs[i]);
}

static void binary_label_define(emitter_v *e, bb_label_t *lbl)
{
    (void)e;
    /* Make sure mode is BINARY before calling bb_label_define */
    bb_emit_mode_t saved = bb_emit_mode;
    bb_emit_mode = EMIT_BINARY;
    bb_label_define(lbl);
    bb_emit_mode = saved;
}

static const uint8_t jmp_opcodes_rel8[6]  = { 0xEB, 0x74, 0x75, 0x7C, 0x7D, 0x7F };
static const uint8_t jmp_opcodes_rel32[6][2] = {
    { 0xE9, 0x00 },       /* JMP_JMP:  E9 <rel32> */
    { 0x0F, 0x84 },       /* JMP_JE:   0F 84 <rel32> */
    { 0x0F, 0x85 },       /* JMP_JNE:  0F 85 <rel32> */
    { 0x0F, 0x8C },       /* JMP_JL:   0F 8C <rel32> */
    { 0x0F, 0x8D },       /* JMP_JGE:  0F 8D <rel32> */
    { 0x0F, 0x8F },       /* JMP_JG:   0F 8F <rel32> */
};

static void binary_emit_jmp(emitter_v *e, bb_label_t *target, jmp_kind_t kind)
{
    (void)e;
    /* Always use rel32 for safety (rel8 might overflow for forward refs) */
    int k = (int)kind;
    if (k < 0 || k > 5) k = 0;
    if (k == 0) {
        /* unconditional: E9 <rel32> */
        bb_emit_byte(0xE9);
        bb_emit_patch_rel32(target);
    } else {
        /* two-byte opcode: 0F xx <rel32> */
        bb_emit_byte(jmp_opcodes_rel32[k][0]);
        bb_emit_byte(jmp_opcodes_rel32[k][1]);
        bb_emit_patch_rel32(target);
    }
}

static void binary_call_rax(emitter_v *e)
{
    (void)e;
    bb_emit_byte(0xFF); bb_emit_byte(0xD0);
}

static void binary_call_imm64(emitter_v *e, uint64_t addr, const char *anno)
{
    (void)e; (void)anno;
    /* REX.W B8 <imm64> : mov rax, addr */
    bb_emit_byte(0x48); bb_emit_byte(0xB8);
    bb_emit_u64(addr);
    /* FF D0 : call rax */
    bb_emit_byte(0xFF); bb_emit_byte(0xD0);
}

static void binary_section_text(emitter_v *e)  { (void)e; /* no-op */ }
static void binary_global_sym(emitter_v *e, const char *name)
                                                { (void)e; (void)name; /* no-op */ }
static void binary_intel_syntax(emitter_v *e)  { (void)e; /* no-op */ }

static int binary_pos(emitter_v *e)
{
    (void)e;
    return bb_emit_pos;
}

static void binary_fprintf_raw(emitter_v *e, const char *fmt, ...)
{
    /* no-op in binary mode */
    (void)e; (void)fmt;
}

/* ── constructor ──────────────────────────────────────────────────────────── */

static const emitter_v binary_vtable_template = {
    .emit_bytes    = binary_emit_bytes,
    .label_define  = binary_label_define,
    .emit_jmp      = binary_emit_jmp,
    .emit_call_rax = binary_call_rax,
    .emit_call_imm64 = binary_call_imm64,
    .section_text  = binary_section_text,
    .global_sym    = binary_global_sym,
    .intel_syntax  = binary_intel_syntax,
    .pos           = binary_pos,
    .fprintf_raw   = binary_fprintf_raw,
    .ctx           = NULL,
};

emitter_v *emitter_binary_new(bb_buf_t buf, int size)
{
    emitter_v *e = malloc(sizeof(emitter_v));
    if (!e) return NULL;
    *e = binary_vtable_template;
    e->ctx = NULL;   /* state lives in bb_emit.c globals for now */

    /* Attach the global emit session */
    bb_emit_mode = EMIT_BINARY;
    bb_emit_begin(buf, size);
    return e;
}
