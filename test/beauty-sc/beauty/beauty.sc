// beauty.sc — Snocone port of beauty.sno
// Library procedures (global/is/FENCE/io/case/assign/match/counter/stack/tree/
// ShiftReduce/TDump/Gen/Qize/ReadWrite/XDump/semantic/omega/trace) supplied via
// separate -sc includes.  This file contains only the main-program body.

&FULLSCAN  = 1;
&MAXLNGTH  = 524288;

ppStop   = ARRAY('1:4');
ppStop[1] = 15;  ppStop[2] = 30;  ppStop[3] = 33;  ppStop[4] = 80;
ppSmBump  = 5;   ppLgBump  = 42;

//  Argument parsing
ppArgs        = HOST(0);
ppTokBreakEq  = BREAK('= ');
ppTokBreakSp  = BREAK(' ');
ppTokNamePat  = *ppTokBreakEq | REM;
ppTokValPat   = *ppTokBreakSp | REM;

while (1) {
    ppArgs ? (POS(0) && SPAN(' ') && '') = ;
    if (~DIFFER(ppArgs)) { break; }
    // Positive match form avoids if(~(s ? *deref . cap)) emitter underflow
    ppTokName = '';
    ppArgs ? ('--' && *ppTokNamePat . ppTokName) = ;
    if (IDENT(ppTokName)) { break; }   // no --name found
    ppTokVal = '';
    if (ppArgs ? (POS(0) && '=')) { ppArgs ? ('=' && *ppTokValPat . ppTokVal) = ; }
    if      (IDENT(ppTokName, 'micro'))  { ppStop[1]=11; ppStop[2]=26; ppStop[3]=29; ppStop[4]=55; ppSmBump=3; ppLgBump=21;
    } else if (IDENT(ppTokName, 'small'))  { ppStop[1]=13; ppStop[2]=28; ppStop[3]=31; ppStop[4]=60; ppSmBump=4; ppLgBump=24;
    } else if (IDENT(ppTokName, 'medium')) { ppStop[1]=15; ppStop[2]=30; ppStop[3]=33; ppStop[4]=80; ppSmBump=5; ppLgBump=42;
    } else if (IDENT(ppTokName, 'large'))  { ppStop[1]=17; ppStop[2]=32; ppStop[3]=35; ppStop[4]=96; ppSmBump=6; ppLgBump=54;
    } else if (IDENT(ppTokName, 'wide'))   { ppStop[1]=19; ppStop[2]=34; ppStop[3]=37; ppStop[4]=110; ppSmBump=7; ppLgBump=68;
    } else if (IDENT(ppTokName, 's1'))     { ppStop[1]  = INTEGER(ppTokVal);
    } else if (IDENT(ppTokName, 's2'))     { ppStop[2]  = INTEGER(ppTokVal);
    } else if (IDENT(ppTokName, 's3'))     { ppStop[3]  = INTEGER(ppTokVal);
    } else if (IDENT(ppTokName, 's4'))     { ppStop[4]  = INTEGER(ppTokVal);
    } else if (IDENT(ppTokName, 'smbump')) { ppSmBump   = INTEGER(ppTokVal);
    } else if (IDENT(ppTokName, 'lgbump')) { ppLgBump   = INTEGER(ppTokVal);
    } else if (IDENT(ppTokName, 'auto'))   { ppAutoMode = 1;
    } else { OUTPUT = '* Warning: unknown switch --' && ppTokName; }
}

