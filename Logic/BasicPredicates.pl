%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Generic-use predicates
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% counts the number of elements in a given list 
countElementsInList([], 0).
countElementsInList([_ | Tail], Count) :-
	countElementsInList(Tail, Sum),
	Count is Sum + 1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Specific predicates to handle graphs
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Predicate to test if personA is connected with personB

isConnected(PersonA,PersonB):-connects(PersonA,PersonB,_,_);connects(PersonB,PersonA,_,_),PersonA\=PersonB.

%Predicate to test if PersonA is colleague of PersonB

isColleague(PersonA,PersonB):-connects(PersonA,PersonB,1,_);connects(PersonB,PersonA,1,_).

%Predicate to test if PersonA is friend of personB

isFriend(PersonA,PersonB):-connects(PersonA,PersonB,2,_);connects(PersonB,PersonA,2,_).

%Predicate to test if PersonA is family of PersonB

isFamily(PersonA,PersonB):-connects(PersonA,PersonB,3,_);connects(PersonB,PersonA,3,_).

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

listCommonFriends(PersonA,PersonB,L):-findall(PersonC,(isFriend(PersonA,PersonC),isFriend(PersonB,PersonC)),L).

%Prediate to list every common family between PersonA and PersonB

listCommonFamily(PersonA,PersonB,L):-findall(PersonC,(isFamily(PersonA,PersonC),isFamily(PersonB,PersonC)),L).


connectionCost(PersonA, PersonB, Cost) :- 
	(connects(PersonA, PersonB, Cost1, _);connects(PersonB, PersonA, Cost1, _)), Cost is Cost1.


% predicate to find all connections with same tag
findAllConnectionsByTag(Tag, List):-tag(Tag, ListTag1), 
	append([Tag], ListTag1, ListTag),
	findAllConnectionsByTag_helper(ListTag, ListTemp),
	append(ListTemp, List).

findAllConnectionsByTag_helper([],[]).

findAllConnectionsByTag_helper([Head|Tail], [DefList|List]):-findall((UserA,UserB),(connects(UserA,UserB,_,ListTags),hasTag(ListTags, Head)), DefList),
	findAllConnectionsByTag_helper(Tail, List).

hasTag([Tag|_], Tag).

hasTag([_|List], Tag):-hasTag(List, Tag).

%create connections
createConnection(UserA, UserB, ConnectionStrenght):-not(isConnected(UserA, UserB)),
	assert(connects(UserA, UserB, ConnectionStrenght, [])).

%remove connections
removeConnection(UserA, UserB):-(connects(UserA, UserB, Strenght, TagList),
								retractall(connects(UserA, UserB, Strenght, TagList)));
								(connects(UserB, UserA, Strenght, TagList),
								retractall(connects(UserB, UserA, Strenght, TagList))).

%edit connection by Strenght
editConnectionByStrenght(UserA, UserB, ConnectionStr):-(connects(UserA, UserB, StrTemp, List),
	retractall(connects(UserA, UserB, StrTemp, List)),
	assert(connects(UserA, UserB, ConnectionStr, List)));
	(connects(UserB, UserA, StrTemp, List),
	retractall(connects(UserB, UserA, StrTemp, List)),
	assert(connects(UserB, UserA, ConnectionStr, List))).

%edit connection by tag (add)
editConnectionByAddTag(UserA, UserB, Tag):-(connects(UserA, UserB, Value, TempList),
	ListTag = [Tag|TempList],
	retractall(connects(UserA, UserB, Value, TempList)),
	assert(connects(UserA, UserB, Value, ListTag)));
	(connects(UserB, UserA, Value, TempList),
	ListTag = [Tag|TempList],
	retractall(connects(UserB, UserA, Value, TempList)),
	assert(connects(UserB, UserA, Value, ListTag))).

%edit connection by tag (remove)
editConnectionByRemoveTag(UserA, UserB, Tag):-(connects(UserA, UserB, Value, TempList),
	delete(TempList, Tag, ListTag),
	retractall(connects(UserA, UserB, Value, TempList)),
	assert(connects(UserA, UserB, Value, ListTag)));
	(connects(UserB, UserA, Value, TempList),
	delete(TempList, Tag, ListTag),
	retractall(connects(UserB, UserA, Value, TempList)),
	assert(connects(UserB, UserA, Value, ListTag))).

%list of all friends (3rd)
listAllNodes(User, L):-listAllConnections(User, L1), 
	listAll2ndFriends(L1, L2),
	union(L1, L2, L).

listAll2ndFriends([],[]).

listAll2ndFriends([User|T], List):-listAllConnections(User, L1),
	listAll2ndFriends(T, L2),
	union(L1,L2,List).

%list of all connections(3rd)
listAllPaths(User, L):-listAllNodes(User, Nodes),
	findall((User1, User2, S, Tag), (connects(User1, User2, S, Tag), member(User1, Nodes), member(User2, Nodes)), L).

