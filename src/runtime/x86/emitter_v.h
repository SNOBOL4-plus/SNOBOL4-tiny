/*
 * emitter_v.h — Emitter vtable for dual-mode x86-64 code generation
 *
 * One vtable, two implementations:
 *   emitter_text_new(FILE*)    — writes GAS text directives
 *   emitter_binary_new(buf, size) — writes raw bytes into bb_pool buffer
 *
 * The walker (bb_flat.c, bb_build.c, sm_codegen_x64_emit.c) takes
 * emitter_v * and is generic over the emission target.  No
 * if (bb_emit_mode == EMIT_TEXT) branches outside emitter_text.c /
 * emitter_binary.c.
 *
 * This is the contract the Milestone-3 matrix (x86/JVM/.NET/WASM/JS)
 * and the Snocone bootstrap (EXPRESSION as templates, multiple backends
 * sharing one walker) both require.  Each column of that matrix is one
 * emitter_v instance; the walker is unchanged across columns.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Sprint:  EM-7b' / GOAL-MODE4-EMIT
 */

#ifndef EMITTER_V_H
#define EMITTER_V_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "bb_emit.h"   /* bb_label_t, bb_patch_kind_t, bb_buf_t */

/* ── jump kind ────────────────────────────────────────────────────────────── */

typedef enum {
    JMP_JMP,     /* unconditional */
    JMP_JE,      /* je  (ZF=1) */
    JMP_JNE,     /* jne (ZF=0) */
    JMP_JL,      /* jl  (SF≠OF) */
    JMP_JGE,     /* jge (SF=OF) */
    JMP_JG,      /* jg  (ZF=0 && SF=OF) */
} jmp_kind_t;

/* ── vtable ───────────────────────────────────────────────────────────────── */

typedef struct emitter_v emitter_v;

struct emitter_v {
    /*
     * emit_bytes — write N raw bytes.
     *   TEXT:   emits ".byte 0xNN[, 0xNN]...\n" lines (one per call).
     *   BINARY: writes bytes into the buffer, advances pos.
     *   anno:   optional inline comment suffix (TEXT only, may be NULL).
     */
    void (*emit_bytes)(emitter_v *e, const uint8_t *bs, int n,
                       const char *anno_or_null);

    /*
     * label_define — plant a label at the current position.
     *   TEXT:   emits "name:\n".
     *   BINARY: records offset; patches all pending forward refs.
     */
    void (*label_define)(emitter_v *e, bb_label_t *lbl);

    /*
     * emit_jmp — emit a jump instruction of the given kind to target.
     *   TEXT:   emits "    jmp/je/... name\n" (symbolic, assembler resolves).
     *   BINARY: emits opcode + rel8 or rel32 displacement with forward-ref
     *           tracking via the emitter's own patch list.
     */
    void (*emit_jmp)(emitter_v *e, bb_label_t *target, jmp_kind_t kind);

    /*
     * emit_call_rax — emit "call rax" (indirect call through rax).
     *   TEXT:   emits "    call    rax\n"  (with .intel_syntax in effect).
     *   BINARY: FF D0.
     */
    void (*emit_call_rax)(emitter_v *e);

    /*
     * emit_call_imm64 — load addr into rax, call rax.
     *   TEXT:   emits "    mov     rax, 0x<addr>\n    call    rax\n" + anno.
     *   BINARY: REX.W B8 <imm64> + FF D0.
     *   anno:   optional annotation (TEXT only, may be NULL).
     */
    void (*emit_call_imm64)(emitter_v *e, uint64_t addr,
                            const char *anno_or_null);

    /*
     * section_text — emit a .text section directive.
     *   TEXT:   emits ".text\n".
     *   BINARY: no-op (we are already writing into an RX-destined buffer).
     */
    void (*section_text)(emitter_v *e);

    /*
     * global_sym — emit a .global directive for a symbol name.
     *   TEXT:   emits ".global name\n".
     *   BINARY: no-op (symbols are linker-level; binary mode uses offsets).
     */
    void (*global_sym)(emitter_v *e, const char *name);

