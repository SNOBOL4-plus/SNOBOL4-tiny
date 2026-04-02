;; bb_rem.wat — Dynamic Byrd Box: REM
;; Direct port of bb_rem.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_rem_a: adv=Ω-Δ; Δ=Ω; return adv
  ;; bb_rem_b: return -1
  (func $bb_rem_a (export "bb_rem_a")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (local $adv i32)
    (local.set $adv (i32.sub (global.get $Ω) (global.get $Δ)))
    (global.set $Δ (global.get $Ω))
    (local.get $adv))
  (func $bb_rem_b (export "bb_rem_b")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (i32.const -1))
)