%list of all users
listAllUsers(List):-findall(([U1, U2]), (connects(U1, U2, _, _)), L1),
	append(L1, L2),
	list_to_set(L2, List).

%list of all users (with network dimension)
listAllUsersWithDimension(List):-listAllUsers(L1),
	calculateDimension(L1, L2),
	sort(L2, L3),
	reverse(L3, List).

calculateDimension([],[]).

calculateDimension([H|T], [(S, H)|L]):- listAllNodes(H, Nodes),
	length(Nodes, S1),
	S is S1 - 1,
	calculateDimension(T, L).

%list of tag counts
getCountTags(List):-getAllTags(TagList),
	getConnectionsByTagNoSemantics(AllConnectionTagList),
	countTags(TagList, AllConnectionTagList, List).

getAllTags(List):-findall(Tag, tag(Tag, _), List).

getConnectionsByTagNoSemantics(List):-findall(L, connects(_, _, _, L), L1), append(L1, List).

countTags([], _, []).

countTags([H|T], ListToCount, [R|L]):-countInList(H, ListToCount, R),
	countTags(T, ListToCount, L).

countInList(H, List, R):-countInList_Helper(H, List, 0, S),
	R = (H, S).

countInList_Helper(_, [], N, N).

countInList_Helper(H, [H|T], N,R):- N1 is N + 1,
	countInList_Helper(H, T, N1,R).

countInList_Helper(H, [_|T], N,R):-countInList_Helper(H, T, N,R).

%list of tag counts (auth user)
getCountTagsUser(User, List):-getAllTags(TagList),
	getConnectionsByTagNoSemanticsUser(User, AllConnectionTagList),
	countTags(TagList, AllConnectionTagList, L1),
	removeZeroTag(L1, List).

getConnectionsByTagNoSemanticsUser(User, List):-listAllNodes(User, Friends),
	findall(L, (connects(U1, U2, _, L), member(U1, Friends), member(U2, Friends)), L1), 
	append(L1, List).	

removeZeroTag([],[]).

removeZeroTag([(_, 0)|T], R):-removeZeroTag(T, R).

removeZeroTag([H|T], [H|R]):-removeZeroTag(T, R).

%friend request
requestFriend(User1, User2):-not(isConnected(User1, User2)),
	 assert(pending(User1, User2, 'request')).

%friend request game response
gameResponseRequest(User1, User2):-pending(User2, User1, 'request'),
	retractall(pending(User2, User1, 'request')),
	assert(pending(User2, User1, 'game')).

%friend request accept response
acceptResponseRequest(User2, User1):-(pending(User1, User2, 'request'),!,
	retractall(pending(User1, User2, 'request')),
	assert(pending(User1, User2, 'accept')),
	assert(connects(User1, User2, 0, [])));
	(pending(User2, User1, 'game'),!,
	retractall(pending(User2, User1, 'game')),
	assert(pending(User1, User2, 'accept')),
	assert(connects(User1, User2, 0, []))).

%check the received friend requests
checkUserRequests(User, L):-findall(U, pending(U, User, 'request'), L).

%check the game response friend requests
checkGameResponseRequest(User, L):-findall(U, pending(User, U, 'game'), L).

%check friend request notifications
checkFriendRequestNot(User, L):-findall(U, pending(User, U, 'accept'), L).

%remove friend request
removeRequest(User1, User2):-(pending(User1, User2, _), 
	retractall(pending(User1, User2, _)));
	(pending(User2, User1, _),
	retractall(pending(User2, User1, _))).

%check pending user request
checkPendingRequest(User, L):-findall(U, (pending(User, U, _);
	pending(U, User, _)), L1),
	list_to_set(L1, L).
	
%save word
saveWord(Word):- not(phrases(_,Word)),
	assert(phrases('user_added',Word)).
	
%list common Nodes
listCommonNodes(User1, User2, [User1, User2 | L]):-listAllConnections(User1, F1),
	listAllConnections(User2, F2),
	intersection(F1, F2, L).

%list common connections
listCommonPaths(User1, User2, L):-listCommonNodes(User1, User2, Nodes),
	findall((U1, U2, S, Tag), (connects(U1, U2, S, Tag), member(U1, Nodes), member(U2, Nodes)), L).

%ordered list by network Strenght
networkStrenght(L):-listAllUsers(Users),
	calculateStrenght(Users, L2),
	sort(L2, L3),
	reverse(L3, L).

calculateStrenght([],[]).

calculateStrenght([H|T], [(S, H)|L]):-findall(S1, (connects(H, _, S1, _); connects(_, H, S1, _)), R),
	sumList(R, S),
	calculateStrenght(T, L).

% The sum of all elements in list in prolog

% empty list
sumList([], 0).

sumList([H | T], S) :- sumList(T, S1),
	S is H + S1.