/*
 * bb_flat.c — Flat-Glob Invariant Pattern Emitter (M-DYN-FLAT)
 *
 * Emits an entire invariant PATND_t tree as one contiguous x86-64 blob.
 * All sub-boxes are inlined flat; control flows via direct jmp, never call/ret.
 * r10 = &Δ loaded once at pattern entry; all cursor ops use [r10].
 *
 * EM-7b': Converted to emitter_v * throughout.  The TEXT/BINARY
 * discrimination now lives entirely in emitter_text.c / emitter_binary.c;
 * this file contains zero if (bb_emit_mode == EMIT_TEXT) branches.
 *
 * Public API is unchanged:
 *   bb_build_flat(p)                    — BINARY: allocates bb_pool, seals, returns fn ptr
 *   bb_build_flat_text(p, out, prefix)  — TEXT: writes GAS to `out`
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Sprint:  RT-129 / M-DYN-FLAT (converted EM-7b')
 */

#include "bb_flat.h"
#include "bb_emit.h"
#include "emitter_v.h"
#include "snobol4.h"
#include "bb_box.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define FLAT_BUF_MAX  (16 * 1024)
#define FLAT_DATA_MAX  256

static int g_flat_node_id = 0;

typedef struct { int index; } flat_slot_t;
static int g_flat_slot_count = 0;
static flat_slot_t flat_alloc_slot(void) {
    flat_slot_t s; s.index = g_flat_slot_count++; return s;
}

/* ── low-level byte emitters ─────────────────────────────────────────────── */

static void ev_load_delta(emitter_v *e) {
    ev_byte3(e, 0x41, 0x8B, 0x02);
}
static void ev_store_delta(emitter_v *e) {
    ev_byte3(e, 0x41, 0x89, 0x02);
}
static void ev_add_delta_imm(emitter_v *e, int32_t v) {
    uint32_t u = (uint32_t)v;
    uint8_t bs[5] = { 0x05, (uint8_t)(u), (uint8_t)(u>>8), (uint8_t)(u>>16), (uint8_t)(u>>24) };
    ev_load_delta(e);
    e->emit_bytes(e, bs, 5, NULL);
    ev_store_delta(e);
}
static void ev_sub_delta_imm(emitter_v *e, int32_t v) {
    uint32_t u = (uint32_t)v;
    uint8_t bs[5] = { 0x2D, (uint8_t)(u), (uint8_t)(u>>8), (uint8_t)(u>>16), (uint8_t)(u>>24) };
    ev_load_delta(e);
    e->emit_bytes(e, bs, 5, NULL);
    ev_store_delta(e);
}
static void ev_imm64_bytes(emitter_v *e, uint64_t v) {
    uint8_t bs[8] = {
        (uint8_t)(v), (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24),
        (uint8_t)(v>>32), (uint8_t)(v>>40), (uint8_t)(v>>48), (uint8_t)(v>>56)
    };
    e->emit_bytes(e, bs, 8, NULL);
}
static void ev_load_sigma(emitter_v *e) {
    uint64_t addr = (uint64_t)(uintptr_t)&Σ;
    ev_byte2(e, 0x48, 0xB9); ev_imm64_bytes(e, addr);
    ev_byte3(e, 0x48, 0x8B, 0x01);
}
static void ev_load_omega(emitter_v *e) {
    uint64_t addr = (uint64_t)(uintptr_t)&Σlen;
    ev_byte2(e, 0x48, 0xB9); ev_imm64_bytes(e, addr);
    ev_byte2(e, 0x8B, 0x01);
}
static void ev_sigma_plus_delta(emitter_v *e) {
    ev_load_sigma(e);
    ev_byte3(e, 0x49, 0x63, 0x0A);
    ev_byte4(e, 0x48, 0x8D, 0x04, 0x08);
}
static void ev_load_r10_delta_ptr(emitter_v *e) {
    uint64_t addr = (uint64_t)(uintptr_t)&Δ;
    ev_byte2(e, 0x49, 0xBA); ev_imm64_bytes(e, addr);
}
static void ev_cmp_eax_sigmalen(emitter_v *e) {
    uint64_t addr = (uint64_t)(uintptr_t)&Σlen;
    ev_byte2(e, 0x48, 0xB9); ev_imm64_bytes(e, addr);
    ev_byte2(e, 0x3B, 0x01);
}
static void ev_add_eax_imm32(emitter_v *e, uint32_t v) {
    uint8_t bs[5] = { 0x05, (uint8_t)(v), (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24) };
    e->emit_bytes(e, bs, 5, NULL);
}
static void ev_mov_rdi_imm64(emitter_v *e, uint64_t v) { ev_byte2(e, 0x48, 0xBF); ev_imm64_bytes(e, v); }
static void ev_mov_esi_imm32(emitter_v *e, uint32_t v) {
    uint8_t bs[5] = { 0xBE, (uint8_t)(v), (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24) };
    e->emit_bytes(e, bs, 5, NULL);
}
static void ev_mov_rax_imm64(emitter_v *e, uint64_t v) { ev_byte2(e, 0x48, 0xB8); ev_imm64_bytes(e, v); }
static void ev_test_eax_eax(emitter_v *e) { ev_byte2(e, 0x85, 0xC0); }
static void ev_test_rax_rax(emitter_v *e) { ev_byte3(e, 0x48, 0x85, 0xC0); }
static void ev_mov_eax_imm32(emitter_v *e, uint32_t v) {
    uint8_t bs[5] = { 0xB8, (uint8_t)(v), (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24) };
    e->emit_bytes(e, bs, 5, NULL);
}
static void ev_xor_edx_edx(emitter_v *e) { ev_byte2(e, 0x31, 0xD2); }
static void ev_ret(emitter_v *e) { ev_byte1(e, 0xC3); }
static void ev_cmp_eax_imm32(emitter_v *e, uint32_t v) {
    uint8_t bs[5] = { 0x3D, (uint8_t)(v), (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24) };
    e->emit_bytes(e, bs, 5, NULL);
}
static void ev_sub_eax_imm32(emitter_v *e, uint32_t v) {
    uint8_t bs[5] = { 0x2D, (uint8_t)(v), (uint8_t)(v>>8), (uint8_t)(v>>16), (uint8_t)(v>>24) };
    e->emit_bytes(e, bs, 5, NULL);
}
static void ev_mov_ecx_eax(emitter_v *e) { ev_byte2(e, 0x89, 0xC1); }
static void ev_cmp_eax_ecx(emitter_v *e) { ev_byte2(e, 0x39, 0xC8); }
static void ev_mov_rdx_rax(emitter_v *e) { ev_byte3(e, 0x48, 0x89, 0xC2); }
static void ev_cmp_esi_imm8(emitter_v *e, uint8_t imm) { ev_byte3(e, 0x83, 0xFE, imm); }

