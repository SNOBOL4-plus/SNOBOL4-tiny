# doc/CORPUS_MIGRATION.md — Corpus Migration Execution Checklist

This file is the verify condition for M-G0-CORPUS-AUDIT execution.
**Open this file first at every session that touches corpus migration.**
Do not declare the migration complete until every box is checked.

Naming convention in corpus: `rungNN_name_testname.ext`
No prefix. No serial numbers. Extension reflects actual content (.s not .c).
Runners stay in each compiler repo pointing at `$CORPUS_REPO/programs/<lang>/`.

---

## Icon (`programs/icon/`)

- [x] Copy `test/frontend/icon/corpus/rung01–38/` → `corpus/programs/icon/` (flat, rungNN_name_testname.icn)
- [x] Update all 38 `run_rung*.sh` runners to use `CORPUS_REPO`
- [x] Correct `.c` → `.s` extension (NASM x64 assembly)
- [ ] **DELETE** `one4all/test/frontend/icon/corpus/` from one4all

## Prolog (`programs/prolog/`)

- [ ] Copy `test/frontend/prolog/corpus/rung*/` → `corpus/programs/prolog/` (flat, rungNN_name_testname.ext)
- [ ] Update prolog rung runners to use `CORPUS_REPO`
- [ ] **DELETE** `one4all/test/frontend/prolog/corpus/` from one4all

## SNOBOL4 smoke (`programs/snobol4/smoke/`)

- [ ] Copy `test/frontend/snobol4/*.sno` → `corpus/programs/snobol4/smoke/`
- [ ] Update any runners
- [ ] **DELETE** originals from one4all

## Beauty tests (`programs/snobol4/beauty/`)

- [ ] Copy `test/beauty/*/driver.sno` + subsystems → `corpus/programs/snobol4/beauty/`
- [ ] Update any runners
- [ ] **DELETE** originals from one4all

## Feat tests (`programs/snobol4/feat/`)

- [ ] Copy `test/feat/f*.sno` → `corpus/programs/snobol4/feat/`
- [ ] Update any runners
- [ ] **DELETE** originals from one4all

## JVM J3 tests (`programs/snobol4/jvm_j3/`)

- [ ] Copy `test/jvm_j3/*.sno` → `corpus/programs/snobol4/jvm_j3/`
- [ ] Update any runners
- [ ] **DELETE** originals from one4all

## Snocone (`programs/snocone/`)

- [ ] Copy `test/frontend/snocone/sc_asm_corpus/*.sc` → `corpus/programs/snocone/`
- [ ] Copy `test/crosscheck/sc_corpus/*.sc` → `corpus/crosscheck/snocone/`
- [ ] Update any runners
- [ ] **DELETE** originals from one4all

## Rebus (`programs/rebus/`)

- [ ] Copy `test/rebus/*.reb` → `corpus/programs/rebus/`
- [ ] Update any runners
- [ ] **DELETE** originals from one4all

---

## Done when

Every box above is checked. `one4all/test/` contains no `.icn`, `.pl`, `.sno`,
`.sc`, or `.reb` corpus source files. All runners use `CORPUS_REPO`.
