# RS-5 Scan Findings — Middle-Tier and Backend

**Date:** 2026-05-02  
**Session goal:** Read-only scan of middle-tier and backend files; no code changes.  
**Files scanned:**

| File | Lines |
|------|-------|
| `src/runtime/x86/sm_lower.c` | 1,254 |
| `src/runtime/x86/sm_interp.c` | 925 |
| `src/runtime/x86/sm_prog.c` | 252 |
| `src/runtime/interp/icn_runtime.c` | 1,768 |
| `src/runtime/interp/pl_runtime.c` | 2,257 |
| `src/ir/ir.h` | 392 |
| `src/runtime/x86/bb_broker.c` (+bb_*.c) | 87 (+3,448) |

---

## 1. Duplicate Helper Routines

### 1A — Integer-to-string coercion: `icn_runtime.c` vs `VARVAL_d_fn`

**Sites:**
- `icn_runtime.c:239` and `icn_runtime.c:1097` — both convert `DT_I → DT_S` via `GC_malloc(32)` + `snprintf(nbuf, 32, "%lld", ...)`, then `STRVAL(nbuf)`.
- `snobol4.c` exports `VARVAL_d_fn(DESCR_t)` and `VARVAL_fn(DESCR_t) → char*` which already perform `DT_I → string` coercion for the SNOBOL4 runtime.

**Observation:** The two `icn_runtime.c` sites (both in the IC-8 "coerce numeric scalars to image-string before string-iterate" path for `E_BANG_BINARY`) replicate the integer→string work that `VARVAL_d_fn` already does. The `icn_runtime.c` code also handles `DT_R` by calling `icn_real_str` (the round-trip formatter in `interp_eval.c`), which is the correct formatter; `VARVAL_d_fn` uses a different format (TBD by reading `snobol4.c` at RS-6 time).

**Proposed unification:** A shared `descr_to_str_icn(DESCR_t d, char **buf_out) → DESCR_t` in `runtime/common/coerce.c` that branches on `DT_I` (snprintf %lld), `DT_R` (icn_real_str), `DT_S`/`DT_SNUL` (identity). Both `icn_runtime.c` sites and the CONCAT path (see 1B) become one-liners.

**Risk:** Low. The two icn_runtime sites are self-contained IC-8 fixes; touching them requires a smoke_icon gate pass.

---

### 1B — Real-to-string formatting: `icon_gen.c` vs `icn_real_str`

**Sites:**
- `icon_gen.c:391` and `icon_gen.c:396` — `ICN_BINOP_CONCAT` coerces `DT_R → string` via `snprintf(lbuf, 64, "%.15g", lv.r)` followed by appending `.0` if no decimal point.
- `interp_eval.c:232` — `icn_real_str()` uses an adaptive precision loop (15..17) to find the shortest round-trip representation, then appends `.0` if no decimal/exponent.

**Observation:** The two implementations diverge: `icon_gen.c` hardcodes `%.15g` (always 15 significant digits), while `icn_real_str` tries 15, 16, 17 and picks the shortest that parses back to the same `double`. For values where 15 digits is sufficient they agree. For values needing 16 or 17 digits, `icon_gen.c` will emit a string that does not round-trip. This is a correctness divergence for generative string concatenation.

**Proposed unification:** `icon_gen.c ICN_BINOP_CONCAT` should call `icn_real_str` for `DT_R` operands instead of using `%.15g`. `icn_real_str` should move to the shared `runtime/common/` or stay in `interp_eval.c` with a declaration in a shared header. No format-string changes to `VARVAL_d_fn` (SNOBOL4 formatting semantics are separate — see RULES.md DATATYPE-case precedent for "no silent changes").

**Risk:** Low-medium. Correctness fix — any generator test with round-trip reals will catch regressions.

---

### 1C — String concatenation: `icon_gen.c ICN_BINOP_CONCAT` vs `CONCAT_fn`

**Sites:**
- `icon_gen.c:384–401` — `ICN_BINOP_CONCAT` allocates `GC_malloc(ll+rl+1)`, copies left then right, builds a `DT_S` DESCR manually.
- `snobol4.c` exports `CONCAT_fn(DESCR_t a, DESCR_t b)` which does the same operation including `DT_FAIL` propagation, using `STRCONCAT_fn` internally.

**Observation:** `ICN_BINOP_CONCAT` reimplements `CONCAT_fn` with two differences: (a) it coerces numeric operands to string first (via snprintf), then concatenates; (b) it does not propagate `DT_FAIL`. `CONCAT_fn` handles the raw string bytes but does not coerce numerics — so the two diverge for `int ||| string` operands.

**Proposed unification:** Add a `CONCAT_COERCE_fn(DESCR_t a, DESCR_t b)` to `snobol4.c` (or `runtime/common/coerce.c`) that calls `VARVAL_d_fn`/`icn_real_str` on each operand before calling `CONCAT_fn`. `ICN_BINOP_CONCAT` becomes a call to `CONCAT_COERCE_fn`. This also resolves the real-formatting divergence from 1B for the concat path.

