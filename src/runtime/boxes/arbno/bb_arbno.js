'use strict';
/*
 * bb_arbno.js — Dynamic Byrd Box: ARBNO
 * Direct port of bb_arbno.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_arbno(body) {
    const stack = [];
    return {
        α() {
            stack.length = 0;
            stack.push({ start: _Δ });
            while (true) {
                const frame = stack[stack.length - 1];
                const br = body.α();
                if (_is_fail(br)) return _spec(stack[0].start, _Δ - stack[0].start);
                if (_Δ === frame.start) return _spec(stack[0].start, _Δ - stack[0].start);
                stack.push({ start: _Δ });
            }
        },
        β() {
            if (stack.length <= 1) return _FAIL;
            stack.pop();
            _Δ = stack[stack.length - 1].start;
            return _spec(stack[0].start, _Δ - stack[0].start);
        }
    };
}
module.exports = { bb_arbno, bb_set_state };
