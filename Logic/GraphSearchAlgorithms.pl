% Algorithms for graph search

% load this file 
% ['/Users/joaocarreira/Desktop/NaaiTek/Logic/GraphSearchAlgorithms.pl'].

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
