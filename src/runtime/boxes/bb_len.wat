;; bb_len.wat — Dynamic Byrd Box: LEN
;; Direct port of bb_len.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_len_a: if Δ+n>Ω → -1; Δ+=n; return n
  ;; bb_len_b: Δ-=n; return -1
  (func $bb_len_a (export "bb_len_a")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (if (i32.gt_u (i32.add (global.get $Δ) (local.get $n)) (global.get $Ω))
      (then (return (i32.const -1))))
    (global.set $Δ (i32.add (global.get $Δ) (local.get $n)))
    (local.get $n))

  (func $bb_len_b (export "bb_len_b")
        (param $n i32) (param $p1 i32) (param $state i32)
        (result i32)
    (global.set $Δ (i32.sub (global.get $Δ) (local.get $n)))
    (i32.const -1))
)
