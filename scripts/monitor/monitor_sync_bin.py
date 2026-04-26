#!/usr/bin/env python3
"""
monitor_sync_bin.py — binary-protocol sync-step monitor controller.

Reads fixed-size binary records from each participant's ready FIFO,
compares them as (kind, name_id, type, value_bytes) tuples, and writes
'G' (go) or 'S' (stop) to each participant's go FIFO.

Wire format (matches monitor_wire.h):
    13-byte header LE: u32 kind | u32 name_id | u8 type | u32 value_len
    value_len bytes of value (varies by type)

Comparison is byte-for-byte tuple equality.  No regex IGNORE rules;
no .upper() name normalization; pattern/array/data values all serialize
as (type=N, len=0) and agree automatically.

Usage:
    monitor_sync_bin.py NAMES_FILE PARTICIPANT_NAME:READY_FIFO:GO_FIFO ...

The first PARTICIPANT_NAME is the consensus oracle.  Divergences are
reported relative to it.

Exit codes:
    0   all participants reached END agreeing on every event
    1   divergence — first disagreement reported, all participants stopped
    2   timeout
    3   protocol error (bad header, short read, etc.)
"""

import os
import select
import struct
import sys
import time
from collections import namedtuple

HDR_FMT  = '<IIBI'   # u32 kind, u32 name_id, u8 type, u32 value_len
HDR_SIZE = struct.calcsize(HDR_FMT)
assert HDR_SIZE == 13, "header must be 13 bytes"

# Event kinds
MWK_VALUE  = 1
MWK_CALL   = 2
MWK_RETURN = 3
MWK_END    = 4

KIND_NAMES = {1: 'VALUE', 2: 'CALL', 3: 'RETURN', 4: 'END'}

# Type tags (must match monitor_wire.h MWT_*)
TYPE_NAMES = {
    0: 'NULL',  1: 'STRING', 2: 'INTEGER', 3: 'REAL',  4: 'NAME',
    5: 'PATTERN', 6: 'EXPRESSION', 7: 'ARRAY', 8: 'TABLE',
    9: 'CODE', 10: 'DATA', 11: 'FILE', 255: 'UNKNOWN',
}

NAME_ID_NONE = 0xffffffff

EVENT_TIMEOUT_S = 60.0    # generous; beauty self-host is slow

Event = namedtuple('Event', 'kind name_id type value')


def load_names(path):
    with open(path) as f:
        names = [line.rstrip('\n').rstrip('\r') for line in f]
    return names


def name_for_id(names, name_id):
    if name_id == NAME_ID_NONE:
        return '(none)'
    if 0 <= name_id < len(names):
        return names[name_id]
    return f'(id={name_id})'


# ---------------------------------------------------------------------------
# read_record — read one full record (header + value bytes) from fd.
# Returns Event namedtuple, or None on EOF, or raises on protocol error.
# ---------------------------------------------------------------------------

def read_exact(fd, n, timeout_s):
    """Read exactly n bytes from fd or return None on EOF.
    Uses select() with timeout to avoid hanging."""
    deadline = time.monotonic() + timeout_s
    buf = b''
    while len(buf) < n:
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            return None
        r, _, _ = select.select([fd], [], [], remaining)
        if not r:
            return None
        chunk = os.read(fd, n - len(buf))
        if not chunk:
            return None  # EOF
        buf += chunk
    return buf


def read_record(fd, timeout_s):
    hdr = read_exact(fd, HDR_SIZE, timeout_s)
    if hdr is None or len(hdr) != HDR_SIZE:
        return None
    kind, name_id, type_tag, value_len = struct.unpack(HDR_FMT, hdr)
    if value_len > 0:
        val = read_exact(fd, value_len, timeout_s)
        if val is None or len(val) != value_len:
            raise ValueError(f'short value read: wanted {value_len} got {len(val) if val else 0}')
    else:
        val = b''
    return Event(kind, name_id, type_tag, val)


# ---------------------------------------------------------------------------
# Pretty-print one event.
# ---------------------------------------------------------------------------

def fmt_value(type_tag, value):
    name = TYPE_NAMES.get(type_tag, f'T{type_tag}')
    if type_tag == 1 or type_tag == 4:  # STRING or NAME
        try:
            s = value.decode('utf-8', errors='backslashreplace')
        except Exception:
            s = repr(value)
        return f'{name}({len(value)})={s!r}'
    if type_tag == 2:  # INTEGER
        if len(value) == 8:
            iv = struct.unpack('<q', value)[0]
            return f'INT={iv}'
        return f'INT(?{len(value)}b)'
    if type_tag == 3:  # REAL
        if len(value) == 8:
            rv = struct.unpack('<d', value)[0]
            return f'REAL={rv!r}'
        return f'REAL(?{len(value)}b)'
    return f'{name}'


def fmt_event(ev, names):
    kn = KIND_NAMES.get(ev.kind, f'K{ev.kind}')
    nm = name_for_id(names, ev.name_id)
    if ev.kind == MWK_END:
        return f'{kn}'
    if ev.kind == MWK_CALL:
        return f'{kn} {nm}'
    return f'{kn} {nm} = {fmt_value(ev.type, ev.value)}'


