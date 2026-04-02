'use strict';
/*
 * bb_dvar.js — Dynamic Byrd Box: DVAR
 * Direct port of bb_dvar.c  |  part of src/runtime/boxes/
 * Author: Claude Sonnet 4.6 — SJ-5, 2026-04-02
 */

let _Σ = '', _Δ = 0, _Ω = 0;
function bb_set_state(s) { _Σ = s.Σ; _Δ = s.Δ; _Ω = s.Ω; }
const _FAIL = null;
function _spec(start, len) { return (len === 0) ? '' : _Σ.slice(start, start + len); }
function _is_fail(v) { return v === null; }

function bb_dvar(name, vars, build_pattern_fn) {
    let child = null, last_val = undefined;
    const { bb_lit } = require('./bb_lit.js');
    const { bb_fail } = require('./bb_fail.js');
    return {
        α() {
            const val = vars[name];
            if (val !== last_val) {
                last_val = val;
                if (val && typeof val === 'object' && val._pat)
                    child = build_pattern_fn(val);
                else if (typeof val === 'string')
                    child = bb_lit(val);
                else
                    child = bb_fail();
            }
            return child ? child.α() : _FAIL;
        },
        β() { return child ? child.β() : _FAIL; }
    };
}
module.exports = { bb_dvar, bb_set_state };
