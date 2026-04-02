;; bb_span.wat — Dynamic Byrd Box: SPAN
;; Direct port of bb_span.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_span_a: scan while in chars; if δ==0 → -1; Δ+=δ; return δ
  ;; bb_span_b: Δ-=saved δ; return -1
  (func $strchr_sp (param $ptr i32) (param $c i32) (result i32)
    (local $ch i32)
    (block $found (block $nf
      (loop $sc
        (local.set $ch (i32.load8_u (local.get $ptr)))
        (br_if $nf (i32.eqz (local.get $ch)))
        (br_if $found (i32.eq (local.get $ch) (local.get $c)))
        (local.set $ptr (i32.add (local.get $ptr) (i32.const 1)))
        (br $sc))
      (return (i32.const 0)))
    (return (i32.const 1)))
    (i32.const 0))

  (func $bb_span_a (export "bb_span_a")
        (param $chars_ptr i32) (param $p1 i32) (param $state i32)
        (result i32)
    (local $d i32)
    (local.set $d (i32.const 0))
    (block $done (loop $scan
      (br_if $done (i32.ge_u (i32.add (global.get $Δ) (local.get $d)) (global.get $Ω)))
      (br_if $done (i32.eqz (call $strchr_sp (local.get $chars_ptr)
          (i32.load8_u (i32.add (i32.add (global.get $Σ) (global.get $Δ)) (local.get $d))))))
      (local.set $d (i32.add (local.get $d) (i32.const 1)))
      (br $scan)))
    (if (i32.le_s (local.get $d) (i32.const 0)) (then (return (i32.const -1))))
    (i32.store (local.get $state) (local.get $d))
    (global.set $Δ (i32.add (global.get $Δ) (local.get $d)))
    (local.get $d))

  (func $bb_span_b (export "bb_span_b")
        (param $chars_ptr i32) (param $p1 i32) (param $state i32)
        (result i32)
    (global.set $Δ (i32.sub (global.get $Δ) (i32.load (local.get $state))))
    (i32.const -1))
)
