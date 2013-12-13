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
% this predicates gets a possible path between PersonA and PersonB,
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


