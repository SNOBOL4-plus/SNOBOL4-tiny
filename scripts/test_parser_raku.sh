#!/usr/bin/env bash
# test_parser_raku.sh — PARSER-RK-N gate (currently rung 0: atoms).
#
# For each .raku program in corpus/programs/raku/parser/, runs two
# frontends and compares their output byte-for-byte after whitespace
# normalization:
#
#   1. The PARSER-RK Snocone driver — corpus/programs/scrip/parser_raku.sc
#      loaded as part of the standard scrip runtime blob.  The .raku source
#      is piped to stdin (the driver reads it line-by-line via INPUT) and
#      emits canonical IR-tree-line output via TDump (one line per STMT).
#
#   2. The existing scrip Raku frontend — invoked via `--dump-ir` to
#      emit the canonical IR tree.  This is the in-process oracle
#      (PARSER-RK never edits the existing frontend to make trees match —
#      per GOAL-PARSER-RAKU.md invariants).
#
# Whitespace normalization: ir_dump_program emits multi-line trees when
# an internal node has 2+ children, while TDump emits one-line trees.
# Collapse runs of whitespace to a single space and strip the
# space-before-close-paren artefact in BOTH streams before comparing —
# both forms describe the same tree.
#
# A program PASSes when the two normalized streams are byte-identical
# (including the both-empty case for inputs the existing frontend
# rejects — the parser must agree with the oracle's rejection).
# Reports cumulative `PASS=N FAIL=M`, exits 1 on any FAIL.
#
# Per RULES.md self-contained-scripts conventions:
#   - paths derived from $0
#   - every scrip call gets `timeout 8` and explicit stdin redirection
#   - corpus path hardcoded to /home/claude/corpus, SKIP if missing
#   - script is idempotent, never sources another script's env
#
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
# DATE: 2026-05-03

set -u

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="/home/claude/corpus"
SRC_RUNTIME="$CORPUS/programs/scrip"
SRC_TESTS="$CORPUS/programs/raku/parser"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not found at $SCRIP — run scripts/build_scrip.sh first"
    exit 0
fi
if [ ! -d "$SRC_RUNTIME" ]; then
    echo "SKIP corpus scrip runtime not found at $SRC_RUNTIME"
    exit 0
fi
if [ ! -d "$SRC_TESTS" ]; then
    echo "SKIP corpus raku parser fixtures not found at $SRC_TESTS"
    exit 0
fi
if [ ! -f "$SRC_RUNTIME/parser_raku.sc" ]; then
    echo "SKIP parser_raku.sc not present in $SRC_RUNTIME"
    exit 0
fi

# Normalize a tree dump for comparison: collapse runs of whitespace to a
# single space, strip any space immediately preceding a `)`, and trim
# leading/trailing whitespace.  Both ir_dump_program (multi-line) and
# TDump (single-line) collapse to the same canonical form.
normalize() {
    tr -s '[:space:]' ' ' | sed 's/ )/)/g' | sed 's/^ *//; s/ *$//'
}

echo "=== PARSER-RK smoke ==="
PASS=0
FAIL=0
FAILED_NAMES=""

for raku in "$SRC_TESTS"/*.raku; do
    [ -f "$raku" ] || continue
    name=$(basename "$raku" .raku)

    # Parser driver: run parser_raku.sc with the shared SC runtime blob,
    # feeding the fixture on stdin.  --ir-run uses the IR interpreter.
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

    # Oracle: existing Raku frontend via --dump-ir.
    oracle_raw=$(timeout 8 "$SCRIP" --dump-ir "$raku" 2>&1 < /dev/null)
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
        echo "    parser (parser_raku.sc, normalized):"
        echo "      $parser_norm"
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL"
[ $FAIL -eq 0 ] || exit 1
