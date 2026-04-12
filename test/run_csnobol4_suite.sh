#!/usr/bin/env bash
# run_csnobol4_suite.sh — scrip-interp regression: Budne csnobol4-suite (116 tests) + FENCE tests (10)
# Usage: bash test/run_csnobol4_suite.sh
# From: /home/claude/one4all/

set -uo pipefail
INTERP="${INTERP:-./scrip-interp}"
CORPUS="${CORPUS:-/home/claude/corpus}"
TIMEOUT="${TIMEOUT:-10}"
SUITE="$CORPUS/programs/csnobol4-suite"
FENCE="$CORPUS/crosscheck/patterns"

PASS=0; FAIL=0
FAILURES=""

SKIP_LIST="bench.sno breakline.sno genc.sno k.sno ndbm.sno sleep.sno time.sno line2.sno"

run_test() {
    local label="$1" sno="$2" ref="$3" input="${4:-}"
    [ ! -f "$ref" ] && return
    local got exp
    if [ -n "$input" ] && [ -f "$input" ]; then
        got=$(timeout "$TIMEOUT" $INTERP "$sno" < "$input" 2>/dev/null || true)
    else
        got=$(timeout "$TIMEOUT" $INTERP "$sno" 2>/dev/null || true)
    fi
    exp=$(cat "$ref")
    if [ "$got" = "$exp" ]; then
        PASS=$((PASS+1))
    else
        FAIL=$((FAIL+1))
        FAILURES="${FAILURES}  FAIL ${label}\n"
    fi
}

should_skip() {
    local base; base=$(basename "$1")
    for s in $SKIP_LIST; do [ "$base" = "$s" ] && return 0; done
    return 1
}

# ── Budne csnobol4 suite (116 tests) ─────────────────────────────────────────
echo "── csnobol4-suite ──"
for sno in $(ls "$SUITE"/*.sno | sort); do
    should_skip "$sno" && continue
    ref="${sno%.sno}.ref"
    input="${sno%.sno}.input"
    run_test "$(basename "$sno" .sno)" "$sno" "$ref" "$input"
done

# ── FENCE tests (10 tests: 058–067) ──────────────────────────────────────────
echo "── fence tests ──"
for sno in $(ls "$FENCE"/05[89]_pat_fence*.sno "$FENCE"/06[0-7]_pat_fence*.sno 2>/dev/null | sort); do
    ref="${sno%.sno}.ref"
    run_test "$(basename "$sno" .sno)" "$sno" "$ref" ""
done

echo ""
echo "PASS=$PASS FAIL=$FAIL  ($(( PASS + FAIL )) total)"
[ -n "$FAILURES" ] && printf "$FAILURES" | head -40
