# Artifacts — beauty_tramp generated C snapshots

| Session | Date | Lines | MD5 | Compile | Status |
|---------|------|-------|-----|---------|--------|
| 73 | 2026-03-15 | — | 2028344da06f3f862deae3efedf4bc9b | 0 errors | quote-strip fixes |
| 76 | 2026-03-15 | — | 2028344da06f3f862deae3efedf4bc9b | 0 errors | M-CNODE cnode-wire done |
| 77 | 2026-03-14 | 31773 | e784cec765f711df8bbe7a2427689eae | 0 errors | CHANGED — pat_lit strv() bug fixed |

## Session 77 notes
- Commit: `0113d90 fix(emit_cnode): pat_lit takes const char* not SnoVal — remove strv() wrapper in build_pat E_STR`
- pat_lit(strv("...")) → pat_lit("...") in build_pat E_STR case
- Binary compiles 0 errors
- START → START now works ✅
- Active bug: $expr indirect read uses e->left (NULL) instead of e->right — see SESSION.md
- `$'@S' = link(...)` stores STRING not link UDEF — broken until E_DEREF fix applied
