// driver.sc — test driver for match.sc (Snocone)
// Oracle: compare to beauty_match_driver.ref

procedure match(subject, pattern) {
    match = .dummy;
    if (subject ? pattern) { nreturn; } else { freturn; }
}

procedure notmatch(subject, pattern) {
    notmatch = .dummy;
    if (subject ? pattern) { freturn; } else { nreturn; }
}

&STLIMIT = 1000000;

if (match('hello', ANY('aeiou'))) { OUTPUT = 'PASS: 1 match ANY hit'; } else { OUTPUT = 'FAIL: 1 match ANY hit'; }
if (~match('xyz', ANY('aeiou'))) { OUTPUT = 'PASS: 2 match ANY miss'; } else { OUTPUT = 'FAIL: 2 match ANY miss'; }
if (notmatch('xyz', ANY('aeiou'))) { OUTPUT = 'PASS: 3 notmatch miss'; } else { OUTPUT = 'FAIL: 3 notmatch miss'; }
if (~notmatch('hello', ANY('aeiou'))) { OUTPUT = 'PASS: 4 notmatch hit'; } else { OUTPUT = 'FAIL: 4 notmatch hit'; }
if (match('   foo', SPAN(' '))) { OUTPUT = 'PASS: 5 match SPAN'; } else { OUTPUT = 'FAIL: 5 match SPAN'; }
if (match('anything', LEN(0))) { OUTPUT = 'PASS: 6 match LEN(0)'; } else { OUTPUT = 'FAIL: 6 match LEN(0)'; }
if (match('abc', RPOS(0))) { OUTPUT = 'PASS: 7 match RPOS(0)'; } else { OUTPUT = 'FAIL: 7 match RPOS(0)'; }

// 8: word-list hit — 'DEFINE LABEL END' contains 'LABEL' as a word
wList = 'DEFINE LABEL END';
tx = 'LABEL';
if (match(wList, tx)) { OUTPUT = 'PASS: 8 word list hit'; } else { OUTPUT = 'FAIL: 8 word list hit'; }

// 9: word-list miss — 'GOTO' is not in wList
if (notmatch(wList, 'GOTO')) { OUTPUT = 'PASS: 9 word list miss'; } else { OUTPUT = 'FAIL: 9 word list miss'; }
