# beauty_tramp_session64.c — Artifact

## Changes since last artifact (session 63)

### Commits this session
- `09e5a5d` — fix(emit_byrd): E_DEREF right-child varname + unary $'lit' output capture + sideeffect emit_imm
- `5e90712` — fix(emit_byrd): sync C static on do_assign + byrd_cs() helper

### What changed
1. **emit_imm side-effect fix** — `nPush() $'('` parsed as `E_IMM(left=nPush(), right=E_STR("("))`.
   Was: treating nPush() as pattern child → infinite recursion into pat_Expr.
   Now: emits nPush() inline as statement, then matches+captures literal `(` to OUTPUT.
   Helpers added: `is_sideeffect_call()`, `emit_sideeffect_call_inline()`.

2. **E_DEREF varname fix** — grammar produces `E_DEREF(left=NULL, right=E_VAR("x"))`.
   Was: only checked `pat->left` → varname always `""` → 31 `var_get("")` fallbacks.
   Now: checks `pat->right` first, then `pat->left`, then `E_CALL` left → 33→9 match_pattern_at.

3. **Unary $'lit' fix** — `$'('` parses as `E_DEREF(NULL, E_STR("("))`.
   Was: fell through to `var_get("")` interpreter fallback.
   Now: emits literal match + OUTPUT capture directly.

4. **C static sync** — inline byrd match do_assign only called `var_set(name, val)`.
   `get(_nl)` reads C static `_nl` which was never updated → nl/tab/etc always empty.
   Now: `byrd_cs()` helper added; do_assign emits both `var_set(name,val)` and
   `_name = val` (skipping `_` discard variable).

5. **expr_contains_pattern** — also fixed to check `e->right` for E_DEREF.

## Stats
- Lines: 26769
- md5: 35328720ed4d49c025505fc6dc653b87
- gcc: 0 errors, 0 warnings (-w)
- match_pattern_at calls: 9 (down from 33 in session 63)
  - 6× IDENT (runtime-defined, correct fallback)
  - 1× upr (runtime-defined, correct fallback)
  - 1× qqdlm (dynamic local, correct fallback)
  - 1× bch (dynamic local, correct fallback)

## Compile status
0 errors / 0 warnings (with -w)

## Active bug / current symptom
`printf 'X = 1\n' | beauty_tramp_bin` → "Parse Error" + passthrough.

Root cause pinned: `pat_Parse` fails on `"X = 1\n"`. Two hypotheses:
1. `pat_Command` grammar requires correct `nl` inside compiled pattern bodies —
   these use `var_get("nl")` (hash), which IS now set. Need to verify this path.
2. Runtime init at line 782 assigns `Space = pat_alt(pat_span(...), pat_var("epsilon"))`
   via `sno_pat_*` interpreter objects, potentially overwriting the compiled `pat_Space`
   registration or conflicting with the compiled path.

## ONE NEXT ACTION
Add a temporary fprintf to `stmt_427` (line 790 in beauty_tramp.c) to print
`_subj2209` (Src contents) and the `pat_Parse` return value — confirm whether
Src contains `"X = 1\n"` and whether pat_Parse is returning FAIL_VAL.
If Src is correct and pat_Parse fails → dig into pat_Command failing on `"X = 1"`.
If Src is empty → nl C static sync didn't propagate into concat_sv correctly.
