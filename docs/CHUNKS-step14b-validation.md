# CHUNKS-step14b — gen-local slot infrastructure

**Closed:** session #71, 2026-05-07.  Pure addition; zero behaviour change for
existing programs (no producer emits the new opcodes yet).

## What landed

Two new opcodes:

```
SM_LOAD_GLOCAL  a[0].i = slot   ; push SmGenState->locals[slot] onto value stack
SM_STORE_GLOCAL a[0].i = slot   ; pop TOS into SmGenState->locals[slot] (and re-push)
```

A new fixed-size `locals[SM_GEN_LOCAL_MAX]` array on `SmGenState` (capacity 8 —
the largest CHUNKS-step15 generator kind needs 4).  The slots survive
`SM_SUSPEND`/`SM_RESUME` automatically because `SmGenState` is the persistent
envelope that `bb_broker_drive_sm` allocates per generator invocation.

Outside a generator drive (`g_current_gen_state == NULL`), both opcodes
push `FAILDESCR` and clear `last_ok` — mirrors `SM_PUSH_VAR`'s FAIL-on-unset
discipline.

`g_current_gen_state` was promoted from `static` (in `sm_interp.c`) to
file-scope-extern so the JIT codegen mirrors can reach it.  Declaration
added to `sm_interp.h`.

## Why

`SM_SUSPEND` snapshots the value stack across yields, but the stack is a
LIFO with no `DUP`, `SWAP`, or `PEEK_AT_DEPTH`.  Mid-loop generators
(`E_TO`, `E_TO_BY`, `E_LIMIT`, `E_BANG_BINARY`, …) need to maintain
`lo`/`hi`/`step`/`cur`-style state across yields and compute on it without
losing it.  Storing in NV doesn't isolate concurrent invocations of the
same chunk (e.g. nested `every i := 1 to 10 do every j := 1 to 5 do …`)
— two `SmGenState`s with the same `entry_pc` would share the NV slot and
clobber each other.

The `locals[]` array on `SmGenState` is the per-invocation equivalent of
`icn_to_state_t` allocated fresh by `coro_bb_to`.  Each chunk invocation
gets its own.  Static count per kind, encoded in the lowering.

## Tests

`scripts/test_sm_generator_ch14.sh` (existing CH-14 gate, extended) runs
three test suites in `src/runtime/x86/sm_interp_test.c`:

1. `test_generator_suspend_resume` — CH-14 baseline (3-yield hand-emitted
   gen).  Still 7/7 PASS, no behaviour change.
2. `test_gen_locals_survive_suspend` — initialize `locals[0]=100`, yield,
   `locals[0]++`, yield, `locals[0]++`, yield.  Verifies 100/101/102 — the
   locals are preserved across SUSPEND/RESUME.  5/5 PASS.
3. `test_gen_locals_isolated_per_invocation` — two `SmGenState`s on the
   same chunk, each getting its own `locals[0]`.  Both yield 100/101 with
   no cross-contamination.  6/6 PASS.

Total CH-14 gate: 18/18 PASS (was 7/7 pre-step).

## Gates

- Smoke ×6: 7+5+5+5+5+4 = 31 PASS, 0 FAIL (byte-identical to baseline)
- Isolation: PASS
- Unified broker: PASS=49 (baseline)
- csnobol4 Budne: PASS=36 (≥34, baseline)
- Icon corpus: 186 PASS / 47 FAIL / 30 XFAIL / 263 TOTAL (byte-identical to baseline)

## Scope boundary

This rung is pure addition.  No producer emits `SM_LOAD_GLOCAL` /
`SM_STORE_GLOCAL` yet; the consumer paths exist and are tested in isolation.
The first producer is `CHUNKS-step15a` (E_TO + E_TO_BY migration), which
will allocate 3 slots (lo/hi/cur) and 4 slots (lo/hi/step/cur) respectively
for those kinds.

JIT support: SM_LOAD_GLOCAL / SM_STORE_GLOCAL handlers in `sm_codegen.c`
mirror the SM-interp handlers exactly.  Both run against the same global
`g_current_gen_state` set by `bb_broker_drive_sm`.  Generator JIT proper
(SM_SUSPEND / SM_RESUME bytecode emission) remains M5/EM-10 territory and
still aborts with named FATAL.
