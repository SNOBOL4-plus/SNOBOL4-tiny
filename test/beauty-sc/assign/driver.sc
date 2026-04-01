// driver.sc — test driver for assign.sc (Snocone)
// Oracle: compare output to test/beauty/assign/driver.ref (SNOBOL4 golden)

// inline assign.sc
procedure assign(name, expression) {
    assign = .dummy;
    if (IDENT(DATATYPE(expression), 'EXPRESSION')) {
        $name = EVAL(expression);
        nreturn;
    }
    $name = expression;
    nreturn;
}

&STLIMIT = 1000000;

// 1: basic string assign
assign('a', 'hello');
if (IDENT(a, 'hello')) {
    OUTPUT = 'PASS: 1 basic string assign';
} else {
    OUTPUT = 'FAIL: 1 basic string assign';
}

// 2: assign integer
assign('b', 99);
if (IDENT(b, 99)) {
    OUTPUT = 'PASS: 2 assign integer';
} else {
    OUTPUT = 'FAIL: 2 assign integer';
}

// 3: reassign (overwrite existing value)
assign('c', 'first');
assign('c', 'second');
if (IDENT(c, 'second')) {
    OUTPUT = 'PASS: 3 reassign';
} else {
    OUTPUT = 'FAIL: 3 reassign';
}

// 4: assign returns null (always succeeds, no value)
r = assign('d', 'val');
if (IDENT(r, '')) {
    OUTPUT = 'PASS: 4 assign returns null';
} else {
    OUTPUT = 'FAIL: 4 assign returns null';
}

// 5: assign empty string
assign('e', '');
if (IDENT(e, '')) {
    OUTPUT = 'PASS: 5 assign empty string';
} else {
    OUTPUT = 'FAIL: 5 assign empty string';
}

// 6: assign via indirect variable name
vname = 'myvar';
assign(vname, 'indirect');
if (IDENT(myvar, 'indirect')) {
    OUTPUT = 'PASS: 6 indirect varname';
} else {
    OUTPUT = 'FAIL: 6 indirect varname';
}

// 7: chain assign
assign('x', 'chain');
assign('y', x);
if (IDENT(y, 'chain')) {
    OUTPUT = 'PASS: 7 chain assign';
} else {
    OUTPUT = 'FAIL: 7 chain assign';
}
