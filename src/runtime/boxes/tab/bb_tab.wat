;; bb_tab.wat — Dynamic Byrd Box: TAB
;; Direct port of bb_tab.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_tab_a: if Δ>n → -1; advance=n-Δ; Δ=n; return advance
  ;; bb_tab_b: Δ-=state[saved_advance]; return -1
  ;; state = mutable i32 slot for saved advance
  (func $bb_tab_a (export "bb_tab_a")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (local $adv i32)
    (if (i32.gt_u (global.get $Δ) (local.get $n)) (then (return (i32.const -1))))
    (local.set $adv (i32.sub (local.get $n) (global.get $Δ)))
    (i32.store (local.get $state) (local.get $adv))
    (global.set $Δ (local.get $n))
    (local.get $adv))

  (func $bb_tab_b (export "bb_tab_b")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (global.set $Δ (i32.sub (global.get $Δ) (i32.load (local.get $state))))
    (i32.const -1))
)
