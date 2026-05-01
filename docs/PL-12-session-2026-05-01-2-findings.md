# PL-12 session 2026-05-01 #2 — Step D decided; plunit suite-skip fix landed

## Outcome

Step D (harness scoring decision) closed: keep block-level scoring. One
new corpus commit (`programs/prolog/plunit.pl` — suite-level skip
support). Bridge stays held back. SWI baseline 43/57 preserved on every
gate run; smoke 5/5 green; broker 49/49 green.

| # | Repo | Commit | Effect |
|---|------|--------|--------|
| Step D decision | (doc only) | this file | block-level scoring kept |
| plunit suite-skip | corpus | `ada87b6` | begin_tests/2 honors `condition(...)`; pj_skip_cond pattern-matches goal shapes |

## Step D — harness scoring decision: keep block-level

**Decision: keep block-level scoring (`util_swi_match.py` unchanged).**

Reasoning:

1. The block-level metric is honest about correctness. A block PASSes
   only when every sub-test in it actually runs and produces the right
   answer. SCRIP claiming "Prolog conformance" with bridge-induced 80%
   coverage where individual blocks are half-broken is the kind of false
   claim the bootstrap-rigor culture rejects.

2. The 43/57 baseline is corrupt (silent-success false-positives), but
   switching to per-test scoring with the **same** false-positives
   yields bigger false numbers, not better signal. The honest move is
   to land the bridge and accept the dip until sub-test work makes
   bridge-on coverage genuinely cross 80%.

3. PL-12's "PASS ≥ 80% of tests in suite" is operationalised by the
   harness at block level. The gate is what the harness measures.
   Changing measurement doesn't change correctness.

