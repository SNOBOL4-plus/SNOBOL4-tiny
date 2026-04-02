;; bb_notany.wat — Dynamic Byrd Box: NOTANY
;; Direct port of bb_notany.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_notany_a: if Δ>=Ω or Σ[Δ] IN chars → -1; Δ++; return 1
  ;; bb_notany_b: Δ--; return -1
  (func $strchr_na (param $ptr i32) (param $c i32) (result i32)
    (local $ch i32)
    (block $found
      (block $notfound
        (loop $scan
          (local.set $ch (i32.load8_u (local.get $ptr)))
          (br_if $notfound (i32.eqz (local.get $ch)))
          (br_if $found (i32.eq (local.get $ch) (local.get $c)))
          (local.set $ptr (i32.add (local.get $ptr) (i32.const 1)))
          (br $scan))
        (return (i32.const 0)))
      (return (i32.const 1)))
    (i32.const 0))

  (func $bb_notany_a (export "bb_notany_a")
        (param $chars_ptr i32) (param $p1 i32) (param $state i32)
        (result i32)
    (if (i32.ge_u (global.get $Δ) (global.get $Ω)) (then (return (i32.const -1))))
    (if (call $strchr_na (local.get $chars_ptr)
          (i32.load8_u (i32.add (global.get $Σ) (global.get $Δ))))
      (then (return (i32.const -1))))
    (global.set $Δ (i32.add (global.get $Δ) (i32.const 1)))
    (i32.const 1))

  (func $bb_notany_b (export "bb_notany_b")
        (param $chars_ptr i32) (param $p1 i32) (param $state i32)
        (result i32)
    (global.set $Δ (i32.sub (global.get $Δ) (i32.const 1)))
    (i32.const -1))
)