//  --auto two-pass mode
if (DIFFER(ppAutoMode)) {
    ppTab     = CHAR(9);
    ppSpOrEps = SPAN(' ' && ppTab) | epsilon;
    ppSFOrEps = ANY('SF') | epsilon;
    ppBrOrEps = ANY('(<') | epsilon;
    ppGSfx    = *ppSpOrEps && ':' && *ppSFOrEps && *ppBrOrEps && REM;
    ppGPat    = BREAK(':') . ppGCon && *ppGSfx;
    ppTrimPat = SPAN(' ' && ppTab) && RPOS(0) . ppDrop;
    ppNg      = 0;
    ppWArr    = ARRAY(2000);
    ppTmpFile = '/tmp/beauty_auto_' && HOST(1) && '.sno';
    output__(.ppTmp, 3, '', ppTmpFile);
    ppStmt = '';
    while (DIFFER(ppLn = INPUT)) {
        ppTmp = ppLn;
        if (ppLn ? (POS(0) && ANY('*-'))) { goto ppAutoNext; }
        if (ppLn ? (POS(0) && ANY('.+'))) { ppStmt = ppStmt && ' ' && ppLn; goto ppAutoNext; }
        if (DIFFER(ppStmt) && (ppStmt ? ppGPat)) {
            ppGConT = ppGCon;
            ppGConT ? *ppTrimPat = ;
            ppW = SIZE(ppGConT);
            if (DIFFER(ppW, 0)) { ppNg = ppNg + 1; ppWArr[ppNg] = ppW; }
        }
        ppStmt = ppLn;
        ppAutoNext:
    }
    ENDFILE(3);
    for (ppI = 1; LT(ppI, ppNg); ppI = ppI + 1) {
        ppJ = ppI; ppKey = ppWArr[ppJ];
        while (GT(ppJ, 1) && LT(ppKey, ppWArr[ppJ - 1])) {
            ppWArr[ppJ] = ppWArr[ppJ - 1]; ppJ = ppJ - 1;
        }
        ppWArr[ppJ] = ppKey;
    }
    ppP90i = (ppNg * 9) / 10;
    if (LT(ppP90i, 1)) { ppP90i = 1; }
    ppP90 = ppWArr[ppP90i];
    if (LT(ppP90, 20)) { ppP90 = 20; }
    ppStop[4] = ppP90 + 6;
    if (LT(ppStop[4], 40)) { ppStop[4] = 40; }
    ppStop[1] = 11 + (ppStop[4] - 40) * 8 / 70;
    ppStop[2] = ppStop[1] + 15;
    ppStop[3] = ppStop[2] + 3;
    ppSmBump  = 3 + (ppStop[4] - 40) * 4 / 70;
    ppLgBump  = ppStop[4] - ppStop[3] - 5;
    if (LT(ppLgBump, 10)) { ppLgBump = 10; }
    OUTPUT = '* --auto: n=' && ppNg
          && ' p90=' && ppP90
          && ' s1='  && ppStop[1] && ' s2=' && ppStop[2]
          && ' s3='  && ppStop[3] && ' s4=' && ppStop[4]
          && ' smbump=' && ppSmBump && ' lgbump=' && ppLgBump;
    input__(.INPUT, 1, '', ppTmpFile);
}

//  Parser patterns
Integer    = SPAN(digits);
DQ         = '"' && BREAK('"' && nl) && '"';
SQ         = "'" && BREAK("'" && nl) && "'";
String     = *SQ | *DQ;
Real       = ( SPAN(digits)
            && ('.' && FENCE(SPAN(digits) | epsilon) | epsilon)
            && ('E' | 'e') && ('+' | '-' | epsilon) && SPAN(digits)
           | SPAN(digits) && '.' && FENCE(SPAN(digits) | epsilon)
           );
Id         = ANY(&UCASE && &LCASE) && FENCE(SPAN('.' && digits && &UCASE && '_' && &LCASE) | epsilon);
Function   = SPAN('.' && digits && &UCASE && '_' && &LCASE) . tx . *match(Functions, TxInList);
BuiltinVar = SPAN('.' && digits && &UCASE && '_' && &LCASE) . tx . *match(BuiltinVars, TxInList);
SpecialNm  = SPAN('.' && digits && &UCASE && '_' && &LCASE) . tx . *match(SpecialNms, TxInList);
ProtKwd    = '&' && SPAN(&UCASE && &LCASE) . tx . *match(ProtKwds, TxInList);
UnprotKwd  = '&' && SPAN(&UCASE && &LCASE) . tx . *match(UnprotKwds, TxInList);
Gray  = *White | epsilon;
White = SPAN(' ' && tab) && FENCE(nl && ('+' | '.') && FENCE(SPAN(' ' && tab) | epsilon) | epsilon)
      | nl && ('+' | '.') && FENCE(SPAN(' ' && tab) | epsilon);
