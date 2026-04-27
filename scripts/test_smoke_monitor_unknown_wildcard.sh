#!/usr/bin/env bash
# test_smoke_monitor_unknown_wildcard.sh — verify monitor_sync_bin.py's
# keys_match treats MWT_UNKNOWN as a wildcard on the type field.
#
# Background: SPITBOL's bridge (x64/osint/monitor_ipc_runtime.c
# spl_block_to_wire) emits MWT_UNKNOWN(=255) for nmblk/ptblk/atblk/
# tbblk/cdblk/efblk because their type-words are not yet exported.
# CSN and dot emit the correct typed kind.  The compare must succeed
# when one side is UNKNOWN and the other is a real type, as long as
# kind+name+value match.
#
# Expected outcome: a 2-way `spl dot` run on a program that creates
# a TABLE() (which spl tags UNKNOWN, dot tags TABLE) should NOT
# DIVERGE — the run reaches END.
#
# This test does NOT require Python keys_match unit-style; it
# integration-tests via the real harness.
#
# S-2-bridge-7 + SN-26-bridge-coverage cross-ref, Mon Apr 28 2026.

set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

HARNESS="${HARNESS:-$HERE/test_monitor_3way_sync_step_auto.sh}"

if [ ! -x /home/claude/x64/bin/sbl ]; then echo "SKIP: spitbol not built"; exit 0; fi
if [ ! -x "$(command -v dotnet)" ]; then echo "SKIP: dotnet missing"; exit 0; fi
if [ ! -f /home/claude/snobol4dotnet/Snobol4/bin/Release/net10.0/Snobol4.dll ]; then
    echo "SKIP: Snobol4.dll not built"; exit 0; fi
if [ ! -f "$HARNESS" ]; then echo "SKIP: harness missing at $HARNESS"; exit 0; fi

TD="$(mktemp -d -t monitor_unknown_wildcard.XXXXXX)"
trap 'rm -rf "$TD"' EXIT

# A program that exercises the UNKNOWN-vs-TABLE divergence path in spl.
# Without wildcard support, spl emits VALUE T = UNKNOWN and dot emits
# VALUE T = TABLE → step 2 DIVERGE.  With wildcard support, the run
# reaches END normally.
cat > "$TD/probe.sno" << 'EOF'
                  T = TABLE()
END
EOF

PASS=0; FAIL=0

# Check 1: 2-way spl+dot reaches END (exit 0) with TABLE() probe.
# Without UNKNOWN-wildcard, this would DIVERGE on type byte (UNKNOWN vs TABLE).
out=$(PARTICIPANTS="spl dot" bash "$HARNESS" "$TD/probe.sno" 2>&1)
rc=$?
if [ $rc -eq 0 ]; then
    if echo "$out" | grep -q "all reached END"; then
        PASS=$((PASS+1))
    else
        echo "FAIL Check 1: harness exit 0 but no END marker"
        echo "$out" | tail -10
        FAIL=$((FAIL+1))
    fi
else
    echo "FAIL Check 1: harness exit=$rc on TABLE() probe"
    echo "$out" | tail -15
    FAIL=$((FAIL+1))
fi

# Check 2: <lval> sentinel as wildcard on name field.
# Probe creates a table and stores into a slot.  dot now emits the
# collection name 'd' (post S-2-bridge-7-lval) for the slot store;
# spl emits its own (possibly junk) name.  But IF spl were emitting
# <lval> per protocol contract, the wildcard should make the run
# pass.  We verify by feeding a probe that elicits <lval> from
# at least one bridge — currently csn's lvalue_name_id() guarantees
# this, but csn has the label-only-skip bug at startup that breaks
# 2-way csn+dot.  Use a synthetic test: confirm keys_match accepts
# <lval> on either side.
python3 - << 'PY' || FAIL=$((FAIL+1))
import sys, os
sys.path.insert(0, '/home/claude/one4all/scripts/monitor')
import monitor_sync_bin as m

# Construct two events: one with name 'UTF', one with '<lval>'
# Same kind, same type, same value → wildcard should make them match.
kinds = m.MWK_VALUE
type_str = 1  # STRING
value = b'NO_BREAK_SPACE'

a = (kinds, 'UTF',     type_str, value)
b = (kinds, '<lval>',  type_str, value)
assert m.keys_match(a, b), "wildcard <lval> failed (a vs b)"
assert m.keys_match(b, a), "wildcard <lval> failed (b vs a)"

# Sanity: distinct real names must DIVERGE.
c = (kinds, 'UTF',     type_str, value)
d = (kinds, 'OTHER',   type_str, value)
assert not m.keys_match(c, d), "name divergence not detected"

# Sanity: value bytes mismatch must DIVERGE even if name is <lval>.
e = (kinds, 'UTF',     type_str, b'X')
f = (kinds, '<lval>',  type_str, b'Y')
assert not m.keys_match(e, f), "value-byte divergence not detected (with <lval>)"

# Sanity: kind mismatch must DIVERGE.
g = (m.MWK_VALUE, 'UTF', type_str, value)
h = (m.MWK_LABEL, 'UTF', type_str, value)
assert not m.keys_match(g, h), "kind divergence not detected"

print("OK")
PY
if [ $? -eq 0 ]; then PASS=$((PASS+1)); else echo "FAIL Check 2: keys_match wildcard semantics"; fi

# Check 3: combined UNKNOWN + <lval> wildcards (regression of Check 1's
# UNKNOWN logic still working in the new code).
python3 - << 'PY' || FAIL=$((FAIL+1))
import sys
sys.path.insert(0, '/home/claude/one4all/scripts/monitor')
import monitor_sync_bin as m

# Aggregate-element store: spl emits UNKNOWN type + junk name; dot emits
# correct type + collection name.  With both wildcards, this matches.
a = (m.MWK_VALUE, '<lval>', m.MWT_UNKNOWN, b'NO_BREAK_SPACE')   # csn emits this
b = (m.MWK_VALUE, 'UTF',    1,             b'NO_BREAK_SPACE')   # dot emits this
assert m.keys_match(a, b), "combined UNKNOWN+<lval> wildcard failed"
print("OK")
PY
if [ $? -eq 0 ]; then PASS=$((PASS+1)); else echo "FAIL Check 3: combined wildcards"; fi

echo "PASS=$PASS FAIL=$FAIL"
[ $FAIL -eq 0 ]
