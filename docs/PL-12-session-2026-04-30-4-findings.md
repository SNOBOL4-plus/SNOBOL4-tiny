# PL-12 — session 2026-04-30 #4 findings

## Baseline at session start

- one4all `75d5775b`, corpus `2a69e92`, .github `94e86ca`.
- smoke 5/5, broker 49/49, SWI suite **43/57 = 75%** (gate ≥ 80%).
- Previous session #3 landed the directive-cenv fix in polyglot.c (per-directive
  pl_env_new for var-binding propagation).

## What was attempted

Session #2's plan was to implement `pl_invoke_term` with a proper Term→EXPR
bridge so that `catch(Var, _, _)` (where Var holds a callable Term) actually
runs the bound Goal — addressing the silent-success bug behind `bips`,
`arg`, `is_most_general_term`, `term_singletons` MISS suites.

This session implemented exactly that, plus discovered and fixed two latent
bugs that the bridge surfaced. **All three changes together produce a
worse regression than session #2 (43 → 7) and were NOT committed.**

## Three changes attempted (all in src/runtime/interp/pl_runtime.c)

### Change A: pl_term_to_goal_expr / pl_invoke_term (197 lines)

New static helpers:
- `pl_synth_expr_new(EKind)` — constructor for synthesized EXPR_t.
- `pl_synth_expr_add_child(EXPR_t*, EXPR_t*)` — append child, realloc-grown.
- `pl_env_lookup_or_add(Term*, Term***, int*, int*)` — dedup TT_VAR Term*s
  by pointer identity into a tenv array.