**Risk:** Low. `ICN_BINOP_CONCAT` is only called from `icn_bb_binop_gen` in the generative binary-op Byrd box.

---

## 2. Opcode Convergence

### 2A — Integer/real arithmetic: `sm_arith` vs `icn_bb_binop_eval`

| Semantic op | SM opcode (sm_interp.c) | ICN path (icon_gen.c) |
|-------------|--------------------------|------------------------|
| `a + b` | `SM_ADD` → `sm_arith(l, r, SM_ADD)` | `ICN_BINOP_ADD` → `icn_bb_binop_eval` |
| `a - b` | `SM_SUB` | `ICN_BINOP_SUB` |
| `a * b` | `SM_MUL` | `ICN_BINOP_MUL` |
| `a / b` | `SM_DIV` | `ICN_BINOP_DIV` |
| `a % b` | `SM_CALL "REMDR" 2` | `ICN_BINOP_MOD` |

**Observation:** When an Icon arithmetic expression is **not** generative, `sm_lower.c` emits `SM_ADD`/`SM_SUB`/etc. and `sm_interp.c` handles them via `sm_arith`. When an Icon arithmetic expression **is** generative (one or both operands are generators), `icn_eval_gen` builds an `icn_bb_binop_gen` box that calls `icn_bb_binop_eval` from `icon_gen.c`. The two arithmetic implementations (`sm_arith` and `icn_bb_binop_eval`) agree on integer paths but differ slightly on real promotion: `sm_arith` casts `DT_I → double` before real ops; `icn_bb_binop_eval` uses `IS_INT_fn`/`IS_REAL_fn` guards and separate `li`/`ld` variables — same semantics, different expression. Division-by-zero: `sm_arith` returns `FAILDESCR`; `icn_bb_binop_eval` also returns `FAILDESCR` (for real) or `FAILDESCR` (for int). These agree.

**Modulo divergence:** SNOBOL4 maps `E_MOD → SM_CALL "REMDR" 2` (a named function call). ICN maps `E_MOD → ICN_BINOP_MOD → icn_bb_binop_eval` (inline `li % ri`). There is no shared SM opcode for modulo. If Raku or Snocone also need modulo, each would need its own path or a `SM_MOD` opcode.

**Proposed unification:** Add `SM_MOD` opcode alongside `SM_ADD`..`SM_EXP`. All languages lower `E_MOD` to `SM_MOD`. `sm_arith` handles `SM_MOD` inline (integer: `l.i % r.i`; real: `fmod`). `icn_bb_binop_eval` becomes the single modulo path for generative cases. Eliminates the `SM_CALL "REMDR"` dispatch overhead for SNOBOL4's rare integer modulo.

**Risk:** Low. `E_MOD` is infrequent in SNOBOL4 and is already inline in ICN.

---

### 2B — String/list indexing: `SM_CALL "IDX"` vs ICN `E_IDX` via `icn_eval_gen`

**Observation:** Non-generative `E_IDX` → `SM_CALL "IDX" N` (a named dispatch via the call table). Generative `E_IDX` (e.g. `s[1 to 3]`) → `icn_eval_gen` builds a cat-gen Byrd box. No SM opcode for subscript — it goes through the string function dispatch. This is intentional: subscript semantics differ enough per language (SNOBOL4 TABLE vs Icon list vs Prolog) that a shared opcode would need a large lang-switch inside the handler. Leave as-is.

**Conclusion:** No convergence candidate here; the named-call path is appropriate.

---

### 2C — Numeric comparison: `SM_ACOMP` (SNOBOL4) vs `ICN_BINOP_EQ/LT/…`

**Observation:** SNOBOL4 relops lower to `SM_ACOMP` which leaves a −1/0/1 value on the stack; the SNOBOL4 goto logic reads it. Icon relops lower to `SM_ADD`-family or (if generative) `icn_bb_binop_eval` which returns the right-hand value on success and `FAILDESCR` on failure (goal-directed semantics). These are semantically different — SNOBOL4 relops fail/succeed via goto; Icon relops return a value. The different opcodes are intentional and correct.

**Conclusion:** No convergence candidate; semantics differ by design.

---

## 3. Per-Language Branches in `sm_lower.c`

**Two branches, both in `lower_stmt`:**

```c
// sm_lower.c:1082–1096
if (s->lang == LANG_ICN) {
    sm_emit_ptr(p, SM_PUSH_EXPR, (void *)s->subject);
    sm_emit(p, SM_BB_PUMP);
    goto emit_gotos;
}
if (s->lang == LANG_PL) {
    if (s->subject) lower_expr(p, lt, s->subject);
    else            sm_emit(p, SM_PUSH_NULL);
    sm_emit(p, SM_BB_ONCE);
    goto emit_gotos;
}
```

