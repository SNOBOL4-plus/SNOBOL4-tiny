#!/usr/bin/env python3
"""
beauty_subexpr_gen.py — Sub-expression oracle test generator for beauty drivers.

TECHNIQUE:
  For each beauty driver, take K sample points (stlimit values) spread across
  its execution. At each sample point, run the driver to stlimit=N (live
  context: globals, DATA structures, function defs all established), evaluate
  sub-expressions in that context under SPITBOL, capture oracle values, emit
  one .sno + .ref regression test per (driver, stlimit, sub-expression).

  No state restoration needed — the driver runs to stlimit=N and stops.
  Handles function calls, DATA structs, indirect vars, patterns — everything.

Usage:
    python3 test/beauty_subexpr_gen.py [options]
    python3 test/beauty_subexpr_gen.py --drivers counter omega --samples 10 --subs 8 --run --verbose
"""

import re, sys, os, subprocess, tempfile, argparse, random
from pathlib import Path

SBL    = '/home/claude/x64/bin/sbl'
SCRIP  = '/home/claude/one4all/scrip'
BEAUTY = Path('/home/claude/corpus/programs/snobol4/beauty')
OUT    = Path('/home/claude/corpus/programs/snobol4/subexpr')

# ─── Tokenizer ────────────────────────────────────────────────────────────────

TOKEN_RE = re.compile(
    r"('(?:[^']|'')*')"            # STR     g1
    r'|(\d+(?:\.\d+)?)'            # NUM     g2
    r'|([A-Za-z][A-Za-z0-9]*)'    # ID      g3
    r'|(\*\*|[+\-*/])'            # OP      g4
    r'|(\$)'                       # DREF    g5
    r'|(&[A-Za-z][A-Za-z0-9]*)'  # KW      g6
    r'|(\.[A-Za-z][A-Za-z0-9]*)' # NAMEREF g7
    r'|(@[A-Za-z][A-Za-z0-9]*)' # CURSOR  g8
    r'|(\()'                       # LP      g9
    r'|(\))'                       # RP      g10
    r'|(\,)'                       # COMMA   g11
    r'|([^\s])'                    # OTHER   g12
)

def tokenize(s):
    toks = []
    for m in TOKEN_RE.finditer(s):
        v = m.group(0)
        g = m.lastindex
        k = {1:'STR',2:'NUM',3:'ID',4:'OP',5:'DREF',6:'KW',
             7:'NAMEREF',8:'CURSOR',9:'LP',10:'RP',11:'COMMA'}.get(g,'OTHER')
        toks.append((k, v))
    return toks

# ─── Sub-expression recursive extractor ─────────────────────────────────────

class SubP:
    def __init__(self, toks):
        self.t = toks; self.i = 0; self.subs = []
    def peek(self): return self.t[self.i] if self.i < len(self.t) else ('EOF','')
    def eat(self):  r = self.t[self.i]; self.i += 1; return r
    def parse_top(self):
        parts = []
        while self.peek()[0] not in ('EOF','RP','COMMA'):
            if self.peek()[1] in (':',';'): break
            t = self._arith()
            if t is None: break
            parts.append(t)
        if len(parts) > 1: self.subs.append(' '.join(parts))
        return ' '.join(parts)
    def _arith(self):
        left = self._unary()
        if left is None: return None
        while self.peek()[0]=='OP' and self.peek()[1] in ('+','-','*','/','**'):
            op = self.eat()[1]; right = self._unary()
            if right is None: break
            left = f'{left} {op} {right}'; self.subs.append(left)
        return left
    def _unary(self):
        if self.peek()[0]=='OP' and self.peek()[1] in ('+','-'):
            op = self.eat()[1]; inner = self._primary()
            if inner is None: return None
            r = f'{op}{inner}'; self.subs.append(r); return r
        return self._primary()
    def _primary(self):
        k, v = self.peek()
        if k in ('STR','NUM','KW','NAMEREF','CURSOR'): self.eat(); return v
        if k == 'DREF':
            self.eat(); inner = self._primary()
            if inner is None: return None
            r = f'${inner}'; self.subs.append(r); return r
        if k == 'ID':
            self.eat(); name = v
            if self.peek()[0] == 'LP':
                self.eat(); args = []
                while self.peek()[0] not in ('RP','EOF'):
                    sp2 = SubP(self.t[self.i:])
                    arg = sp2.parse_top(); self.subs.extend(sp2.subs); self.i += sp2.i
                    args.append(arg)
                    if self.peek()[0]=='COMMA': self.eat()
                if self.peek()[0]=='RP': self.eat()
                r = f'{name}({", ".join(args)})'; self.subs.append(r); return r
            return name
        if k == 'LP':
            self.eat(); sp2 = SubP(self.t[self.i:])
            inner = sp2.parse_top(); self.subs.extend(sp2.subs); self.i += sp2.i
            if self.peek()[0]=='RP': self.eat()
            r = f'({inner})'; self.subs.append(r); return r
        return None

