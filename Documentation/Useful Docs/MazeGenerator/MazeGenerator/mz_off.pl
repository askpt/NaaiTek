testpredicate(A,A).

:- dynamic pos/2.

findPath(L):-findall((A, B), pos(A, B), L).

eachpos(X,Y):-findPath(L),
	eachpos_hel(L, X, Y).

eachpos_hel([(X,Y)],X,Y):-!.

eachpos_hel([(X,Y)|_],X,Y).

eachpos_hel([_|T], X, Y):-eachpos_hel(T, X, Y).




	