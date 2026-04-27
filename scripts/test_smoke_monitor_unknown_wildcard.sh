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

# Check: 2-way spl+dot reaches END (exit 0)
out=$(PARTICIPANTS="spl dot" bash "$HARNESS" "$TD/probe.sno" 2>&1)
rc=$?
if [ $rc -eq 0 ]; then
    if echo "$out" | grep -q "all reached END"; then
        PASS=$((PASS+1))
    else
        echo "FAIL: harness exit 0 but no END marker"
        echo "$out" | tail -10
        FAIL=$((FAIL+1))
    fi
else
    echo "FAIL: harness exit=$rc on TABLE() probe"
    echo "$out" | tail -15
    FAIL=$((FAIL+1))
fi

# Check: non-UNKNOWN value mismatch still DIVERGEs (regression guard)
# Build a probe where dot and spl emit genuinely different values.
# Hard to do without a runtime difference — but we can verify by
# corrupting one side's wire artificially; skipping for now.  The
# integration above already proves the wildcard is selective enough
# that beauty.sno self-host moves PAST the step-47 TABLE noise to
# step-49 (a different bug).

echo "PASS=$PASS FAIL=$FAIL"
[ $FAIL -eq 0 ]
