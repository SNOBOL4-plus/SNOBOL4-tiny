# Artifacts

One file per artifact. Git history is the archive — no numbered session copies.
When an artifact changes, overwrite it and commit. `git log -p artifacts/asm/foo.s` shows the full evolution.

---

## ASM backend — `artifacts/asm/`

### beauty_prog.s
- **what:** beauty.sno compiled via `-asm` backend — the primary sprint oracle
- **assemble:** `nasm -f elf64 -I src/runtime/asm/ beauty_prog.s -o /dev/null`
- **lines:** 11656 (session176)
- **status:** assembles clean (one `db-empty` warning — known, benign)
- **update:** `src/sno2c/sno2c -asm -I$INC $BEAUTY > artifacts/asm/beauty_prog.s`

### null.s
- **what:** Sprint A0 oracle — empty program, assembles+links+runs → exit 0
- **milestone:** M-ASM-HELLO ✅ session145
- **assemble:** `nasm -f elf64 null.s -o null.o && ld null.o -o null && ./null`

### lit_hello.s
- **what:** Sprint A1 oracle — LIT node, inline repe cmpsb, α/β/γ/ω real labels
- **milestone:** M-ASM-LIT ✅ session146

### pos0_rpos0.s / cat_pos_lit_rpos.s
- **what:** Sprint A2–A3 oracles — POS/RPOS and SEQ (CAT) wiring
- **milestone:** M-ASM-SEQ ✅ session146

### alt_first.s / alt_second.s / alt_fail.s
- **what:** Sprint A4 oracles — ALT left/right arms and backtrack
- **milestone:** M-ASM-ALT ✅ session147

### arbno_match.s / arbno_empty.s / arbno_alt.s
- **what:** Sprint A5 oracles — ARBNO with cursor stack
- **milestone:** M-ASM-ARBNO ✅ session147

### any_vowel.s / notany_consonant.s / span_digits.s / break_space.s
- **what:** Sprint A6 oracles — ANY/NOTANY/SPAN/BREAK charset nodes
- **milestone:** M-ASM-CHARSET ✅ session147

### assign_lit.s / assign_digits.s
- **what:** Sprint A7 oracles — `$` capture into flat .bss buffers
- **milestone:** M-ASM-ASSIGN ✅ session148

### ref_astar_bstar.s / anbn.s
- **what:** Sprint A8 oracles — named patterns, flat labels, 2-way jmp dispatch
- **milestone:** M-ASM-NAMED ✅ session148

### multi_capture_abc.s / star_deref_capture.s
- **what:** Sprint A9 — multi-capture and `*VAR` indirect pattern reference
- **milestone:** M-ASM-CROSSCHECK ✅ session151

### stmt_output_lit.s / stmt_assign.s / stmt_goto.s
- **what:** Program-mode statement emitter oracles — OUTPUT, assignment, goto
- **milestone:** contributed to M-ASM-BEAUTY work (sessions 154+)

---

## C backend — `artifacts/`

### beauty_tramp_session95.c (historical)
- **md5:** cc34e62fee07676e12d0824c14fe6e85
- **lines:** 15639
- **status:** compiles with warnings; 106/106 crosscheck at time of generation

---

## Protocol

**When to update an artifact:**
- `beauty_prog.s` — any session that changes `emit_byrd_asm.c` or the `.mac` file
- Test fixture `.s` files — when the corresponding node type changes behavior
- Never create `foo_sessionN.s` — overwrite `foo.s` and let git track the diff

**Commit message format:**
```
sessionN: artifacts — beauty_prog.s updated (reason)
```
