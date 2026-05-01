# PL-12 session #7 (2026-04-30) — copy_term_rec trail bug discovered; bridge+enrichment incomplete

## Outcome

**NOT COMMITTED** to runtime or corpus. Working tree reverted at handoff.
Two diffs preserved as committed docs:
- `docs/PL-12-session-2026-04-30-7-attempt.diff` (252 lines) — pl_runtime.c
  with v3 bridge re-applied + Change C (findall pl_copy_term) + new slot fix
- `docs/PL-12-session-2026-04-30-7-plunit.diff` (93 lines) — corpus
  plunit.pl stdlib enrichment (~50 lines added)

Final SWI gate state at session end (after revert): 43/57 baseline.
Mid-session combined-changes state was 7/57 — better than session #6's
5/57 but still well below baseline due to a separate FPE in test_arith
that emerged late.

## Major new finding — copy_term_rec creates un-trailable vars

Session #6 concluded the 43→5 collapse was purely "corpus stdlib gap
exposed by removing silent-success default arm." Session #7 disproves
that as the complete picture.

**Decisive new repro** (with bridge + Change C + plunit enrichment, but
WITHOUT the copy_term_rec slot fix):

```prolog
main :- findall(G, test_t(G), Tests),
        Tests = [Goal|_],
        ( catch(Goal, _, fail) -> write(success) ; write(failed) ), nl.
:- assertz(test_t(memberchk(f(X,a), [f(x,b), f(y,a)]))).
```

Output: `failed`. Direct call (no findall) of the same goal: succeeds.

**Root cause:** `copy_term_rec` in `pl_runtime.c:472,480` creates fresh
vars via `term_new_var(-1)`. The `-1` slot means "anonymous wildcard" —
`bind()` in `prolog_unify.c:73` skips trailing such vars. So when
memberchk's first clause head-unifies, fails, backtracks — the
`trail_unwind` cannot undo the binding (it was never trailed). The
second clause sees the variable still bound to the wrong value and
fails too. memberchk fails overall.

**Fix:** synthetic non-negative slot. Tested working: `(1<<20)+nmap`
in `copy_term_rec`. Repro now prints `success`. Smoke 5/5 unchanged.

## Why this matters more than session #6 thought

Session #6's "silent-success ceiling" was real but only one of two bugs.
The bridge alone with corpus enrichment alone still gives 5/57, because
findall+catch combinations hit the trail bug. The slot fix is required
for any non-trivial test that goes through plunit's findall snapshot
machinery.

## Late-session FPE blocker (not investigated)

After applying all three changes together (bridge + Change C + plunit
enrichment + slot fix), `test_arith` crashes with SIGFPE immediately.
Was at 8/57 then dropped due to this. No diagnostic time remained;
likely a divide-by-zero or modulo-by-zero in some arith path that
plunit's `between/3` or similar new builtin provokes through
arithmetic test inputs.

## Confirmed-working repros at end of mid-session combined state

| Repro | Outcome |
|-------|---------|
| `G=fail; catch(G,_,write(caught))` | failed ✅ |
| `G=(X=5); catch(G,_,fail), [ok,X]` | [ok,5] ✅ |
| `G=(A is 3+4); catch(G,_,fail), [ok,A]` | [ok,7] ✅ |
| `findall+catch+memberchk` (decisive new) | success ✅ |
| Direct memberchk (no findall) | [ok,y] ✅ |

## Combined-state changes (in saved diffs)

**one4all/src/runtime/interp/pl_runtime.c** (252 lines diff):

1. **v3 bridge** — `pl_term_to_synth_expr` + `pl_invoke_var_goal` +
   wired into catch/3 E_VAR else-branch (211 lines, same as session #6
   diff).

2. **Change C** — findall snapshot uses `pl_copy_term` (preserves var
   sharing) instead of `pl_unified_deep_copy` (collapsed every TT_VAR to
   atom `_`). Single line change at the findall site.

3. **copy_term_rec slot fix** (NEW this session) — both
   `term_new_var(-1)` callsites in `copy_term_rec` changed to
   `term_new_var((1 << 20) + *nmap)`. Slot becomes synthetic but
   non-negative, so `bind()` trails the binding and backtracking works.

**corpus/programs/prolog/plunit.pl** (93 lines diff):

Added ~50 lines of naive Prolog stdlib in `/* PL-12 session #7 */` block:
memberchk/2, length/2, between/3, false/0, call/1..3, apply/2,
term_variables/2 (with helpers pj_tv, pj_not_member, pj_reverse),
numbervars/3 (delegates to /4), compound_name_arity/3,
compound_name_arguments/3, is_most_general_term/1, clause/2 stub,
op/3 stub, user/0, setof/3, setup_call_cleanup/3, format/3 (delegates
to /2), expand_term/2, expand_goal/2 (identity), string_chars/2,
string_codes/2, string_lower/2, string_upper/2, string_length/2,
number_string/2, atom_to_term/3 stub, stream_property/2 stub,
$current_prolog_flag/5 stub.

## Path to PL-12 ≥80% gate — REVISED 3-step plan (supersedes session #6's 2-step)

**Step A — corpus plunit.pl stdlib enrichment** (corpus repo).
Diff at `one4all/docs/PL-12-session-2026-04-30-7-plunit.diff`. Apply
to `corpus/programs/prolog/plunit.pl`. Re-measure baseline (without
bridge, gate-neutral expected).

**Step B — runtime fixes (one4all `pl_runtime.c`)** — apply in this order
to bisect cleanly:

  B.1. **`copy_term_rec` slot fix** (8 lines). Standalone, independent.
       Worth committing alone first to verify no regressions. Likely
       gate-neutral or small uptick.
  B.2. **Change C** — findall pl_copy_term (1 line). Independent.
       Should be small uptick.
  B.3. **v3 bridge** (211 lines) — depends on B.1+B.2 to actually move
       the gate. After B.1+B.2 land, the bridge's correctness is
       provable end-to-end through findall+catch chains.

After Step A + B.1 + B.2 + B.3: expected gate clearance — but the
test_arith FPE will need investigating first or the suite collapse will
mask the actual gain.

**Step C — investigate test_arith FPE** (NEW step, after B.1+B.2+B.3
land in working tree but before commit). Reproduce by running the
suite and examining whether plunit's `between/3` impl provokes arith
divisions. If FPE is in `between/3` (e.g. degenerate case), patch
plunit. If runtime, that's a separate runtime bug.

## Files committed this session

- `one4all/docs/PL-12-session-2026-04-30-7-attempt.diff` — full 252-line
  pl_runtime.c diff (bridge + Change C + slot fix combined).
- `one4all/docs/PL-12-session-2026-04-30-7-plunit.diff` — 93-line
  corpus plunit.pl stdlib enrichment.
- `one4all/docs/PL-12-session-2026-04-30-7-findings.md` — this narrative.
- one4all/src/runtime/interp/pl_runtime.c — REVERTED to pre-session.
- corpus/programs/prolog/plunit.pl — REVERTED to pre-session.
