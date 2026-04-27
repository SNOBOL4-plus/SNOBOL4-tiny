#!/usr/bin/env python3
"""
monitor_sync_bin.py — binary-protocol sync-step monitor controller.

Reads fixed-size binary records from each participant's ready FIFO,
compares them as (kind, name_string, type, value_bytes) tuples, and
writes 'G' (go) or 'S' (stop) to each participant's go FIFO.

Wire format (matches monitor_wire.h):
    13-byte header LE: u32 kind | u32 name_id | u8 type | u32 value_len
    value_len bytes of value (varies by type)

SN-26-bridge-coverage-e — streaming intern on the wire.

Names are NOT loaded from a sidecar file.  Participants emit MWK_NAME_DEF
records inline before any record using a fresh name_id.  The controller
maintains a per-participant intern table populated from those NAME_DEF
records.  NAME_DEFs are acked with 'G' like any other record but are
not surfaced as semantic events for divergence comparison — different
participants may assign the same name different ids without diverging,
since comparison is on the resolved name string, not the id.

CLI shape (single, simple):

    monitor_sync_bin.py NAME:READY_FIFO:GO_FIFO ...

The first PARTICIPANT is the consensus oracle.  Divergences are reported
relative to it.

Exit codes:
    0   all participants reached END agreeing on every event
    1   divergence — first disagreement reported, all participants stopped
    2   timeout / bad CLI
    3   protocol error (bad header, short read, etc.)
"""

import os
import struct
import sys
import time
from collections import namedtuple

HDR_FMT  = '<IIBI'   # u32 kind, u32 name_id, u8 type, u32 value_len
HDR_SIZE = struct.calcsize(HDR_FMT)
assert HDR_SIZE == 13, "header must be 13 bytes"

# Event kinds — keep aligned with monitor_wire.h MWK_*
MWK_VALUE     = 1
MWK_CALL      = 2
MWK_RETURN    = 3
MWK_END       = 4
MWK_LABEL     = 5
MWK_NAME_DEF  = 6

KIND_NAMES = {
    1: 'VALUE', 2: 'CALL', 3: 'RETURN', 4: 'END',
    5: 'LABEL', 6: 'NAME_DEF',
}

# Type tags (must match monitor_wire.h MWT_*)
TYPE_NAMES = {
    0: 'NULL',  1: 'STRING', 2: 'INTEGER', 3: 'REAL',  4: 'NAME',
    5: 'PATTERN', 6: 'EXPRESSION', 7: 'ARRAY', 8: 'TABLE',
    9: 'CODE', 10: 'DATA', 11: 'FILE', 255: 'UNKNOWN',
}

NAME_ID_NONE = 0xffffffff

EVENT_TIMEOUT_S = 60.0    # generous; beauty self-host is slow

# Raw record off the wire — name_id is dialect-local until resolved.
Event = namedtuple('Event', 'kind name_id type value')


def name_for_id(names_table, name_id):
    """Resolve a name_id against a participant's intern table.

    names_table is a dict {id -> bytes}. NAME_ID_NONE -> '' (used for END/LABEL).
    Unknown ids surface as '(id=N)' so downstream comparison still has a stable
    string — should not happen with well-formed wire output.
    """
    if name_id == NAME_ID_NONE:
        return ''
    nm = names_table.get(name_id)
    if nm is None:
        return f'(id={name_id})'
    try:
        return nm.decode('utf-8', errors='backslashreplace')
    except Exception:
        return repr(nm)


# ---------------------------------------------------------------------------
# read_record — read one full record (header + value bytes) from fd.
# ---------------------------------------------------------------------------

def read_exact(fd, n, timeout_s):
    """Read exactly n bytes from fd or return None on EOF."""
    deadline = time.monotonic() + timeout_s
    buf = b''
    while len(buf) < n:
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            return None
        try:
            chunk = os.read(fd, n - len(buf))
        except BlockingIOError:
            time.sleep(0.001)
            continue
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


def read_semantic_record(f, timeout_s):
    """Read records from participant f until a non-NAME_DEF record is seen.

    ONLY NAME_DEF records are absorbed here — they are wire-protocol
    bookkeeping (binding name_id -> name bytes) and carry no semantics
    of their own.  Every other record kind (VALUE, CALL, RETURN, END,
    LABEL) is returned to the caller for sync-step comparison.

    LABEL records are EXPLICITLY comparison-eligible: a LABEL divergence
    means the runtimes entered different statements (different STNO),
    which is a structural-flow bug that must surface immediately.  Do
    not extend this absorption loop to LABEL or any future "informational"
    kind without an explicit goal-level decision — silently filtering
    LABEL would hide exactly the class of bug the monitor exists to
    catch (control-flow disagreement before any value disagreement).

    NAME_DEF records are acked with 'G' here so the participant can
    continue; the participant still cannot run ahead of the controller
    because each ack is one-record-at-a-time.

    Returns Event or None on EOF.  Raises ValueError on protocol error.
    """
    while True:
        ev = read_record(f['rd'], timeout_s)
        if ev is None:
            return None
        if ev.kind != MWK_NAME_DEF:
            return ev
        # Streaming intern: register binding, ack, loop for next record.
        f['names'][ev.name_id] = ev.value
        try:
            os.write(f['gw'], b'G')
        except OSError:
            return None  # participant closed early