/* ── forward declaration ─────────────────────────────────────────────────── */
static void flat_emit_node(emitter_v *e, PATND_t *p,
                           bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                           bb_label_t *lbl_beta);

/* ── XCAT ───────────────────────────────────────────────────────────────── */
static void flat_emit_xcat(emitter_v *e, PATND_t *p,
                           bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                           bb_label_t *lbl_beta)
{
    int id = g_flat_node_id++;
    bb_label_t mid_g, right_o, left_b, right_b, xcat_o;
    bb_label_initf(&mid_g,   "xcat%d_mid_g",   id);
    bb_label_initf(&right_o, "xcat%d_right_o", id);
    bb_label_initf(&left_b,  "xcat%d_left_b",  id);
    bb_label_initf(&right_b, "xcat%d_right_b", id);
    bb_label_initf(&xcat_o,  "xcat%d_o",       id);

    if (p->nchildren == 0) {
        EV_JMP(e, lbl_succ, JMP_JMP);
        EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
        EV_LABEL(e, &xcat_o); EV_JMP(e, lbl_fail, JMP_JMP);
        EV_LABEL(e, &mid_g); EV_LABEL(e, &right_o);
        EV_LABEL(e, &right_b); EV_LABEL(e, &left_b);
        return;
    }
    if (p->nchildren == 1) {
        flat_emit_node(e, p->children[0], lbl_succ, lbl_fail, &left_b);
        EV_LABEL(e, lbl_beta); EV_JMP(e, &left_b, JMP_JMP);
        EV_LABEL(e, &xcat_o); EV_JMP(e, lbl_fail, JMP_JMP);
        EV_LABEL(e, &mid_g); EV_LABEL(e, &right_o); EV_LABEL(e, &right_b);
        return;
    }

    flat_emit_node(e, p->children[0], &mid_g, &xcat_o, &left_b);
    EV_LABEL(e, &mid_g);

    if (p->nchildren == 2) {
        flat_emit_node(e, p->children[1], lbl_succ, &right_o, &right_b);
    } else {
        int nc = p->nchildren;
        bb_label_t *mids  = alloca(sizeof(bb_label_t) * (nc - 1));
        bb_label_t *betas = alloca(sizeof(bb_label_t) * (nc - 1));
        for (int i = 0; i < nc - 1; i++) {
            bb_label_initf(&mids[i],  "xcat%d_mid%d_g", id, i+1);
            bb_label_initf(&betas[i], "xcat%d_mid%d_b", id, i+1);
        }
        for (int i = 1; i < nc; i++) {
            bb_label_t *s = (i < nc-1) ? &mids[i-1] : lbl_succ;
            flat_emit_node(e, p->children[i], s, &right_o, &betas[i-1]);
            if (i < nc-1) EV_LABEL(e, &mids[i-1]);
        }
    }

    EV_LABEL(e, &right_o); EV_JMP(e, &left_b, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, &right_b, JMP_JMP);
    EV_LABEL(e, &xcat_o);  EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── XOR (alternation) ──────────────────────────────────────────────────── */
static void flat_emit_alt(emitter_v *e, PATND_t *p,
                          bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                          bb_label_t *lbl_beta)
{
    int id = g_flat_node_id++;
    int nc = p->nchildren;
    if (nc == 0) { EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP); return; }
    if (nc == 1) { flat_emit_node(e, p->children[0], lbl_succ, lbl_fail, lbl_beta); return; }

    bb_label_t *ci_betas = alloca((size_t)nc * sizeof(bb_label_t));
    bb_label_t *ci_fails = alloca((size_t)nc * sizeof(bb_label_t));
    for (int i = 0; i < nc; i++) {
        bb_label_initf(&ci_betas[i], "alt%d_c%db", id, i);
        bb_label_initf(&ci_fails[i], "alt%d_c%df", id, i);
    }
    for (int i = 0; i < nc; i++) {
        bb_label_t alt_o;
        bb_label_initf(&alt_o, "alt%d_c%do", id, i);
        bb_label_t *f = (i < nc-1) ? &ci_fails[i] : &alt_o;
        flat_emit_node(e, p->children[i], lbl_succ, f, &ci_betas[i]);
        if (i < nc-1) EV_LABEL(e, &ci_fails[i]);
        else          EV_LABEL(e, &alt_o);
    }
    EV_JMP(e, lbl_fail, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, &ci_betas[0], JMP_JMP);
}