**Observation:**
- `LANG_ICN` takes the subject EXPR_t* as-is (via `SM_PUSH_EXPR`) without lowering it, because `icn_eval_gen` in `sm_interp.c` builds the Byrd box lazily at runtime.
- `LANG_PL` calls `lower_expr` on the subject (to handle E_CHOICE/E_CLAUSE trees that may contain nested sub-expressions), then emits `SM_BB_ONCE`.
- The two branches differ because ICN needs the raw tree (for generative semantics) while PL can be partially lowered. This asymmetry is semantic, not accidental.

**LANG_RAKU and LANG_REB:** No branches in `sm_lower.c` for Raku (`LANG_RAKU = 3`) or Rebus (`LANG_REB = 5`). Raku statements use `lower_expr` to produce SM opcodes (E_IF, E_WHILE, E_UNTIL, etc. are all handled in `lower_expr`). Rebus is not yet wired to sm_lower. Snocone has no LANG_* constant yet.

**Proposed improvement:** The ICN branch comment says "build a bb_node_t … then drive via bb_broker(BB_PUMP)" but the actual bb_node_t construction happens inside `sm_interp.c:SM_BB_PUMP`. A cleaner factoring (RS-6 scope) would introduce a `lower_stmt_icn` shim that calls `icn_lower_stmt` in `icn_runtime.c`, making the dispatch table explicit:

```c
static void (*lang_lower_stmt_fn[LANG_COUNT])(SM_Program*, LabelTable*, const STMT_t*) = {
    lower_stmt_snobol4,   /* LANG_SNO */
    lower_stmt_icn,       /* LANG_ICN */
    lower_stmt_pl,        /* LANG_PL  */
    lower_stmt_snobol4,   /* LANG_RAKU (uses same lower_expr dispatch) */
    NULL,                 /* LANG_SCRIP */
    NULL,                 /* LANG_REB */
};
```

**Risk:** Medium. The two branches are only 8 lines total and well-commented; a table adds indirection. Defer to RS-6 unless Snocone or Rebus add a third/fourth branch that makes the if-chain unwieldy.

---

## 4. BB Broker Duplication

**`bb_broker.c`** (87 lines) is clean — one entry point, three `BrokerMode` cases (`BB_SCAN`, `BB_PUMP`, `BB_ONCE`), no language-specific logic.

**`bb_boxes.c`** (794 lines) — SNOBOL4 pattern boxes (`bb_lit`, `bb_seq`, `bb_alt`, `bb_any`, etc.). All use the SIL globals `Δ`/`Σ`/`Σlen` for cursor arithmetic. No ICN or PL logic present.

**`bb_build.c`** (1,471 lines) — builds pattern bb_node_t trees from the SNOBOL4 SM pattern stack. No per-language duplication found.

**`bb_emit.c`** (408 lines), **`bb_flat.c`** (599 lines), **`bb_pool.c`** (176 lines) — JIT/emit plumbing. No per-language duplication found.

**ICN Byrd boxes** (`icn_oneshot_box`, `icn_lazy_box`, `icn_bb_fnc_gen`, `icn_bb_cat_gen`, `icn_bb_assign_gen`, `icn_bb_revassign`, `icn_bb_revswap`, `icn_bb_identical_gen`) — all live in `icn_runtime.c`. They do not duplicate anything in `bb_boxes.c`; the pattern boxes and the ICN generator boxes serve completely different semantic roles.

**PL Byrd boxes** (`pl_choice_fn`, `pl_box_choice`, `pl_box_choice_call`) — live in `pl_broker.c`. No duplication with `bb_boxes.c` or `icn_runtime.c`.

**Conclusion:** No BB broker duplication. The three box populations (SNOBOL4 pattern, ICN generators, PL choice) are correctly separate.

---

## 5. Summary of Actionable Items

| ID | File(s) | Finding | Proposed action | Risk |
|----|---------|---------|-----------------|------|
| **D-1** | `icn_runtime.c:239,1097` | Int-to-string coercion duplicates `VARVAL_d_fn` | Extract `descr_to_str_icn()` to `runtime/common/coerce.c` | Low |
| **D-2** | `icon_gen.c:391,396` | Real-to-string uses `%.15g`; `icn_real_str` uses adaptive 15–17 | Replace with `icn_real_str()` call | Low–Med |
| **D-3** | `icon_gen.c:384–401` | `ICN_BINOP_CONCAT` reimplements `CONCAT_fn` + coercion | Add `CONCAT_COERCE_fn`; replace body | Low |
| **OC-1** | `sm_lower.c`, `sm_interp.c` | `E_MOD` lowers to `SM_CALL "REMDR"` for SNOBOL4, but inline for ICN | Add `SM_MOD` opcode; all languages use it | Low |
| **SM-1** | `sm_lower.c:1082–1096` | Two-branch `lang ==` chain for ICN/PL | Leave as-is (only 2 langs); revisit at RS-6 if ≥3 langs need special casing | — |

Items D-1 through D-3 are candidates for a single `runtime/common/coerce.c` file. OC-1 is a one-opcode addition to `sm_prog.h`/`sm_interp.c`. SM-1 is deferred.

Code changes go in RS-6+.
