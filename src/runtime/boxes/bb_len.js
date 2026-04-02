'use strict';
/*
 * bb_len.js — Dynamic Byrd Box: LEN
 * Direct port of bb_len.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_len(n) {
    return {
        α() {
            if (_Δ + n > _Ω) return _FAIL;
            const r = _spec(_Δ, n); _Δ += n; return r;
        },
        β() { _Δ -= n; return _FAIL; }
    };
}
module.exports = { bb_len, bb_set_state };
