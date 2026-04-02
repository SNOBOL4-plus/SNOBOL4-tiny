;; bb_fence.wat — Dynamic Byrd Box: FENCE
;; Direct port of bb_fence.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_fence_a: return 0 (zero-width γ — always succeeds)
  ;; bb_fence_b: return -1 (cut)
  (func $bb_fence_a (export "bb_fence_a")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (i32.const 0))
  (func $bb_fence_b (export "bb_fence_b")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (i32.const -1))
)
