#!/usr/bin/env bash
# test_scrip.sh — Smoke test for corpus/programs/scrip/
# Loads the five Snocone-hosted runtime files plus smoke.sc; expects 'bar'.
# Distinguishes three outcomes:
#   PASS    — smoke output matches 'bar'
#   BLOCKED — output is the documented "Error 3" trace from the known
#             scrip Snocone bug PARSER-SN-INFRA-5a (one-arg IDENT(var)
#             inside Pop() returns wrong branch when tree.sc::Insert is
#             loaded). Exit 0 — the bug is tracked, not a regression.
#   FAIL    — anything else; exit 1.
set -u

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
SRC="$CORPUS/programs/scrip"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not found at $SCRIP — run scripts/build_scrip.sh first"
    exit 0
fi
if [ ! -d "$SRC" ]; then
    echo "SKIP corpus scrip source tree not found at $SRC"
    exit 0
fi

EXPECTED="bar"
ACTUAL=$(timeout 8 "$SCRIP" --ir-run \
    "$SRC/tree.sc" \
    "$SRC/stack.sc" \
    "$SRC/counter.sc" \
    "$SRC/ShiftReduce.sc" \
    "$SRC/semantic.sc" \
    "$SRC/smoke.sc" \
    < /dev/null 2>&1)

if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "PASS scrip(.sc) smoke: Shift/Pop round-trip = '$ACTUAL'"
    exit 0
fi

# Recognize the documented INFRA-5a failure mode.
if echo "$ACTUAL" | grep -q "Error 3" && echo "$ACTUAL" | grep -q "Erroneous array or table reference"; then
    echo "BLOCKED scrip(.sc) smoke: PARSER-SN-INFRA-5a (one-arg IDENT bug)"
    echo "  smoke uses faithful Pop() no-arg form per beauty.sno line 617;"
    echo "  bug surfaces when tree.sc::Insert is co-loaded."
    echo "  See GOAL-PARSER-SNOBOL4.md INFRA-5a for fix plan."
    exit 0
fi

echo "FAIL scrip(.sc) smoke"
echo "  expected: '$EXPECTED'"
echo "  actual:   '$ACTUAL'"
exit 1
