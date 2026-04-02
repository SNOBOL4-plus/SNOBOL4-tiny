'use strict';
/*
 * bb_arb.js — Dynamic Byrd Box: ARB
 * Direct port of bb_arb.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_arb() {
    let count = 0, start = 0;
    return {
        α() { count = 0; start = _Δ; return _spec(_Δ, 0); },
        β() {
            count++;
            if (start + count > _Ω) return _FAIL;
            _Δ = start;
            const r = _spec(_Δ, count); _Δ += count;
            return r;
        }
    };
}
module.exports = { bb_arb, bb_set_state };
