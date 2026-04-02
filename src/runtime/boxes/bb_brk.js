'use strict';
/*
 * bb_brk.js — Dynamic Byrd Box: BRK
 * Direct port of bb_brk.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_brk(chars) {
    let δ = 0;
    return {
        α() {
            δ = 0;
            while (_Δ + δ < _Ω && chars.indexOf(_Σ[_Δ + δ]) < 0) δ++;
            if (_Δ + δ >= _Ω) return _FAIL;
            const r = _spec(_Δ, δ); _Δ += δ; return r;
        },
        β() { _Δ -= δ; return _FAIL; }
    };
}
module.exports = { bb_brk, bb_set_state };
