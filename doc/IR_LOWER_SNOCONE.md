# IR_LOWER_SNOCONE.md — Phase 5 audit: Snocone frontend lower-to-IR

*Authored: 2026-03-29, G-9 s14, Claude Sonnet 4.6*
*Milestone: M-G5-LOWER-SNOCONE-AUDIT*

## Executive summary

**Snocone does not plug into the unified emitters. It reuses the SNOBOL4
`Program*` IR (`scrip_cc.h` `EXPR_t`/`STMT_t`) and routes through
`asm_emit` / `jvm_emit` / `net_emit` — the SNOBOL4 backends.**

This is the central gap. Everything else in this audit follows from it.

---

## Method

Read `snocone_lower.c`, `snocone_driver.c`, `snocone_cf.c/.h`,
`snocone_driver.h`, `snocone_lower.h`, and `src/driver/main.c`.

---

## Current pipeline

```
snocone_lex()
  → snocone_parse()  (shunting-yard → postfix ScPToken[])
  → snocone_lower()  (postfix → EXPR_t/STMT_t using scrip_cc.h types)
  → Program*         (SNOBOL4's IR struct)
  → asm_emit / jvm_emit / net_emit   ← SNOBOL4 backends
```

For `-asm` (x64), the control-flow pass `snocone_cf_compile()` is used
instead of `snocone_compile()`. Both produce `Program*`.

In `main.c`:
```c
if (file_sc) {
    prog = asm_mode ? snocone_cf_compile(src, ...) : snocone_compile(src, ...);
}
// then falls through to:
if      (asm_mode) asm_emit(prog, out);
else if (jvm_mode) jvm_emit(prog, out, infile);
else if (net_mode) net_emit(prog, out, infile);
```

Snocone is **invisible** to the unified `EKind` enum in `ir.h`. It never
touches `ICN_*`, `E_CHOICE`, or any of the node kinds shared by the
Phase 4 refactor.

---

## Node kinds currently emitted (SNOBOL4 IR)

| `EXPR_t` kind (scrip_cc.h) | Source operator | Canonical ir.h name |
|---|---|---|
| `E_ILIT` | integer literal | `E_ILIT` ✅ |
| `E_FLIT` | real literal | `E_FLIT` ✅ |
| `E_QLIT` | string literal | `E_QLIT` ✅ |
| `E_VART` | identifier | `E_VAR` (alias) ✅ |
| `E_KW` | `&keyword` | `E_KW` ✅ |
| `E_ADD` | `+` | `E_ADD` ✅ |
| `E_SUB` | `-` binary | `E_SUB` ✅ |
| `E_MNS` | `-` unary | `E_NEG` (alias) ✅ |
| `E_MPY` | `*` binary | `E_MPY` ✅ |
| `E_DIV` | `/` | `E_DIV` ✅ |
| `E_EXPOP` | `^` | `E_POW` (alias) ✅ |
| `E_INDR` | `*` unary / `$` unary | `E_INDR` ✅ |
| `E_CONCAT` | `\|\|`, `\|`, `&&` | `E_CONCAT` ✅ |
| `E_NAM` | `.` conditional capture | `E_CAPT_COND` (alias) ✅ |
| `E_DOL` | `$` binary immediate capture | `E_CAPT_IMM` (alias) ✅ |
| `E_ATP` | `@var` cursor capture | `E_CAPT_CUR` (alias) ✅ |
| `E_ASGN` | `=` assignment | `E_ASGN` ✅ |
| `E_FNC` | function calls and all comparison ops | `E_FNC` ✅ |
| `E_IDX` | array subscript | `E_IDX` ✅ |

All comparisons (`EQ`, `NE`, `LT`, `GT`, `LE`, `GE`, `IDENT`, `DIFFER`,
`LLT`, `LGT`, `LLE`, `LGE`, `LEQ`, `LNE`, `REMDR`) are lowered to
`E_FNC` with the SNOBOL4 built-in name. Correct for the current backend.

Control-flow keywords (`if`, `while`, `for`, `procedure`, `return`,
`goto`) are handled by `snocone_cf.c` for the ASM backend only; JVM and
.NET backends receive expression-only IR (Sprint SC3 limitation).

---

## The architectural gap

Snocone speaks `scrip_cc.h` SNOBOL4 IR, not the unified `EKind` enum
from `ir.h`. Consequences:

1. **No path to shared backends.** The Phase 4 shared node infrastructure
   (M-G4-SHARED-*) is keyed by `EKind`. Snocone cannot benefit from
   shared CONCAT, ALT, ARBNO, etc. because its IR nodes are a different
   C type entirely.

2. **Tied to SNOBOL4 emitter limitations.** Any gap in
   `asm_emit`/`jvm_emit`/`net_emit` for SNOBOL4 is inherited by Snocone.
   Snocone-specific constructs cannot be added without touching the
   SNOBOL4 emitters.

3. **JVM/NET control flow missing.** `snocone_cf_compile` is only called
   for `asm_mode`. JVM and .NET use `snocone_compile` (expression-only),
   so `if`/`while`/`procedure` are silently absent for those backends.

4. **10/10 corpus PASS is not at risk** — the current corpus only
   exercises expression-level constructs that SNOBOL4 IR handles. Any
   future control-flow or snocone-specific test will expose the gap.

---

## Gap table

| # | Gap | Severity | Notes |
|---|---|---|---|
| G1 | Snocone outputs `Program*` (SNOBOL4 IR), not `EKind` IR | **Architecture** | Root cause of all other gaps |
| G2 | No `snocone_lower_eir()` producing `EKind` node tree | **Architecture** | Required to plug into shared x64/JVM/NET emitters |
| G3 | JVM/NET backends receive expression-only IR (no control flow) | High | `snocone_cf_compile` only used for `asm_mode` |
| G4 | `~` → `E_FNC("NOT",1)` not `E_NOT` | Low | Works today via SNOBOL4 NOT built-in; loses type info post-reorg |
| G5 | `?` → `E_FNC("DIFFER",...)` not a first-class node | Low | Same as G4 |

---

## Fix milestone: M-G5-LOWER-SNOCONE-FIX

The fix is a new lowering pass producing `EKind` IR directly:

```c
/* Proposed new API in snocone_lower_eir.c */
ENode *snocone_lower_eir(const ScPToken *ptoks, int count, const char *filename);
```

The `EXPR_t`→`EKind` mapping is 1-to-1 for all arithmetic and capture
nodes (the alias table in ir.h already covers them). Comparison ops
(`EQ`, `NE`, etc.) remain as `E_FNC` nodes — no change needed.
Control-flow constructs map to the SNOBOL4 goto/label model already in
`EKind`.

`main.c` gets a new branch: when `-sc` is set, call `snocone_lower_eir`
and dispatch to the unified `emit_x64` / `emit_jvm` / `emit_net`
functions (bypassing `asm_emit`/`jvm_emit`/`net_emit`).

**Scope:** Post-reorg (after M-G7-UNFREEZE). The SNOBOL4-IR path
continues to work unchanged until then.

**Prerequisite for M-G6-SNOCONE-JVM / M-G6-SNOCONE-NET.**

---

## Conclusion

**M-G5-LOWER-SNOCONE-AUDIT: COMPLETE — 1 architectural gap + 4 derivative gaps.**

Current corpus PASS (10/10) is safe. G1/G2 must be fixed before any
Phase 6 Snocone multi-backend work can proceed.

---
*IR_LOWER_SNOCONE.md — authored G-9 s14. Do not add content beyond this line.*
