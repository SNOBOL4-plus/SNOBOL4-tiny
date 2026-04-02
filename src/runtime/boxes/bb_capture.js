'use strict';
/*
 * bb_capture.js — Dynamic Byrd Box: CAPTURE
 * Direct port of bb_capture.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

let _pending = [];
function bb_reset_captures() { _pending = []; }
function bb_get_pending()    { return _pending; }
function bb_capture(child, varname, immediate, vars) {
    return {
        α() {
            const cr = child.α();
            if (_is_fail(cr)) return _FAIL;
            _do_capture(cr, varname, immediate, vars); return cr;
        },
        β() {
            const cr = child.β();
            if (_is_fail(cr)) return _FAIL;
            _do_capture(cr, varname, immediate, vars); return cr;
        }
    };
}
function _do_capture(cr, varname, immediate, vars) {
    if (!varname) return;
    if (immediate) vars[varname] = cr;
    else _pending.push({ varname, value: cr });
}
module.exports = { bb_capture, bb_reset_captures, bb_get_pending, bb_set_state };
