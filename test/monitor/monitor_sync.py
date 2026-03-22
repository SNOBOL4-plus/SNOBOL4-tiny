#!/usr/bin/env python3
"""
monitor_sync.py — synchronous barrier-step monitor controller.

Each participant blocks after each trace event waiting for a 1-byte ack.
Controller reads one event from each of N event FIFOs, compares to oracle
(participant 0 = CSNOBOL4), sends 'G' (go) or 'S' (stop) to each ack FIFO.

Usage:
    monitor_sync.py <timeout> <names> <event_fifos> <ack_fifos>

    names       = comma-separated: csn,spl,asm,jvm,net
    event_fifos = comma-separated paths (same order as names)
    ack_fifos   = comma-separated paths (same order as names)

Exit 0 = all participants reached END agreeing with oracle.
Exit 1 = divergence detected (first diverging event printed).
Exit 2 = timeout or error.
"""

import sys
import os
import select
import time

def open_fifos(paths, mode, timeout=30):
    """Open FIFOs non-blocking for reading, blocking for writing."""
    fds = []
    for p in paths:
        if mode == 'r':
            fd = os.open(p, os.O_RDONLY | os.O_NONBLOCK)
            flags = fcntl_module.fcntl(fd, fcntl_module.F_GETFL)
            fcntl_module.fcntl(fd, fcntl_module.F_SETFL, flags & ~os.O_NONBLOCK)
        else:
            fd = os.open(p, os.O_WRONLY)
        fds.append(fd)
    return fds

import fcntl as fcntl_module

def main():
    if len(sys.argv) < 5:
        print("Usage: monitor_sync.py <timeout> <names> <event_fifos> <ack_fifos>")
        sys.exit(2)

    timeout    = float(sys.argv[1])
    names      = sys.argv[2].split(',')
    evt_paths  = sys.argv[3].split(',')
    ack_paths  = sys.argv[4].split(',')
    n          = len(names)

    assert len(evt_paths) == n and len(ack_paths) == n

    print(f"[sync monitor] opening {n} event FIFOs (read)...", flush=True)
    # Open event FIFOs read-side non-blocking (no writer yet — that's ok)
    evt_fds = []
    for p in evt_paths:
        fd = os.open(p, os.O_RDONLY | os.O_NONBLOCK)
        flags = fcntl_module.fcntl(fd, fcntl_module.F_GETFL)
        fcntl_module.fcntl(fd, fcntl_module.F_SETFL, flags & ~os.O_NONBLOCK)
        evt_fds.append(os.fdopen(fd, 'r', buffering=1))

    print(f"[sync monitor] opening {n} ack FIFOs (write)...", flush=True)
    ack_fds = []
    for p in ack_paths:
        fd = os.open(p, os.O_WRONLY)
        ack_fds.append(fd)

    print(f"[sync monitor] all FIFOs open — signalling ready", flush=True)
    # Signal ready to run_monitor_sync.sh via stdout line
    print("READY", flush=True)

    step      = 0
    alive     = list(range(n))  # indices of still-running participants
    bufs      = [''] * n        # line buffers per participant
    done      = [False] * n     # EOF seen

    while True:
        # Collect one complete line from each alive participant
        events = [None] * n
        deadline = time.monotonic() + timeout

        remaining = list(alive)
        while remaining:
            now = time.monotonic()
            if now >= deadline:
                for i in remaining:
                    print(f"TIMEOUT [{names[i]}] at step {step} — last event seen: {events[i]!r}")
                # Send STOP to everyone still waiting
                for i in alive:
                    if events[i] is not None:
                        try: os.write(ack_fds[i], b'S')
                        except: pass
                sys.exit(2)

            readable, _, _ = select.select(
                [evt_fds[i] for i in remaining], [], [],
                deadline - now)

            for fobj in readable:
                i = evt_fds.index(fobj)
                line = fobj.readline()
                if line == '':
                    # EOF — participant exited
                    done[i] = True
                    remaining.remove(i)
                    events[i] = '__EOF__'
                else:
                    events[i] = line.rstrip('\n')
                    remaining.remove(i)

        step += 1

        # Check: all EOF → done
        if all(done[i] or events[i] == '__EOF__' for i in alive):
            print(f"PASS — all {n} participants reached END after {step} steps")
            sys.exit(0)

        # Oracle is participant 0 (csn)
        oracle_event = events[0]

        diverged = []
        for i in alive:
            if events[i] != oracle_event and events[i] != '__EOF__':
                diverged.append(i)

        if diverged:
            print(f"\nDIVERGENCE at step {step}:")
            print(f"  oracle [{names[0]}]: {oracle_event!r}")
            for i in diverged:
                print(f"  FAIL   [{names[i]}]: {events[i]!r}")
            agree = [i for i in alive if i not in diverged and i != 0]
            if agree:
                print(f"  AGREE  [{','.join(names[i] for i in agree)}]: {oracle_event!r}")
            # Send STOP to all
            for i in alive:
                try: os.write(ack_fds[i], b'S')
                except: pass
            sys.exit(1)

        # All agree — send GO to all alive
        for i in alive:
            if not done[i]:
                try: os.write(ack_fds[i], b'G')
                except: pass

        # Remove finished participants
        alive = [i for i in alive if not done[i]]
        if not alive:
            print(f"PASS — all participants finished cleanly after {step} steps")
            sys.exit(0)

if __name__ == '__main__':
    main()
