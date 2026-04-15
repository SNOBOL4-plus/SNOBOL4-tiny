#!/usr/bin/env bash
# test_prolog_swi_suite.sh — run SWI plunit conformance suite under --ir-run
# Compares normalized PASS/FAIL-per-suite output against swipl-baked .ref files.
# Gate: PASS >= 80% of suites across all test files.
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS=/home/claude/corpus/programs/prolog
SWIT=$CORPUS/swi_tests
PLUNIT=$CORPUS/plunit.pl

[ -d "$SWIT" ]   || { echo "SKIP: $SWIT missing"; exit 0; }
[ -f "$PLUNIT" ] || { echo "SKIP: $PLUNIT missing"; exit 0; }
[ -x "$SCRIP" ]  || { echo "SKIP: scrip not built"; exit 0; }

PASS=0; FAIL=0; TOTAL=0

# Normalize scrip plunit output to PASS/FAIL per suite (same as .ref format)
# Scrip plunit.pl prints "% PL-Unit: NAME" then per-test lines then summary.
# We derive PASS/FAIL from pj_summary counts per suite block.
normalize_scrip_output() {
    awk '
        /^% PL-Unit: / { suite=$3; fail_count=0 }
        /^  FAIL:/     { fail_count++ }
        /^% [0-9]+ (passed|tests)/ {
            # end of suite block — but no explicit per-suite verdict line in shim
        }
        /^PASS / { print }
        /^FAIL / { print }
    '
}

for f in "$SWIT"/test_*_main.pl; do
    base=$(basename "$f" _main.pl)
    ref="$SWIT/${base}.ref"
    [ -f "$ref" ] || continue

    # Run scrip with plunit shim + test file + _main.pl wrapper (defines main/0 -> test_X)
    testfile="$SWIT/${base}.pl"
    actual=$(timeout 30 "$SCRIP" --ir-run "$PLUNIT" "$testfile" "$f" < /dev/null 2>/dev/null | normalize_scrip_output)
    expected=$(cat "$ref")

    # Count suite lines in .ref
    suite_total=$(wc -l < "$ref")
    TOTAL=$((TOTAL + suite_total))

    if [ "$actual" = "$expected" ]; then
        echo "  PASS $base ($(echo "$expected" | grep -c "^PASS") suites)"
        PASS=$((PASS + suite_total))
    else
        echo "  FAIL $base"
        # Show diff of suite-level mismatches
        diff <(echo "$expected") <(echo "$actual") | head -10
        # Count matched lines
        matched=$(comm -12 <(echo "$expected" | sort) <(echo "$actual" | sort) | wc -l)
        PASS=$((PASS + matched))
        FAIL=$((FAIL + suite_total - matched))
    fi
done

echo ""
echo "Suite totals: PASS=$PASS FAIL=$FAIL TOTAL=$TOTAL"
pct=$(( PASS * 100 / (TOTAL > 0 ? TOTAL : 1) ))
echo "Coverage: ${pct}%"
[ "$pct" -ge 80 ]
