# CHUNKS Step 1 — emit_push_expr Site Audit

**Session:** #63, 2026-05-05  
**Goal:** GOAL-CHUNKS Step 1 — Survey + scaffolding  
**File audited:** `src/runtime/x86/sm_lower.c`

---

## SmChunk_t — descriptor choice

Chosen representation: **struct stored inline in SM_Instr.a[]**.

```c
/* sm_prog.h — add alongside SM_PUSH_EXPR */
typedef struct {
    int entry_pc;   /* index into SM_Program::instrs[] where chunk starts */
    int arity;      /* args expected on SM value stack at entry; 0 = thunk */
} SmChunk_t;
```

`SM_PUSH_CHUNK` encodes `entry_pc` in `a[0].i` and `arity` in `a[1].i` —
both fields already exist in `SM_Instr` as `union { int i; ... }`.
No new union member needed.  No pointer indirection at dispatch time.

---

## New opcodes (added to sm_prog.h enum)

```c
SM_PUSH_CHUNK,   /* push DT_E chunk descriptor; a[0].i=entry_pc, a[1].i=arity */
SM_CALL_CHUNK,   /* pop chunk descriptor, push return frame, jump to entry_pc */
```

Both have FATAL stubs in `sm_interp.c` and `sm_codegen.c` until their
producer sites are migrated.

---

## emit_push_expr call-site inventory

### Site 0 — helper definition (line 39)

```c
static void emit_push_expr(SM_Program *p, const EXPR_t *e) {
    sm_emit_ptr(p, SM_PUSH_EXPR, (void *)expr_gc_clone(e));
}
```

Not a call site — the helper itself.  Deleted in Step 18 once all
callers are gone.

---

### Site A — lines 345 / 386 (E_CAPT_DEFERRED / E_CAPT_IMMED_ASGN, pattern-capture fn args)

**Context:** `. *fn(args)` and `$ *fn(args)` capture nodes.  When the
captured function has non-E_QLIT arguments, each arg is deferred so it
evaluates at match time, not pattern-build time (SN-26c, SN-32b).

**EXPR_t kind of deferred value:** any non-E_QLIT arg — E_VAR, E_FNC,
E_ADD, etc.  General expression.

**Immediate consumer:** `SM_PAT_CAPTURE_FN_ARGS` pops N descriptors and
calls `pat_assign_callcap`.  Each descriptor is a DT_E; at match time
`EVAL_fn → EXPVAL_fn` thaws it.

**Consumer reads from EXPR_t:** `snobol4_pattern.c:222` —
`frozen->kind`, `frozen->sval`, `frozen->nchildren`, `frozen->children[i]`.
Full IR walk.

**Chunk shape:** general thunk (arity=0); body = lower_expr of the arg
subtree + SM_RETURN.

**Migration step:** Step 4 (bundled with site B — same consumer).

---

### Site B — line 470 (E_DEFER / pattern non-QLIT arg, SM_PAT_USERCALL_ARGS)

**Context:** `*upr(tx)` — user-defined pattern function call with args.
Non-E_QLIT args deferred so inner function fires at match time.

**EXPR_t kind:** same as Site A — any non-E_QLIT arg.

**Immediate consumer:** `SM_PAT_USERCALL_ARGS` pops N descriptors; each
thawed by `bb_usercall` via `EVAL_fn` at match time.

**Consumer reads from EXPR_t:** same path as Site A via `snobol4_pattern.c`.

**Chunk shape:** general thunk (arity=0).

**Migration step:** Step 3 (standalone — slightly different consumer than
A, but same thaw path).  Actually shares the same DT_E thaw consumer as
Site A; can be bundled.  Decision: Step 3 handles this site alone as
warm-up; Step 4 handles Site A (two sites same consumer).

---

### Site C — line 573 (E_DEFER in value context — `*expr`)

**Context:** `*expr` in a value/statement context (not pattern context).
Freezes the child expression as DT_E so EVAL() can thaw it.

**EXPR_t kind:** child of E_DEFER — any expression.  The key case Lon
raised: if the child is already a compiled pattern/BB chunk (E_VAR
holding a DT_P), the `entry_pc` can reference the existing chunk
directly (no new body needed).  For general expressions
(`*("Hello " person ", " age + 1)`) a fresh chunk body is required.

**Immediate consumer:** `snobol4_pattern.c:221–250` DT_E thaw:
- `frozen->kind == E_FNC` → evaluate as function call
- `frozen->kind == E_VAR` → look up by name
- otherwise → `EXPVAL_fn` (execute as expression)

Also `snobol4_pattern.c:798` (pattern-argument thaw path).

**Consumer reads from EXPR_t:** `frozen->kind`, `frozen->sval`,
`frozen->nchildren`, `frozen->children[i]`.  Full IR walk until
replaced by `SM_CALL_CHUNK`.

