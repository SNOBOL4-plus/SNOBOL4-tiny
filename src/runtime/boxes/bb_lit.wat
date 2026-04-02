;; bb_lit.wat — Dynamic Byrd Box: LIT
;; Direct port of bb_lit.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_lit_a: if Δ+len>Ω → -1; memcmp fail → -1; Δ+=len; return len
  ;; bb_lit_b: Δ-=len; return -1
  ;; params: lit_ptr(i32) lit_len(i32) saved_Δ(i32) [unused]
  (func $bb_lit_a (export "bb_lit_a")
        (param $lit_ptr i32) (param $lit_len i32) (param $state i32)
        (result i32)
    (local $i i32)
    (if (i32.gt_u (i32.add (global.get $Δ) (local.get $lit_len)) (global.get $Ω))
      (then (return (i32.const -1))))
    (local.set $i (i32.const 0))
    (block $fail
      (loop $cmp
        (br_if $fail (i32.ge_u (local.get $i) (local.get $lit_len)))
        (br_if $fail (i32.ne
          (i32.load8_u (i32.add (i32.add (global.get $Σ) (global.get $Δ)) (local.get $i)))
          (i32.load8_u (i32.add (local.get $lit_ptr) (local.get $i)))))
        (local.set $i (i32.add (local.get $i) (i32.const 1)))
        (br $cmp))
      (global.set $Δ (i32.add (global.get $Δ) (local.get $lit_len)))
      (return (local.get $lit_len)))
    (i32.const -1))

  (func $bb_lit_b (export "bb_lit_b")
        (param $lit_ptr i32) (param $lit_len i32) (param $state i32)
        (result i32)
    (global.set $Δ (i32.sub (global.get $Δ) (local.get $lit_len)))
    (i32.const -1))
)
