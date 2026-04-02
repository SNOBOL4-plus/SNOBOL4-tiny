'use strict';
/*
 * bb_seq.js — Dynamic Byrd Box: SEQ
 * Direct port of bb_seq.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_seq(left, right) {
    return {
        α() {
            const lr = left.α();
            if (_is_fail(lr)) return _FAIL;
            let rr = right.α();
            while (_is_fail(rr)) {
                const lr2 = left.β();
                if (_is_fail(lr2)) return _FAIL;
                rr = right.α();
            }
            return rr;
        },
        β() {
            let rr = right.β();
            while (_is_fail(rr)) {
                const lr = left.β();
                if (_is_fail(lr)) return _FAIL;
                rr = right.α();
            }
            return rr;
        }
    };
}
module.exports = { bb_seq, bb_set_state };
