;; bb_atp.wat — Dynamic Byrd Box: ATP
;; Direct port of bb_atp.c  |  part of src/runtime/boxes/
;; Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
;;
;; Match state globals ($Σ $Δ $Ω) injected by bb_set_state / exec_stmt Phase 1.
;; Return convention: ≥0 = γ (matched len), -1 = ω (fail).

(module
  (memory (import "env" "memory") 1)
  (global $Σ (import "env" "Σ") (mut i32))
  (global $Δ (import "env" "Δ") (mut i32))
  (global $Ω (import "env" "Ω") (mut i32))

  ;; bb_atp_a: write Δ as ATP capture entry; return 0
  ;; bb_atp_b: return -1
  ;; p0 = varname_ptr; capture list base at 0x70000
  (func $bb_atp_a (export "bb_atp_a")
        (param $varname_ptr i32) (param $p1 i32) (param $state i32)
        (result i32)
    (local $cnt i32) (local $base i32)
    (local.set $cnt (i32.load (i32.const 0x70000)))
    (local.set $base (i32.add (i32.const 0x70004)
                              (i32.mul (local.get $cnt) (i32.const 12))))
    (i32.store (local.get $base)                              (i32.const 2))
    (i32.store (i32.add (local.get $base) (i32.const 4))     (local.get $varname_ptr))
    (i32.store (i32.add (local.get $base) (i32.const 8))     (global.get $Δ))
    (i32.store (i32.const 0x70000) (i32.add (local.get $cnt) (i32.const 1)))
    (i32.const 0))
  (func $bb_atp_b (export "bb_atp_b")
        (param $p0 i32) (param $p1 i32) (param $state i32) (result i32)
    (i32.const -1))
)
