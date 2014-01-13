% test predicates
testpredicate(A,A).

:- dynamic pos/2.

findPath(L):-findall((A, B), pos(A, B), L).

%main predicate for find maze solution
eachPosMazeSol(X,Y, Xi, Yi, Xf, Yf):-pos(Xi, Yi),
	pos(Xn, Yn),
	findMazeSol(Xi, Yi, Xf, Yf, L, [(Xi, Yi)]),
	eachpos_hel(L, X, Y).

%each position helper predicates
eachpos_hel([(X,Y)],X,Y):-!.

eachpos_hel([(X,Y)|_],X,Y).

eachpos_hel([_|T], X, Y):-eachpos_hel(T, X, Y).


%find maze solution main predicate
findMazeSol(Xf, Yf, Xf, Yf, [(Xf, Yf)], _):-!.

findMazeSol(Xi, Yi, Xf, Yf, [(Xi, Yi)|L], Lt):- next_pos(Xi, Yi, Xn, Yn), 
	not(member((Xn, Yn), Lt)),
	findMazeSol(Xn, Yn, Xf, Yf, L, [(Xi, Yi)|Lt]).

next_pos(Xi, Yi, Xi, Yn) :- Yn is Yi + 1, pos(Xi, Yn).
next_pos(Xi, Yi, Xi, Yn) :- Yn is Yi - 1, pos(Xi, Yn).
next_pos(Xi, Yi, Xn, Yi) :- Xn is Xi + 1, pos(Xn, Yi).
next_pos(Xi, Yi, Xn, Yi) :- Xn is Xi - 1, pos(Xn, Yi).

