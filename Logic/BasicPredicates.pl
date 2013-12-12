%Predicate to test if personA is connected with personB

isConnected(PersonA,PersonB):-connects(PersonA,PersonB,_);connects(PersonB,PersonA,_).

%Predicate to test if PersonA is friend of PersonB


% isFriend(PersonA,PersonB):-connects(PersonA,PersonB);connects(PersonB,PersonA).
%

%Predicate to test if PersonA is common friend of PersonB

% commonFriend(PersonA,PersonB,PersonC):-isFriend(PersonA,PersonB),isFriend(PersonB,PersonC).
%

%Predicate to build a list of every friends of a single person

%listFriends(PersonA,Friends):-


%Precicate to list every common friends between PersonA and PersonB



%Predicate to test if PersonA is family of PersonB




%Predicate to test if PersonA is colleague of PersonB

%Predicate to test if PersonA is connected to PersonB

