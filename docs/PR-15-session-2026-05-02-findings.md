# PR-15 Session 2026-05-02 Findings

## Status: 4/5 tests pass, NOT committed (regression-class: test 03 still fails)

## What was attempted

Added ISO error throwing to `pl_runtime.c`:
- `pl_throw_iso_error(Term*)` — wraps in `error/2`, longjmps to innermost matching catch frame
- `pl_throw_instantiation_error()` — throws `error(instantiation_error, context)`
- `pl_throw_type_error_evaluable(name, arity)` — throws `error(type_error(evaluable,Name/Arity), context)`
- `pl_throw_existence_error_procedure(name, arity)` — throws `error(existence_error(procedure,Name/Arity), context)`

Wired into:
- `pl_unified_eval_arith_term` E_VAR: unbound → `pl_throw_instantiation_error()`
- `pl_unified_eval_arith_term` E_FNC fallthrough: non-evaluable → `pl_throw_type_error_evaluable()`
- `interp_exec_pl_builtin` two undefined-pred fprintf sites → `pl_throw_existence_error_procedure()`

## Gate results

| Test | Expected | Actual | Status |
|------|----------|--------|--------|
| 01_type_error       | caught_type_error          | caught_type_error          | PASS |
| 02_instantiation_error | caught_instantiation_error | caught_instantiation_error | PASS |
| 03_existence_error  | caught_existence_error     | (empty)                    | FAIL |
| 04_user_throw       | 42 hello                   | 42 hello                   | PASS |
| 05_nested_catch     | inner_caught               | inner_caught               | PASS |

## Root cause of test 03 failure

`no_such_pred(42)` is dispatched through `interp.c`'s `E_CLAUSE`/`E_CHOICE`
handler via `pl_box_choice`, NOT through `interp_exec_pl_builtin`. When the
predicate table has no clauses for `no_such_pred/1`, `pl_box_choice` returns
an empty OR-box which `bb_broker` evaluates to omega (failure). The throw
never fires.

The two `pl_throw_existence_error_procedure` calls I added in
`interp_exec_pl_builtin` are unreachable for this case — they guard the
fallthrough path inside the E_FNC handler, which is only reached when the
goal's EXPR kind is E_FNC (a builtin-dispatch path), not when it's
dispatched as a user predicate via E_CLAUSE/E_CHOICE.

## Fix plan for next session

1. Apply `docs/PR-15-session-2026-05-02-attempt.diff` (the helpers + is/2 fixes).
2. Find in `interp.c` (or `pl_broker.c`) where `pl_box_choice` is called with
   a Prolog predicate that has zero clauses. The right hook is either:
   - In `pl_box_choice` itself (in `pl_broker.c`): if clause list is empty,
     throw `existence_error(procedure, Name/Arity)` instead of returning
     empty OR-box.
   - In `interp.c`'s E_CLAUSE/E_CHOICE handler for LANG_PL: after
     `pl_box_choice` returns, check if the OR-box is empty and throw.
3. Verify test 03 PASS.
4. Run all gates: smoke_prolog 5/5, broker 49/49, rung31-38 all 5/5.
5. Commit.

## Diff location

`one4all/docs/PR-15-session-2026-05-02-attempt.diff` — 100 lines, applies
clean to HEAD `e72c900f`.

Verified gates WITH this diff applied:
- smoke_prolog 5/5 ✓
- smoke_unified_broker 49/49 ✓
- rung31-37 all 5/5 ✓
- rung38_iso_errors 4/5 (test 03 still FAIL)