**Optimisation note (Lon):** When the child is a bound pattern variable
(already a compiled DT_P / BB chunk), `SM_PUSH_CHUNK` can bake in
`existing_entry_pc` with no new chunk body — just a reference.  This
optimisation is **not** required for correctness in Step 2; record here
for a later pass.  Step 2 emits the general form (forward-jump + body +
SM_RETURN) for all children.

**Chunk shape:** thunk (arity=0); body = lower_expr(child) + SM_RETURN.

**Migration step:** Step 2 (first and simplest — single site, clear
consumer).

---

### Site D — lines 975 / 986 / 993 (Icon generators, Prolog clauses, Raku CASE)

**Line 975 — Icon generators** (E_EVERY, E_TO, E_TO_BY, E_SUSPEND,
E_BANG_BINARY, E_LCONCAT, E_LIMIT, E_RANDOM, E_SECTION, E_SECTION_MINUS,
E_SECTION_PLUS):

**Context:** Generator node — entire subtree handed to BB broker.
`SM_BB_PUMP` drives the suspended coroutine.

**EXPR_t kind:** the generator node itself (e.g. E_TO, E_EVERY).

**Immediate consumer:** `SM_BB_PUMP` → `bb_broker_drive_expr(EXPR_t*)` →
walks entire subtree with `coro_eval` / `coro_drive`.  This is the
largest IR-walking consumer; requires SM_SUSPEND / SM_RESUME
infrastructure (Step 14) before migration.

**Migration step:** Steps 14–15 (generator infrastructure + per-kind).

---

**Line 986 — Prolog backtracking nodes** (E_CHOICE, E_CLAUSE, E_CUT,
E_UNIFY, E_TRAIL_MARK, E_TRAIL_UNWIND):

**Context:** Prolog clause body — handed to BB via SM_BB_ONCE.

**Immediate consumer:** `SM_BB_ONCE` → `bb_broker_drive_expr` with
backtracking semantics.

**Migration step:** Step 16.

---

**Line 993 — Raku E_CASE:**

**Context:** `case` expression — handed to BB via SM_BB_PUMP.

**Migration step:** Step 13.

---

### Site E — line 1232 (Icon main() synthesis)

**Context:** Synthesised E_FNC call node `main()` for Icon programs.
`SM_BB_PUMP` drives it after the stmt loop.

**EXPR_t kind:** freshly constructed E_FNC with sval="main", one E_VAR
child.

**Immediate consumer:** `SM_BB_PUMP` → `coro_call(EXPR_t *proc, ...)`
which walks `proc->children[]` for parameter names and E_GLOBAL
declarations.  Separate side-channel from the proc_table
(`proc_table[i].proc` set by `polyglot_init`).

**Migration step:** Step 12 (simplest M4 site — generator-free main
bodies; conditional on Step 14 if main uses generators).

---

## Summary table

| Site | Line(s) | E_kind deferred | Consumer opcode | Migration step |
|------|---------|-----------------|-----------------|----------------|
| A    | 345, 386 | non-E_QLIT arg  | SM_PAT_CAPTURE_FN_ARGS | Step 4 |
| B    | 470     | non-E_QLIT arg  | SM_PAT_USERCALL_ARGS   | Step 3 |
| C    | 573     | E_DEFER child (any) | DT_E thaw in snobol4_pattern.c | Step 2 |
| D1   | 975     | Icon generator node | SM_BB_PUMP → bb_broker | Step 14–15 |
| D2   | 986     | Prolog clause node  | SM_BB_ONCE → bb_broker | Step 16 |
| D3   | 993     | Raku E_CASE node    | SM_BB_PUMP → bb_broker | Step 13 |
| E    | 1232    | synth E_FNC main    | SM_BB_PUMP → coro_call | Step 12 |

Sites migrated in M1 (Steps 2–4): **C, B, A** — all SNOBOL4/Snocone
paths.  Sites D1/D2/D3/E are M4 work; they require generator
infrastructure not yet present.

---

## Consumer classification

**Full IR-walk consumers (must be replaced):**
- `snobol4_pattern.c:222` — DT_E thaw: reads `frozen->kind`,
  `->sval`, `->nchildren`, `->children[i]`.  Replaced by `SM_CALL_CHUNK`
  dispatch in Steps 2–4.
- `bb_broker_drive_expr(EXPR_t*)` — entire BB engine.  Replaced by
  `bb_broker_drive_sm(entry_pc)` in Steps 14–16.
- `coro_call(EXPR_t *proc, ...)` — walks param names + E_GLOBAL decls.
  Replaced by `sm_call_proc(int entry_pc, ...)` in Step 17.

**Reference-only consumers (already compiled, need only entry_pc):**
- Pattern variables already bound to DT_P: optimisation noted in
  Site C above; not required for Step 2 correctness.
