;; bb_abort.wat — Dynamic Byrd Box: ABORT
;; Direct port of bb_abort.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_abort_a / bb_abort_b: set abort global, return -1
  (global $abort_flag (mut i32) (i32.const 0))
  (func $bb_abort_a (export "bb_abort_a")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (global.set $abort_flag (i32.const 1)) (i32.const -1))
  (func $bb_abort_b (export "bb_abort_b")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (global.set $abort_flag (i32.const 1)) (i32.const -1))
  (func $bb_aborted (export "bb_aborted") (result i32) (global.get $abort_flag))
)
