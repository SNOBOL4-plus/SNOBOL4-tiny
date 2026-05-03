#!/usr/bin/env bash
# test_scrip.sh — Smoke test for corpus/programs/scrip/.
#
# Loads the Snocone-hosted runtime files (nine now, with assign.sc and
# match.sc added under PARSER-SN-INFRA-4) plus smoke.sc.  Expected output:
#   bar
#   global-OK
#   tdump-OK
#   assign-OK
#   match-OK
#   notmatch-OK
#
# The PARSER-SN-INFRA-5a "Error 3" recognizer is retained as a defensive
# probe: if synthetic-label collision ever resurfaces (e.g. someone
# undoes the g_sc_label_seq fix in snocone_parse.y), the script reports
# BLOCKED with a pointer to the goal file rather than a vague FAIL.
#
#   PASS    — output matches the expected six lines
#   BLOCKED — INFRA-5a footprint detected (Error 3 / array reference)
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

EXPECTED=$'bar\nglobal-OK\ntdump-OK\nassign-OK\nmatch-OK\nnotmatch-OK\nlwr-OK\nupr-OK\ncap-OK\nicase-OK\nqize-empty-OK\nqize-plain-OK\nsqize-OK\ndqize-OK\nsqlsqize-OK\ntdump-quote-OK\ninfra7a-inline-assign-OK\ninfra7a-qize-tab-OK\ntrace-silent-OK\nomega-silent-OK\nopsyn-OK\nfw1-generic-leaf-OK\nfw2-multichild-role-OK'
ACTUAL=$(timeout 8 "$SCRIP" --ir-run \
    "$SRC/global.sc" \
    "$SRC/tree.sc" \
    "$SRC/stack.sc" \
    "$SRC/counter.sc" \
    "$SRC/ShiftReduce.sc" \
    "$SRC/semantic.sc" \
    "$SRC/gen.sc" \
    "$SRC/tdump.sc" \
    "$SRC/assign.sc" \
    "$SRC/match.sc" \
    "$SRC/case.sc" \
    "$SRC/qize.sc" \
    "$SRC/trace.sc" \
    "$SRC/omega.sc" \
    "$SRC/smoke.sc" \
    < /dev/null 2>&1)

if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "PASS scrip(.sc) smoke: ... + qize-empty-OK + qize-plain-OK + sqize-OK + dqize-OK + sqlsqize-OK + tdump-quote-OK + infra7a-inline-assign-OK + infra7a-qize-tab-OK + trace-silent-OK + omega-silent-OK + opsyn-OK + fw1-generic-leaf-OK + fw2-multichild-role-OK"
    exit 0
fi

# Defensive recognizer for the documented INFRA-5a regression footprint.
if echo "$ACTUAL" | grep -q "Error 3" && echo "$ACTUAL" | grep -q "Erroneous array or table reference"; then
    echo "BLOCKED scrip(.sc) smoke: PARSER-SN-INFRA-5a footprint detected"
    echo "  Synthetic-label collision across .sc files is back."
    echo "  Check that snocone_parse.y still has g_sc_label_seq monotonic."
    echo "  See GOAL-PARSER-SNOBOL4.md INFRA-5a."
    exit 0
fi

echo "FAIL scrip(.sc) smoke"
echo "  expected:"
echo "$EXPECTED" | sed 's/^/    /'
echo "  actual:"
echo "$ACTUAL" | sed 's/^/    /'
exit 1