def extract_subexprs(expr_text):
    toks = tokenize(expr_text)
    p = SubP(toks); p.parse_top()
    seen = set(); result = []
    for s in p.subs:
        s = s.strip()
        if not s or s in seen: continue
        seen.add(s)
        if re.match(r"^'[^']*'$", s): continue   # bare string literal
        if re.match(r'^\d+$', s): continue        # bare integer
        if re.match(r'^&\w+$', s): continue       # bare keyword
        if re.match(r'^[A-Za-z]\w*$', s): continue  # bare variable name
        result.append(s)
    return result

# ─── Driver source reader ─────────────────────────────────────────────────────

GOTO_TAIL = re.compile(r'\s*:[SF(].*$')

def rhs_from_line(raw):
    body = re.sub(r'^[A-Za-z][A-Za-z0-9]*\s+', '', raw.strip(), count=1)
    depth = 0; in_str = False
    for idx, ch in enumerate(body):
        if ch == "'": in_str = not in_str
        elif not in_str:
            if ch == '(': depth += 1
            elif ch == ')': depth -= 1
            elif ch == '=' and depth == 0:
                return GOTO_TAIL.sub('', body[idx+1:]).strip()
    return None

def read_driver_subexprs(driver_path):
    """Return unique sub-expressions from a driver's assignment RHS."""
    lines = Path(driver_path).read_text(errors='replace').splitlines()
    stmts = []
    i = 0
    while i < len(lines):
        line = lines[i]; i += 1
        if not line.strip() or line.strip().startswith('*'): continue
        if re.match(r'^-INCLUDE|^\s*END\s*$', line.strip(), re.I): continue
        while i < len(lines) and lines[i].startswith('+'):
            line = line.rstrip() + ' ' + lines[i][1:].strip(); i += 1
        stmts.append(line)
    seen = set(); result = []
    for raw in stmts:
        rhs = rhs_from_line(raw)
        if not rhs: continue
        for s in extract_subexprs(rhs):
            if s not in seen: seen.add(s); result.append(s)
    return result

def build_driver_src(driver_path, beauty_dir):
    """
    Return driver source with -INCLUDEs expanded to full absolute paths.
    global.sno included exactly once at top.
    Strips &STLIMIT lines from driver body (we control stlimit externally).
    """
    src = Path(driver_path).read_text(errors='replace')
    lines_out = [f"-INCLUDE '{beauty_dir}/global.sno'"]
    for line in src.splitlines():
        ls = line.strip()
        # Skip global.sno include — already prepended
        if re.match(r"^-INCLUDE\s+'global\.sno'", ls, re.I): continue
        # Expand other includes to full path
        if re.match(r'^-INCLUDE', ls, re.I):
            m = re.search(r"'([^']+)'", line)
            if m:
                inc = Path(beauty_dir) / m.group(1)
                if inc.exists():
                    lines_out.append(f"-INCLUDE '{inc}'")
                    continue
        # Strip &STLIMIT lines (we set stlimit externally)
        if re.match(r'\s*&STLIMIT\s*=', ls, re.I): continue
        if re.match(r'^\s*END\s*$', line): continue
        lines_out.append(line)
    return '\n'.join(lines_out)

# ─── SPITBOL runner ───────────────────────────────────────────────────────────

