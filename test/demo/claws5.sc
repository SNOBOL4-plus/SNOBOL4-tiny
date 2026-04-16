// claws5.sc — CLAWS5 POS-tagged corpus tokenizer (Snocone port)
// ENG 685, Lon Cherryholmes Sr.
// Run: scrip --ir-run claws5.sc < CLAWS5inTASA.dat
//
// Faithful Snocone port of claws5.sno / assignment3.py (claws_info pattern).
// Function names match Python exactly: init(), new_sent(), add_tok().
// Each is a single nreturn procedure used as epsilon . *fn() in the pattern.
//
// SNOBOL4→Snocone translation:
//   +num             → INTEGER(num)    (no unary + in scrip)
//   INPUT(.rdch,8,f) → slurp stdin    (file I/O not yet in scrip runtime)
//   juxtaposition    → &&             (string concat)
//   *fn()            → *fn()          (same)

&TRIM     = 1;
&ANCHOR   = 0;
&FULLSCAN = 1;
&ALPHABET POS(10) LEN(1) . nl;
DIGITS = '0123456789';
UCASE  = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';

//--- init() — λ("mem = dict()") ---------------------------------------------

procedure init() {
    mem    = TABLE();
    sentno = 0;
    init   = .dummy;
    nreturn;
}

//--- new_sent() — λ("num=int(num)") + λ("mem[num]=dict()") -----------------
//    Reads global: num (captured by SPAN(DIGITS) . num just before)

procedure new_sent() {
    sentno      = INTEGER(num);
    mem[sentno] = TABLE();
    new_sent    = .dummy;
    nreturn;
}

//--- add_tok() — λ("mem[num][wrd][tag] += 1") -------------------------------
//    Reads globals: sentno, wrd, tag

procedure add_tok() {
    if (DIFFER(mem[sentno][wrd])) { goto have_wrd; }
    mem[sentno][wrd] = TABLE();
    have_wrd:
    if (DIFFER(mem[sentno][wrd][tag])) { goto have_tag; }
    mem[sentno][wrd][tag] = 1;
    goto done;
    have_tag:
    mem[sentno][wrd][tag] = mem[sentno][wrd][tag] + 1;
    done:
    add_tok = .dummy;
    nreturn;
}

//--- claws_pat — direct port of Python claws_info ---------------------------
//  POS(0) + λ("mem=dict()")
//  + ARBNO(
//      ( SPAN(DIGITS)%"num" + σ('_CRD :_PUN') + λ("num=int(num)") + ...
//      | (NOTANY("_")+BREAK("_"))%"wrd" + σ('_')
//        + (ANY(UCASE)+SPAN(DIGITS+UCASE))%"tag" + λ("mem[num][wrd][tag]+=1")
//      ) + σ(' ')
//    )
//  + RPOS(0)

claws_pat =
    POS(0)
    && (epsilon . *init())
    && ARBNO(
        ( (SPAN(DIGITS) . num) && '_CRD :_PUN'
          && (epsilon . *new_sent())
        | (NOTANY('_') && BREAK('_')) . wrd
          && '_'
          && (ANY(UCASE) && SPAN(DIGITS && UCASE)) . tag
          && (epsilon . *add_tok())
        )
        && SPAN(' ')
    )
    && RPOS(0);

//--- pp_mem: pretty-print mem with 3-space indent ---------------------------

procedure pp_mem(  sk, wk, tk, si, wi, ti, sk1, wk1, tk1) {
    OUTPUT = '{';
    sk = SORT(mem);
    si = 0;
    pp_s:
    si = si + 1;
    sk1 = sk[si, 1];
    if (~DIFFER(sk1)) { goto pp_s_done; }
    if (GT(si, 1)) { OUTPUT = '   },'; }
    OUTPUT = '   ' && sk1 && ': {';
    wk = SORT(mem[sk1]);
    wi = 0;
    pp_w:
    wi = wi + 1;
    wk1 = wk[wi, 1];
    if (~DIFFER(wk1)) { goto pp_w_done; }
    if (GT(wi, 1)) { OUTPUT = '      },'; }
    OUTPUT = '      ' && wk1 && ': {';
    tk = SORT(mem[sk1][wk1]);
    ti = 0;
    pp_t:
    ti = ti + 1;
    tk1 = tk[ti, 1];
    if (~DIFFER(tk1)) { goto pp_t_done; }
    OUTPUT = '         ' && tk1 && ': ' && mem[sk1][wk1][tk1] && ',';
    goto pp_t;
    pp_t_done:
    OUTPUT = '      }';
    goto pp_w;
    pp_w_done:
    OUTPUT = '   }';
    goto pp_s;
    pp_s_done:
    OUTPUT = '}';
    pp_mem = .dummy;
    return;
}

//--- Main: slurp stdin, match, print ----------------------------------------

src = '';
slurp:
line = INPUT;
if (~DIFFER(line)) { goto slurp_done; }
src = src && line && ' ';
goto slurp;
slurp_done:
if (src ? claws_pat) { goto matched; }
OUTPUT = 'Pattern match failed';
goto END;
matched:
dummy = pp_mem();
END:
