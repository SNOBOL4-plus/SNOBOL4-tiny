%-------------------------------------------------------------------------------
% 3
% Dorothy, Jean, Virginia, Bill, Jim, and Tom are six young persons who have
% been close friends from their childhood. Tom, who is older than Jim, is
% Dorothy's brother. Virginia is the oldest girl. The total age of each
% couple-to-be is the same although no two of us are the same age.
% Jim and Jean are together as old as Bill and Dorothy.
% What three engagements were announced at the party?
%-------------------------------------------------------------------------------
:- initialization(main). main :- puzzle; true.

age(1). age(2). age(3). age(4). age(5). age(6).

puzzle :-
    age(D), age(J), age(V), age(B), age(Ji), age(T),
    differ6(D, J, V, B, Ji, T),
    T > Ji,                          % Tom older than Jim
    V > D, V > J,                    % Virginia oldest girl
    Ji + J =:= B + D,                % Jim+Jean = Bill+Dorothy
    equal_sums(B, Ji, T, D, J, V),   % equal couple totals
    not_dorothy(T, D, J, V, B, Ji),  % Tom is Dorothy's brother, not her partner
    display(B, Ji, T, D, J, V),
    fail.

% equal_sums: assign girls to boys so all couple totals match
% boys B,Ji,T paired with girls from {D,J,V} in some order
equal_sums(B, Ji, T, G1, G2, G3) :-
    B + G1 =:= Ji + G2, Ji + G2 =:= T + G3.
equal_sums(B, Ji, T, G1, G2, G3) :-
    B + G1 =:= Ji + G3, Ji + G3 =:= T + G2.
equal_sums(B, Ji, T, G1, G2, G3) :-
    B + G2 =:= Ji + G1, Ji + G1 =:= T + G3.
equal_sums(B, Ji, T, G1, G2, G3) :-
    B + G2 =:= Ji + G3, Ji + G3 =:= T + G1.
equal_sums(B, Ji, T, G1, G2, G3) :-
    B + G3 =:= Ji + G1, Ji + G1 =:= T + G2.
equal_sums(B, Ji, T, G1, G2, G3) :-
    B + G3 =:= Ji + G2, Ji + G2 =:= T + G1.

% Tom not paired with Dorothy (they are siblings)
not_dorothy(T, D, J, V, B, Ji) :-
    \+ partner_is(T, D, D, J, V, B, Ji).

partner_is(T, D, D, _, _, _, _) :- T + D =:= T + D, fail. % placeholder
% Instead encode: find Tom's girl partner and assert it's not Dorothy
not_dorothy(T, D, J, V, B, Ji) :-
    ( B  + D =:= Ji + J, Ji + J =:= T + V -> G_T = V
    ; B  + D =:= Ji + V, Ji + V =:= T + J -> G_T = J
    ; B  + J =:= Ji + D, Ji + D =:= T + V -> G_T = V
    ; B  + J =:= Ji + V, Ji + V =:= T + D -> G_T = D
    ; B  + V =:= Ji + D, Ji + D =:= T + J -> G_T = J
    ; B  + V =:= Ji + J, Ji + J =:= T + D -> G_T = D
    ; fail ),
    G_T =\= D.

display(B, Ji, T, D, J, V) :-
    find_couples(B, Ji, T, D, J, V, GB, GJi, GT),
    girl_name(D, J, V, GB,  GBn),
    girl_name(D, J, V, GJi, GJin),
    girl_name(D, J, V, GT,  GTn),
    write('Bill+'), write(GBn),
    write(' Jim+'), write(GJin),
    write(' Tom+'), write(GTn),
    write('\n').

find_couples(B, Ji, T, D, J, V, D, J, V) :- B+D =:= Ji+J, Ji+J =:= T+V.
find_couples(B, Ji, T, D, J, V, D, V, J) :- B+D =:= Ji+V, Ji+V =:= T+J.
find_couples(B, Ji, T, D, J, V, J, D, V) :- B+J =:= Ji+D, Ji+D =:= T+V.
find_couples(B, Ji, T, D, J, V, J, V, D) :- B+J =:= Ji+V, Ji+V =:= T+D.
find_couples(B, Ji, T, D, J, V, V, D, J) :- B+V =:= Ji+D, Ji+D =:= T+J.
find_couples(B, Ji, T, D, J, V, V, J, D) :- B+V =:= Ji+J, Ji+J =:= T+D.

girl_name(D, _, _, G, dorothy)  :- G =:= D.
girl_name(_, J, _, G, jean)     :- G =:= J.
girl_name(_, _, V, G, virginia) :- G =:= V.

differ6(A,B,C,D,E,F) :-
    A=\=B, A=\=C, A=\=D, A=\=E, A=\=F,
    B=\=C, B=\=D, B=\=E, B=\=F,
    C=\=D, C=\=E, C=\=F,
    D=\=E, D=\=F, E=\=F.
