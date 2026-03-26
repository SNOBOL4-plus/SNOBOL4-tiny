#!/bin/bash
# run_rung36.sh — rung36_jcon JCON oracle corpus runner
# 75 tests from JCON test suite (jcon-master/test/), ordered easy→hard.
# .expected files are the authoritative JCON oracle outputs (.std renamed).
# .stdin files feed stdin (from JCON .dat files) for tests that use read().
# .xfail files mark tests requiring unimplemented features (SET, BIGINT, COEXPR, etc.)
#
# Usage: bash run_rung36.sh [/path/to/icon_driver]
# Default driver: /tmp/icon_driver

DRIVER="${1:-/tmp/icon_driver}"
JASMIN="$(dirname "$0")/../../../src/backend/jvm/jasmin.jar"
CORPUS="$(dirname "$0")/corpus/rung36_jcon"
PASS=0; FAIL=0; XFAIL=0

for icn in "$CORPUS"/t*.icn; do
  base="${icn%.icn}"
  exp="$base.expected"
  [ -f "$exp" ] || continue

  if [ -f "$base.xfail" ]; then
    XFAIL=$((XFAIL+1))
    echo "XFAIL: $(basename $icn)"
    continue
  fi

  "$DRIVER" -jvm "$icn" -o /tmp/t36.j 2>/dev/null
  java -jar "$JASMIN" /tmp/t36.j -d /tmp/ 2>/dev/null
  cls=$(grep -m1 '\.class' /tmp/t36.j | awk '{print $NF}')

  stdin_f="$base.stdin"
  if [ -f "$stdin_f" ]; then
    got=$(timeout 10 java -cp /tmp/ "$cls" < "$stdin_f" 2>/dev/null)
  else
    got=$(timeout 10 java -cp /tmp/ "$cls" 2>/dev/null)
  fi

  want=$(cat "$exp")
  if [ "$got" = "$want" ]; then
    PASS=$((PASS+1))
    echo "PASS: $(basename $icn)"
  else
    FAIL=$((FAIL+1))
    echo "FAIL: $(basename $icn)"
    echo "  want: $(echo "$want" | tr '\n' '|')"
    echo "  got:  $(echo "$got"  | tr '\n' '|')"
  fi
done

echo "--- rung36: $PASS pass, $FAIL fail, $XFAIL xfail ---"
[ $FAIL -eq 0 ]
