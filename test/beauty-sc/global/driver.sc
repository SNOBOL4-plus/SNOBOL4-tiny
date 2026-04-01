// driver.sc — test driver for global.sc (Snocone)
// Oracle: compare output to beauty_global_driver.ref (SNOBOL4 golden)

// inline global.sc
nul       = CHAR(0);
bs        = CHAR(8);
ht        = CHAR(9);
tab       = CHAR(9);
nl        = CHAR(10);
lf        = CHAR(10);
vt        = CHAR(11);
ff        = CHAR(12);
cr        = CHAR(13);
fSlash    = CHAR(47);
semicolon = CHAR(59);
bSlash    = CHAR(92);
TRUE   = 1;
FALSE  = 0;
digits = '0123456789';
UTF = TABLE();
UTF[CHAR(194) && CHAR(169)] = 'COPYRIGHT_SIGN';
UTF[CHAR(194) && CHAR(174)] = 'REGISTERED_SIGN';
UTF[CHAR(226) && CHAR(128) && CHAR(148)] = 'EM_DASH';
UTF_Array = SORT(UTF);
_utf_n = SIZE(UTF_Array);
i = 0;
while (1) {
    i = i + 1;
    if (GT(i, _utf_n)) { break; }
    _nm = UTF_Array[i, 2];
    $_nm = UTF_Array[i, 1];
}
UTF_Array = '';
_utf_n = '';
i = '';
_nm = '';

&STLIMIT = 1000000;

// Character constants
if (EQ(SIZE(nul),1) && IDENT(nul,CHAR(0))) { OUTPUT = 'PASS: nul'; } else { OUTPUT = 'FAIL: nul'; }
if (EQ(SIZE(bs),1)  && IDENT(bs,CHAR(8)))  { OUTPUT = 'PASS: bs';  } else { OUTPUT = 'FAIL: bs';  }
if (EQ(SIZE(ht),1)  && IDENT(ht,CHAR(9)))  { OUTPUT = 'PASS: ht';  } else { OUTPUT = 'FAIL: ht';  }
if (EQ(SIZE(tab),1) && IDENT(tab,CHAR(9))) { OUTPUT = 'PASS: tab'; } else { OUTPUT = 'FAIL: tab'; }
if (EQ(SIZE(nl),1)  && IDENT(nl,CHAR(10))) { OUTPUT = 'PASS: nl';  } else { OUTPUT = 'FAIL: nl';  }
if (EQ(SIZE(lf),1)  && IDENT(lf,CHAR(10))) { OUTPUT = 'PASS: lf';  } else { OUTPUT = 'FAIL: lf';  }
if (EQ(SIZE(vt),1)  && IDENT(vt,CHAR(11))) { OUTPUT = 'PASS: vt';  } else { OUTPUT = 'FAIL: vt';  }
if (EQ(SIZE(ff),1)  && IDENT(ff,CHAR(12))) { OUTPUT = 'PASS: ff';  } else { OUTPUT = 'FAIL: ff';  }
if (EQ(SIZE(cr),1)  && IDENT(cr,CHAR(13))) { OUTPUT = 'PASS: cr';  } else { OUTPUT = 'FAIL: cr';  }
if (IDENT(fSlash,CHAR(47)))    { OUTPUT = 'PASS: fSlash';    } else { OUTPUT = 'FAIL: fSlash';    }
if (IDENT(semicolon,CHAR(59))) { OUTPUT = 'PASS: semicolon'; } else { OUTPUT = 'FAIL: semicolon'; }
if (IDENT(bSlash,CHAR(92)))    { OUTPUT = 'PASS: bSlash';    } else { OUTPUT = 'FAIL: bSlash';    }

// TRUE/FALSE/digits
if (IDENT(CONVERT(TRUE,'STRING'),'1'))  { OUTPUT = 'PASS: TRUE';   } else { OUTPUT = 'FAIL: TRUE';   }
if (IDENT(CONVERT(FALSE,'STRING'),'0')) { OUTPUT = 'PASS: FALSE';  } else { OUTPUT = 'FAIL: FALSE';  }
if (IDENT(digits,'0123456789'))         { OUTPUT = 'PASS: digits'; } else { OUTPUT = 'FAIL: digits'; }

// UTF table spot-checks
if (IDENT(UTF[CHAR(194) && CHAR(169)],'COPYRIGHT_SIGN'))    { OUTPUT = 'PASS: UTF COPYRIGHT_SIGN';  } else { OUTPUT = 'FAIL: UTF COPYRIGHT_SIGN';  }
if (IDENT(UTF[CHAR(194) && CHAR(174)],'REGISTERED_SIGN'))   { OUTPUT = 'PASS: UTF REGISTERED_SIGN'; } else { OUTPUT = 'FAIL: UTF REGISTERED_SIGN'; }
if (IDENT(UTF[CHAR(226) && CHAR(128) && CHAR(148)],'EM_DASH')) { OUTPUT = 'PASS: UTF EM_DASH';   } else { OUTPUT = 'FAIL: UTF EM_DASH';   }

// Indirect assign spot-checks
if (IDENT(COPYRIGHT_SIGN, CHAR(194) && CHAR(169)))              { OUTPUT = 'PASS: UTF indirect COPYRIGHT_SIGN'; } else { OUTPUT = 'FAIL: UTF indirect COPYRIGHT_SIGN'; }
if (IDENT(EM_DASH, CHAR(226) && CHAR(128) && CHAR(148)))        { OUTPUT = 'PASS: UTF indirect EM_DASH';        } else { OUTPUT = 'FAIL: UTF indirect EM_DASH';        }