TxInList    = (POS(0) | ' ') && EVAL('upr(tx)') && (' ' | RPOS(0));
SpecialNms  = 'ABORT CONTINUE END FRETURN NRETURN RETURN SCONTINUE START';
BuiltinVars = 'ABORT ARB BAL FAIL FENCE INPUT OUTPUT REM TERMINAL';
ProtKwds    = 'ABORT ALPHABET ARB BAL FAIL FENCE FILE FNCLEVEL '
           && 'LASTFILE LASTLINE LASTNO LCASE LINE REM RTNTYPE '
           && 'STCOUNT STNO SUCCEED UCASE';
UnprotKwds  = 'ABEND ANCHOR CASE CODE COMPARE DUMP ERRLIMIT '
           && 'ERRTEXT ERRTYPE FTRACE INPUT MAXLNGTH OUTPUT '
           && 'PROFILE STLIMIT TRACE TRIM FULLSCAN';
Functions   = 'ANY APPLY ARBNO ARG ARRAY ATAN BACKSPACE BREAK BREAKX '
           && 'CHAR CHOP CLEAR CODE COLLECT CONVERT COPY COS DATA '
           && 'DATATYPE DATE DEFINE DETACH DIFFER DUMP DUPL EJECT '
           && 'ENDFILE EQ EVAL EXIT EXP FENCE FIELD GE GT HOST '
           && 'IDENT INPUT INTEGER ITEM LE LEN LEQ LGE LGT LLE '
           && 'LLT LN LNE LOAD LOCAL LPAD LT NE NOTANY OPSYN OUTPUT '
           && 'POS PROTOTYPE REMDR REPLACE REVERSE REWIND RPAD RPOS '
           && 'RSORT RTAB SET SETEXIT SIN SIZE SORT SPAN SQRT STOPTR '
           && 'SUBSTR TAB TABLE TAN TIME TRACE TRIM UNLOAD';
$'='  = *White && '=' && *White;   $'?'  = *White && '?' && *White;
$'|'  = *White && '|' && *White;   $'+'  = *White && '+' && *White;
$'-'  = *White && '-' && *White;   $'/'  = *White && '/' && *White;
$'*'  = *White && '*' && *White;   $'^'  = *White && '^' && *White;
$'!'  = *White && '!' && *White;   $'**' = *White && '**' && *White;
$'$'  = *White && '$' && *White;   $'.'  = *White && '.' && *White;
$'&'  = *White && '&' && *White;   $'@'  = *White && '@' && *White;
$'#'  = *White && '#' && *White;   $'%'  = *White && '%' && *White;
$'~'  = *White && '~' && *White;   $','  = *Gray  && ',' && *Gray;
$'('  = '(' && *Gray;  $'['  = '[' && *Gray;  $'<'  = '<' && *Gray;
$')'  = *Gray && ')';  $']'  = *Gray && ']';  $'>'  = *Gray && '>';
ExprList = nPush() && *XList && ("'ExprList'" & '*(GT(nTop(), 1) nTop())') && nPop();
XList    = nInc() && (*Expr | epsilon . '') && FENCE($',' && *XList | epsilon);
Expr     = *Expr0;
Expr0    = *Expr1  && FENCE($'='  && *Expr0  && ("'='"  & 2) | epsilon);
Expr1    = *Expr2  && FENCE($'?'  && *Expr1  && ("'?'"  & 2) | epsilon);
Expr2    = *Expr3  && FENCE($'&'  && *Expr2  && ("'&'"  & 2) | epsilon);
Expr3    = nPush() && *X3  && ("'|'"  & '*(GT(nTop(), 1) nTop())') && nPop();
X3       = nInc()  && *Expr4 && FENCE($'|' && *X3 | epsilon);
Expr4    = nPush() && *X4  && ("'..'" & '*(GT(nTop(), 1) nTop())') && nPop();
X4       = nInc()  && *Expr5 && FENCE(*White && *X4 | epsilon);
Expr5    = *Expr6  && FENCE($'@'  && *Expr5  && ("'@'"  & 2) | epsilon);
Expr6    = *Expr7  && FENCE($'+'  && *Expr6  && ("'+'"  & 2) | $'-' && *Expr6 && ("'-'" & 2) | epsilon);
Expr7    = *Expr8  && FENCE($'#'  && *Expr7  && ("'#'"  & 2) | epsilon);
Expr8    = *Expr9  && FENCE($'/'  && *Expr8  && ("'/'"  & 2) | epsilon);
Expr9    = *Expr10 && FENCE($'*'  && *Expr9  && ("'*'"  & 2) | epsilon);
Expr10   = *Expr11 && FENCE($'%'  && *Expr10 && ("'%'"  & 2) | epsilon);
Expr11   = *Expr12 && FENCE(($'^' | $'!' | $'**') && *Expr11 && ("'^'" & 2) | epsilon);
Expr12   = *Expr13 && FENCE($'$'  && *Expr12 && ("'$'"  & 2) | $'.' && *Expr12 && ("'.'" & 2) | epsilon);
Expr13   = *Expr14 && FENCE($'~'  && *Expr13 && ("'~'"  & 2) | epsilon);
Expr14   = '@' && *Expr14 && ("'@'" & 1) | '~' && *Expr14 && ("'~'" & 1)
         | '?' && *Expr14 && ("'?'" & 1) | *ProtKwd   . '' && 'ProtKwd'
         | *UnprotKwd . '' && 'UnprotKwd' | '&' && *Expr14 && ("'&'" & 1)
         | '+' && *Expr14 && ("'+'" & 1)  | '-' && *Expr14 && ("'-'" & 1)
         | '*' && *Expr14 && ("'*'" & 1)  | '$' && *Expr14 && ("'$'" & 1)
         | '.' && *Expr14 && ("'.'" & 1)  | '!' && *Expr14 && ("'!'" & 1)
         | '%' && *Expr14 && ("'%'" & 1)  | '/' && *Expr14 && ("'/'" & 1)
         | '#' && *Expr14 && ("'#'" & 1)  | '=' && *Expr14 && ("'='" & 1)
         | '|' && *Expr14 && ("'|'" & 1)  | *Expr15;
