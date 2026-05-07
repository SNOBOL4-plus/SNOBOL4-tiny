/*
 * bb_flat.c — Flat-Glob Invariant Pattern Emitter (M-DYN-FLAT)
 *
 * Emits an entire invariant PATND_t tree as one contiguous x86-64 blob.
 * All sub-boxes are inlined flat; control flows via direct jmp, never call/ret.
 *
 * EM-7b'': Zero byte knowledge in this file.  Every emission is a named
 * helper call (ev_load_delta, ev_mov_rax_imm64, etc.) that routes through
 * emitter_v * -> emit_insn -> TEXT (readable mnemonic) or BINARY (bytes).
 * The walker reads as a description of pattern-matcher semantics, not x86.
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6
 * Sprint:  RT-129 / M-DYN-FLAT (EM-7b'')
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

static int g_flat_node_id   = 0;
static int g_flat_slot_count = 0;

/* ── address constants — EM-7c-symbolic ──────────────────────────────────── */
/* Symbol names for globals exported by libscrip_rt.so.                      */
/* TEXT mode: ev_lea_rcx_sym emits  lea rcx, [rip + sym]                     */
/* BINARY mode: ev_lea_rcx_sym emits mov rcx, imm64  (process-address)       */
#define SYM_SIGMA   "\xCE\xA3"          /* UTF-8: Σ */
#define SYM_SIGLEN  "\xCE\xA3""len"     /* UTF-8: Σlen */
#define SYM_DELTA   "\xCE\x94"          /* UTF-8: Δ */
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)

/* ── intern_str hook — set by sm_codegen_x64_emit before calling bb_build_flat_text */
/* NULL = no strtab available (BINARY mode or standalone use).               */
static const char *(*g_flat_intern_str)(emitter_v *e, const char *s) = NULL;

void bb_flat_set_intern_str(const char *(*fn)(emitter_v *, const char *))
{
    g_flat_intern_str = fn;
}

/* ── forward declarations ────────────────────────────────────────────────── */
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

