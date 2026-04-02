'use strict';
/*
 * bb_abort.js — Dynamic Byrd Box: ABORT
 * Direct port of bb_abort.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

let _abort_flag = false;
function bb_reset_abort() { _abort_flag = false; }
function bb_aborted()     { return _abort_flag; }
function bb_abort() {
    return {
        α() { _abort_flag = true; return _FAIL; },
        β() { _abort_flag = true; return _FAIL; }
    };
}
module.exports = { bb_abort, bb_reset_abort, bb_aborted, bb_set_state };
