;; bb_rtab.wat — Dynamic Byrd Box: RTAB
;; Direct port of bb_rtab.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_rtab_a: target=Ω-n; if Δ>target → -1; Δ=target; return advance
  ;; bb_rtab_b: Δ-=saved; return -1
  (func $bb_rtab_a (export "bb_rtab_a")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (local $target i32) (local $adv i32)
    (local.set $target (i32.sub (global.get $Ω) (local.get $n)))
    (if (i32.gt_u (global.get $Δ) (local.get $target)) (then (return (i32.const -1))))
    (local.set $adv (i32.sub (local.get $target) (global.get $Δ)))
    (i32.store (local.get $state) (local.get $adv))
    (global.set $Δ (local.get $target))
    (local.get $adv))

  (func $bb_rtab_b (export "bb_rtab_b")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (global.set $Δ (i32.sub (global.get $Δ) (i32.load (local.get $state))))
    (i32.const -1))
)