Expr15   = *Expr17 && FENCE(nPush() && *Expr16 && ("'[]'" & 'nTop() + 1') && nPop() | epsilon);
Expr16   = nInc() && ($'[' && *ExprList && $']' | $'<' && *ExprList && $'>') && FENCE(*Expr16 | epsilon);
Expr17   = FENCE(
             nPush() && $'(' && *Expr
               && ($',' && *XList && ("','" & 'nTop() + 1') | epsilon . '' && ("'()'" & 1))
               && $')' && nPop()
           | *Function  . '' && 'Function' && $'(' && *ExprList && $')' && ("'Call'" & 2)
           | *Id        . '' && 'Id'       && $'(' && *ExprList && $')' && ("'Call'" & 2)
           | *BuiltinVar . '' && 'BuiltinVar' | *SpecialNm  . '' && 'SpecialNm'
           | *Id         . '' && 'Id'         | *String     . '' && 'String'
           | *Real       . '' && 'Real'        | *Integer    . '' && 'Integer'
           );
SGoto    = ('S' | 's') . *assign(.SorF, *'S');
FGoto    = ('F' | 'f') . *assign(.SorF, *'F');
SorF     = *SGoto | *FGoto;
Target   = $'(' . *assign(.Brackets, *'()') && *Expr && $')'
         | $'<' . *assign(.Brackets, *'<>') && *Expr && $'>';
Goto     = *Gray && ':' && *Gray
        && FENCE(
             *Target && ("*(':' Brackets)" & 1) && epsilon . ''
           | (*SGoto | *FGoto) && *Target && ("*(':' SorF Brackets)" & 1)
             && FENCE(*Gray && (*SGoto | *FGoto) && *Target && ("*(':' SorF Brackets)" & 1) | epsilon . '')
           );
