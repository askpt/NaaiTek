%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Libraries and compiles
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% prolog server
:- use_module(library(http/thread_httpd)).
:- use_module(library(http/http_dispatch)).
:- use_module(library(http/http_parameters)).

% supporting json
:- use_module(library(http/json)).
:- use_module(library(http/json_convert)).
:- use_module(library(http/http_json)).

% needed compiles
:- compile('../LoadFiles.pl').



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Declaration of API calls
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% test request
:- http_handler(root(give_me_json), give_me_json, []).          

% request for depth search on graph
:- http_handler(root(depth_search), depth_search, []).

% request for breath search on graph
:- http_handler(root(breadth_search), breadth_search, []).

% request for branch and bound search on graph
:- http_handler(root(branch_and_bound), branch_and_bound, []).

% request for create connection
:- http_handler(root(create_connection), create_connection, []).

% request for remove connection
:- http_handler(root(remove_connection), remove_connection, []).

% request for update connection strenght
:- http_handler(root(update_connection_strenght), update_connection_strenght, []).

% request for add connection tag
:- http_handler(root(add_connection_tag), add_connection_tag, []).

% request for remove connection tag
:- http_handler(root(remove_connection_tag), remove_connection_tag, []).

% request for get nodes
:- http_handler(root(get_nodes), get_nodes, []).

% request for get paths
:- http_handler(root(get_paths), get_paths, []).

% request for get graph
:- http_handler(root(get_graph), get_graph, []).

% request for get all users
:- http_handler(root(get_users), get_users, []).

% request for get all users with dimension
:- http_handler(root(get_users_dimension), get_users_dimension, []).

% request to get all users with strenght
:- http_handler(root(get_users_strenght), get_users_strenght, []).

% request for get tag count
:- http_handler(root(get_tag_count), get_tag_count, []).

% request for get user tag count
:-http_handler(root(get_user_tag_count), get_user_tag_count, []).

% request for make a request friend
:-http_handler(root(request_friend), request_friend, []).

% request for response with game 
:-http_handler(root(game_response), game_response, []).

% request for accept friend request
:-http_handler(root(accept_response), accept_response, []).

% request to check the user friend requests
:-http_handler(root(check_requests), check_requests, []).

% request to check the game requests
:-http_handler(root(check_game_requests), check_game_requests, []).

% request to check the friend notifications
:-http_handler(root(check_friend_notifications), check_friend_notifications, []).

% request to remove friend requests
:-http_handler(root(remove_request), remove_request, []).

% request to get all tags
:-http_handler(root(get_all_tags), get_all_tags, []).

% request to get all pending requests
:- http_handler(root(get_all_pending_requests), get_all_pending_requests, []).

% request for adding a word to the hangman minigame
:- http_handler(root(add_word_hangman), add_word_hangman, []).

% request for get common friends graph
:- http_handler(root(get_common_graph), get_common_graph, []).

% request the user_added words from the server
:- http_handler(root(get_user_added), get_user_added, []).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Implementaion
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% initiates server at given port
server(Port) :-                                         
        http_server(http_dispatch, [port(Port)]).


% creating a json object
:- json_object path(path:list, status:atom).
:- json_object message(details:atom, status:atom).
:- json_object nodes(nodes:list, status:atom).

:- json_object allpaths(paths:list, status:atom).
:- json_object paths(user1:atom, user2:atom, connection:number, tags:list).

:- json_object graph(nodes:list, paths:list, status:atom).

:- json_object users(users:list, status:atom).

:- json_object user(user:atom, dimension:number).

:- json_object tags(tags:list, nr_connections:number, status:atom).
:- json_object tag(tag:atom, count:number).

:- json_object tags(tags:list, status:atom).
%test handler
%handle(Request) :-
        %http_read_json(Request, JSONIn),
        %json_to_prolog(JSONIn, PrologIn),
        %<compute>(PrologIn, PrologOut),                % application body
        %prolog_to_json(PrologOut, JSONOut),
        %prolog_to_json(circle(coord(3, 5)), JSON_Object),
        %reply_json(JSON_Object).