/* ── leaf emitters ──────────────────────────────────────────────────────── */

static void flat_emit_lit(emitter_v *e, const char *lit, int len,
                          bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                          bb_label_t *lbl_beta)
{
    int id = g_flat_node_id++;
    bb_label_t lit_b;
    bb_label_initf(&lit_b, "lit%d_b", id);
    (void)lit_b; /* used via lbl_beta */

    /* bounds check: Δ + len > Σlen → fail */
    ev_load_delta(e);
    ev_add_eax_imm32(e, (uint32_t)len);
    ev_cmp_eax_sigmalen(e);
    EV_JMP(e, lbl_fail, JMP_JG);

    /* memcmp(Σ+Δ, lit, len) */
    ev_sigma_plus_delta(e);                           /* rax = Σ+Δ */
    ev_byte3(e, 0x48, 0x89, 0xC7);                   /* mov rdi, rax */
    ev_byte2(e, 0x48, 0xBE); ev_imm64_bytes(e, (uint64_t)(uintptr_t)lit);
    ev_byte2(e, 0x48, 0xBA); ev_imm64_bytes(e, (uint64_t)(uint32_t)len);
    ev_mov_rax_imm64(e, (uint64_t)(uintptr_t)memcmp);
    e->emit_call_rax(e);
    ev_test_eax_eax(e);
    EV_JMP(e, lbl_fail, JMP_JNE);

    ev_add_delta_imm(e, len);
    EV_JMP(e, lbl_succ, JMP_JMP);

    EV_LABEL(e, lbl_beta);
    ev_sub_delta_imm(e, len);
    EV_JMP(e, lbl_fail, JMP_JMP);
}