    /*
     * intel_syntax — emit an .intel_syntax noprefix directive.
     *   TEXT:   emits ".intel_syntax noprefix\n".
     *   BINARY: no-op.
     */
    void (*intel_syntax)(emitter_v *e);

    /*
     * pos — current emission cursor in bytes.
     *   TEXT:   returns an informational byte count (useful for size
     *           estimates; not used for label patching in text mode).
     *   BINARY: returns bb_emit_pos — the actual buffer write position.
     */
    int (*pos)(emitter_v *e);

    /*
     * fprintf_raw — emit arbitrary formatted text (TEXT mode only).
     *   BINARY: no-op.
     *   Used for section headers, banners, comment blocks that have no
     *   binary counterpart.
     */
    void (*fprintf_raw)(emitter_v *e, const char *fmt, ...);

    /* Implementation-private state */
    void *ctx;
};

/* ── constructors ─────────────────────────────────────────────────────────── */

/*
 * emitter_text_new — create a text-mode emitter writing to `out`.
 * Pass NULL for out to default to stdout.
 * Caller owns the emitter; free with emitter_free().
 */
emitter_v *emitter_text_new(FILE *out);

/*
 * emitter_binary_new — create a binary-mode emitter writing into `buf[0..size)`.
 * The emitter does NOT own buf; caller must call bb_emit_end_v() then bb_seal().
 * Free with emitter_free() after sealing.
 */
emitter_v *emitter_binary_new(bb_buf_t buf, int size);

/*
 * emitter_free — release resources.  Does not free buf or close FILE*.
 */
void emitter_free(emitter_v *e);

/*
 * emitter_end — binary mode: resolve pending patches, return bytes written.
 *   TEXT mode: returns the informational byte counter.
 *   Aborts on unresolved forward references (binary mode only).
 */
int emitter_end(emitter_v *e);

/* ── convenience wrappers ─────────────────────────────────────────────────── */
/*
 * ev_byte1..ev_byte4: emit 1–4 literal bytes.  These replace the variadic
 * EV_BYTES macro (which hit C89 compound-literal issues in some build
 * configurations).  bb_flat.c uses up to 4 bytes per call site.
 */
static inline void ev_byte1(emitter_v *e, uint8_t b0)
{ uint8_t bs[1] = {b0}; e->emit_bytes(e, bs, 1, NULL); }

static inline void ev_byte2(emitter_v *e, uint8_t b0, uint8_t b1)
{ uint8_t bs[2] = {b0,b1}; e->emit_bytes(e, bs, 2, NULL); }

static inline void ev_byte3(emitter_v *e, uint8_t b0, uint8_t b1, uint8_t b2)
{ uint8_t bs[3] = {b0,b1,b2}; e->emit_bytes(e, bs, 3, NULL); }

static inline void ev_byte4(emitter_v *e, uint8_t b0, uint8_t b1,
                            uint8_t b2, uint8_t b3)
{ uint8_t bs[4] = {b0,b1,b2,b3}; e->emit_bytes(e, bs, 4, NULL); }

/* EV_BYTES(e, b0 [,b1 [,b2 [,b3]]]) — dispatches to ev_byteN */
/* Use the inline helpers directly in emitter_v consumer code;
 * or use the EV_BYTES_RAW helper for a slice of a uint8_t array. */
#define EV_BYTES_RAW(e, arr, n)  (e)->emit_bytes((e), (arr), (n), NULL)

#define EV_LABEL(e, lbl)         (e)->label_define((e), (lbl))
#define EV_JMP(e, lbl, kind)     (e)->emit_jmp((e), (lbl), (kind))
#define EV_CALL_RAX(e)           (e)->emit_call_rax((e))
#define EV_POS(e)                (e)->pos((e))
#define EV_GLOBAL(e, name)       (e)->global_sym((e), (name))
#define EV_TEXT(e, ...)          (e)->fprintf_raw((e), __VA_ARGS__)

#endif /* EMITTER_V_H */
