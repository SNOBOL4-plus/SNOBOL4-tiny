#!/usr/bin/env bash
# test_parser_snobol4.sh — PARSER-SN-N gate (currently rung 0: atoms).
#
# For each .sno program in corpus/programs/snobol4/parser/, this script
# runs two frontends against it:
#
#   1. The PARSER-SN Snocone driver — corpus/programs/scrip/parser_snobol4.sc
#      loaded as part of the standard scrip runtime blob.  The .sno source
#      is piped to stdin (the driver reads it line-by-line via INPUT) and
#      the driver emits canonical IR-tree-line output.
#
#   2. The existing scrip SNOBOL4 frontend — invoked via `--dump-parse`
#      to emit the same canonical lines.  This is the in-process oracle
#      (per GOAL-PARSER-SNOBOL4.md "PARSER-SN never edits existing-frontend
#      code to make trees match" invariant).
#
# A program PASSes when the two byte streams are byte-identical.  The
# script reports a cumulative `PASS=N FAIL=M` and exits 1 on any FAIL.
#
# This is the rung gate for PARSER-SN-0.  Each subsequent PARSER-SN
# rung adds new .sno programs to corpus/programs/snobol4/parser/ and
# extends parser_snobol4.sc's `Command` rule; the gate's wiring stays
# the same.
#
# Per RULES.md self-contained-scripts conventions:
#   - paths derived from $0
#   - every scrip call gets `timeout 8` and explicit stdin redirection
#   - corpus path hardcoded to /home/claude/corpus, SKIP if missing
#   - script is idempotent
#   - never sources another script's env

set -u

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
SRC_RUNTIME="$CORPUS/programs/scrip"
SRC_TESTS="$CORPUS/programs/snobol4/parser"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not found at $SCRIP — run scripts/build_scrip.sh first"
    exit 0
fi
if [ ! -d "$SRC_RUNTIME" ]; then
    echo "SKIP corpus scrip runtime not found at $SRC_RUNTIME"
    exit 0
fi
if [ ! -d "$SRC_TESTS" ]; then
    echo "SKIP corpus parser tests not found at $SRC_TESTS"
    exit 0
fi
if [ ! -f "$SRC_RUNTIME/parser_snobol4.sc" ]; then
    echo "SKIP parser_snobol4.sc not present in $SRC_RUNTIME"
    exit 0
fi

# normalize(text) — collapse runs of whitespace to a single space and
# strip the ' )' artefact that Gen-based TDump can emit.  Applied to
# both parser output and oracle output before byte-comparison.  This
# is the canonical whitespace-normalization pattern adopted by sibling
# sessions (test_parser_icon.sh, test_parser_prolog.sh) per the FW-6
# variant-B decision: Gen-based TDump produces inline-or-multiline
# output depending on width budget; --dump-parse always emits multi-
# line.  Structural divergence (wrong IR kind, wrong tree shape) is
# not masked — only whitespace layout differences are elided.
normalize() {
    echo "$1" | tr -s '[:space:]' ' ' | sed 's/ )/)/g' | sed 's/^ //;s/ $//'
}

echo "=== PARSER-SN smoke ==="
PASS=0
FAIL=0
FAILED_NAMES=""

# Iterate the rung corpus.  Order is filesystem order (sorted) so the
# report is deterministic across runs.
for sno in "$SRC_TESTS"/*.sno; do
    [ -f "$sno" ] || continue
    name=$(basename "$sno" .sno)

    # Two-frontend output capture.  --ir-run is the canonical entry point
    # for Snocone-hosted runtime blobs (per test_scrip.sh).  --dump-parse
    # is the existing SNOBOL4 frontend's tree-dump mode (per scrip.c).
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
        "$SRC_RUNTIME/parser_snobol4.sc" \
        < "$sno" 2>&1)
    parser_rc=$?

    oracle_out=$(timeout 8 "$SCRIP" --dump-parse "$sno" 2>&1 < /dev/null)
    oracle_rc=$?

    if [ "$parser_rc" -ne 0 ]; then
        echo "  FAIL $name — parser driver exit $parser_rc"
        echo "    parser stderr/stdout:"
        echo "$parser_out" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi
    if [ "$oracle_rc" -ne 0 ]; then
        echo "  FAIL $name — oracle (--dump-parse) exit $oracle_rc"
        echo "    oracle stderr/stdout:"
        echo "$oracle_out" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi

    parser_norm=$(normalize "$parser_out")
    oracle_norm=$(normalize "$oracle_out")

    if [ "$parser_norm" = "$oracle_norm" ]; then
        echo "  PASS $name"
        PASS=$((PASS + 1))
    else
        echo "  FAIL $name — tree divergence"
        echo "    oracle (--dump-parse, normalized):"
        echo "$oracle_norm" | sed 's/^/      /'
        echo "    parser (parser_snobol4.sc, normalized):"
        echo "$parser_norm" | sed 's/^/      /'
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
