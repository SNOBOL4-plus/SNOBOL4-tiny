// treebank-prepend.sc — Penn Treebank s-expression parser (LISP-style, Snocone)
// ENG 685, Lon Cherryholmes Sr.
// Run: scrip --ir-run treebank-prepend.sc < VBGinTASA.dat
//
// LISP-style cons-list: children PREPENDED to head, list_reverse corrects
// order at pop time.  Mirrors treebank-prepend.sno exactly.
// Compare: treebank-append.sc (TABLE-based, append at end, no reversal).
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

//--- Linked list DATA type --------------------------------------------------

DATA('list(head,tail)');

//--- list_reverse(lst) -------------------------------------------------------

procedure list_reverse(lst,   acc, cur) {
    acc = '';
    cur = lst;
    lr1:
    if (~DIFFER(cur)) { goto lr_done; }
    acc = list(head(cur), acc);
    cur = tail(cur);
    goto lr1;
    lr_done:
    list_reverse = acc;
    return;
}

//--- Stack primitives --------------------------------------------------------

procedure stk_push_frame(v) {
    stk            = list(list(v, ''), stk);
    stk_push_frame = .dummy;
    return;
}

procedure stk_push_item(v) {
    head(stk)      = list(v, head(stk));
    stk_push_item  = .dummy;
    return;
}

procedure stk_pop_into_parent(  child) {
    child           = list_reverse(head(stk));
    stk             = tail(stk);
    head(stk)       = list(child, head(stk));
    stk_pop_into_parent = .dummy;
    return;
}

procedure stk_pop_final(var) {
    $var           = list_reverse(head(stk));
    stk            = tail(stk);
    stk_pop_final  = .dummy;
    return;
}

//--- init_list(v) — λ(f"{v}=None") + λ("stack=[]") -------------------------

procedure init_list(v) {
    $v         = '';
    stk        = '';
    init_list  = .dummy;
    nreturn;
}

//--- push_list(v) — λ("stack.append(list())") + λ("stack[-1].append(v)") ---
//    Called as: (word . tag) . *push_list(tag)  or  epsilon . *push_list('BANK')

procedure push_list(v) {
    dummy     = stk_push_frame(v);
    push_list = .dummy;
    nreturn;
}

//--- push_item(v) — λ("stack[-1].append(v)") --------------------------------
//    Called as: (word . wrd) . *push_item(wrd)

procedure push_item(v) {
    dummy     = stk_push_item(v);
    push_item = .dummy;
    nreturn;
}

//--- pop_list() — λ("stack[-2].append(tuple(stack.pop()))") -----------------

procedure pop_list() {
    dummy    = stk_pop_into_parent();
    pop_list = .dummy;
    nreturn;
}

//--- pop_final(v) — λ(f"{v}=tuple(stack.pop())") ----------------------------

procedure pop_final(v) {
    dummy     = stk_pop_final(v);
    pop_final = .dummy;
    nreturn;
}

//--- Sub-patterns ------------------------------------------------------------

delim  = SPAN(' ' && nl);
word   = NOTANY('( )' && nl) && BREAK('( )' && nl);

//--- group — direct port of Python:
//    σ('(') + word%"tag" + push_list(tag)
//    + ARBNO(delim + (ζ(λ:group) | word%"wrd" + push_item(wrd)))
//    + pop_list() + σ(')')

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

//--- treebank — direct port of Python:
//    POS(0) + init_list("bank") + push_list("'BANK'")
//    + ARBNO(push_list("'ROOT'") + ARBNO(group) + pop_list() + delim)
//    + pop_final("bank") + RPOS(0)

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

//--- pp_node: recursive pretty-printer --------------------------------------

procedure pp_node(node, indent,   child, nxt, pad) {
    pad = DUPL(' ', indent);
    if (IDENT(REPLACE(DATATYPE(node), &LCASE, &UCASE), 'STRING')) {
        OUTPUT = pad && node;
        return;
    }
    OUTPUT = pad && '(' && head(node);
    child  = tail(node);
    pp_ch:
    if (~DIFFER(child)) { goto pp_close; }
    nxt   = tail(child);
    dummy = pp_node(head(child), indent + 3);
    child = nxt;
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