static void flat_emit_eps(emitter_v *e, bb_label_t *lbl_succ,
                          bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    EV_JMP(e, lbl_succ, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

static void flat_emit_fail(emitter_v *e, bb_label_t *lbl_succ,
                           bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    (void)lbl_succ;
    EV_JMP(e, lbl_fail, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

static void flat_emit_pos(emitter_v *e, int n, bb_label_t *lbl_succ,
                          bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    ev_load_delta(e);
    ev_cmp_eax_imm32(e, (uint32_t)n);
    EV_JMP(e, lbl_fail, JMP_JNE);
    EV_JMP(e, lbl_succ, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

static void flat_emit_rpos(emitter_v *e, int n, bb_label_t *lbl_succ,
                           bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    ev_load_omega(e);
    ev_sub_eax_imm32(e, (uint32_t)n);
    ev_mov_ecx_eax(e);
    ev_load_delta(e);
    ev_cmp_eax_ecx(e);
    EV_JMP(e, lbl_fail, JMP_JNE);
    EV_JMP(e, lbl_succ, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

static void flat_emit_charset_call(emitter_v *e, bb_box_fn c_fn,
                                   const char *chars,
                                   bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                                   bb_label_t *lbl_beta)
{
    typedef struct { const char *chars; int delta; } cs_t;
    cs_t *z = calloc(1, sizeof(cs_t));
    z->chars = chars;

    ev_mov_rdi_imm64(e, (uint64_t)(uintptr_t)z);
    ev_mov_esi_imm32(e, 0);
    ev_mov_rax_imm64(e, (uint64_t)(uintptr_t)c_fn);
    e->emit_call_rax(e);
    ev_test_rax_rax(e);
    EV_JMP(e, lbl_succ, JMP_JNE);
    EV_JMP(e, lbl_fail, JMP_JMP);

    EV_LABEL(e, lbl_beta);
    ev_mov_rdi_imm64(e, (uint64_t)(uintptr_t)z);
    ev_mov_esi_imm32(e, 1);
    ev_mov_rax_imm64(e, (uint64_t)(uintptr_t)c_fn);
    e->emit_call_rax(e);
    ev_test_rax_rax(e);
    EV_JMP(e, lbl_succ, JMP_JNE);
    EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── flat_emit_node dispatch ─────────────────────────────────────────────── */

extern DESCR_t bb_span(void *zeta, int entry);
extern DESCR_t bb_any(void *zeta, int entry);
extern DESCR_t bb_brk(void *zeta, int entry);
extern DESCR_t bb_notany(void *zeta, int entry);
extern int memcmp(const void *, const void *, size_t);

static void flat_emit_node(emitter_v *e, PATND_t *p,
                           bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                           bb_label_t *lbl_beta)
{
    if (!p) { flat_emit_eps(e, lbl_succ, lbl_fail, lbl_beta); return; }
    switch (p->kind) {
    case XCHR: {
        const char *lit = p->STRVAL_fn ? p->STRVAL_fn : "";
        flat_emit_lit(e, lit, (int)strlen(lit), lbl_succ, lbl_fail, lbl_beta);
        break;
    }
    case XEPS:  flat_emit_eps(e, lbl_succ, lbl_fail, lbl_beta); break;
    case XFAIL: flat_emit_fail(e, lbl_succ, lbl_fail, lbl_beta); break;
    case XPOSI: flat_emit_pos(e, (int)p->num, lbl_succ, lbl_fail, lbl_beta); break;
    case XRPSI: flat_emit_rpos(e, (int)p->num, lbl_succ, lbl_fail, lbl_beta); break;
    case XCAT:  flat_emit_xcat(e, p, lbl_succ, lbl_fail, lbl_beta); break;
    case XOR:   flat_emit_alt(e, p, lbl_succ, lbl_fail, lbl_beta); break;
    case XSPNC:
        flat_emit_charset_call(e, bb_span,
            p->STRVAL_fn ? p->STRVAL_fn : "",
            lbl_succ, lbl_fail, lbl_beta); break;
    case XANYC:
        flat_emit_charset_call(e, bb_any,
            p->STRVAL_fn ? p->STRVAL_fn : "",
            lbl_succ, lbl_fail, lbl_beta); break;
    case XBRKC:
        flat_emit_charset_call(e, bb_brk,
            p->STRVAL_fn ? p->STRVAL_fn : "",
            lbl_succ, lbl_fail, lbl_beta); break;
    case XNNYC:
        flat_emit_charset_call(e, bb_notany,
            p->STRVAL_fn ? p->STRVAL_fn : "",
            lbl_succ, lbl_fail, lbl_beta); break;
    default:
        EV_LABEL(e, lbl_beta);
        EV_JMP(e, lbl_fail, JMP_JMP);
        EV_JMP(e, lbl_fail, JMP_JMP);
        break;
    }
}

/* ── invariance check ────────────────────────────────────────────────────── */
static int flat_is_eligible(PATND_t *p)
{
    if (!p) return 1;
    switch (p->kind) {
    case XDSAR: case XVAR: case XATP:
    case XFNME: case XNME: case XFARB:
    case XSTAR: case XARBN: case XCALLCAP:
    case XLNTH: case XTB:  case XRTB: case XBRKX:
    case XSPNC: case XANYC: case XBRKC: case XNNYC:
    case XFNCE:
        return 0;
    default: break;
    }
    if (p->kind == XCAT && p->nchildren > 2) return 0;
    for (int i = 0; i < p->nchildren; i++)
        if (!flat_is_eligible(p->children[i])) return 0;
    return 1;
}

/* ── shared emission body ─────────────────────────────────────────────────── */
static int flat_emit_body_v(emitter_v *e, PATND_t *p,
                            const char *prefix, int text_externalise)
{
    bb_label_t lbl_alpha, lbl_succ, lbl_fail, lbl_beta;
    bb_label_initf(&lbl_alpha, "%s_alpha", prefix);
    bb_label_initf(&lbl_succ,  "%s_gamma", prefix);
    bb_label_initf(&lbl_fail,  "%s_omega", prefix);
    bb_label_initf(&lbl_beta,  "%s_beta",  prefix);

    if (text_externalise) {
        EV_GLOBAL(e, lbl_alpha.name);
        EV_GLOBAL(e, lbl_beta.name);
        EV_GLOBAL(e, lbl_succ.name);
        EV_GLOBAL(e, lbl_fail.name);
    }

    /* entry: r10 = &Δ; cmp esi, 0; je alpha; jmp beta */
    ev_load_r10_delta_ptr(e);
    ev_cmp_esi_imm8(e, 0x00);
    EV_JMP(e, &lbl_alpha, JMP_JE);
    EV_JMP(e, &lbl_beta,  JMP_JMP);

    EV_LABEL(e, &lbl_alpha);
    flat_emit_node(e, p, &lbl_succ, &lbl_fail, &lbl_beta);

    /* PAT_gamma: success — return DESCR_t{v=DT_S=1, rdx=sigma+delta} */
    EV_LABEL(e, &lbl_succ);
    ev_sigma_plus_delta(e);
    ev_mov_rdx_rax(e);
    ev_mov_eax_imm32(e, 1);
    ev_ret(e);

    /* PAT_omega: failure — return DT_FAIL=99 */
    EV_LABEL(e, &lbl_fail);
    ev_mov_eax_imm32(e, 99);
    ev_xor_edx_edx(e);
    ev_ret(e);

    return 0;
}

/* ── bb_build_flat — BINARY entry point ─────────────────────────────────── */
bb_box_fn bb_build_flat(PATND_t *p)
{
    if (!flat_is_eligible(p)) return NULL;

    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;

    g_flat_slot_count = 0;
    g_flat_node_id    = 0;

    emitter_v *e = emitter_binary_new(buf, FLAT_BUF_MAX);
    if (!e) { bb_free(buf, FLAT_BUF_MAX); return NULL; }

    flat_emit_body_v(e, p, "pat_flat", 0);

    int nbytes = emitter_end(e);
    emitter_free(e);

    if (nbytes <= 0 || nbytes > FLAT_BUF_MAX) {
        bb_free(buf, FLAT_BUF_MAX);
        return NULL;
    }
    bb_seal(buf, (size_t)nbytes);
    return (bb_box_fn)buf;
}

/* ── bb_build_flat_text — TEXT entry point (EM-7b/EM-7b') ───────────────── */
/*
 * EM-7b': No save/restore of bb_emit_mode — the vtable eliminates that.
 * NOTE (EM-7c): internal labels not yet namespaced per-prefix; one flat
 * pattern per .s emission scope is safe for now.
 */
int bb_build_flat_text(PATND_t *p, FILE *out, const char *prefix)
{
    if (!flat_is_eligible(p)) return -1;

    g_flat_slot_count = 0;
    g_flat_node_id    = 0;

    emitter_v *e = emitter_text_new(out);
    if (!e) return -1;

    int rc = flat_emit_body_v(e, p, prefix, 1);

    emitter_end(e);
    emitter_free(e);
    return rc;
}
