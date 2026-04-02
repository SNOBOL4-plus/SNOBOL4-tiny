'use strict';
/*
 * bb_alt.js — Dynamic Byrd Box: ALT
 * Direct port of bb_alt.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_alt(children) {
    let current = 0, saved_Δ = 0;
    return {
        α() {
            saved_Δ = _Δ; current = 0;
            while (current < children.length) {
                _Δ = saved_Δ;
                const r = children[current].α();
                if (!_is_fail(r)) return r;
                current++;
            }
            return _FAIL;
        },
        β() {
            if (current >= children.length) return _FAIL;
            const r = children[current].β();
            if (!_is_fail(r)) return r;
            current++;
            while (current < children.length) {
                _Δ = saved_Δ;
                const r2 = children[current].α();
                if (!_is_fail(r2)) return r2;
                current++;
            }
            return _FAIL;
        }
    };
}
module.exports = { bb_alt, bb_set_state };
