'use strict';
/*
 * bb_rem.js — Dynamic Byrd Box: REM
 * Direct port of bb_rem.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_rem() {
    return {
        α() { const r = _spec(_Δ, _Ω - _Δ); _Δ = _Ω; return r; },
        β() { return _FAIL; }
    };
}
module.exports = { bb_rem, bb_set_state };