Control   = '-' && BREAK(nl && ';');
Comment   = '*' && BREAK(nl);
Label     = BREAK(' ' && tab && nl && ';') . '' && 'Label';
Stmt      = *Label
         && ( *White && *Expr14
              && FENCE(
                   epsilon . '' && *White && ('=' . '' && *White && *Expr | '=' . '' && epsilon . '')
                 | ($'?' | *White) && *Expr1
                   && FENCE( *White && ('=' . '' && *White && *Expr | '=' . '' && epsilon . '')
                            | epsilon . '' && epsilon . '')
                 | epsilon . '' && epsilon . '' && epsilon . '')
            | epsilon . '' && epsilon . '' && epsilon . '' && epsilon . '')
         && FENCE(*Goto | epsilon . '' && epsilon . '') && *Gray;
Commands  = *Command && FENCE(*Commands | epsilon);
Command   = nInc()
         && FENCE(
              *Comment  . '' && 'Comment' && ("'Comment'" & 1) && nl
            | *Control  . '' && 'Control' && ("'Control'" & 1) && (nl | ';')
            | *Stmt     && ("'Stmt'" & 7) && (nl | ';')
            );
Parse     = nPush() && ARBNO(*Command) && ("'Parse'" & 'nTop()') && nPop();
Compiland = nPush() && ARBNO(*Command) && ("'Parse'" & 'nTop()')
         && (icase('END') && (' ' && BREAK(nl) && nl | nl) && ARBNO(BREAK(nl) && nl) | epsilon)
         && nPop();

//=============================================================================
//  ppLeaf(x, t) — emit a leaf node; return 1 on success
//=============================================================================
procedure ppLeaf(x, t) {
    if (Gen(ss(x))) { return; }
    error();
}

//=============================================================================
//  ppUnOp(x, t, c) — emit a unary operator node
//=============================================================================
procedure ppUnOp(x, t, c) {
    if (Gen(ss(x, ppWidth - GetLevel()))) { return; }
    Gen(t);  pp(c[1]);
    return;
}

//=============================================================================
//  ppBinOp(x, t, c) — emit a binary operator node
//=============================================================================
procedure ppBinOp(x, t, c) {
    if (Gen(ss(x, ppWidth - GetLevel()))) { return; }
    pp(c[1]);
    Gen(nl);  DecLevel();  Gen(t);  IncLevel();  GenTab();
    pp(c[2]);
    return;
}

//=============================================================================
//  ppStmt(x) — columnar layout for Stmt nodes
//=============================================================================
procedure ppStmt(x, c, ppLbl, ppSubj, ppPatrn, ppAsgn, ppRepl, ppGo1, ppGo2) {
    SetLevel(0);  GenSetCont('+');
    ppWidth = ppStop[4];
    c       = c(x);
    ppLbl   = ss(c[1]);   ppSubj  = c[2];  ppPatrn = c[3];
    ppAsgn  = v(c[4]);    ppRepl  = c[5];  ppGo1   = c[6];  ppGo2 = c[7];
    Gen(ppLbl);
    if (DIFFER(t(ppSubj))) {
        Gen(' ');  GenTab(ppStop[1]);  SetLevel(ppStop[1]);
        ppWidth = IDENT(t(ppPatrn)) && IDENT(ppAsgn) && IDENT(t(ppGo1)) && ppStop[4] + ppLgBump;
        pp(ppSubj);
        if (DIFFER(t(ppPatrn))) {
            Gen(' ');  GenTab(ppStop[2]);  SetLevel(ppStop[2]);
            ppWidth = IDENT(ppAsgn) && IDENT(t(ppGo1)) && ppStop[4] + ppLgBump;
            pp(ppPatrn);
            if (DIFFER(ppAsgn)) {
                Gen(' =');
                if (DIFFER(t(ppRepl))) { Gen(' ');  pp(ppRepl); }
            }
        } else if (DIFFER(ppAsgn)) {
            Gen(' ');  GenTab(ppStop[2]);  SetLevel(ppStop[2]);
            Gen('=');
            if (DIFFER(t(ppRepl))) {
                Gen(' ');  GenTab(ppStop[3]);  SetLevel(ppStop[3]);
                ppWidth = IDENT(t(ppGo1)) && ppStop[4] + ppLgBump;
                pp(ppRepl);
            }
        }
    }
    if (DIFFER(t(ppGo1))) {
        ppWidth = 256;
        Gen(' ');  GenTab(ppStop[4]);  SetLevel(ppStop[4]);
        Gen(':');  pp(ppGo1);
        if (DIFFER(t(ppGo2))) { pp(ppGo2); }
    }
    Gen(nl);
    return;
}

