#!/usr/bin/env bash
# run_interp_broad.sh — run scrip-interp over crosscheck corpus, diff vs .ref
# Usage: bash test/run_interp_broad.sh
# From: /home/claude/one4all/

set -uo pipefail
INTERP="${INTERP:-./scrip-interp}"
CORPUS="${CORPUS:-/home/claude/corpus}"
TIMEOUT="${TIMEOUT:-10}"

PASS=0; FAIL=0
FAILURES=""

run_test() {
    local sno="$1"
    local ref="${sno%.sno}.ref"
    local input="${sno%.sno}.input"
    [[ ! -f "$ref" ]] && return
    local got exp
    if [[ -f "$input" ]]; then
        got=$(timeout "$TIMEOUT" "$INTERP" "$sno" < "$input" 2>/dev/null || true)
    else
        got=$(timeout "$TIMEOUT" "$INTERP" "$sno" 2>/dev/null || true)
    fi
    exp=$(cat "$ref")
    if [[ "$got" == "$exp" ]]; then
        PASS=$((PASS+1))
    else
        FAIL=$((FAIL+1))
        FAILURES="${FAILURES}  FAIL $(basename "$sno" .sno)\n"
    fi
}

while IFS= read -r sno; do
    run_test "$sno"
done < <(find "$CORPUS/crosscheck" -name "*.sno" | sort)

echo "PASS=$PASS FAIL=$FAIL  ($(( PASS + FAIL )) total)"
[[ -n "$FAILURES" ]] && printf "$FAILURES" | head -30