- `pl_term_to_goal_expr(Term *t, Term ***penv, int *pn, int *pcap)` — recursive
  Term→EXPR bridge mirroring `prolog_lower.c::lower_term` exactly:
  TT_VAR → E_VAR ival=k with tenv[k]=Term*var,
  TT_INT → E_ILIT, TT_FLOAT → E_FLIT, TT_ATOM → E_FNC nchildren=0,
  =/2 → E_UNIFY, +/-/*///mod → E_ADD/E_SUB/E_MUL/E_DIV/E_MOD,
  general compound → E_FNC sval=fn nchildren=arity.
- `pl_free_synth_expr(EXPR_t*)` — recursive free.
- `pl_invoke_term(Term *gt, Term **caller_env)` — public entry point.
  Builds synth+tenv, dispatches via `pl_box_goal_from_ir(synth, tenv) +
  bb_broker(box, BB_ONCE, NULL, NULL)`, frees, returns ok.

Wired into catch/3 at line ~1755: when `goal_e->kind == E_VAR`, deref the
env-resolved Term and call `pl_invoke_term(gt, env)` instead of falling
through to `interp_exec_pl_builtin`'s default `return 1` arm.

### Change B: pl_unified_term_from_expr — add E_UNIFY/E_CUT/E_NUL cases (12 lines)

The original switch at line 245 covered E_QLIT/E_ILIT/E_FLIT/E_VAR/
E_ADD/E_SUB/E_MUL/E_DIV/E_MOD/E_FNC and fell through to
`default: return term_new_atom(prolog_atom_intern("?"))` for everything
else. Crucially this fired for **E_UNIFY** — meaning any directive of
shape `G = (X = 5)` silently bound G to atom("?") instead of the
unification compound. Repro:

```prolog
:- G = (X = 5), write(G), nl.    % printed `?` (was supposed to be =(X,5))
```

This was a latent bug that fix #2 surfaced (because catch's E_VAR-goal
arm now actually USES the Term value of G; before, the Term contents of
G never mattered).

Added cases:
- `E_UNIFY → term_new_compound("=", 2, [child0, child1])`
- `E_CUT   → term_new_atom("!")`
- `E_NUL   → term_new_atom("[]")`

### Change C: findall snapshot — pl_unified_deep_copy → pl_copy_term (1 line)

Was carryover in the working tree from a prior session — kept intact.
`pl_unified_deep_copy` collapsed every TT_VAR to the literal atom `_`,
destroying variable sharing across one snapshot. plunit's
`findall(t(N,O,G), pj_test(Suite,N,O,G), Tests)` needs Opts and Goal to
share variables (e.g. test_list memberchk's `X` referenced in Opts
`true(X==y)` and in Goal `memberchk(f(X,a),...)`). `pl_copy_term` uses
the existing `CopyVarMap` so vars within ONE snapshot share fresh-var
identity but vars across snapshots are distinct.

## Repro results — all five primary repros pass

`/tmp/pl_invoke_repro1.pl`:

| Repro | Baseline | After all 3 changes |
|---|---|---|
| 1: `catch(fail, _, fail)` literal | failed_good_direct_fail ✅ | failed_good_direct_fail ✅ |
| 2: `G=fail, catch(G,_,fail)` | succeeded_bad ❌ | failed_good_var_fail ✅ |
| 3: `G=(X=5), catch(G,_,fail), X==5` | failed_bad ❌ | succeeded_X_5 ✅ |
| 4: `G=(A is 3+4), catch(G,_,fail), A==7` | failed_bad_arith ❌ | succeeded_A_7 ✅ |
| 5: post-assertz `term_singletons(X+X+_Y, [_,_])` | succeeded_bad_ts ❌ | failed_good_ts ✅ |

## SWI suite regression — 43/57 → 7/57 (12% coverage)

smoke 5/5 ✅, broker 49/49 ✅, but the SWI plunit harness collapsed: 36
suites that previously emitted `pass: SUITE:NAME` now emit `FAIL: SUITE:NAME
(goal failed)`. The verdict count itself is fine — `MISS PASS` lines mean
plunit's individual-test driver dispatched correctly, the test goal just
ran wrong.

### Concrete failing case — test_list / memberchk

Standalone:

```prolog
member(X,[X|_]). member(X,[_|T]) :- member(X,T).
memberchk(X,L) :- member(X,L), !.
main :- catch(memberchk(f(X,a), [f(x,b),f(y,a)]), _, fail), write(X), nl.
%   → prints `y` ✅
```

After plunit's assertz round-trip:

```prolog
:- assertz(stored(memberchk(f(X,a), [f(x,b), f(y,a)]))).
main :- stored(Goal), catch(Goal, _, fail), write(X), nl.
%   → prints `_G1` ❌ (X is unbound; pl_invoke_term bound a different
%      variable — the one inside the asserted-clause cenv — and that
%      cenv was freed before main could observe the binding)
```

The asserted clause has its own cenv allocated at head-unify time inside
`pl_box_choice_call`. When `stored(Goal)` is invoked, head-unify constructs
fresh TT_VAR Terms in that cenv and unifies them with caller args. The
caller's `Goal` slot binds (via TT_REF) to the asserted-clause's
`memberchk(...)` Term, which contains TT_VAR Terms from the
**asserted-clause's cenv**.

When `pl_invoke_term` then walks that Term graph, it finds TT_VAR Terms
that point into the asserted-clause cenv. It places those Term*s into
`tenv[]` and dispatches the synth via `pl_box_goal_from_ir`. The synth's
user-call dispatch (memberchk) creates yet another cenv, runs, binds X
to `y` via the trail. Trail bindings on the asserted-clause cenv's
TT_VARs persist ✓.

But the **caller's source-level X** (e.g. `main`'s `X` in `(X == y -> ...)`)
is in main's cenv — a DIFFERENT Term*. The chain from main's X to the
asserted-clause's X is one TT_REF hop (set during the assertz directive
when `stored(memberchk(f(X,a),...))` Term was built referencing
directive_cenv[X_slot], then asserted, then on retrieval main's X-slot
TT_VAR unified with the structure-shared compound).

Hypothesis: that REF chain is broken/lost somewhere. Either:

(a) The directive-cenv freed by polyglot.c session #3 fix invalidated the
    Term*s that the asserted clause's EXPR references. The asserted
    EXPR_t holds `E_VAR ival=k` indices, not Term* pointers — but on
    retrieval a fresh asserted_cenv is allocated and head-unify chains
    main's X to a TERM IN THAT cenv, NOT to the freed directive_cenv. So
    this is probably not the issue.

(b) `pl_invoke_term`'s tenv usage doesn't preserve the caller-cenv-to-
    asserted-cenv link. When the synth E_VAR ival=k resolves to
    tenv[k] = (asserted_clause cenv's X-Term), the unify happens against
    that Term — which IS the same Term that main's X is TT_REF-chained
    to via head-unify. So the binding SHOULD propagate.

(c) Something subtler: the head-unify code in pl_box_choice_call's
    pl_head_unify_fn at pl_broker.c:223-248 calls `unify(ca, ha, &g_pl_trail)`
    where `ca` is the caller arg and `ha` is built from the asserted
    clause's E_VAR via `pl_unified_term_from_expr(ec->children[i], cenv)`.
    For an asserted clause with no head args (n_args=0) — wait, pj_test
    has 4 args including the goal. So head-unify chains main's
    `Goal` slot to asserted_cenv[Goal_slot] (TT_VAR) which got built from
    the assertz directive carrying directive_cenv[some_slot] which by
    session #3's fix was freed at end of directive. **That's the issue.**
    The assertz body's TT_VAR Term*s reference the directive's cenv array
    that session #3 frees.

This is a critical follow-up: session #3's directive cenv free interacts
badly with assertz's Term storage. The Term graph asserted contains
**Term*** pointers, and if those Terms live in directive_cenv (which is
freed), they become dangling.

But how did 43/57 suites pass under session #3? Because the
**asserted EXPR_t** (built by `pl_assert_term` → `lower_clause`)
re-walks the Term graph and emits FRESH `E_VAR ival=k` nodes — it
doesn't store Term* pointers. The Term graph IS used by `lower_term`
to read `t->saved_slot`, but the resulting EXPR holds slot integers.
Only the ATOMIC content (atom_ids, ints, floats) is preserved.

So on retrieval, `pl_unified_term_from_expr(ec->children[i], asserted_cenv)`
re-builds Term graphs FRESH from the asserted EXPR. The directive-cenv
Terms ARE NOT REFERENCED. ✓

**So the actual regression cause is something else.** Most likely:
my synth + tenv dispatch is binding TT_VARs in tenv directly, but the
caller's X is bound to a different TT_VAR via the head-unify TT_REF
chain. The trail unwind on `pl_box_choice_call` β might be unbinding
the chain too eagerly. Needs more investigation.

## Decision

Per RULES.md "regression-in-error-class": **NOT COMMITTED.**

- Saved diff at `docs/PL-12-session-2026-04-30-4-attempt.diff` (264 lines).
- Reverting `src/runtime/interp/pl_runtime.c` to baseline `75d5775b`.
- Verified smoke 5/5, broker 49/49, SWI 43/57 hold.
- Committing this findings document and the diff as docs.

## Status at end of session

| | start | end |
|---|---|---|
| one4all HEAD | 75d5775b | 75d5775b + docs commit |
| corpus HEAD | 2a69e92 | 2a69e92 |
| .github HEAD | 94e86ca | 94e86ca + goal-file update |
| smoke_prolog | 5/5 | 5/5 |
| smoke_unified_broker | 49/49 | 49/49 |
| SWI suite | 43/57 = 75% | 43/57 = 75% |

## Recommendation for next session — fix #2 v3

Before touching `pl_runtime.c` again, write a smaller failing repro:

```prolog
% Does main's X get bound through assertz round-trip + pl_invoke_term?
:- assertz(stored(=(X, hello))).      % test the simplest case: G is just =/2
main :- stored(Goal), catch(Goal, _, fail), write(X), nl.
%   Expected: hello.   Actual after fix #2: ???
```

Trace from there. Specifically, trace at:
1. `pl_box_choice_call`'s head-unify exit — what is main's X bound to?
2. `pl_invoke_term`'s tenv[k] population — is it the same Term* main's X
   is REF-chained to?
3. `pl_box_unify`'s α and β arms — does β fire spuriously and unwind?

The session #3 directive-cenv fix is correct and stays. Change C
(findall snapshot) is correct and stays. Change B (E_UNIFY in
pl_unified_term_from_expr) is correct and stays. Only change A's
`pl_invoke_term` is failing to propagate caller-binding correctly —
that's the surface-area to investigate next.

If a clean separation can be achieved, change B alone might be safely
committed first as a pre-fix #2 cleanup (it's a bug independent of
fix #2). Verify it doesn't move the gate either way before committing.

Same for change C if it doesn't already live somewhere committed.
