%Predicate to test if personA is connected with personB

isConnected(PersonA,PersonB):-connects(PersonA,PersonB,_);connects(PersonB,PersonA,_).

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







