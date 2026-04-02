;; bb_pos.wat — Dynamic Byrd Box: POS
;; Direct port of bb_pos.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_pos_a: if Δ!=n → -1; return 0 (zero-width)
  ;; bb_pos_b: return -1
  (func $bb_pos_a (export "bb_pos_a")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (if (i32.ne (global.get $Δ) (local.get $n)) (then (return (i32.const -1))))
    (i32.const 0))

  (func $bb_pos_b (export "bb_pos_b")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (i32.const -1))
)