# ---------------------------------------------------------------------------
# Resolve an Event to a comparable tuple using a per-participant names dict.
#
# All non-NAME_DEF kinds participate in this comparison — including LABEL.
# A LABEL divergence (same step, different STNO) means the runtimes are
# executing different statements; that is a real structural-flow bug,
# usually a control-flow disagreement upstream.  Do not be tempted to
# filter LABELs out of the comparison to "reach" a value divergence —
# the LABEL divergence IS the divergence.
# ---------------------------------------------------------------------------

def event_key(ev, names_table):
    """Return (kind, name_string, type, value_bytes) — the comparable tuple."""
    if ev is None:
        return None
    return (ev.kind, name_for_id(names_table, ev.name_id), ev.type, ev.value)


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


def fmt_event(ev, names_table):
    kn = KIND_NAMES.get(ev.kind, f'K{ev.kind}')
    nm = name_for_id(names_table, ev.name_id) or '(none)'
    if ev.kind == MWK_END:
        return f'{kn}'
    if ev.kind == MWK_CALL:
        return f'{kn} {nm}'
    if ev.kind == MWK_LABEL:
        return f'{kn} stno={fmt_value(ev.type, ev.value)}'
    return f'{kn} {nm} = {fmt_value(ev.type, ev.value)}'


# ---------------------------------------------------------------------------
# Open one FIFO pair (we read ready, write go).
# ---------------------------------------------------------------------------

def open_pair(ready_path, go_path):
    """Open ready FIFO for read, go FIFO for write."""
    rd = os.open(ready_path, os.O_RDONLY)
    gw = os.open(go_path,    os.O_WRONLY)
    return rd, gw


# ---------------------------------------------------------------------------
# Run the controller.
# ---------------------------------------------------------------------------

def run(participants):
    """participants: list of (name, ready_path, go_path)."""
    fds = []
    for nm, rp, gp in participants:
        rd, gw = open_pair(rp, gp)
        fds.append({'name': nm, 'rd': rd, 'gw': gw, 'names': {}})
        print(f'[ctrl] opened {nm}: ready={rp} go={gp}', file=sys.stderr)

    diverged = False
    step = 0

    while True:
        step += 1
        # Read one semantic record from each participant.  read_semantic_record
        # absorbs NAME_DEFs internally (acks them, registers bindings).
        events = []
        eof_set = []
        protocol_err = False
        for f in fds:
            try:
                ev = read_semantic_record(f, EVENT_TIMEOUT_S)
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

        # All EOF: clean termination (legacy path, when a runtime exits without
        # emitting MWK_END).
        if len(eof_set) == len(fds):
            print(f'[ctrl] all reached EOF at step {step} (clean termination)',
                  file=sys.stderr)
            return 0

        # Mixed EOF: divergence in event count.
        if eof_set:
            print(f'[ctrl] PARTIAL EOF step {step}: {eof_set} done, others still running',
                  file=sys.stderr)
            for f, ev in events:
                if ev is not None:
                    print(f'  {f["name"]}: still emitting {fmt_event(ev, f["names"])}',
                          file=sys.stderr)
                else:
                    print(f'  {f["name"]}: EOF', file=sys.stderr)
            for ff in fds:
                try: os.write(ff['gw'], b'S')
                except OSError: pass
            return 1

        # Compare against oracle (events[0]) using per-participant name resolution.
        oracle_f, oracle_ev = events[0]
        oracle_key = event_key(oracle_ev, oracle_f['names'])
        agree = True
        for f, ev in events[1:]:
            if event_key(ev, f['names']) != oracle_key:
                agree = False
                break

        if not agree:
            print(f'[ctrl] DIVERGE step {step}', file=sys.stderr)
            for f, ev in events:
                print(f'  {f["name"]}: {fmt_event(ev, f["names"])}', file=sys.stderr)
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


def parse_argv(argv):
    """Return participants or exit(2).

    Spec: NAME:READY:GO  (one per participant).  No sidecar/names paths —
    streaming intern means names live on the wire.
    """
    if len(argv) < 2:
        print('Usage: monitor_sync_bin.py NAME:READY:GO ...', file=sys.stderr)
        sys.exit(2)

    participants = []
    for spec in argv[1:]:
        parts = spec.split(':')
        if len(parts) != 3:
            print(f'bad participant spec (expect NAME:READY:GO): {spec}',
                  file=sys.stderr)
            sys.exit(2)
        participants.append(tuple(parts))
    return participants


def main():
    participants = parse_argv(sys.argv)
    rc = run(participants)
    sys.exit(rc)


if __name__ == '__main__':
    main()
