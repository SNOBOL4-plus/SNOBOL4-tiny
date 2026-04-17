// treebank-append.sc — Penn Treebank s-expression parser (append style, Snocone)
// ENG 685, Lon Cherryholmes Sr.
// Run: scrip --ir-run treebank-append.sc < VBGinTASA.dat
//
// TABLE-based implementation: each frame is a TABLE used as a dynamic array.
// Children are APPENDED at the end (integer keys 1..n).  No reversal needed.
// Mirrors Python list.append semantics and treebank-append.sno exactly.
// Compare: treebank-prepend.sc (LISP-style cons-list with reversal).
//
// Function names match Python exactly:
//   init_list(v)  push_list(v)  push_item(v)  pop_list()  pop_final(v)
//
// Each is a single nreturn procedure usable two ways:
//   (word . tag) . *push_list(tag)    — capture then push at match time
//   (epsilon . *push_list('BANK'))    — zero-width hook with literal

&TRIM     = 1;
&ANCHOR   = 0;
&FULLSCAN = 1;
&ALPHABET POS(10) LEN(1) . nl;

//--- Stack frame storage (TABLE-of-TABLEs, integer-keyed children) ----------
//
//  stk_tag[frame_id]       — tag string for this frame
//  stk_n[frame_id]         — current child count
//  stk_c[frame_id][1..n]   — children in left-to-right parse order
//  stk                     — cons-list of open frame_ids (the parse stack)
//  frame_id                — monotone counter, unique per push_list call

DATA('list(head,tail)');

//--- stk_push_frame(v) -------------------------------------------------------

procedure stk_push_frame(v) {
    frame_id         = frame_id + 1;
    stk_tag[frame_id] = v;
    stk_n[frame_id]  = 0;
    stk_c[frame_id]  = TABLE();
    stk              = list(frame_id, stk);
    stk_push_frame   = .dummy;
    return;
}

//--- stk_push_item(v) --------------------------------------------------------

procedure stk_push_item(v,   cur) {
    cur              = head(stk);
    stk_n[cur]       = stk_n[cur] + 1;
    stk_c[cur][stk_n[cur]] = v;
    stk_push_item    = .dummy;
    return;
}

//--- stk_pop_into_parent() ---------------------------------------------------
//  Pop current frame; append its frame_id as a child of the parent frame.

procedure stk_pop_into_parent(   child, par, n) {
    child            = head(stk);
    stk              = tail(stk);
    par              = head(stk);
    n                = stk_n[par];
    stk_n[par]       = n + 1;
    stk_c[par][n + 1] = child;
    stk_pop_into_parent = .dummy;
    return;
}

//--- stk_pop_final(var) ------------------------------------------------------
//  Pop the outermost frame; store its frame_id in $var.

procedure stk_pop_final(var,   child) {
    child            = head(stk);
    stk              = tail(stk);
    $var             = child;
    stk_pop_final    = .dummy;
    return;
}

//--- pp_node(f, indent) — walk the frame TABLE directly, no conversion -------

procedure pp_node(f, indent,   tag, n, i, pad) {
    pad = DUPL(' ', indent);
    if (IDENT(REPLACE(DATATYPE(f), &LCASE, &UCASE), 'STRING')) {
        OUTPUT = pad && f;
        return;
    }
    tag    = stk_tag[f];
    n      = stk_n[f];
    OUTPUT = pad && '(' && tag;
    i      = 0;
    pp_ch:
    i = LT(i, n) i + 1;
    if (~DIFFER(i)) { goto pp_close; }
    dummy = pp_node(stk_c[f][i], indent + 3);
    goto pp_ch;
    pp_close:
    OUTPUT = pad && ')';
    return;
}

procedure pp_bank() {
    dummy   = pp_node(bank, 0);
    pp_bank = .dummy;
    return;
}

//--- Five helper procedures — mirror Python exactly, no reversal -------------

//  init_list(v): $v = ''; reset frame_id, stk, stk_tag, stk_n, stk_c
procedure init_list(v) {
    $v       = '';
    stk      = '';
    frame_id = 0;
    stk_tag  = TABLE();
    stk_n    = TABLE();
    stk_c    = TABLE();
    init_list = .dummy;
    nreturn;
}

//  push_list(v): open a new frame with tag v
procedure push_list(v) {
    dummy     = stk_push_frame(v);
    push_list = .dummy;
    nreturn;
}

//  push_item(v): append v as next child of current frame
procedure push_item(v) {
    dummy     = stk_push_item(v);
    push_item = .dummy;
    nreturn;
}

//  pop_list(): close current frame, append its id into parent frame
procedure pop_list() {
    dummy    = stk_pop_into_parent();
    pop_list = .dummy;
    nreturn;
}

//  pop_final(v): close outermost frame, store frame_id in $v
procedure pop_final(v) {
    dummy     = stk_pop_final(v);
    pop_final = .dummy;
    nreturn;
}

//--- Sub-patterns ------------------------------------------------------------

delim  = SPAN(' ' && nl);
word   = NOTANY('( )' && nl) && BREAK('( )' && nl);

//--- group — direct port of Python and treebank-append.sno ------------------

group =
    '('
    && (word . tag) . *push_list(tag)
    && ARBNO(
        *delim
        && ( *group
           | (word . wrd) . *push_item(wrd)
           )
    )
    && (epsilon . *pop_list())
    && ')';

//--- treebank — direct port of Python and treebank-append.sno ---------------

treebank =
    POS(0)
    && (epsilon . *init_list('bank'))
    && (epsilon . *push_list('BANK'))
    && ARBNO(
        (epsilon . *push_list('ROOT'))
        && ARBNO(*group)
        && *delim
        && (epsilon . *pop_list())
    )
    && (epsilon . *pop_final('bank'))
    && RPOS(0);

//--- Main: slurp stdin, match, print ----------------------------------------

src = '';
slurp:
line = INPUT;
if (~DIFFER(line)) { goto slurp_done; }
src = src && line && nl;
goto slurp;
slurp_done:
if (src ? treebank) { goto matched; }
OUTPUT = 'Pattern match failed';
goto END;
matched:
dummy = pp_bank();
END:
