#!/usr/bin/env bash
# build_regenerate.sh — regenerate committed parser/lexer C files from their .y/.l sources
#
# When to run: after editing any .y or .l source file.
# The generated .tab.c / .lex.c files are committed so the build never requires
# bison/flex at normal build time — but when sources change, regenerate here
# and commit both the .y/.l and the updated .tab.c/.lex.c together.
#
# Requires: bison >= 3.0, flex >= 2.6  (installed by build_packages.sh)
# Idempotent. Safe to run multiple times.
# Usage: bash build/build_regenerate.sh
set -euo pipefail

ONE4ALL="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SNO="$ONE4ALL/src/frontend/snobol4"
REBUS="$ONE4ALL/src/frontend/rebus"

# ── prerequisite check ───────────────────────────────────────────────────────
for tool in bison flex; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "FAIL $tool not found — run bash build/build_packages.sh first"
        exit 1
    }
done
echo "OK  bison $(bison --version | head -1 | awk '{print $NF}'), flex $(flex --version | awk '{print $NF}')"

# ── snobol4 parser: snobol4.y → snobol4.tab.c + snobol4.tab.h ───────────────
echo "GEN snobol4.tab.c from snobol4.y"
cd "$SNO"
bison -d -o snobol4.tab.c snobol4.y
echo "OK  snobol4.tab.c snobol4.tab.h"

# ── snobol4 lexer: snobol4.l → snobol4.lex.c ────────────────────────────────
echo "GEN snobol4.lex.c from snobol4.l"
flex --noline -o snobol4.lex.c snobol4.l
echo "OK  snobol4.lex.c"

# ── rebus parser: rebus.y → rebus.tab.c + rebus.tab.h ───────────────────────
echo "GEN rebus.tab.c from rebus.y"
cd "$REBUS"
bison -d -o rebus.tab.c rebus.y
echo "OK  rebus.tab.c rebus.tab.h"

# ── rebus lexer: rebus.l → lex.rebus.c ──────────────────────────────────────
echo "GEN lex.rebus.c from rebus.l"
flex --noline -o lex.rebus.c rebus.l
echo "OK  lex.rebus.c"

# ── raku parser: raku.y → raku.tab.c + raku.tab.h ───────────────────────────
RAKU="$ONE4ALL/src/frontend/raku"
echo "GEN raku.tab.c from raku.y"
cd "$RAKU"
bison -d --warnings=none -Wno-yacc -o raku.tab.c raku.y
echo "OK  raku.tab.c raku.tab.h"

# ── raku lexer: raku.l → raku.lex.c ─────────────────────────────────────────
echo "GEN raku.lex.c from raku.l"
flex --noline --prefix=raku_yy -o raku.lex.c raku.l
echo "OK  raku.lex.c"

echo "DONE — commit .y/.l sources and generated .tab.c/.tab.h/.lex.c together"
