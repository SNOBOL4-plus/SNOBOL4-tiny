#!/usr/bin/env bash
# test_scrip.sh — Smoke test for corpus/programs/scrip/
# Loads the five Snocone-hosted runtime files plus smoke.sc; expects 'bar'.
# All six PARSER-* sessions run this gate at session setup.
# Future Icon-hosted and Prolog-hosted equivalents will be added here as
# additional gate steps when those parts of the SCRIP source tree land.
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

# --- Snocone-hosted runtime smoke ---
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
else
    echo "FAIL scrip(.sc) smoke"
    echo "  expected: '$EXPECTED'"
    echo "  actual:   '$ACTUAL'"
    exit 1
fi
