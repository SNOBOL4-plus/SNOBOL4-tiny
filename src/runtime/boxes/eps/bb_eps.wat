;; bb_eps.wat — Dynamic Byrd Box: EPS
;; Direct port of bb_eps.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_eps_a: done flag in state[0]; if done → -1; done=1; return 0
  ;; bb_eps_b: return -1
  (func $bb_eps_a (export "bb_eps_a")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (if (i32.load (local.get $state)) (then (return (i32.const -1))))
    (i32.store (local.get $state) (i32.const 1))
    (i32.const 0))
  (func $bb_eps_b (export "bb_eps_b")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (i32.const -1))
)
