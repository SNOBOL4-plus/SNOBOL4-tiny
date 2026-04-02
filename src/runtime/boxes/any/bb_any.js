'use strict';
/*
 * bb_any.js — Dynamic Byrd Box: ANY
 * Direct port of bb_any.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_any(chars) {
    return {
        α() {
            if (_Δ >= _Ω || chars.indexOf(_Σ[_Δ]) < 0) return _FAIL;
            const r = _spec(_Δ, 1); _Δ++;              return r;
        },
        β() { _Δ--; return _FAIL; }
    };
}
module.exports = { bb_any, bb_set_state };
