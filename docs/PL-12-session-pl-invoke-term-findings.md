# PL-12 — `pl_invoke_term` attempt findings (session 2026-04-30 #2)

## Baseline at session start
- one4all `f71d9dec`, corpus `2a69e92`, .github `6f24875`.
- smoke 5/5, broker 49/49, SWI suite **43/57 = 75%** (gate ≥ 80%).
- 14 MISS suites: rem, float_zero, float_special, float_compare,
  max_integer_size, moded_int (test_arith); bips, arg, is_most_general_term
  (test_bips); snip (test_call); steadfastness, context (test_dcg); string
  (test_string); term_singletons (test_term).

## What was attempted

The previous session's plan called for three fixes; this session targeted
**fix #2**: `catch(Var, _, Recovery)` where `Goal` is a runtime variable bound
to a callable Term.

### Diagnostic phase (reverted before commit)

1. Reproduced `term_singletons` aliasing failure with two minimal repros:
   - `assertz(test_g(term_singletons(X+X+_Y, [_,_])))` + `test_g(G), catch(G,_,fail)`
   - **Direct call** `catch(term_singletons(X+X+_Y, [_,_]), _, fail)` → correctly
     prints `FAILED_GOOD` ✅
   - **Post-assertz** call → prints `SUCCEEDED_BAD` ❌

2. Instrumented `pl_assert_clause` with a recursive EXPR_t dump under
   `DBG_ASSERTZ`. Found that the lowering is **correct**:
   - `n_vars=4`, slots assigned distinctly as `_V0, _V0, _V1, _V3, _V2`.
   - `prolog_lower.c`'s two-pass anon-slot assignment is working.
   - **Fix #1 from the plan (anon-var post-assertz aliasing) is NOT actually
     a lowering-layer bug.** Lowering already produces the correct E_VAR slot
     layout for plunit-spliced test bodies.

3. Instrumented `pl_unified_term_from_expr` with `DBG_UTFE`. Confirmed:
   - When the asserted clause is invoked via head unification, the E_VAR
     children correctly index into a fresh `cenv` of size 4.
   - The actual silent-success path is in `interp_exec_pl_builtin`'s
     `default: return 1` arm when given an `E_VAR` (line 1595 in pre-fix
     pl_runtime.c).

4. Discovered a separate latent bug: directives with a result variable do
   not bind that variable. `:- assertz(test_g(hello)), test_g(G), write(G).`
   prints `_G0`. The directive STMT_t path in `interp.c` line 4445-4453
   calls `interp_eval(s->subject)` with no clause env wrapper. Did not
   investigate further this session.

### Fix attempt: `pl_invoke_term`

Added a 105-line static helper `pl_invoke_term(Term *gt, Term **env)` in
`pl_runtime.c` between `pl_copy_term` and `interp_exec_pl_builtin`.
Wired it into the `catch/3` builtin dispatch with a guard:

```c
if (goal_e && goal_e->kind == E_VAR) {
    Term *gt = pl_unified_term_from_expr(goal_e, env);
    ok = pl_invoke_term(gt, env);
} else if (is_pl_user_call(goal_e)) {
    /* existing path */
}
```

The helper:
- Derefs the Term, extracts functor/arity from TT_ATOM or TT_COMPOUND.
- Looks up user-defined preds in `g_pl_pred_table` and dispatches via
  `pl_box_choice` + `bb_broker(BB_ONCE)`. **This part works correctly**
  for user-defined clauses asserted via `assertz`.
- For builtins (no entry in pred table), synthesizes an `E_FNC` with
  `E_VAR` placeholder children and a temporary `tenv` where each
  `tenv[i]` is unified with `targs[i]`, then dispatches via
  `interp_exec_pl_builtin(&syn, tenv)`.

### Outcome of fix attempt

- Both repro tests now print `FAILED_GOOD` ✅
- smoke 5/5 ✅
- broker 49/49 ✅
- **SWI suite: 10/57 = 17% — MASSIVE REGRESSION**, 33 suites lost.

### Root cause of the regression

