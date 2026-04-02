;; bb_arb.wat — Dynamic Byrd Box: ARB
;; Direct port of bb_arb.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_arb_a: count=0; start=Δ; save to state; return 0 (zero-width γ)
  ;; bb_arb_b: count++; if start+count>Ω → -1; Δ=start+count; return count
  ;; state layout: [0]=count [4]=start
  (func $bb_arb_a (export "bb_arb_a")
        (param $p0 i32) (param $p1 i32) (param $state i32)
        (result i32)
    (i32.store (local.get $state) (i32.const 0))
    (i32.store (i32.add (local.get $state) (i32.const 4)) (global.get $Δ))
    (i32.const 0))

  (func $bb_arb_b (export "bb_arb_b")
        (param $p0 i32) (param $p1 i32) (param $state i32)
        (result i32)
    (local $count i32) (local $start i32)
    (local.set $count (i32.add (i32.load (local.get $state)) (i32.const 1)))
    (local.set $start (i32.load (i32.add (local.get $state) (i32.const 4))))
    (i32.store (local.get $state) (local.get $count))
    (if (i32.gt_u (i32.add (local.get $start) (local.get $count)) (global.get $Ω))
      (then (return (i32.const -1))))
    (global.set $Δ (i32.add (local.get $start) (local.get $count)))
    (local.get $count))
)