def run_sbl(prog_text, timeout=15):
    with tempfile.NamedTemporaryFile(mode='w', suffix='.sno', delete=False,
                                     dir='/tmp', prefix='bsg_') as f:
        f.write(prog_text); fname = f.name
    try:
        r = subprocess.run([SBL, '-b', fname],
                           capture_output=True, text=True, timeout=timeout)
        return r.stdout
    except subprocess.TimeoutExpired:
        return None
    finally:
        try: os.unlink(fname)
        except: pass

def run_scrip(prog_text, timeout=15):
    with tempfile.NamedTemporaryFile(mode='w', suffix='.sno', delete=False,
                                     dir='/tmp', prefix='bsg_sc_') as f:
        f.write(prog_text); fname = f.name
    try:
        r = subprocess.run([SCRIP, '--ir-run', fname],
                           capture_output=True, text=True, timeout=timeout,
                           cwd='/home/claude/one4all')
        return r.stdout
    except subprocess.TimeoutExpired:
        return None
    finally:
        try: os.unlink(fname)
        except: pass

# ─── Step counter ─────────────────────────────────────────────────────────────

def count_stmts(driver_src):
    """Run driver_src fully; return &STCOUNT. Outer stlimit = 10M."""
    prog = (
        f"        &STLIMIT = 10000000\n"
        f"        &ERRLIMIT = 5\n"
        f"        &TRIM = 1\n"
        f"{driver_src}\n"
        f"        OUTPUT = 'XSTCOUNT' &STCOUNT 'XSTCOUNTEND'\n"
        f"END\n"
    )
    out = run_sbl(prog, timeout=30)
    if out is None: return 500
    m = re.search(r'XSTCOUNT(\d+)XSTCOUNTEND', out)
    return int(m.group(1)) if m else 500

# ─── Probe builder ────────────────────────────────────────────────────────────

MS = 'XBSGST'; ME = 'XBSGEND'; MF = 'XBSGFAIL'

def build_probe(driver_src, stlimit, subexpr):
    """One sub-expression per SPITBOL run — avoids side-effect contamination."""
    return (
        f"        &STLIMIT = {stlimit}\n"
        f"        &ERRLIMIT = 10\n"
        f"        &TRIM = 1\n"
        f"{driver_src}\n"
        f"        &STLIMIT = 10000000\n"
        f"        SNBv0 = ({subexpr})\n"
        f"        OUTPUT = '{MS}0|' SNBv0 '{ME}'  :(SNBn0)\n"
        f"SNBf0   OUTPUT = '{MF}0'\n"
        f"SNBn0\n"
        f"END\n"
    )

def probe_one(driver_src, stlimit, subexpr):
    """Run one probe, return oracle value or None."""
    prog = build_probe(driver_src, stlimit, subexpr)
    out  = run_sbl(prog)
    if out is None: return None
    pat = re.compile(re.escape(f'{MS}0|') + r'(.*?)' + re.escape(ME), re.DOTALL)
    m = pat.search(out)
    return m.group(1) if m else None

# ─── Test emitter ─────────────────────────────────────────────────────────────

def emit_test(out_dir, name, driver_src, stlimit, subexpr, oracle_val):
    safe_val = oracle_val.replace("'", "''")
    # Safe display of subexpr in comment only — never in a SNOBOL4 string literal
    sno = (
        f"* subexpr regression: {name}\n"
        f"* stlimit={stlimit}  expr={subexpr[:100]}\n"
        f"* oracle={repr(oracle_val)[:80]}\n"
        f"        &STLIMIT = {stlimit}\n"
        f"        &ERRLIMIT = 10\n"
        f"        &TRIM = 1\n"
        f"{driver_src}\n"
        f"        &STLIMIT = 10000000\n"
        f"        SNBgot = ({subexpr})\n"
        f"        IDENT(SNBgot, '{safe_val}')   :S(SNBpass)\n"
        f"        OUTPUT = 'FAIL'\n"
        f"        :(SNBend)\n"
        f"SNBpass OUTPUT = 'PASS'\n"
        f"SNBend\n"
        f"END\n"
    )
    Path(out_dir, f'{name}.sno').write_text(sno)
    Path(out_dir, f'{name}.ref').write_text('PASS\n')

