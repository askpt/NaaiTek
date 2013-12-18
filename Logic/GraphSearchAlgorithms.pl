% Algorithms for graph search

% load this file
% ['/Users/joaocarreira/Desktop/NaaiTek/Logic/LoadFiles.pl'].

% deph search
% this predicates gets a possible path between PersonA and PersonB
%
% note: it is a depth search, it doesn't guarante that this is better
% path or that PersonB is the closest to PersonA

depth_search(PersonA, PersonB, List) :- depth_search(PersonA, PersonB, [PersonA], List).

depth_search(PersonB, PersonB, _, [PersonB]).

depth_search(PersonA, PersonB, TempList, [PersonA|List]) :- isConnected(PersonA, PersonX),
							not(member(PersonX, TempList)),
							depth_search(PersonX, PersonB, [PersonX|TempList], List).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% breadth search
% this predicate gets a possible path between PersonA and PersonB,
% taking into account the less possible number of nodes needed to go through
%
% note: it is a breadth search, it doesn't guarantee that this is better
% possible path as it doesn't the weight associated with a given arc

breadth_search(PersonA, PersonB, List) :- breadth_search_aux([[PersonA]], PersonB, InvertedList), reverse(InvertedList, List).

breadth_search_aux([First | _], PersonB, First) :- First = [PersonB | _].

breadth_search_aux([[Last | Tail] | Remaining], PersonB, List) :- findall([PersonZ, Last | Tail],
										next_node(Last, Tail, PersonZ), TempList),
										append(Remaining, TempList, TempPath),
										breadth_search_aux(TempPath, PersonB, List).

breadth_search_aux([[PersonB | _] | Remaining], PersonB, List) :- !, breadth_search_aux(Remaining, PersonB, List).

next_node(X, T, Z) :- isConnected(X, Z),
						not(member(Z, T)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% branch and bound search
% this search method evaluates local transistions but always evaluates, in any given moment, whether
% it would be better, or not, to change which node to expand; this evaluation is based on the accumulated
% strength from the root node until the evaluated node (this method will allow to evaluate the path, in the
% social graph, that guarantees that is going through the "strongest" relationships)
%
% in this search method the cost from PersonA to PersonB is an average cost
% example:
% A - (1) - B - (1) - C - (1) - D
% will be sorted ahead of
% A - (2) - D
%
% note that this method is sorting in an ascending order, which means that in order to get the strongest
% connection we need to get the LAST element of the list

branch_and_bound(PersonA, PersonB, List):- findall(L, ex_branch_and_bound(PersonA, PersonB, L), ListAux),
	                                   sort(ListAux, ListAux2), reverse(ListAux2, [[(_,List)]|_]).

ex_branch_and_bound(PersonA, PersonB, [(C,List)]) :- branch_and_bound_aux([(0, [PersonA])], PersonB, [(C,Path)]), reverse(Path, List).

%branch_and_bound_aux([(_, First) | _], PersonB, First) :- First = [PersonB | _].

branch_and_bound_aux([(C, [PersonB | Others]) | _], PersonB, [(C, [PersonB | Others])]).

branch_and_bound_aux([(Cost, [Last | Tail]) | Others], PersonB, Path) :-
					findall((CostC, [PersonZ, Last | Tail]),
					(next_node_b_b(Last, Tail, PersonZ, CostC1),
					countElementsInList([PersonZ, Last | Tail], TotalNodes),
					% line below was before taking average cost
					%CostC is CostC1 + Cost),
					CostC is (CostC1 + Cost) / TotalNodes),
					List),
					append(Others, List, PathN),
					sort(PathN, PathN1),
					% println for test only
					%write(CostC), nl, write(TotalNodes), nl,
					branch_and_bound_aux(PathN1, PersonB, Path).

next_node_b_b(PersonX, List, PersonZ, Cost) :- isConnected(PersonX, PersonZ),
								connectionCost(PersonX, PersonZ, Cost),
								not(member(PersonZ, List)).

