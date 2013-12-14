%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Generic-use predicates
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% counts the number of elements in a given list 
countElementsInList([], 0).
countElementsInList([Head | Tail], Count) :-
	countElementsInList(Tail, Sum),
	Count is Sum + 1.
	

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Specific predicates to handle graphs
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Predicate to test if personA is connected with personB

isConnected(PersonA,PersonB):-connects(PersonA,PersonB,_);connects(PersonB,PersonA,_),PersonA\=PersonB.

%Predicate to test if PersonA is colleague of PersonB

isColleague(PersonA,PersonB):-connects(PersonA,PersonB,1);connects(PersonB,PersonA,1).

%Predicate to test if PersonA is friend of personB

isFriend(PersonA,PersonB):-connects(PersonA,PersonB,2);connects(PersonB,PersonA,2).

%Predicate to test if PersonA is family of PersonB

isFamily(PersonA,PersonB):-connects(PersonA,PersonB,3);connects(PersonB,PersonA,3).

%Predicate to list all connections of a certain person

listAllConnections(PersonA,L):-findall(PersonB,(isConnected(PersonA,PersonB)),L).

%Predicate to list all colleagues of a certain person

listAllColleagues(PersonA,L):-findall(PersonB,(isColleague(PersonA,PersonB)),L).

%Predicate to list all friends of a certain person

listAllFriends(PersonA,L):-findall(PersonB,(isFriend(PersonA,PersonB)),L).

%Predicate to list every family members of a certain person

listAllFamily(PersonA,L):-findall(PersonB,(isFamily(PersonA,PersonB)),L).

%Predicate to test if PersonA has common connections with personB

hasCommonConnections(PersonA,PersonB):-isConnected(PersonA,PersonC),isConnected(PersonB,PersonC).

%Predicate to test if PersonA has common colleagues with personB

hasCommonColleagues(PersonA,PersonB):-isColleague(PersonA,PersonC),isColleague(PersonB,PersonC).

%Predicate to test if PersonA has common friends with PersonB

hasCommonFriends(PersonA,PersonB):-isFriend(PersonA,PersonC),isFriend(PersonB,PersonC).

%Predicate to test if PersonA has common family with PersonB

hasCommonFamily(PersonA,PersonB):-isFamily(PersonA,PersonC),isFamily(PersonB,PersonC).

%Predicate to search common elements between two different lists

intersection([],_,[]).
intersection([X|L1],L2,[X|LI]):-member(X,L2),!,intersection(L1,L2,LI).
intersection([_|L1],L2,LI):-intersection(L1,L2,LI).

% Predicate to list every common connections between two persons
% (PersonA and PersonB)

listCommonConnections(PersonA,PersonB,L):-findall(PersonC,(isConnected(PersonA,PersonC),isConnected(PersonB,PersonC)),L).

%Predicate to list every common colleagues between PersonA and PersonB

listCommonColleagues(PersonA,PersonB,L):-findall(PersonC,(isColleague(PersonA,PersonC),isColleague(PersonB,PersonC)),L).

%Precicate to list every common friends between PersonA and PersonB

listCommonFrieds(PersonA,PersonB,L):-findall(PersonC,(isFriend(PersonA,PersonC),isFriend(PersonB,PersonC)),L).

%Prediate to list every common family between PersonA and PersonB

listCommonFamily(PersonA,PersonB,L):-findall(PersonC,(isFamily(PersonA,PersonC),isFamily(PersonB,PersonC)),L).






