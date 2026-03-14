# Session 62 artifact

## Changes since session 61
- emit_break: fixed wrap bug — goto inside braces, never detaches from if-guard
- emit_span beta: same wrap bug fixed
- parse.c: binary ~ (tilde) now drops right side (tag is tree metadata, not a literal match)
  START now passes clean (no spurious Parse Error before it)

## Stats
- Lines: 24928
- MD5: 0c93128616ae97c968b064c021a1ef85
- GCC: 0 errors

## Active bug
Segfault on real statements (X = 1). Tilde fix exposed a null-pointer somewhere
in the pattern tree — epsilon ~ '' (tag=empty string) now drops the right side,
leaving a CAT with NULL right child. byrd_emit handles NULL as epsilon correctly,
but the segfault suggests something else is being hit. Next: run under gdb or
add null-ptr guard in the tilde parser.
