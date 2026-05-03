# RS-22e — `bb_eval_value` fallthrough survey

**Session:** 2026-05-03 (RS-22e closure)
**Author:** Lon Jones Cherryholmes · Claude Sonnet
**Status:** read-only inventory — no code changes in this document; the
parent rung RS-22e is closed in `GOAL-REWRITE-SCRIP.md`.

This document records the kinds that reach
`coro_value.c:bb_eval_value`'s `interp_eval(e)` fallthrough during the
full Icon corpus (271 programs) but NOT during `smoke_icon` (5
programs).  These kinds are the work boundary for follow-up rungs.

---

## Method

A one-shot per-kind probe was inserted at the fallthrough site (a
`static char visited[256]` table; first sighting of each kind printed
`RS-22e: bb_eval_value fallthrough kind=N` to stderr, then still called
`interp_eval` so behavior was preserved).  All Icon corpus programs
were run with `timeout 8 scrip` and stderr aggregated.

After the survey, the probe was replaced with the rung's hardened
`fprintf + return FAILDESCR` variant to test whether the fallthrough
could be closed.  **smoke_icon stayed 5/5 PASS** with the hard
fallthrough.  However, **unified_broker dropped 49→45 FAIL=4** because
test/icon/palindrome.icn uses `s[i] ~== s[j]` (E_LNE), test/cross_lang
mixes Icon+Prolog generators, and two Raku tests use generators in
ways that hit the fallthrough.  Per the rung's instruction *"Revert
the abort if any smoke FAILs; note remaining kinds"* — the merge gate
is the binding gate for revert — the fallthrough was reverted and the
survey result is captured here.

---

## The 16 kinds, by category

### Generators (lift via coro_eval first-value contract)

| Kind | # | Notes |
|------|---|-------|
| `E_TO`        | 8 | `i to j` — first value `i`. coro_eval already drives this in BB context. |
| `E_ALTERNATE` | 6 | left-then-right alt generator |
| `E_ITERATE`   | 3 | `!E` in value context — first element of T/list/string |
| `E_LIMIT`     | 1 | `E \ N` — limited generator |
| `E_SEQ`       | 6 | goal-directed `&` sequence — last child's value |

These are not direct kinds to evaluate — they should route through
`coro_eval` and consume a single tick.  Today the IR-mode path inside
`interp_eval` already does this when it can.  Lifting requires a
small `bb_first_tick(e)` helper in coro_value or a dispatch table that
sees these kinds and produces a one-shot generator drive.

### Easy peers of already-lifted kinds (mirror RS-22b)

| Kind | # | Notes |
|------|---|-------|
| `E_LEQ` | 9 | string `==` — STRREL macro at interp_eval.c:3412 |
| `E_LNE` | 6 | string `~==` — STRREL macro |
| `E_LGE` | 1 | string `>>=` — STRREL macro |
| `E_LLT` | 1 | string `<<` — STRREL macro |

Plus E_LGT and E_LLE which were not exercised by the corpus but are
peers of these — they should be lifted in the same rung for symmetry.
Pattern: copy `bb_numrel` to `bb_strrel`, `strcmp(VARVAL_fn(l),
VARVAL_fn(r))` against zero, return rhs on success.  ~25 lines.

### Cset arithmetic (Icon character-set operators)

| Kind | # | Notes |
|------|---|-------|
| `E_CSET`        | 10 | literal cset — `e->sval ? STRVAL : NULVCL` |
| `E_CSET_DIFF`   | 2  | `--` cset difference |
| `E_CSET_INTER`  | 1  | `**` cset intersection |
| `E_CSET_COMPL`  | 1  | `~` cset complement |

(`E_CSET_UNION` not exercised but is the obvious peer.)  Implementation
is straightforward set arithmetic on character sets; csets are
represented as DT_S strings of distinct characters.

### Mid-size lifts

| Kind | # | Notes |
|------|---|-------|
| `E_MAKELIST` | 1 | `[e1,e2,...]` — DT_DATA icnlist constructor.  Lift via DEFDAT_fn / DATCON_fn (interp_eval.c:4051).  ~12 lines. |
| `E_SCAN`     | 5 | `subj ? pat` — drives a generator chain via coro/exec_stmt.  Needs first-value contract decision (Icon-mode vs SNOBOL4-mode dispatch — interp_eval.c:3877 has both branches). |
| `E_CASE`     | 1 | `case E of { clauses }` — statement-shaped but case-as-expression returns the matching clause's value. |

---

## Counts table (summary)

```
$ grep "RS-22e" all_icon.err | sort | uniq -c | sort -rn
     10 RS-22e: bb_eval_value fallthrough kind=3    E_CSET
      9 RS-22e: bb_eval_value fallthrough kind=73   E_LEQ
      8 RS-22e: bb_eval_value fallthrough kind=51   E_TO
      6 RS-22e: bb_eval_value fallthrough kind=74   E_LNE
      6 RS-22e: bb_eval_value fallthrough kind=54   E_ALTERNATE
      6 RS-22e: bb_eval_value fallthrough kind=19   E_SEQ
      5 RS-22e: bb_eval_value fallthrough kind=48   E_SCAN
      3 RS-22e: bb_eval_value fallthrough kind=55   E_ITERATE
      2 RS-22e: bb_eval_value fallthrough kind=77   E_CSET_DIFF
      1 RS-22e: bb_eval_value fallthrough kind=93   E_CASE
      1 RS-22e: bb_eval_value fallthrough kind=78   E_CSET_INTER
      1 RS-22e: bb_eval_value fallthrough kind=75   E_CSET_COMPL
      1 RS-22e: bb_eval_value fallthrough kind=72   E_LGE
      1 RS-22e: bb_eval_value fallthrough kind=69   E_LLT
      1 RS-22e: bb_eval_value fallthrough kind=56   E_MAKELIST
      1 RS-22e: bb_eval_value fallthrough kind=53   E_LIMIT
```

62 fallthrough events across 16 distinct kinds.  16 distinct kinds is
small enough that a single sub-rung (proposed RS-22f) can close all of
them, after which the RS-22e gate's hardened-fallthrough variant will
pass the merge gate too.  RS-23 (drop the `interp_eval` extern, promote
coro_value.c into the isolation gate) becomes feasible once that
happens.

---

## Decision

RS-22e gate (smoke_icon 5/5 with hardened fallthrough): **PASS**.
Rung **closed** with the survey complete and the work boundary
documented above.  RS-22f-or-RS-23 will close the remaining 16 kinds.