The matcher is honest in another way too: a quick check confirmed the
43/57 baseline is **not** an artefact of the matcher granting MISS-
credit. plunit emits exactly one block-level `PASS Suite` or
`FAIL Suite` line per `:- begin_tests(Suite).` block. The 43 includes
many blocks where every sub-test silently passes via the catch-Goal
E_VAR fall-through (pj_do_succeed's first clause), but the block-level
emission itself is genuine plunit output, not matcher inference.

## Step F (probe pass) — bridge-applied per-suite breakdown

Applied bridge in a probe to capture FAIL distribution. Results:

| Suite | ref blocks | bridge-on PASS | bridge-on FAIL | sub-pass | sub-fail | sub-skip | notes |
|---|---|---|---|---|---|---|---|
| test_arith | 26 | 7 | 5 (+14 not-emitted) | 74 | 47 | 1 | **segfault inside bigint** kills run partway |
| test_bips | 6 | 0 | 0 | 0 | 0 | 0 | early failure (3 undef preds + segfault) |
| test_call | 9 | 0 | 0 | 0 | 0 | 0 | **127 parse errors** — file doesn't fully parse |
| test_misc | 1 | 0 | 1 | 0 | 3 | 0 | |
| test_string | 2 | 0 | 2 | 5 | 16 | 0 | |
| test_dcg | 5 | 3 | 3 | 24 | 12 | 3 | |
| test_exception | 2 | 1 | 1 | 3 | 2 | 1 | |
| test_term | 5 | 1 | 4 | 22 | 20 | 10 | |
| test_list | 1 | 1 | 0 | — | — | — | |

**Dominant failure mode:** segfault inside `test_arith.bigint` cuts off
14 of 26 blocks before they ever emit a verdict. The bigint suite
explicitly carries `condition(current_prolog_flag(bounded, false))`,
meaning "skip this entire suite if integers are bounded." Our plunit
declares `bounded=true`, so the entire suite **should** skip.

**Why it doesn't skip (root cause #1):** `begin_tests(Suite, Opts)` in
the plunit shim discarded `Opts` entirely. Suite-level options were
never recorded, never checked. Per-test `pj_run_one` calls
`pj_skip_cond(TestOpts)` but there was no equivalent for suite Opts.

**Why even the per-test path didn't work (root cause #2):**
`pj_skip_cond(Opts) :- member(condition(C), Opts), \+ C.` Where C
becomes Var-bound (as it does after `member` binds C from the
condition(...) wrapper), `\+ C` hits the same E_VAR dispatch bug the
v3 bridge fixes for `catch/3`. The Var-bound goal is never evaluated;
`\+` returns success regardless of what C would have evaluated to.
Confirmed direct repros:

```prolog
G = fail, ( \+ G -> ... ; ... )    % NO (should be OK)
G = true, ( \+ G -> ... ; ... )    % NO (should be NO — coincidentally right)
G = fail, ( call(G) -> ... ; ... ) % NO (should be NO — coincidentally right)
G = true, ( call(G) -> ... ; ... ) % NO (should be OK)
```

This is the same defect family the v3 bridge addresses. The bridge
specifically targets `catch/3, \+/1, once/1, not/1` (per the bridge's
own comments).

## The plunit suite-skip fix (this session)

Two changes to `corpus/programs/prolog/plunit.pl`:

1. **`pj_suite/1` → `pj_suite/2`** — store options alongside the suite
   name. `begin_tests(Suite, Opts)` no longer discards Opts.

2. **`pj_run_suite` gets a skip-clause first** — pattern-matches on
   `pj_suite(Suite, SOpts)` and short-circuits to a skip emission if
   `pj_skip_cond(SOpts)` succeeds. The skip emits `PASS Suite`
   (matching SWI semantics: a skipped suite counts as success).

3. **`pj_skip_cond` rewritten to bypass `\+` Var-bound bug** — uses
   `pj_cond_fails/1` clause-head pattern matching to dispatch the
   condition shape directly:

   ```prolog
   pj_cond_fails(fail) :- !.
   pj_cond_fails(false) :- !.
   pj_cond_fails(true) :- !, fail.
   pj_cond_fails(current_prolog_flag(F,V)) :- !, \+ current_prolog_flag(F,V).
   pj_cond_fails(_) :- true.    /* unknown / undefined: assume fails */
   ```

   The `\+ current_prolog_flag(F,V)` call is safe because F and V are
   atoms by then — the goal is a literal compound at that source site,
   not a Var holding a goal.

This fix is **bridge-independent and gate-neutral baseline**:
- without bridge: 43/57 (75%) — exactly the same as before
- with bridge: 15/57 (24%) — up from 14/57

Smoke 5/5, broker 49/49 green on both branches.

The `+1` from bridge-on is bigint-specific: bridge alone leaves bigint
running and segfaulting partway through; with the plunit fix bigint
skips cleanly and emits `PASS bigint`. But the rest of test_arith
still doesn't progress because `minint_promotion` segfaults later in
the same file (not gated by any suite-level condition; just a runtime
arithmetic-overflow crash).

## Other findings (not actioned)

### `:- if/:- endif` directives are no-ops in scrip's Prolog

`pl_runtime.c:586` lists `if/else/endif` among silent-success directives.
There's no parser-level conditional compilation. The body between
`:- if(...)` and `:- endif.` is **always** loaded regardless of the
condition. test_arith uses these directives extensively (lines 668-671,
682-692) to gate out tests that need bigint runtime support. Those
gated tests load anyway and contribute to the segfault path.

Making `:- if/:- endif` actually skip clauses based on the condition
would gate out a meaningful fraction of the segfault-prone tests in
test_arith (and other suites). It's a parser-level change in the
directive evaluation path.

### Arithmetic overflow guards beyond Step C

Step C added a SIGFPE guard for INT_MIN/-1 in IDIV. The minint_promotion
segfault is in different territory — likely overflow in `+`/`-`/`*`
producing a wrapped value that's then fed to a heap-allocating routine
like `format(atom(X), '~w', [N])` which mis-allocates from a
sign-flipped size. Worth tracing under gdb when bandwidth permits.

## Recommendation for next session

Next-session candidates, in rough order of leverage:

1. **`:- if/:- endif` parser-level conditional compilation** —
   medium-size change to the prolog frontend, but unlocks several
   suites whose conditional sections currently load and crash.

2. **Arithmetic overflow guard sweep** — extend Step C's pattern to
   `+`, `-`, `*` (with `__builtin_*_overflow` intrinsics or explicit
   range checks). Stops the minint_promotion segfault.

3. **Land the bridge** — once (1) and/or (2) close the segfault paths
   such that bridge-on coverage stays ≥ 43/57, B.3 becomes safe to
   land cleanly.

4. **Fix the test_call parse errors** — 127 errors in a single file;
   probably a single grammar gap (operator priority, term shape) that
   unlocks 9 blocks worth of coverage.

## Files this session

- `corpus/programs/prolog/plunit.pl` — suite-skip fix (28 lines net,
  3 logical edits: pj_suite/2, pj_run_suite skip-clause,
  pj_cond_fails dispatcher).
- `one4all/docs/PL-12-session-2026-05-01-2-findings.md` — this doc.
- `one4all/docs/PL-12-session-2026-05-01-bridge.diff` — unchanged (still held back).

Working trees clean at handoff. SWI baseline 43/57 preserved.