/* ── leaf: literal string ───────────────────────────────────────────────── */
static void flat_emit_lit(emitter_v *e, const char *lit, int len,
                          bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                          bb_label_t *lbl_beta)
{
    /* α: if Δ + len > Σlen → fail */
    ev_load_delta(e);                                    /* eax = Δ */
    ev_add_eax_imm32(e, (uint32_t)len);                 /* eax += len */
    ev_cmp_eax_siglen(e, ADDR_SIGLEN);                  /* cmp eax, [Σlen] */
    EV_JMP(e, lbl_fail, JMP_JG);

    /* memcmp(Σ+Δ, lit, len): set up rdi=Σ+Δ, rsi=lit, rdx=len */
    ev_sigma_plus_delta(e, ADDR_SIGMA);                 /* rax = Σ+Δ */
    ev_mov_rdi_rax(e);                                  /* rdi = Σ+Δ */
    ev_mov_rdx_imm64(e, (uint64_t)(uint32_t)len);       /* rdx = len */

    /* rsi = lit ptr: TEXT mode → use strtab label; BINARY → raw ptr */
    if (e->is_text && e->intern_str) {
        const char *lbl = e->intern_str(e, lit);        /* e.g. ".Lstr_N" */
        bb_insn_desc_t d = {BB_INSN_LEA_RCX_SYM, (uint64_t)(uintptr_t)lit, 0, 0, lbl};
        e->emit_insn(e, &d);                            /* lea rcx, [rip + .Lstr_N] */
        /* mov rsi, rcx — route through rcx since we have no LEA_RSI_SYM */
        { bb_insn_desc_t d2 = {BB_INSN_MOV_RSI_IMM64, 0, 0, 0, NULL};
          /* TEXT: emit "mov rsi, rcx" — but we lack MOV_RSI_RCX insn.
           * Use fprintf_raw for this one-off: */
          e->fprintf_raw(e, "    mov     rsi, rcx\n");
        }
    } else {
        /* BINARY / no-strtab: bake raw pointer (in-process mode-3 valid) */
        bb_insn_desc_t d = {BB_INSN_MOV_RSI_IMM64, (uint64_t)(uintptr_t)lit, 0, 0, NULL};
        e->emit_insn(e, &d);
    }

    /* call memcmp — TEXT: call memcmp@PLT; BINARY: mov rax, ptr; call rax */
    ev_call_sym_plt(e, "memcmp", (uint64_t)(uintptr_t)memcmp);
    ev_test_eax_eax(e);                                 /* test eax, eax */
    EV_JMP(e, lbl_fail, JMP_JNE);

    /* success: Δ += len */
    ev_add_delta_imm(e, len);
    EV_JMP(e, lbl_succ, JMP_JMP);

    /* β: Δ -= len; fail */
    EV_LABEL(e, lbl_beta);
    ev_sub_delta_imm(e, len);
    EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── leaf: epsilon ──────────────────────────────────────────────────────── */
static void flat_emit_eps(emitter_v *e, bb_label_t *lbl_succ,
                          bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    EV_JMP(e, lbl_succ, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── leaf: always-fail ──────────────────────────────────────────────────── */
static void flat_emit_fail(emitter_v *e, bb_label_t *lbl_succ,
                           bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    (void)lbl_succ;
    EV_JMP(e, lbl_fail, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── leaf: POS(n) ───────────────────────────────────────────────────────── */
static void flat_emit_pos(emitter_v *e, int n, bb_label_t *lbl_succ,
                          bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    ev_load_delta(e);
    ev_cmp_eax_imm32(e, (uint32_t)n);
    EV_JMP(e, lbl_fail, JMP_JNE);
    EV_JMP(e, lbl_succ, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── leaf: RPOS(n) ──────────────────────────────────────────────────────── */
static void flat_emit_rpos(emitter_v *e, int n, bb_label_t *lbl_succ,
                           bb_label_t *lbl_fail, bb_label_t *lbl_beta)
{
    ev_load_siglen(e, ADDR_SIGLEN);     /* eax = Σlen */
    ev_sub_eax_imm32(e, (uint32_t)n);  /* eax = Σlen - n */
    ev_mov_ecx_eax(e);                 /* ecx = Σlen - n */
    ev_load_delta(e);                  /* eax = Δ */
    ev_cmp_eax_ecx(e);                 /* cmp Δ, Σlen-n */
    EV_JMP(e, lbl_fail, JMP_JNE);
    EV_JMP(e, lbl_succ, JMP_JMP);
    EV_LABEL(e, lbl_beta); EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── leaf: charset (ANY/NOTANY/SPAN/BRK) ───────────────────────────────── */
static void flat_emit_charset_call(emitter_v *e, bb_box_fn c_fn,
                                   const char *c_fn_name,
                                   const char *chars,
                                   bb_label_t *lbl_succ, bb_label_t *lbl_fail,
                                   bb_label_t *lbl_beta)
{
    typedef struct { const char *chars; int delta; } cs_t;
    cs_t *z = calloc(1, sizeof(cs_t));
    z->chars = chars;

    /* α: call c_fn(z, 0) */
    ev_mov_rdi_imm64(e, (uint64_t)(uintptr_t)z);
    ev_mov_esi_imm32(e, 0);
    ev_call_sym_plt(e, c_fn_name, (uint64_t)(uintptr_t)c_fn);
    ev_test_rax_rax(e);
    EV_JMP(e, lbl_succ, JMP_JNE);
    EV_JMP(e, lbl_fail, JMP_JMP);

    /* β: call c_fn(z, 1) */
    EV_LABEL(e, lbl_beta);
    ev_mov_rdi_imm64(e, (uint64_t)(uintptr_t)z);
    ev_mov_esi_imm32(e, 1);
    ev_call_sym_plt(e, c_fn_name, (uint64_t)(uintptr_t)c_fn);
    ev_test_rax_rax(e);
    EV_JMP(e, lbl_succ, JMP_JNE);
    EV_JMP(e, lbl_fail, JMP_JMP);
}

/* ── flat_emit_node dispatch ─────────────────────────────────────────────── */

extern DESCR_t bb_span  (void *zeta, int entry);
extern DESCR_t bb_any   (void *zeta, int entry);
extern DESCR_t bb_brk   (void *zeta, int entry);
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
    case XEPS:  flat_emit_eps (e, lbl_succ, lbl_fail, lbl_beta); break;
    case XFAIL: flat_emit_fail(e, lbl_succ, lbl_fail, lbl_beta); break;
    case XPOSI: flat_emit_pos (e, (int)p->num, lbl_succ, lbl_fail, lbl_beta); break;
    case XRPSI: flat_emit_rpos(e, (int)p->num, lbl_succ, lbl_fail, lbl_beta); break;
    case XCAT:  flat_emit_xcat(e, p, lbl_succ, lbl_fail, lbl_beta); break;
    case XOR:   flat_emit_alt (e, p, lbl_succ, lbl_fail, lbl_beta); break;
    case XSPNC: flat_emit_charset_call(e, bb_span,   "bb_span",    p->STRVAL_fn?p->STRVAL_fn:"", lbl_succ, lbl_fail, lbl_beta); break;
    case XANYC: flat_emit_charset_call(e, bb_any,    "bb_any",     p->STRVAL_fn?p->STRVAL_fn:"", lbl_succ, lbl_fail, lbl_beta); break;
    case XBRKC: flat_emit_charset_call(e, bb_brk,    "bb_brk",     p->STRVAL_fn?p->STRVAL_fn:"", lbl_succ, lbl_fail, lbl_beta); break;
    case XNNYC: flat_emit_charset_call(e, bb_notany, "bb_notany",  p->STRVAL_fn?p->STRVAL_fn:"", lbl_succ, lbl_fail, lbl_beta); break;
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
    bb_label_t lbl_alpha, lbl_alpha_body, lbl_succ, lbl_fail, lbl_beta;
    bb_label_initf(&lbl_alpha,      "%s_alpha",      prefix);
    bb_label_initf(&lbl_alpha_body, "%s_alpha_body", prefix);
    bb_label_initf(&lbl_succ,       "%s_gamma",      prefix);
    bb_label_initf(&lbl_fail,       "%s_omega",      prefix);
    bb_label_initf(&lbl_beta,       "%s_beta",       prefix);

    /* TEXT mode: external _alpha label must be at the TRUE function entry
     * (before the r10-setup preamble), so that bb_broker can call fn(ζ,0)
     * or fn(ζ,1) and have r10 = &Δ ready.  The dispatch then jumps to
     * lbl_alpha_body (α path) or lbl_beta (β path).
     * BINARY mode: the function starts at offset 0 which IS the preamble;
     * no external symbols emitted, so the label placement doesn't matter. */
    if (text_externalise) {
        EV_GLOBAL(e, lbl_alpha.name);
        EV_GLOBAL(e, lbl_beta.name);
        EV_GLOBAL(e, lbl_succ.name);
        EV_GLOBAL(e, lbl_fail.name);
        /* External alpha = function entry (before preamble) */
        EV_LABEL(e, &lbl_alpha);
    }

    /* entry: r10 = &Δ; cmp esi, 0; je alpha_body (α path); else jmp beta
     * TEXT:   lea r10, [rip + Δ]   (via BB_INSN_LEA_R10_SYM)
     * BINARY: mov r10, imm64       (via ev_load_r10_delta_ptr)           */
    {   bb_insn_desc_t d = {BB_INSN_LEA_R10_SYM, ADDR_DELTA, 0, 0, SYM_DELTA};
        e->emit_insn(e, &d);
    }
    ev_cmp_esi_imm8(e, 0);
    EV_JMP(e, &lbl_alpha_body, JMP_JE);
    EV_JMP(e, &lbl_beta,       JMP_JMP);

    /* Internal alpha_body label (dispatch target within function).
     * In BINARY mode, lbl_alpha is also defined here (same offset = function
     * start + preamble size; fine since binary blobs are called at offset 0). */
    EV_LABEL(e, &lbl_alpha_body);
    if (!text_externalise) EV_LABEL(e, &lbl_alpha);   /* binary: alpha = alpha_body */
    flat_emit_node(e, p, &lbl_succ, &lbl_fail, &lbl_beta);

    /* PAT_gamma: success → return DESCR_t{v=DT_S=1, rdx=Σ+Δ} */
    EV_LABEL(e, &lbl_succ);
    ev_sigma_plus_delta(e, ADDR_SIGMA);  /* rax = Σ+Δ */
    ev_mov_rdx_rax(e);                  /* rdx = Σ+Δ (σ) */
    ev_mov_eax_imm32(e, 1);             /* rax = DT_S=1 */
    ev_ret(e);

    /* PAT_omega: failure → return DT_FAIL=99 */
    EV_LABEL(e, &lbl_fail);
    ev_mov_eax_imm32(e, 99);
    ev_xor_edx_edx(e);
    ev_ret(e);

    return 0;
}

/* ── public entry points ──────────────────────────────────────────────────── */

bb_box_fn bb_build_flat(PATND_t *p)
{
    if (!flat_is_eligible(p)) return NULL;
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_slot_count = 0; g_flat_node_id = 0;
    emitter_v *e = emitter_binary_new(buf, FLAT_BUF_MAX);
    if (!e) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    flat_emit_body_v(e, p, "pat_flat", 0);
    int nbytes = emitter_end(e);
    emitter_free(e);
    if (nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    return (bb_box_fn)buf;
}

int bb_build_flat_text(PATND_t *p, FILE *out, const char *prefix)
{
    if (!flat_is_eligible(p)) return -1;
    /* EM-7c: do NOT reset g_flat_node_id here — multiple patterns
     * emitted into the same `.s` rely on the monotonic counter to
     * avoid internal-label collisions (xcat0_mid_g, xcat1_mid_g, ...).
     * Use bb_build_flat_text_reset() between unrelated emit runs. */
    emitter_v *e = emitter_text_new(out);
    if (!e) return -1;
    e->intern_str = g_flat_intern_str;  /* wire strtab callback if set */
    int rc = flat_emit_body_v(e, p, prefix, 1);
    emitter_end(e);
    emitter_free(e);
    return rc;
}

void bb_build_flat_text_reset(void)
{
    /* EM-7c: called by sm_codegen_x64_emit at the start of each emit run
     * so internal label IDs (xcatN, altN, ...) start from 0 per output `.s`.
     * Within a single emit run, the counter MUST monotonically increment
     * to keep labels unique across patterns sharing the same namespace. */
    g_flat_slot_count = 0;
    g_flat_node_id    = 0;
}
