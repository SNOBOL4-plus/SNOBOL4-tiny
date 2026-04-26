#!/usr/bin/env python3
"""
inject_traces_bin.py — binary-protocol sync-step monitor trace injector.

Differences from the text-protocol inject_traces.py:
  - LOADs MON_PUT_VALUE / MON_PUT_CALL / MON_PUT_RETURN (typed-arg) instead of
    MON_SEND (string-only).
  - The MV/MC/MR SNOBOL4 callbacks do NO string conversion.  They pass $N
    directly as the second argument; the C side reads the descriptor's
    type tag and raw bytes.
  - Emits a sidecar names file (one name per line) and tells the .so
    where to find it via env var MONITOR_NAMES_FILE.
  - No IGNORE rules in tracepoints_bin.conf — pattern/array/data values all
    serialize as (type, len=0) and compare byte-equal.

Usage:
    python3 inject_traces_bin.py <sno_file>  [tracepoints_conf]  [--names-out=PATH]

If --names-out is omitted, prints the names file path to stderr (line 1)
followed by the instrumented .sno on stdout.  The harness script captures
both.
"""

import sys
import re
import os

# ---------------------------------------------------------------------------
# tracepoints config parser — INCLUDE/EXCLUDE only (no IGNORE).
# ---------------------------------------------------------------------------

def load_conf(path):
    include_rules = []
    exclude_rules = []

    if not os.path.exists(path):
        include_rules.append(re.compile(r'.*'))
        return include_rules, exclude_rules

    with open(path) as f:
        for raw in f:
            line = raw.split('#')[0].strip()
            if not line:
                continue
            parts = line.split()
            if not parts:
                continue
            verb = parts[0].upper()
            if verb == 'INCLUDE' and len(parts) >= 2:
                include_rules.append(re.compile(parts[1], re.IGNORECASE))
            elif verb == 'EXCLUDE' and len(parts) >= 2:
                exclude_rules.append(re.compile(parts[1], re.IGNORECASE))
    return include_rules, exclude_rules


def is_included(name, include_rules, exclude_rules):
    for rx in exclude_rules:
        if rx.fullmatch(name) or rx.search(name):
            return False
    for rx in include_rules:
        if rx.fullmatch(name) or rx.search(name):
            return True
    return False


# ---------------------------------------------------------------------------
# Scan .sno (and recursively any -INCLUDEd .inc files).
# ---------------------------------------------------------------------------

RE_DEFINE = re.compile(
    r"DEFINE\s*\(\s*'([A-Za-z][A-Za-z0-9]*)\s*\(",
    re.IGNORECASE
)

RE_ASSIGN_LHS = re.compile(
    r'^\s{1,}([A-Za-z][A-Za-z0-9]*|&[A-Za-z][A-Za-z0-9]*)\s*='
)

RE_LABEL_LINE = re.compile(r'^[A-Za-z][A-Za-z0-9]*[\s]')

RE_INCLUDE = re.compile(r"^-INCLUDE\s+'([^']+)'", re.IGNORECASE)

# Names that must never be instrumented (internal monitor scaffolding).
INTERNAL_FNS = {'MV', 'MC', 'MR', 'MON_OPEN', 'MON_PUT_VALUE',
                'MON_PUT_CALL', 'MON_PUT_RETURN', 'MON_CLOSE'}


def _scan_lines(lines, include_rules, exclude_rules, seen_fn, seen_var,
                functions, variables, search_dirs, visited):
    for line in lines:
        stripped = line.strip()

        m_inc = RE_INCLUDE.match(stripped)
        if m_inc:
            fname = m_inc.group(1)
            for d in search_dirs:
                candidate = os.path.join(d, fname)
                if candidate in visited:
                    break
                if os.path.isfile(candidate):
                    visited.add(candidate)
                    with open(candidate) as fh:
                        inc_lines = fh.readlines()
                    _scan_lines(inc_lines, include_rules, exclude_rules,
                                seen_fn, seen_var, functions, variables,
                                search_dirs, visited)
                    break
            continue

        if stripped.startswith('*') or stripped.startswith('-'):
            continue

        for m in RE_DEFINE.finditer(line):
            fn = m.group(1)
            if fn.upper() in INTERNAL_FNS:
                continue
            if fn not in seen_fn and is_included(fn, include_rules, exclude_rules):
                seen_fn.add(fn)
                functions.append(fn)

        if not RE_LABEL_LINE.match(line):
            m = RE_ASSIGN_LHS.match(line)
            if m:
                var = m.group(1)
                if var.upper() in INTERNAL_FNS:
                    continue
                if var not in seen_var and is_included(var, include_rules, exclude_rules):
                    seen_var.add(var)
                    variables.append(var)