//=============================================================================
//  ppList(x, sep, open, close) — emit children with separator
//  sep: ',' '|' '..' '[]' — controls layout
//=============================================================================
procedure ppList(x, sep, open, close, c, i, n) {
    c = c(x);  n = n(x);
    if (Gen(ss(x, ppWidth - GetLevel()))) { return; }
    if (DIFFER(open)) { Gen(open);  IncLevel();  GenTab(); }
    pp(c[1]);
    for (i = 2; LE(i, n); i = i + 1) {
        Gen(nl);  DecLevel();  Gen(sep);  IncLevel();  GenTab();
        pp(c[i]);
    }
    if (DIFFER(close)) { Gen(nl);  DecLevel();  Gen(close); }
    return;
}

//=============================================================================
//  pp(x) — pretty-print a tree node to OUTPUT
//=============================================================================
procedure pp(x, c, i, n, t, v) {
    if (~DIFFER(x)) { return; }
    t = t(x);  v = v(x);  n = n(x);  c = c(x);
    if (~DIFFER(t)) { return; }

    if (IDENT(t, 'BuiltinVar') || IDENT(t, 'Function')  || IDENT(t, 'Id')      ||
        IDENT(t, 'Integer')    || IDENT(t, 'Label')      || IDENT(t, 'ProtKwd') ||
        IDENT(t, 'Real')       || IDENT(t, 'SpecialNm')  || IDENT(t, 'String')  ||
        IDENT(t, 'UnprotKwd')  || IDENT(t, ':()') || IDENT(t, ':<>') ||
        IDENT(t, ':S()') || IDENT(t, ':S<>') || IDENT(t, ':F()') || IDENT(t, ':F<>')) {
        ppLeaf(x, t); return;
    }
    if (IDENT(t, 'Parse') || IDENT(t, '0')) {
        ppWidth = ppStop[4];
        for (i = 1; LE(i, n); i = i + 1) { pp(c[i]); }
        return;
    }
    if (IDENT(t, 'Comment') || IDENT(t, 'Control')) {
        SetLevel(0);  GenSetCont();  Gen(v(c[1]) && nl); return;
    }
    if (IDENT(t, 'Stmt'))     { ppStmt(x); return; }
    if (IDENT(t, 'ExprList')) { ppList(x, ',',  '',  ''); return; }
    if (IDENT(t, ','))        { ppList(x, ',',  '(', ')'); return; }
    if (IDENT(t, '..'))       { ppList(x, '',   '',  ''); return; }
    if (IDENT(t, '[]'))       { ppList(x, ']',  '[', ']'); return; }
    if (IDENT(t, '()')) {
        if (Gen(ss(x, ppWidth - GetLevel()))) { return; }
        Gen('(');  IncLevel();  GenTab();  pp(c[1]);
        Gen(nl);   DecLevel();  Gen(')');  return;
    }
    if (IDENT(t, 'Call')) {
        if (Gen(ss(x, ppWidth - GetLevel()))) { return; }
        pp(c[1]);  Gen('(' && nl);
        IncLevel();  GenTab();  pp(c[2]);
        Gen(nl);   DecLevel();  Gen(')');  return;
    }
    if (IDENT(t, '|')) {
        if (EQ(n, 1)) { ppUnOp(x, t, c); return; }
        ppList(x, '|', '', ''); return;
    }
    if (EQ(n, 1)) { ppUnOp(x, t, c); return; }
    if (EQ(n, 2)) { ppBinOp(x, t, c); return; }
    error();
}

