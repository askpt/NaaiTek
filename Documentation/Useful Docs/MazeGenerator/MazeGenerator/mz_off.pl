testpredicate(A,A).

:- dynamic pos/2.

findPath(L):-findall((A, B), pos(A, B), L).