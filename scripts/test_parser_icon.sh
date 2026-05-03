#!/usr/bin/env bash
# test_parser_icon.sh — PARSER-IC-N gate (currently rung 0: atoms).
#
# For each .icn program in corpus/programs/icon/parser/, runs two
# frontends and compares their output byte-for-byte after whitespace
# normalization:
#
#   1. The PARSER-IC Snocone driver — corpus/programs/scrip/parser_icon.sc
#      loaded as part of the standard scrip runtime blob.  The .icn source
#      is piped to stdin (the driver reads it line-by-line via INPUT) and
#      emits canonical IR-tree-line output via TDump (one line per tree).
#
#   2. The existing scrip Icon frontend — invoked via `--dump-ir` to emit
#      the canonical IR tree.  This is the in-process oracle (PARSER-IC
#      never edits the existing frontend to make trees match — per
#      GOAL-PARSER-ICON.md invariants).
#
# Whitespace normalization: ir_dump_program emits multi-line trees when an
# internal node has 2+ children (e.g. (E_FNC main\n  (E_VAR main)\n  ...)),
# while TLump emits one-line trees.  We collapse runs of whitespace to a
# single space and strip the space-before-close-paren artefact in BOTH
# streams before comparing — both forms describe the same tree.
#
# A program PASSes when the two normalized streams are byte-identical.
# Reports cumulative `PASS=N FAIL=M`, exits 1 on any FAIL.
#
# Per RULES.md self-contained-scripts conventions:
#   - paths derived from $0
#   - every scrip call gets `timeout 8` and explicit stdin redirection
#   - corpus path hardcoded to /home/claude/corpus, SKIP if missing
#   - script is idempotent, never sources another script's env

set -u

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
SRC_RUNTIME="$CORPUS/programs/scrip"
SRC_TESTS="$CORPUS/programs/icon/parser"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not found at $SCRIP — run scripts/build_scrip.sh first"
    exit 0
fi
if [ ! -d "$SRC_RUNTIME" ]; then
    echo "SKIP corpus scrip runtime not found at $SRC_RUNTIME"
    exit 0
fi
if [ ! -d "$SRC_TESTS" ]; then
    echo "SKIP corpus icon parser fixtures not found at $SRC_TESTS"
    exit 0
fi
if [ ! -f "$SRC_RUNTIME/parser_icon.sc" ]; then
    echo "SKIP parser_icon.sc not present in $SRC_RUNTIME"
    exit 0
fi

# Normalize a tree dump for comparison: collapse runs of whitespace to a
# single space, strip any space immediately preceding a `)`, and trim
# leading/trailing whitespace.  Both ir_dump_program (multi-line) and
# TLump (single-line) collapse to the same canonical form.
normalize() {
    tr -s '[:space:]' ' ' | sed 's/ )/)/g' | sed 's/^ *//; s/ *$//'
}

echo "=== PARSER-IC smoke ==="
PASS=0
FAIL=0
FAILED_NAMES=""

for icn in "$SRC_TESTS"/*.icn; do
    [ -f "$icn" ] || continue
    name=$(basename "$icn" .icn)

    # Parser driver: run parser_icon.sc with the shared SC runtime blob,
    # feeding the fixture on stdin.  --ir-run uses the IR interpreter.
    parser_raw=$(timeout 8 "$SCRIP" --ir-run \
        "$SRC_RUNTIME/global.sc" \
        "$SRC_RUNTIME/tree.sc" \
        "$SRC_RUNTIME/stack.sc" \
        "$SRC_RUNTIME/ShiftReduce.sc" \
        "$SRC_RUNTIME/qize.sc" \
        "$SRC_RUNTIME/gen.sc" \
        "$SRC_RUNTIME/tdump.sc" \
        "$SRC_RUNTIME/assign.sc" \
        "$SRC_RUNTIME/parser_icon.sc" \
        < "$icn" 2>&1)
    parser_rc=$?

    # Oracle: existing Icon frontend via --dump-ir.
    oracle_raw=$(timeout 8 "$SCRIP" --dump-ir "$icn" 2>&1 < /dev/null)
    oracle_rc=$?

    if [ "$parser_rc" -ne 0 ]; then
        echo "  FAIL $name — parser driver exit $parser_rc"
        echo "    parser output:"
        echo "$parser_raw" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi
    if [ "$oracle_rc" -ne 0 ]; then
        echo "  FAIL $name — oracle (--dump-ir) exit $oracle_rc"
        echo "    oracle output:"
        echo "$oracle_raw" | sed 's/^/      /'
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        continue
    fi

    parser_norm=$(printf '%s' "$parser_raw" | normalize)
    oracle_norm=$(printf '%s' "$oracle_raw" | normalize)

    if [ "$parser_norm" = "$oracle_norm" ]; then
        echo "  PASS $name"
        PASS=$((PASS + 1))
    else
        echo "  FAIL $name — tree divergence"
        echo "    oracle (--dump-ir, normalized):"
        echo "      $oracle_norm"
        echo "    parser (parser_icon.sc, normalized):"
        echo "      $parser_norm"
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
