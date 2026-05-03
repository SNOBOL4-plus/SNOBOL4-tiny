#!/usr/bin/env bash
# test_isolation_ir_sm.sh — RS-15 grep gate.
#
# Verifies that SM-mode runtime files contain no references to the IR-only
# entry points. Comment-only references are allowed.
#
# IR-only symbols: execute_program, interp_eval, interp_eval_pat,
# interp_eval_ref, call_user_function. label_lookup is allowed since shared
# files (interp_label.c) define it; SM files just must not call it.
#
# Returns 0 on PASS (zero leaks), 1 on FAIL.

set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"

SM_FILES=(
    "$ROOT/src/runtime/x86/sm_lower.c"
    "$ROOT/src/runtime/x86/sm_interp.c"
    "$ROOT/src/runtime/x86/sm_codegen.c"
    "$ROOT/src/runtime/x86/sm_prog.c"
    "$ROOT/src/runtime/x86/bb_broker.c"
    "$ROOT/src/runtime/x86/bb_boxes.c"
    "$ROOT/src/runtime/x86/bb_build.c"
    "$ROOT/src/runtime/x86/bb_emit.c"
    "$ROOT/src/runtime/x86/bb_flat.c"
    "$ROOT/src/runtime/x86/bb_pool.c"
    "$ROOT/src/runtime/x86/name_t.c"
    "$ROOT/src/runtime/x86/stmt_exec.c"
    "$ROOT/src/runtime/x86/snobol4_invoke.c"
    "$ROOT/src/runtime/x86/snobol4_pattern.c"
    "$ROOT/src/runtime/x86/snobol4_argval.c"
    "$ROOT/src/runtime/x86/eval_code.c"
)
# coro_runtime.c and pl_runtime.c are intentionally EXCLUDED — they are
# the Icon and Prolog drive engines, which evaluate IR subexpressions
# during Byrd-box drive. See ARCH-SCRIP.md "Exception — Icon and Prolog
# generators" for the architectural rationale.

IR_SYMS=(
    "execute_program"
    "interp_eval"
    "interp_eval_pat"
    "interp_eval_ref"
    "call_user_function"
)

leaks=0
for f in "${SM_FILES[@]}"; do
    [ -f "$f" ] || continue
    for sym in "${IR_SYMS[@]}"; do
        # Look for the symbol followed by '(' (a call), ignoring lines that
        # are pure comments (start with /* or //, or are inside a /* ... */
        # block). Simple filter: skip lines whose code-portion is entirely
        # within a // or /* ... */ comment.
        hits=$(grep -n -E "\\b${sym}\\b[[:space:]]*\\(" "$f" 2>/dev/null \
               | grep -vE '^[0-9]+:[[:space:]]*(/\*|\*|//)' \
               || true)
        if [ -n "$hits" ]; then
            echo "FAIL  $f calls IR-only $sym:"
            echo "$hits" | sed 's/^/    /'
            leaks=$((leaks+1))
        fi
    done
done

if [ $leaks -gt 0 ]; then
    echo
    echo "FAIL  $leaks IR-only symbol leak(s) in SM runtime files"
    exit 1
fi
echo "PASS  no IR-only symbol leaks in SM runtime files"
exit 0
