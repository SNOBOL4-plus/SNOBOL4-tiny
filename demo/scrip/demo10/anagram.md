```SNOBOL4
*  SCRIP DEMO10 -- Detect anagrams (SNOBOL4 section)
*  Idiom: BSORT char-ARRAY -> canonical key; TABLE groups words by key;
*         CONVERT(T,'ARRAY') iterates table entries
        &TRIM = 1
        DEFINE('BSORT(A,I,N)J,K,V)')        :(BSORT_END)
BSORT   J  = I
BS1     J  = J + 1  LT(J,N)                :F(RETURN)
        K  = J
        V  = A<J>
BS2     K  = K - 1  GT(K,I)                :F(BS_RO)
        A<K + 1>  = LGT(A<K>,V)  A<K>      :S(BS2)
        A<K + 1>  = V                       :(BS1)
BS_RO   A<I>  = V                          :(BS1)
BSORT_END
        DEFINE('SORTCHARS(W)A,I,N,KEY)')    :(SC_END)
SORTCHARS
        N      = SIZE(W)
        A      = ARRAY(N)
        I      = 0
SC1     I      = I + 1  GT(I, N)           :S(SC2)
        A<I>   = SUBSTR(W, I, 1)           :(SC1)
SC2     BSORT(A, 1, N)
        KEY    =
        I      = 0
SC3     I      = I + 1  GT(I, N)           :S(SC_RET)
        KEY    = KEY A<I>                  :(SC3)
SC_RET  SORTCHARS = KEY                    :(RETURN)
SC_END
        T      = TABLE()
        WORDS  = 'eat tea tan ate nat bat'
        WPAT   = BREAK(' ') . W
WLOOP   WORDS  WPAT =                      :F(WDONE)
        KEY    = SORTCHARS(W)
        T<KEY> = IDENT(T<KEY>) W            :S(WLOOP)
        T<KEY> = T<KEY> ' ' W              :(WLOOP)
WDONE   T<SORTCHARS('bat')> = SORTCHARS(IDENT(WORDS)) * skip solo entries
        ROWS   = CONVERT(T, 'ARRAY')
        I      = 0
RLOOP   I      = I + 1
        GT(I, PROTOTYPE(ROWS))              :S(END)
        ENTRY  = ROWS<I,2>
        ENTRY  BREAK(' ')                   :F(RLOOP)
        OUTPUT = ENTRY                      :(RLOOP)
END
```

```Icon
# SCRIP DEMO10 -- Detect anagrams (Icon section)
# Idiom: sort string chars as list -> canonical key; table groups by key
procedure sortchars(w)
    chars := []
    every put(chars, !w)
    chars := sort(chars)
    key := ""
    every key ||:= !chars
    return key
end

procedure main()
    words := ["eat", "tea", "tan", "ate", "nat", "bat"]
    t := table()
    every w := !words do {
        key := sortchars(w)
        /t[key] := []
        put(t[key], w)
    }
    every pair := !sort(t) do {
        if *pair[2] > 1 then {
            out := ""
            every out ||:= (if *out = 0 then "" else " ") || !pair[2]
            write(out)
        }
    }
end
```

```Prolog
% SCRIP DEMO10 -- Detect anagrams (Prolog section)
% Idiom: msort on char list gives canonical key; group_pairs_by_key groups
:- initialization(main, main).

main :-
    Words = ["eat","tea","tan","ate","nat","bat"],
    maplist([W,K]>>(string_chars(W,Cs), msort(Cs,S), string_chars(K,S)),
            Words, Keys),
    pairs_keys_values(Pairs, Keys, Words),
    keysort(Pairs, Sorted),
    group_pairs_by_key(Sorted, Groups),
    forall(
        (member(_-Grp, Groups), length(Grp, L), L > 1),
        (atomic_list_concat(Grp, ' ', Line), write(Line), nl)
    ).
```
