# PL-12 session #6 (2026-04-30) — Fix #2 v3 attempt: bridge correct, exposes corpus stdlib gaps

## Outcome

Fix #2 v3 (Term→synth-EXPR bridge dispatching via `interp_exec_pl_builtin`)
**implemented and proven mechanically correct**, but full SWI suite collapsed
from 43/57 → 5/57. **Per RULES.md regression-in-error-class, NOT COMMITTED.**

This session diff is at `docs/PL-12-session-2026-04-30-6-attempt.diff`.

The collapse is NOT a runtime regression. It is the **silent-success default
arm being eliminated**, which exposes a swarm of pre-existing
undefined-predicate gaps in the SWI test corpus's `plunit.pl`. The bridge
is correct; the baseline 43/57 was a false-positive ceiling.

## Mechanical proof of correctness

Three primary repros (sessions #2/#3/#4 inherited targets):

| Repro | Baseline | After v3 |
|---|---|---|
| 1: `G=fail, (catch(G,_,write(caught)) -> succ ; failed)` | succeeded ❌ | failed ✅ |
| 2: `G=(X=5), catch(G,_,fail), [ok,X]` | failed ❌ | [ok,5] ✅ |
| 3: `G=(A is 3+4), catch(G,_,fail), [ok,A]` | failed ❌ | [ok,7] ✅ |

Decisive bridge-correctness repro (with stdlib defined locally):

```prolog
member(X,[X|_]).
member(X,[_|T]) :- member(X,T).
memberchk(X, L) :- member(X, L), !.
main :- G = memberchk(f(X,a), [f(x,b), f(y,a)]),
        ( catch(G, _, fail) -> write([ok,X]) ; write(failed) ), nl.
```

Output: `[ok,y]`. The bridge correctly invokes the asserted goal,
memberchk binds X via deep TT_REF chain through the bridge's tenv slot
back to the caller's source-level X. **Session #4's hypothesised
lifecycle bug does not exist** when the predicate is defined.

## Why the suite collapses anyway

The bridge correctly dispatches `catch(Goal,_,_)` to actually invoke the
goal. The previous default-arm silent-success made plunit's
`pj_do_succeed` register a PASS for any test that didn't trip a recognised
control-flow path. With the bridge, the test goal really runs — and many
of them error with `undefined predicate <name>/<arity>` because plunit.pl
doesn't define them.

Per-suite undefined-predicate report (collected by running each test_*.pl
through scrip with the bridge active and grepping stderr):

| Suite | Undefined predicates surfaced |
|-------|-------------------------------|
| test_arith | format/3 |
| test_bips | atom_to_term/3, is_most_general_term/1, length/2, stream_property/2, term_variables/2 |
| test_call | apply/2, call/0, call/1, call/2, call/3, clause/2, false/0, op/3, setup_call_cleanup/3, user/0 |
| test_dcg | \+/3, expand_goal/2, expand_term/2, setof/3 |
| test_list | memberchk/2 |
| test_misc | $current_prolog_flag/5 |
| test_string | number_string/2, split_string/4, string_bytes/3, string_chars/2, string_codes/2, string_lower/2, string_upper/2 |
| test_term | between/3, clause/2, compound_name_arguments/3, compound_name_arity/3, numbervars/4 |

## Architecture / lifecycle conclusion

Session #4's narrative speculated that "the asserted-clause cenv's TT_VARs
that pl_invoke_term binds are not visible to the caller's source-level
vars." **That hypothesis is incorrect.** The decisive repro proves the
bindings DO propagate when the predicate is defined.

The actual root cause of the 43→5 (or 43→7 in session #4) collapse:
silent-success was hiding a corpus stdlib gap of ~25 predicates. Removing
silent-success exposes them all at once.

## v3 bridge design

`pl_term_to_synth_expr(t, tenv, &n_tenv)` walks the Term recursively,
emitting an EXPR_t whose:
- TT_VAR -> E_VAR ival=slot, with tenv[slot] = the original Term* (deduped
  by pointer identity so X+X collapses to one slot)
- TT_INT -> E_ILIT, TT_FLOAT -> E_FLIT
- TT_ATOM -> E_FNC nchildren=0 sval=name
- TT_COMPOUND with `=`/2 -> E_UNIFY
- TT_COMPOUND with arith ops -> E_ADD/SUB/MUL/DIV/MOD
- TT_COMPOUND otherwise -> E_FNC sval=fn nchildren=n

`pl_invoke_var_goal(gt, caller_env)`:
1. deref gt; reject if NULL/var/number
2. allocate tenv (stack array, PL_SYNTH_TENV_MAX=64)
3. build synth via `pl_term_to_synth_expr`
4. dispatch via `interp_exec_pl_builtin(synth, tenv)`  — NOT pl_box_goal_from_ir
   (avoiding session #4's bb_broker entry; the recursive interp path
   already handles `,/`, `;`, `->`, `\+`, `once`, builtins, user-call)
5. free synth, return ok

Key difference from session #4:
- Session #4 dispatched via `pl_box_goal_from_ir + bb_broker(BB_ONCE)`
- v3 dispatches via direct `interp_exec_pl_builtin` recursion
- Both should be correct in principle; v3 is simpler and the proof
  repro shows it works end-to-end when predicates are defined

Wired into catch/3 at line 1721 (post Change-B numbering):
- new branch: `if (goal_e && goal_e->kind == E_VAR) ok = pl_invoke_var_goal(...)`
- existing user-call and else branches preserved

## NOT wired into \+ /once /not /call

Same defect applies — `\+/1` (line 681), `once/1` (line 687), and any
`call/1` if it existed all dispatch `interp_exec_pl_builtin(child, env)`
which falls through default for E_VAR. Wiring v3 into these would expose
even more corpus gaps; deferred until corpus stdlib is patched.

## Path to PL-12 ≥80% gate (2-step)

**Step A — corpus stdlib enrichment** (~25 predicates above):
add to `/home/claude/corpus/programs/prolog/plunit.pl` or as a separate
`stdlib_swi.pl` that the suite script concatenates. Easiest implementations:

```prolog
memberchk(X, L) :- member(X, L), !.
length([], 0).
length([_|T], N) :- length(T, N0), N is N0 + 1.
between(L, H, X) :- integer(X), !, X >= L, X =< H.
between(L, H, L) :- L =< H.
between(L, H, X) :- L < H, L1 is L+1, between(L1, H, X).
false :- fail.
... etc.
```

After Step A, baseline suite count should already rise (silent-success
will start producing real PASS for tests where the goal genuinely succeeds).
Re-measure baseline 43/57 → likely 50+/57.

**Step B — land Fix #2 v3** (this session's bridge code):
the diff is preserved at `docs/PL-12-session-2026-04-30-6-attempt.diff`,
re-apply, rebuild, re-run suite. Expected: clears 80% gate.

Step A must come first because the bridge's correctness is gated on the
corpus actually defining the predicates the tests call.

## Files committed this session

- `docs/PL-12-session-2026-04-30-6-attempt.diff` — full v3 bridge diff (211 lines)
- `docs/PL-12-session-2026-04-30-6-findings.md` — this narrative
- `.github/GOAL-LANG-PROLOG.md` — appended session #6 narrative
- `.github/PLAN.md` — PL-12 step text refreshed