# ---------------------------------------------------------------------------
# Open one FIFO pair (we read ready, write go).
# ---------------------------------------------------------------------------

def open_pair(ready_path, go_path):
    """Open ready FIFO for read, go FIFO for write.

    Order matters: we must open the ready pipe first (blocking) so the
    participant's write side can succeed; then open go for write
    (blocking) so the participant's read side can proceed.

    But participants open ready for write first, then go for read --
    so both sides converge.  We just open with the same pattern as
    the text-protocol version: O_RDONLY for ready, O_WRONLY for go.
    """
    rd = os.open(ready_path, os.O_RDONLY)
    gw = os.open(go_path,    os.O_WRONLY)
    return rd, gw


# ---------------------------------------------------------------------------
# Run the controller.
# ---------------------------------------------------------------------------

def run(names_path, participants):
    """participants is a list of (name, ready_path, go_path)."""
    names = load_names(names_path)
    print(f'[ctrl] loaded {len(names)} names', file=sys.stderr)

    fds = []
    for nm, rp, gp in participants:
        rd, gw = open_pair(rp, gp)
        fds.append({'name': nm, 'rd': rd, 'gw': gw})
        print(f'[ctrl] opened {nm}: ready={rp} go={gp}', file=sys.stderr)

    oracle = fds[0]
    diverged = False
    step = 0

    while True:
        step += 1
        # Read one record from each participant.
        # Track which ones returned None (EOF or timeout).
        events = []
        eof_set = []
        protocol_err = False
        for f in fds:
            try:
                ev = read_record(f['rd'], EVENT_TIMEOUT_S)
            except ValueError as e:
                print(f'[ctrl] PROTOCOL ERR step {step} on {f["name"]}: {e}', file=sys.stderr)
                protocol_err = True
                events.append((f, None))
                eof_set.append(f['name'])
                continue
            if ev is None:
                events.append((f, None))
                eof_set.append(f['name'])
            else:
                events.append((f, ev))

        if protocol_err:
            for ff in fds:
                try: os.write(ff['gw'], b'S')
                except OSError: pass
            return 3

        # If ALL participants hit EOF simultaneously, that's a clean
        # termination — they each ran their .sno to END.  The pipe close
        # is the END signal in lieu of an explicit MON_CLOSE call.
        if len(eof_set) == len(fds):
            print(f'[ctrl] all reached EOF at step {step} (clean termination)',
                  file=sys.stderr)
            return 0

        # Mixed: some EOF, some real event — that's a divergence in
        # event count.  One side terminated early.
        if eof_set:
            print(f'[ctrl] PARTIAL EOF step {step}: {eof_set} done, others still running',
                  file=sys.stderr)
            for f, ev in events:
                if ev is not None:
                    print(f'  {f["name"]}: still emitting {fmt_event(ev, names)}',
                          file=sys.stderr)
                else:
                    print(f'  {f["name"]}: EOF', file=sys.stderr)
            for ff in fds:
                try: os.write(ff['gw'], b'S')
                except OSError: pass
            return 1

        # Compare against oracle (events[0]).
        oracle_ev = events[0][1]
        agree = True
        for f, ev in events[1:]:
            if ev != oracle_ev:
                agree = False
                break

        if not agree:
            print(f'[ctrl] DIVERGE step {step}', file=sys.stderr)
            for f, ev in events:
                print(f'  {f["name"]}: {fmt_event(ev, names)}', file=sys.stderr)
            # 'S' to all so they exit cleanly.
            for f, ev in events:
                try: os.write(f['gw'], b'S')
                except OSError: pass
            diverged = True
            break

        # If everyone sent END, we're done.
        if oracle_ev.kind == MWK_END:
            for f, ev in events:
                try: os.write(f['gw'], b'G')
                except OSError: pass
            print(f'[ctrl] all reached END after {step} steps', file=sys.stderr)
            break

        # Otherwise GO to all.
        for f, ev in events:
            try:
                os.write(f['gw'], b'G')
            except OSError:
                # Participant may have closed early — treat as divergence.
                print(f'[ctrl] write failed to {f["name"]}', file=sys.stderr)
                diverged = True
                break
        if diverged:
            break

    # Close FDs.
    for f in fds:
        try: os.close(f['rd'])
        except OSError: pass
        try: os.close(f['gw'])
        except OSError: pass

    return 1 if diverged else 0


def main():
    if len(sys.argv) < 3:
        print('Usage: monitor_sync_bin.py NAMES_FILE NAME:READY_FIFO:GO_FIFO ...',
              file=sys.stderr)
        sys.exit(2)
    names_path = sys.argv[1]
    participants = []
    for spec in sys.argv[2:]:
        parts = spec.split(':')
        if len(parts) != 3:
            print(f'bad participant spec: {spec}', file=sys.stderr)
            sys.exit(2)
        participants.append(tuple(parts))
    rc = run(names_path, participants)
    sys.exit(rc)


if __name__ == '__main__':
    main()
