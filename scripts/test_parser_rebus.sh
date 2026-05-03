#!/usr/bin/env bash
# test_parser_rebus.sh — PARSER-RB-N gate (currently rung 0: atoms).
#
# For each .reb program in corpus/programs/rebus/parser/, runs two
# frontends and compares their output byte-for-byte:
#
#   1. The PARSER-RB Snocone driver — corpus/programs/scrip/parser_rebus.sc
#      loaded as part of the standard scrip runtime blob.  The .reb source
#      is piped to stdin (the driver reads it line-by-line via INPUT) and
#      emits canonical IR-tree-line output via TDump (one line per tree).
#
#   2. The existing scrip Rebus frontend — invoked via `--dump-ir` to emit
#      the canonical IR tree.  This is the in-process oracle (PARSER-RB
#      never edits the existing frontend to make trees match — per
#      GOAL-PARSER-REBUS.md invariants).
#
# A program PASSes when the two byte streams are byte-identical.
# Reports cumulative PASS=N FAIL=M, exits 1 on any FAIL.
#
# Per RULES.md self-contained-scripts conventions:
#   - paths derived from $0
#   - every scrip call gets timeout 8 and explicit stdin redirection
#   - corpus path hardcoded to /home/claude/corpus, SKIP if missing
#   - script is idempotent, never sources another script's env

set -u

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
SRC_RUNTIME="$CORPUS/programs/scrip"
SRC_TESTS="$CORPUS/programs/rebus/parser"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not found at $SCRIP — run scripts/build_scrip.sh first"
    exit 0
fi
if [ ! -d "$SRC_RUNTIME" ]; then
    echo "SKIP corpus scrip runtime not found at $SRC_RUNTIME"
    exit 0
fi
if [ ! -d "$SRC_TESTS" ]; then
    echo "SKIP corpus rebus parser fixtures not found at $SRC_TESTS"
    exit 0
fi
if [ ! -f "$SRC_RUNTIME/parser_rebus.sc" ]; then
    echo "SKIP parser_rebus.sc not present in $SRC_RUNTIME"
    exit 0
fi

echo "=== PARSER-RB smoke ==="
PASS=0
FAIL=0
FAILED_NAMES=""

for reb in "$SRC_TESTS"/*.reb; do
    [ -f "$reb" ] || continue
    name=$(basename "$reb" .reb)

    parser_out=$(timeout 8 "$SCRIP" --ir-run \
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
        "$SRC_RUNTIME/parser_rebus.sc" \
        < "$reb" 2>&1)
    parser_rc=$?

    oracle_out=$(timeout 8 "$SCRIP" --dump-ir "$reb" 2>&1 < /dev/null)
    oracle_rc=$?

    if [ "$parser_rc" -ne 0 ]; then
        echo "  FAIL $name — parser driver exit $parser_rc"
        echo "    parser output:"
        echo "$parser_out" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi
    if [ "$oracle_rc" -ne 0 ]; then
        echo "  FAIL $name — oracle (--dump-ir) exit $oracle_rc"
        echo "    oracle output:"
        echo "$oracle_out" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi

    if [ "$parser_out" = "$oracle_out" ]; then
        echo "  PASS $name"
        PASS=$((PASS + 1))
    else
        echo "  FAIL $name — tree divergence"
        echo "    oracle (--dump-ir):"
        echo "$oracle_out" | sed 's/^/      /'
        echo "    parser (parser_rebus.sc):"
        echo "$parser_out" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL"

if [ "$FAIL" -gt 0 ]; then
    echo "Failed:$FAILED_NAMES"
    exit 1
fi
exit 0