//=============================================================================
//  ss helpers
//=============================================================================
//=============================================================================
//  ss_leaf(t, v, c, len) — stringify leaf/goto nodes; freturn if too long
//  Returns the string in ss_leaf (caller assigns to ss).
//=============================================================================
procedure ss_leaf(t, v, c, len) {
    if      (IDENT(t,'BuiltinVar')||IDENT(t,'Function')||IDENT(t,'ProtKwd')||
             IDENT(t,'SpecialNm') ||IDENT(t,'UnprotKwd')) { ss_leaf = upr(v); }
    else if (IDENT(t,'Id')||IDENT(t,'Integer')||IDENT(t,'Real')||IDENT(t,'String')) {
        ss_leaf = v;
    }
    else if (IDENT(t, 'Label')) {
        if (v ? (POS(0) && *SpecialNm && RPOS(0))) { ss_leaf = upr(v); } else { ss_leaf = v; }
    }
    else if (IDENT(t,':()'))  { ss_leaf='('  && ss(c[1],len-2) && ')'; if(DIFFER(ss_leaf)){return;}else{freturn;} }
    else if (IDENT(t,':<>'))  { ss_leaf='<'  && ss(c[1],len-2) && '>'; if(DIFFER(ss_leaf)){return;}else{freturn;} }
    else if (IDENT(t,':S()')) { ss_leaf='S(' && ss(c[1],len-3) && ')'; if(DIFFER(ss_leaf)){return;}else{freturn;} }
    else if (IDENT(t,':S<>')) { ss_leaf='S<' && ss(c[1],len-3) && '>'; if(DIFFER(ss_leaf)){return;}else{freturn;} }
    else if (IDENT(t,':F()')) { ss_leaf='F(' && ss(c[1],len-3) && ')'; if(DIFFER(ss_leaf)){return;}else{freturn;} }
    else if (IDENT(t,':F<>')) { ss_leaf='F<' && ss(c[1],len-3) && '>'; if(DIFFER(ss_leaf)){return;}else{freturn;} }
    else { freturn; }   // not a leaf — caller handles compound
    if (LE(SIZE(ss_leaf), len)) { return; } else { freturn; }
}

//=============================================================================
//  ss(x, len) — stringify a tree node; freturn if result exceeds len chars
//=============================================================================
procedure ss(x, len, c, i, n, t, v) {
    if (~DIFFER(x)) { return; }
    if (IDENT(len)) { len = 1024; }
    if (~GT(len, 0)) { freturn; }
    t = t(x);  v = v(x);  n = n(x);  c = c(x);
    if (~DIFFER(t)) { return; }
    // Try leaf first
    ss = ss_leaf(t, v, c, len);
    if (DIFFER(ss)) { return; }
    // Compound nodes
    if (IDENT(t, '|') && EQ(n, 1)) { goto ss_unop; }
    if (IDENT(t, '|')) {
        ss = ss(c[1], len);  if (~DIFFER(ss)) { freturn; }
        for (i = 2; LE(i, n); i = i + 1) {
            ss = ss && ' | ' && ss(c[i], len - SIZE(ss) - 3);
            if (~DIFFER(ss)) { freturn; }
        }
        return;
    }
    if (IDENT(t, '..')) {
        ss = ss(c[1], len);  if (~DIFFER(ss)) { freturn; }
        for (i = 2; LE(i, n); i = i + 1) {
            ss = ss && ' ' && ss(c[i], len - SIZE(ss) - 1);
            if (~DIFFER(ss)) { freturn; }
        }
        return;
    }
    if (IDENT(t, 'ExprList')) {
        ss = ss(c[1], len);  if (~DIFFER(ss)) { freturn; }
        for (i = 2; LE(i, n); i = i + 1) {
            ss = ss && ', ' && ss(c[i], len - SIZE(ss) - 2);
            if (~DIFFER(ss)) { freturn; }
        }
        return;
    }
    if (IDENT(t, ',')) {
        ss = '(' && ss(c[1], len - 4);  if (~DIFFER(ss)) { freturn; }
        for (i = 2; LE(i, n); i = i + 1) {
            ss = ss && ', ' && ss(c[i], len - SIZE(ss) - 3);
            if (~DIFFER(ss)) { freturn; }
        }
        ss = ss && ')'; return;
    }
    if (IDENT(t, '[]')) {
        ss = ss(c[1], len);  if (~DIFFER(ss)) { freturn; }
        for (i = 2; LE(i, n); i = i + 1) {
            ss = ss && '[' && ss(c[i], len - SIZE(ss) - 2) && ']';
            if (~DIFFER(ss)) { freturn; }
        }
        return;
    }
    if (IDENT(t, '()')) {
        ss = '(' && ss(c[1], len - 2) && ')';
        if (DIFFER(ss)) { return; } else { freturn; }
    }
    if (IDENT(t, 'Call')) {
        ss = ss(c[1]) && '(' && ss(c[2], len - SIZE(v) - 2) && ')';
        if (DIFFER(ss)) { return; } else { freturn; }
    }
    if (EQ(n, 1)) { goto ss_unop; }
    if (EQ(n, 2)) { goto ss_binop; }
    error();

    ss_unop:
    ss = t && ss(c[1], len - SIZE(t));
    if (DIFFER(ss)) { return; } else { freturn; }

    ss_binop:
    ss = ss(c[1], len);
    if (~DIFFER(ss)) { freturn; }
    ss = ss && ' ' && t && ' ' && ss(c[2], len - SIZE(ss) - SIZE(t) - 2);
    if (DIFFER(ss)) { return; } else { freturn; }
}

