#!/usr/bin/env bash
# test_parser_raku_coverage.sh — PARSER-RK coverage gate (post-PIVOT, RK-28+).
#
# Parse-only gate.  No oracle comparison.  For each .raku program in
# corpus/programs/raku/parser-coverage/, runs parser_raku.sc on stdin
# and asserts:
#
#   1. parser exits 0
#   2. stdout is non-empty
#   3. stdout contains no `Parse Error` substring
#   4. first non-whitespace line of stdout starts with `(STMT`
#
# A program PASSes iff all four conditions hold.  Tree shape, oracle
# parity, and execution semantics are NOT checked here — that is the
# job of test_parser_raku.sh (which keeps the 147 oracle-parity
# fixtures in corpus/programs/raku/parser/ as a regression guard).
#
# Reports cumulative `COV_PASS=N COV_FAIL=M`, exits 1 on any FAIL.
# When the parser-coverage/ directory is empty (or doesn't exist
# yet), reports COV_PASS=0 COV_FAIL=0 and exits 0 — the script is
# wired and working but has nothing to grade.
#
# This gate exists so the post-PIVOT RK-28..RK-50 ladder can extend
# parser_raku.sc with grammar that the C frontend (`--dump-ir`) does
# NOT support.  Such programs cannot use the oracle-parity gate
# (oracle would reject them), but they still need to parse without
# abort and emit a structurally sane tree.  This gate verifies
# exactly that.
#
# Per RULES.md self-contained-scripts conventions:
#   - paths derived from $0
#   - every scrip call gets `timeout 8` and explicit stdin redirection
#   - corpus path hardcoded to /home/claude/corpus, SKIP if missing
#   - script is idempotent, never sources another script's env
#
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet
# DATE: 2026-05-07 (RK-28-A)

set -u

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
SRC_RUNTIME="$CORPUS/programs/scrip"
SRC_TESTS="$CORPUS/programs/raku/parser-coverage"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not found at $SCRIP — run scripts/build_scrip.sh first"
    exit 0
fi
if [ ! -d "$SRC_RUNTIME" ]; then
    echo "SKIP corpus scrip runtime not found at $SRC_RUNTIME"
    exit 0
fi
if [ ! -f "$SRC_RUNTIME/parser_raku.sc" ]; then
    echo "SKIP parser_raku.sc not present in $SRC_RUNTIME"
    exit 0
fi
if [ ! -d "$SRC_TESTS" ]; then
    echo "SKIP corpus raku parser-coverage fixtures not found at $SRC_TESTS"
    echo "COV_PASS=0 COV_FAIL=0"
    exit 0
fi

echo "=== PARSER-RK coverage (parse-only) ==="
PASS=0
FAIL=0
FAILED_NAMES=""

# Empty-directory case: still wire the script, report zero counts.
shopt -s nullglob
fixtures=("$SRC_TESTS"/*.raku)
shopt -u nullglob

if [ ${#fixtures[@]} -eq 0 ]; then
    echo "  (no .raku fixtures in $SRC_TESTS yet — gate is wired)"
    echo ""
    echo "COV_PASS=0 COV_FAIL=0"
    exit 0
fi

for raku in "${fixtures[@]}"; do
    [ -f "$raku" ] || continue
    name=$(basename "$raku" .raku)

    # Parser driver: same runtime blob as test_parser_raku.sh.
    parser_raw=$(timeout 8 "$SCRIP" --ir-run \
        "$SRC_RUNTIME/global.sc" \
        "$SRC_RUNTIME/tree.sc" \
        "$SRC_RUNTIME/stack.sc" \
        "$SRC_RUNTIME/counter.sc" \
        "$SRC_RUNTIME/ShiftReduce.sc" \
        "$SRC_RUNTIME/semantic.sc" \
        "$SRC_RUNTIME/qize.sc" \
        "$SRC_RUNTIME/gen.sc" \
        "$SRC_RUNTIME/tdump.sc" \
        "$SRC_RUNTIME/assign.sc" \
        "$SRC_RUNTIME/parser_raku.sc" \
        < "$raku" 2>&1)
    parser_rc=$?

    # Condition 1: exit code 0.
    if [ "$parser_rc" -ne 0 ]; then
        echo "  FAIL $name — parser exit $parser_rc"
        echo "    parser output:"
        echo "$parser_raw" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi

    # Condition 2: stdout non-empty (after trimming whitespace).
    trimmed=$(printf '%s' "$parser_raw" | tr -d '[:space:]')
    if [ -z "$trimmed" ]; then
        echo "  FAIL $name — parser produced empty output"
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi

    # Condition 3: no "Parse Error" substring.
    if printf '%s' "$parser_raw" | grep -q 'Parse Error'; then
        echo "  FAIL $name — parser reported Parse Error"
        echo "    parser output:"
        echo "$parser_raw" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi

    # Condition 4: first non-blank line begins with (STMT.
    first_line=$(printf '%s' "$parser_raw" | sed -n '/[^[:space:]]/{p;q;}' | sed 's/^[[:space:]]*//')
    case "$first_line" in
        '(STMT'*) ;;
        *)
            echo "  FAIL $name — first line does not start with (STMT"
            echo "    first line: $first_line"
            FAIL=$((FAIL + 1))
            FAILED_NAMES="$FAILED_NAMES $name"
            continue
            ;;
    esac

    echo "  PASS $name"
    PASS=$((PASS + 1))
done

echo ""
echo "COV_PASS=$PASS COV_FAIL=$FAIL"
if [ -n "$FAILED_NAMES" ]; then
    echo "Failed:$FAILED_NAMES"
fi
[ $FAIL -eq 0 ] || exit 1