The synthetic-EXPR-with-tenv approach for builtin dispatch breaks
arithmetic. Specifically, `is/2` in `interp_exec_pl_builtin` calls
`pl_unified_eval_arith_term(goal->children[1], env)` — but
`goal->children[1]` is now an `E_VAR ival=1` whose `tenv[1]` holds the
runtime Term `5+5` (a TT_COMPOUND). `pl_unified_eval_arith_term`'s `E_VAR`
branch returns the Term as-is without traversing it as an arithmetic
tree, so `is/2` sees the unevaluated compound and unifies the LHS with
it — incorrect.

The same defect bites every test that uses `catch(Goal,...)` from
plunit's `pj_has_true`/`pj_do_*` predicates around an `is/2`, `=:=/2`,
`=/2`, `==/2`, `format/2`, etc. body. That covers most of test_arith,
test_bips, test_call, test_term, etc.

The architectural problem: `interp_exec_pl_builtin` expects EXPR_t
trees built by the lowerer for arithmetic / unification arguments —
not arbitrary runtime Terms reached through env. Bridging Term→EXPR
for arbitrary-shaped goal arguments would require either:

  (a) walking the Term and rebuilding a full EXPR_t tree (E_ADD/E_SUB/...
      for arith Terms, E_FNC for compound user calls, recursively); OR
  (b) extending `pl_unified_eval_arith_term` (and friends) to recognise
      a TT_COMPOUND-via-E_VAR and recurse on the Term's args; OR
  (c) using `pl_box_goal_from_ir` to build a goal box from a synthesized
      EXPR_t that more faithfully reflects the Term's structure.

None of these is small.

## Decision

Per RULES.md "regression-in-error-class" guideline: **NOT COMMITTED.**

- Saved the diff at `docs/PL-12-session-pl-invoke-term-attempt.diff` (143 lines).
- Reverted `pl_runtime.c` to baseline.
- Verified clean baseline: smoke 5/5, broker 49/49, SWI 43/57 = 75%.

## Recommendation for next session

The original plan's fix #1 (`pl_copy_term` in `pl_assert_clause`) is
**not the actual bug** — diagnostics confirm lowering produces
distinct slots for the two `_` placeholders in plunit-spliced asserts.
**Reframe fix #1 as: investigate the directive-binding bug** — the
deeper issue that `:- assertz(test_g(hello)), test_g(G), write(G)`
prints `_G0` instead of `hello`. The plunit harness threads test goals
through assertz/retrieve cycles inside directive bodies; if directives
allocated a proper top-level binding env, the term_singletons path
might come right without touching `pl_invoke_term`.

The original plan's fix #2 (`pl_invoke_term`) is **architecturally
correct** for user-defined assertz'd Goals, but needs a **proper
Term→EXPR bridge** for builtin arithmetic/unification. Approach (a)
above (walk Term, build full EXPR tree) is the most likely path:
recursive helper `term_to_goal_expr(Term *t)` returning a synthesized
EXPR_t whose structure matches the lowerer's output for the equivalent
source. Combine with `pl_box_goal_from_ir(synth, NULL)` to dispatch.

Specifically — for `is/2` Term `is(A, +(5,5))`:
  - Build E_FNC sval="is" arity=2.
  - Child 0 = synthesized E_VAR or E_QLIT depending on `A`'s tag.
  - Child 1 = synthesized E_ADD with children for `5` and `5`.
A small mapping table from atom_id → EKind handles the operators
(`+` → E_ADD, `=` → E_UNIFY, `,` → E_FNC sval="," etc.). For
unrecognised functors, fall back to E_FNC with the functor name.

The plan's fix #3 (plunit `pj_do_fail` throw-vs-succeed) remains
unblocked and could be tried independently — it changes only corpus
plunit.pl, no runtime change required, and gives `string` +1.

## Status at end of session

| | start | end |
|---|---|---|
| one4all HEAD | f71d9dec | f71d9dec |
| corpus HEAD | 2a69e92 | 2a69e92 |
| .github HEAD | 6f24875 | 6f24875 |
| smoke_prolog | 5/5 | 5/5 |
| smoke_unified_broker | 49/49 | 49/49 |
| SWI suite | 43/57 = 75% | 43/57 = 75% |

No commits. Two new files in working tree (uncommitted, can be discarded
or committed as docs):
- `docs/PL-12-session-pl-invoke-term-attempt.diff`
- `docs/PL-12-session-pl-invoke-term-findings.md` (this file)