procedure bVisit(x, fnc, i) {
    if (~APPLY(fnc, x)) { return; }
    for (i = 1; LE(i, n(x)); i = i + 1) { bVisit(c(x)[i], fnc); }
    return;
}

Refs = '';
procedure findRefs(x, n, v) {
    if (~DIFFER(x)) { return; }
    if (IDENT(t(x), 'Call')) {
        for (n = 2; LE(n, n(x)); n = n + 1) { bVisit(c(x)[n], .findRefs); }
        freturn;
    }
    if      (IDENT(t(x), '&') && EQ(n(x), 1)) { v = ss(x); }
    else if (IDENT(t(x), 'Id'))                { v = v(x);  }
    else                                        { return;    }
    if (~(v ? (POS(0) && SPAN('0123456789' && &UCASE && '_') && RPOS(0)))) { freturn; }
    if (DIFFER(Refs)) { Refs = Refs && ' ' && v; } else { Refs = v; }
    freturn;
}

procedure refs(p, c, n, s, subj) {
    c = c(p);
    for (n = 1; LE(n, n(p)); n = n + 1) {
        if (~IDENT(t(c[n]), 'Stmt'))               { goto refs_next; }
        s = s + 1;
        if (~IDENT(t(c(c[n])[3])))                 { goto refs_next; }
        if (~IDENT(t(c(c[n])[4]), '='))             { goto refs_next; }
        if (~(IDENT(t(c(c[n])[2]), 'Id') ||
              IDENT(t(c(c[n])[2]), '$'))) { goto refs_next; }
        subj = ss(c(c[n])[2]);
        Refs = '';
        bVisit(c(c[n])[5], .findRefs);
        OUTPUT = LPAD(s, 3, 0) && ': ' && RPAD(subj, 38) && ' ' && Refs;
        refs_next:
    }
    return;
}

//=============================================================================
//  Main loop
//=============================================================================
doDebug = 0;
Space   = SPAN(' ' && tab) | epsilon;

while (DIFFER(Line = INPUT)) {
    Src = '';
    while (Line ? (POS(0) && ANY('*-'))) {
        OUTPUT = Line;
        Line = INPUT;
        if (~DIFFER(Line)) { goto END; }
    }
    while (1) {
        Src  = Src && Line && nl;
        Line = INPUT;
        if (~DIFFER(Line)) {
            if (~(Src ? (POS(0) && *Parse && *Space && RPOS(0)))) { goto mainErr1; }
            sno = Pop();
            if (~DIFFER(sno)) { goto mainErr2; }
            pp(sno);
            goto END;
        }
        if (~(Line ? (POS(0) && ANY('.+')))) { break; }
    }
    if (~(Src ? (POS(0) && *Parse && *Space && RPOS(0)))) { goto mainErr1; }
    sno = Pop();
    if (~DIFFER(sno)) { goto mainErr2; }
    pp(sno);
}
goto END;

mainErr1:  OUTPUT = 'Parse Error';    OUTPUT = Src;  goto END;
mainErr2:  OUTPUT = 'Internal Error'; OUTPUT = Src;

END:
