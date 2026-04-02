'use strict';
/*
 * bb_tab.js — Dynamic Byrd Box: TAB
 * Direct port of bb_tab.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_tab(n) {
    let advance = 0;
    return {
        α() {
            if (_Δ > n) return _FAIL;
            advance = n - _Δ;
            const r = _spec(_Δ, advance); _Δ = n; return r;
        },
        β() { _Δ -= advance; return _FAIL; }
    };
}
module.exports = { bb_tab, bb_set_state };