% test request
give_me_json(_Request) :-
                %http_parameters(_Request, [name(Module, [])]),                                 
        prolog_to_json(path(['Vicky', 'Ana', 'Joao', 'Andre', 'Diogo', 'Francisco', 'JoseCid'], 'ok'), JSON_Object),
        reply_json(JSON_Object).


% depth search
depth_search(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),        
                (depth_search(PersonA, PersonB, List),
                prolog_to_json(path(List, 'ok'), JSON_Object);
                prolog_to_json(message('path not found', 'error'), JSON_Object)),
        reply_json(JSON_Object).

% breath search
breadth_search(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                (breadth_search(PersonA, PersonB, List),
                prolog_to_json(path(List, 'ok'), JSON_Object);
                prolog_to_json(message('path not found', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

% branch and bound
branch_and_bound(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
               (branch_and_bound(PersonA, PersonB, List),
                prolog_to_json(path(List, 'ok'), JSON_Object);
                prolog_to_json(message('path not found', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

%create connection
create_connection(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, []), strenght(Strenght, [])]),
               (createConnection(PersonA, PersonB, Strenght),
                prolog_to_json(message('connection created', 'ok'), JSON_Object);
                prolog_to_json(message('connection not created', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

%remove connection
remove_connection(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
               (removeConnection(PersonA, PersonB),
                prolog_to_json(message('connection removed', 'ok'), JSON_Object);
                prolog_to_json(message('connection not removed or not exists', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

%update connection strenght
update_connection_strenght(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, []), strenght(Strenght, [])]),
                atom_number(Strenght, Number),
               (editConnectionByStrenght(PersonA, PersonB, Number),
                prolog_to_json(message('connection edited', 'ok'), JSON_Object);
                prolog_to_json(message('connection not edited or not exists', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

%add connection tag
add_connection_tag(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, []), tag(Tag, [])]),
               (editConnectionByAddTag(PersonA, PersonB, Tag),
                prolog_to_json(message('connection edited', 'ok'), JSON_Object);
                prolog_to_json(message('connection not edited or not exists', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

%remove connection tag
remove_connection_tag(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, []), tag(Tag, [])]),
               (editConnectionByRemoveTag(PersonA, PersonB, Tag),
                prolog_to_json(message('connection edited', 'ok'), JSON_Object);
                prolog_to_json(message('connection not edited or not exists', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

%get user nodes
get_nodes(_Request) :-  http_parameters(_Request, [user(User, [])]),
                listAllNodes(User, List),
                prolog_to_json(nodes(List, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%get user paths
get_paths(_Request) :-  http_parameters(_Request, [user(User, [])]),
                listAllPaths(User, List),
                create_json_paths(List, ListJson),
                prolog_to_json(allpaths(ListJson, 'ok'), JSON_Object),
                reply_json(JSON_Object).

create_json_paths([],[]).

create_json_paths([(U1, U2, S, L)|T], [Json_ret|Lj]):-prolog_to_json(paths(U1,U2,S,L), Json_ret),
    create_json_paths(T, Lj).

%get user graph
get_graph(_Request) :-  http_parameters(_Request, [user(User, [])]),
                listAllPaths(User, List),
                create_json_paths(List, ListJsonP),
                listAllNodes(User, ListN),
                prolog_to_json(graph(ListN, ListJsonP, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%get all users
get_users(_Request) :- listAllUsers(List),
                prolog_to_json(users(List, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%get all users with dimension
get_users_dimension(_Request) :- listAllUsersWithDimension(List),
                create_json_users(List, List_Json),
                prolog_to_json(users(List_Json, 'ok'), JSON_Object),
                reply_json(JSON_Object).

create_json_users([],[]).

create_json_users([(S, U)|T], [Json_ret|Lj]):-prolog_to_json(user(U,S), Json_ret),
                create_json_users(T, Lj).

%get all users with strenght
get_users_strenght(_Request) :- networkStrenght(List),
                create_json_users(List, List_Json),
                prolog_to_json(users(List_Json, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%get tag count
get_tag_count(_Request):-getCountTags(L),
                create_json_tags(L, List_Json),
                count_connections(Result),
                prolog_to_json(tags(List_Json, Result, 'ok'), JSON_Object),
                reply_json(JSON_Object).

create_json_tags([],[]).

create_json_tags([(Tag, S)|T], [Json_ret|Lj]):-prolog_to_json(tag(Tag, S), Json_ret),
    create_json_tags(T, Lj).

count_connections(R):-findall((A, B), connects(A, B, _, _), L),
                length(L, R).

%get user tag count
get_user_tag_count(_Request):- http_parameters(_Request, [user(User, [])]),
                getCountTagsUser(User, L),
                create_json_tags(L, List_Json),
                count_user_connections(User, Result),
                prolog_to_json(tags(List_Json, Result, 'ok'), JSON_Object),
                reply_json(JSON_Object).

count_user_connections(U, R):-listAllPaths(U, L),
                length(L, R).

%make a friend request
request_friend(_Request):-http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                requestFriend(PersonA, PersonB),
                prolog_to_json(message('Friend requested successfully', 'ok'), JSON_Object),
                reply_json(JSON_Object).

%make a friend request game response
game_response(_Request):-http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                gameResponseRequest(PersonA, PersonB),
                prolog_to_json(message('Friend game requested successfully', 'ok'), JSON_Object),
                reply_json(JSON_Object).


%make a friend request accept response
accept_response(_Request):-http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                acceptResponseRequest(PersonA, PersonB),
                prolog_to_json(message('Friend accepted successfully', 'ok'), JSON_Object),
                reply_json(JSON_Object).


%check the received friend requests
check_requests(_Request):-http_parameters(_Request, [user(User, [])]),
                checkUserRequests(User, L),
                prolog_to_json(users(L, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%check the game response friend requests
check_game_requests(_Request):-http_parameters(_Request, [user(User, [])]),
                checkGameResponseRequest(User, L),
                prolog_to_json(users(L, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%check friend request notifications
check_friend_notifications(_Request):-http_parameters(_Request, [user(User, [])]),
                checkFriendRequestNot(User, L),
                prolog_to_json(users(L, 'ok'), JSON_Object),
                reply_json(JSON_Object).

%remove friend request
remove_request(_Request):-http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                removeRequest(PersonA, PersonB),
                prolog_to_json(message('Friend request removed successfully','ok'), JSON_Object),
                reply_json(JSON_Object).

%get all tags
get_all_tags(_Request):-getAllTags(List),
                prolog_to_json(tags(List, 'ok'), JSON_Object),
                reply_json(JSON_Object).
                
%get all pending requests
get_all_pending_requests(_Request):-http_parameters(_Request, [user(User, [])]),
                checkPendingRequest(User, List),
                prolog_to_json(users(List, 'ok'), JSON_Object),
                reply_json(JSON_Object).
				
% assert words to user added categories
add_word_hangman(_Request) :-
                http_parameters(_Request, [word(Word, [])]),        
                ((saveWord(Word),
				prolog_to_json(message('Word added successfully', 'ok'), JSON_Object));
				(prolog_to_json(message('Word exists', 'fail'), JSON_Object))),
                reply_json(JSON_Object).

% get common firends graph
get_common_graph(_Request) :-  http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                listCommonPaths(PersonA, PersonB, List),
                create_json_paths(List, ListJsonP),
                listCommonNodes(PersonA, PersonB, ListN),
                prolog_to_json(graph(ListN, ListJsonP, 'ok'), JSON_Object),
                reply_json(JSON_Object).

% request the user_added words from the server
get_user_added(_Request) :-  getPhrase(user_added, W),
                prolog_to_json(message(W,'ok'), JSON_Object),
                reply_json(JSON_Object).
