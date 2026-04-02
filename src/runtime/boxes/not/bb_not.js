'use strict';
/*
 * bb_not.js — Dynamic Byrd Box: NOT
 * Direct port of bb_not.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_not(child) {
    return {
        α() {
            const saved = _Δ;
            const cr = child.α();
            _Δ = saved;
            return _is_fail(cr) ? _spec(_Δ, 0) : _FAIL;
        },
        β() { return _FAIL; }
    };
}
module.exports = { bb_not, bb_set_state };
