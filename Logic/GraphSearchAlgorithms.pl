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

breadth_search_aux([First | Remaining], PersonB, First) :- First = [PersonB | _].

breadth_search_aux([[Last | Tail] | Remaining], PersonB, List) :- findall([PersonZ, Last | Tail], 
										next_node(Last, Tail, PersonZ), TempList),
										append(Remaining, TempList, TempPath),
										breadth_search_aux(TempPath, PersonB, List).

breadth_search_aux([[PersonB | Tail] | Remaining], PersonB, List) :- !, breadth_search_aux(Remaining, PersonB, List).

next_node(X, T, Z) :- connects(X, Z, _), 
						not(member(Z, T)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% branch and bound search
% this search method evaluates local transistions but always evaluates, in any given moment, whether
% it would be better, or not, to change which node to expand; this evaluation is based on the accumulated
% strength from the root node until the evaluated node (this method will allow to evaluate the path, in the 
% social graph, that guarantees that is going through the "strongest" relationships)

branch_and_bound(PersonA, PersonB, List) :- branch_and_bound_aux([(0, [PersonA])], PersonB, Path), reverse(Path, List).

branch_and_bound_aux([(_, First) | _], PersonB, First) :- First = [PersonB | _].

branch_and_bound_aux([(_, [PersonB | _]) | Remaining], PersonB, Path) :- !, branch_and_bound_aux(Remaining, PersonB, Path).

branch_and_bound_aux([(Cost, [Last | Tail]) | Others], PersonB, Path) :- 
					findall((CostC, [PersonZ, Last | Tail]),
					(next_node_b_b(Last, Tail, PersonZ, CostC1), CostC is CostC1 + Cost), List),
					append(Others, List, PathN),
					sort(PathN, PathN1),
					branch_and_bound_aux(PathN1, PersonB, Path).

next_node_b_b(PersonX, List, PersonZ, Cost) :- connects(PersonX, PersonZ, Cost), not(member(PersonZ, List)).