def scan_sno(lines, include_rules, exclude_rules, sno_path=None):
    functions = []
    variables = []
    seen_fn   = set()
    seen_var  = set()

    search_dirs = []
    if sno_path:
        search_dirs.append(os.path.dirname(os.path.abspath(sno_path)))
    inc_env = os.environ.get('SNO_LIB') or os.environ.get('INC') or ''
    for d in inc_env.split(':'):
        if d and os.path.isdir(d):
            search_dirs.append(d)

    visited = set()
    if sno_path:
        visited.add(os.path.abspath(sno_path))

    _scan_lines(lines, include_rules, exclude_rules, seen_fn, seen_var,
                functions, variables, search_dirs, visited)
    return functions, variables


# ---------------------------------------------------------------------------
# Monitor preamble — binary-protocol version.
# ---------------------------------------------------------------------------
#
# Compared with the text-protocol preamble:
#   - LOADs MON_PUT_VALUE / MON_PUT_CALL / MON_PUT_RETURN (typed second arg)
#     instead of MON_SEND (string-only).
#   - The SNOBOL4 callbacks pass $N (the variable's value descriptor) as
#     a typed argument.  Beware: SPITBOL's LOAD() rejects 'STRING' for a
#     non-string argument value.  We therefore declare the second arg of
#     MON_PUT_VALUE / MON_PUT_RETURN as 'NUMERIC' (which both dialects
#     accept for a wide range of types) — but actually, neither dialect's
#     LOAD enforces argument types at call time (see CSNOBOL4 lib/load.h
#     comment "XXX check nargs?? check datatypes???").  So we declare the
#     prototype as STRING (the most common case) and rely on the C side to
#     inspect args[1].v at runtime.
#
# &TRACE = 16000000  (SPITBOL max ~16M; CSNOBOL4 accepts any).
# &STLIMIT = 5000000 (avoid premature stop on long beauty runs).

MONITOR_PREAMBLE = """\
* --- MONITOR PREAMBLE (BINARY): injected by inject_traces_bin.py ---
*     Sync-step IPC: MON_PUT_VALUE/CALL/RETURN block on go-pipe ack.
*     MONITOR_READY_PIPE   = ready FIFO (participant writes binary records)
*     MONITOR_GO_PIPE      = ack   FIFO (participant reads 'G' or 'S')
*     MONITOR_NAMES_FILE   = sidecar names table (one name per line)
*     MONITOR_SO           = monitor_ipc_bin_*.so path
        &TRACE         =  16000000
        &STLIMIT       =  5000000
*
        MON_READY_     =  HOST(4,'MONITOR_READY_PIPE')
        MON_GO_        =  HOST(4,'MONITOR_GO_PIPE')
        MON_NAMES_     =  HOST(4,'MONITOR_NAMES_FILE')
        MON_SO_        =  HOST(4,'MONITOR_SO')
*
*     Load IPC functions.  Fall through to no-op mode on any failure so a
*     program instrumented for monitoring still runs without the harness.
        IDENT(MON_SO_)                                     :S(MON_NOOP_)
        LOAD('MON_OPEN(STRING,STRING,STRING)INTEGER',       MON_SO_)   :F(MON_NOOP_)
        LOAD('MON_PUT_VALUE(STRING,STRING)INTEGER',         MON_SO_)   :F(MON_NOOP_)
        LOAD('MON_PUT_CALL(STRING)INTEGER',                 MON_SO_)   :F(MON_NOOP_)
        LOAD('MON_PUT_RETURN(STRING,STRING)INTEGER',        MON_SO_)   :F(MON_NOOP_)
        LOAD('MON_CLOSE()INTEGER',                          MON_SO_)   :F(MON_NOOP_)
        IDENT(MON_READY_)                                  :S(MON_NOOP_)
        IDENT(MON_NAMES_)                                  :S(MON_NOOP_)
        MON_OPEN(MON_READY_, MON_GO_, MON_NAMES_)          :F(MON_NOOP_)
        MON_ON_        =  '1'                              :(MON_DEFS_)
MON_NOOP_
        MON_ON_        =  ''
MON_DEFS_
*
*     Three thin callback functions.  They receive (name, value-of-$name)
*     from the TRACE() machinery and pass them straight to the C-side.
*     No CONVERT, no string concatenation, no IGNORE pre-filtering — the
*     C side reads the descriptor's type tag and raw bytes directly.
        DEFINE('MV(N,T)V')                                  :(MV_END_)
MV      IDENT(MON_ON_,'1')                                  :F(RETURN)
        MON_PUT_VALUE(N, $N)                                :S(RETURN)F(END)
MV_END_
        DEFINE('MC(N,T)')                                   :(MC_END_)
MC      IDENT(MON_ON_,'1')                                  :F(RETURN)
        MON_PUT_CALL(N)                                     :S(RETURN)F(END)
MC_END_
        DEFINE('MR(N,T)V')                                  :(MR_END_)
MR      IDENT(MON_ON_,'1')                                  :F(RETURN)
        MON_PUT_RETURN(N, $N)                               :S(RETURN)F(END)
MR_END_
* --- MONITOR PREAMBLE END ---
"""


