;; bb_arbno.wat — Dynamic Byrd Box: ARBNO
;; Direct port of bb_arbno.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

;; bb_arbno: composite — α/β in sno_engine.wat
  (func $bb_arbno_a (export "bb_arbno_a") (param $p0 i32)(param $p1 i32)(param $s i32)(result i32) (i32.const -1))
  (func $bb_arbno_b (export "bb_arbno_b") (param $p0 i32)(param $p1 i32)(param $s i32)(result i32) (i32.const -1))
)
