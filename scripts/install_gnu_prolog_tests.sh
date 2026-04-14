#!/usr/bin/env bash
# install_gnu_prolog_tests.sh — copy GNU Prolog test suite files into corpus
# Source: https://github.com/didoudiaz/gprolog/tree/master/src/TestsPl
# Target: /home/claude/corpus/programs/prolog/gnu_tests/
# Idempotent: skips files already present.

set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TARGET=/home/claude/corpus/programs/prolog/gnu_tests
UPSTREAM=https://github.com/didoudiaz/gprolog.git

TESTS=(
    t_arith.pl
    t_atom.pl
    t_control.pl
    t_dcg.pl
    t_list.pl
)

mkdir -p "$TARGET"

# --- Check if all targets already present ---
ALL_PRESENT=1
for t in "${TESTS[@]}"; do
    [ -f "$TARGET/$t" ] || { ALL_PRESENT=0; break; }
done

if [ "$ALL_PRESENT" = "1" ]; then
    echo "SKIP: GNU Prolog test files already present in $TARGET"
    exit 0
fi

TMPDIR_BASE=/tmp/gprolog_install_$$

# --- Sparse clone ---
echo "Installing GNU Prolog tests from $UPSTREAM ..."
mkdir -p "$TMPDIR_BASE"
trap 'rm -rf "$TMPDIR_BASE"' EXIT

git clone --depth=1 --filter=blob:none --sparse "$UPSTREAM" "$TMPDIR_BASE/gprolog" 2>/dev/null
cd "$TMPDIR_BASE/gprolog"
git sparse-checkout set src/TestsPl

INSTALLED=0
for t in "${TESTS[@]}"; do
    src="src/TestsPl/$t"
    if [ -f "$src" ]; then
        cp "$src" "$TARGET/$t"
        echo "  copied $t"
        INSTALLED=$((INSTALLED+1))
    else
        echo "  WARN: $t not found at $src"
    fi
done

echo "Installed $INSTALLED/${#TESTS[@]} GNU Prolog test files into $TARGET"