def build_trace_registrations(functions, variables):
    """Emit TRACE() calls.  Each register the corresponding MV/MC/MR
    callback to fire on every assignment / call / return of the named
    item."""
    lines = ['* --- MONITOR: TRACE registrations (binary) ---\n']
    if functions:
        lines.append("        &FTRACE        =  16000000\n")
    for fn in functions:
        lines.append(f"        TRACE('{fn}',CALL,   '','MC')\n")
        lines.append(f"        TRACE('{fn}',RETURN, '','MR')\n")
    for var in variables:
        lines.append(f"        TRACE('{var}',VALUE,  '','MV')\n")
    lines.append('* --- MONITOR: end TRACE registrations ---\n')
    return lines


# ---------------------------------------------------------------------------
# Names sidecar file emission.
# ---------------------------------------------------------------------------

def emit_names_file(path, all_names):
    """Write names table — one name per line.  C side mmap()s on MON_OPEN.
    The order here defines the name_id used on the wire."""
    with open(path, 'w') as f:
        for n in all_names:
            f.write(n)
            f.write('\n')


# ---------------------------------------------------------------------------
# Emit instrumented .sno.
# ---------------------------------------------------------------------------

def emit_instrumented(lines, functions, variables, out):
    # Split: everything up to and including last -directive goes first
    split = 0
    for i, line in enumerate(lines):
        if line.strip().startswith('-'):
            split = i + 1

    for line in lines[:split]:
        out.write(line)

    out.write(MONITOR_PREAMBLE)

    if functions or variables:
        for reg_line in build_trace_registrations(functions, variables):
            out.write(reg_line)

    for line in lines[split:]:
        out.write(line)


# ---------------------------------------------------------------------------
# Main.
# ---------------------------------------------------------------------------

def main():
    args     = sys.argv[1:]
    names_out = None
    rest = []
    for a in args:
        if a.startswith('--names-out='):
            names_out = a.split('=', 1)[1]
        else:
            rest.append(a)

    if len(rest) < 1:
        print('Usage: inject_traces_bin.py <sno_file> [tracepoints_conf] [--names-out=PATH]',
              file=sys.stderr)
        sys.exit(1)

    sno_path  = rest[0]
    conf_path = rest[1] if len(rest) > 1 else \
                os.path.join(os.path.dirname(__file__), 'tracepoints_bin.conf')

    include_rules, exclude_rules = load_conf(conf_path)

    with open(sno_path) as f:
        lines = f.readlines()

    functions, variables = scan_sno(lines, include_rules, exclude_rules,
                                    sno_path=sno_path)

    # Names file: variables first, then functions.  Order defines name_id.
    # The TRACE() registration order does NOT need to match — name_id
    # lookups in the .so are content-keyed, not order-keyed.
    all_names = list(variables) + list(functions)

    if names_out:
        emit_names_file(names_out, all_names)
    else:
        # Write to a side temp file and report path to stderr line 1.
        import tempfile
        tf = tempfile.NamedTemporaryFile(mode='w', delete=False, suffix='.names')
        for n in all_names:
            tf.write(n); tf.write('\n')
        tf.close()
        print(f'NAMES_FILE={tf.name}', file=sys.stderr)

    emit_instrumented(lines, functions, variables, sys.stdout)


if __name__ == '__main__':
    main()
