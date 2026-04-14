#!/usr/bin/env bash
# build_setup.sh — full environment setup for a snobol4ever/one4all Claude session
#
# Run this at the start of every session after cloning the repos.
# Installs all system packages, builds scrip, oracles, and verifies the gate.
#
# Usage: bash /home/claude/one4all/scripts/build_full_session_environment.sh
# Idempotent. Safe to run multiple times.
set -euo pipefail

ONE4ALL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT="$(cd "$ONE4ALL/.." && pwd)"

echo "=== snobol4ever session setup ==="
echo "ROOT:    $ROOT"
echo "ONE4ALL: $ONE4ALL"

# ── 1. git identity ───────────────────────────────────────────────────────────
git config --global user.name  "LCherryholmes"
git config --global user.email "lcherryh@yahoo.com"
for repo in one4all .github; do
    [ -d "$ROOT/$repo/.git" ] && git -C "$ROOT/$repo" config user.name  "LCherryholmes"
    [ -d "$ROOT/$repo/.git" ] && git -C "$ROOT/$repo" config user.email "lcherryh@yahoo.com"
done
echo "OK  git identity: LCherryholmes <lcherryh@yahoo.com>"

# ── 2. system packages (includes bison + flex) ────────────────────────────────
bash "$ONE4ALL/scripts/install_system_packages.sh"

# ── 3. scrip ─────────────────────────────────────────────────────────────────
bash "$ONE4ALL/scripts/build_scrip.sh"

# ── 4. SPITBOL x64 oracle ────────────────────────────────────────────────────
if [ -d "$ROOT/x64/.git" ]; then
    bash "$ONE4ALL/scripts/build_spitbol_oracle.sh"
else
    echo "SKIP spitbol (x64 not cloned — clone snobol4ever/x64 to $ROOT/x64)"
fi

# ── 5. CSNOBOL4 oracle ───────────────────────────────────────────────────────
if [ -d "$ROOT/csnobol4/.git" ]; then
    bash "$ONE4ALL/scripts/build_csnobol4_oracle.sh"
else
    echo "SKIP csnobol4 (not cloned — clone snobol4ever/csnobol4 to $ROOT/csnobol4)"
fi

# ── 6. smoke gate ────────────────────────────────────────────────────────────
echo "--- smoke gate ---"
cd "$ONE4ALL" && bash "$ONE4ALL/scripts/test_smoke_scrip_all_modes.sh"
echo "=== setup complete ==="
