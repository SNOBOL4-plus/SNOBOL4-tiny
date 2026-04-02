;; bb_rpos.wat — Dynamic Byrd Box: RPOS
;; Direct port of bb_rpos.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_rpos_a: if Δ!=Ω-n → -1; return 0
  ;; bb_rpos_b: return -1
  (func $bb_rpos_a (export "bb_rpos_a")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (if (i32.ne (global.get $Δ) (i32.sub (global.get $Ω) (local.get $n)))
      (then (return (i32.const -1))))
    (i32.const 0))

  (func $bb_rpos_b (export "bb_rpos_b")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (i32.const -1))
)