# ─── Main generator ───────────────────────────────────────────────────────────

DRIVER_PAT = re.compile(r'beauty_(\w+)_driver\.sno$')

def generate(beauty_dir, out_dir, n_samples, n_subs, verbose, drivers=None, seed=42):
    beauty_dir = Path(beauty_dir); out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    rng = random.Random(seed)

    all_drivers = sorted(beauty_dir.glob('beauty_*_driver.sno'))
    if drivers:
        all_drivers = [d for d in all_drivers
                       if DRIVER_PAT.match(d.name).group(1) in drivers]

    grand = 0
    for dp in all_drivers:
        m = DRIVER_PAT.match(dp.name)
        if not m: continue
        subsys = m.group(1)
        ref = beauty_dir / f'beauty_{subsys}_driver.ref'
        if not ref.exists(): print(f'skip {subsys} — no .ref'); continue

        print(f'\n=== {subsys} ===')
        driver_src = build_driver_src(str(dp), beauty_dir)
        subexprs   = read_driver_subexprs(str(dp))
        print(f'  sub-expressions: {len(subexprs)}')
        if not subexprs: continue

        total = count_stmts(driver_src)
        print(f'  total stmts: {total}')

        # Sample points: skip first 10% (preamble/global init), spread rest
        start = max(10, total // 10)
        step  = max(1, (total - start) // (n_samples + 1))
        samples = [start + step * i for i in range(1, n_samples + 1)
                   if start + step * i < total]
        if not samples: samples = [total // 2]

        subsys_n = 0
        for sl in samples:
            pick = rng.sample(subexprs, min(n_subs, len(subexprs)))
            if verbose: print(f'  stlimit={sl}: {len(pick)} exprs')
            emitted = 0
            for i, sub in enumerate(pick):
                val = probe_one(driver_src, sl, sub)
                if val is None:
                    if verbose: print(f'    [{sub[:50]}] → no value')
                    continue
                name = f'{subsys}_sl{sl:06d}_s{i:02d}'
                emit_test(out_dir, name, driver_src, sl, sub, val)
                subsys_n += 1; emitted += 1
                if verbose: print(f'    EMIT {name}: [{sub[:50]}] = {repr(val)[:40]}')

        print(f'  → {subsys_n} tests')
        grand += subsys_n

    print(f'\nTotal: {grand} tests → {out_dir}')
    return grand

# ─── Runner ──────────────────────────────────────────────────────────────────

def run_suite(out_dir, verbose=False):
    tests = sorted(Path(out_dir).glob('*.sno'))
    passed = failed = errors = 0
    for sno in tests:
        ref = sno.with_suffix('.ref')
        if not ref.exists(): continue
        want = ref.read_text().strip()
        got_raw = run_scrip(sno.read_text())
        got = got_raw.strip() if got_raw else ''
        if got == want:
            passed += 1
            if verbose: print(f'PASS {sno.name}')
        elif got_raw is None:
            errors += 1
            print(f'TIMEOUT {sno.name}')
        else:
            failed += 1
            print(f'FAIL {sno.name}')
            if verbose: print(f'     want={want!r}  got={got!r}')
    print(f'--- PASS={passed} FAIL={failed} TIMEOUT={errors} TOTAL={passed+failed+errors}')
    return passed, failed

def main():
    ap = argparse.ArgumentParser(description=__doc__,
             formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--beauty',  default=str(BEAUTY))
    ap.add_argument('--out',     default=str(OUT))
    ap.add_argument('--samples', type=int, default=10)
    ap.add_argument('--subs',    type=int, default=8)
    ap.add_argument('--drivers', nargs='*', metavar='NAME')
    ap.add_argument('--run',     action='store_true', help='Also run tests under scrip')
    ap.add_argument('--verbose', '-v', action='store_true')
    ap.add_argument('--seed',    type=int, default=42)
    args = ap.parse_args()

    generate(args.beauty, args.out, args.samples, args.subs,
             args.verbose, args.drivers, args.seed)
    if args.run:
        print('\n--- scrip --ir-run ---')
        run_suite(args.out, args.verbose)

if __name__ == '__main__':
    main()
