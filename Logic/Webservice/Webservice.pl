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



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Implementaion
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% initiates server at given port
server(Port) :-                                         
        http_server(http_dispatch, [port(Port)]).


% creating a json object
:- json_object path(path:list, status:atom).
:- json_object error(details:atom, status:atom).


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
                prolog_to_json(error('path not found', 'error'), JSON_Object)),
        reply_json(JSON_Object).

% breath search
breadth_search(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
                (breadth_search(PersonA, PersonB, List),
                prolog_to_json(path(List, 'ok'), JSON_Object);
                prolog_to_json(error('path not found', 'error'), JSON_Object)), 
        reply_json(JSON_Object).

% branch and bound
branch_and_bound(_Request) :-
                http_parameters(_Request, [personA(PersonA, []), personB(PersonB, [])]),
               (branch_and_bound(PersonA, PersonB, List),
                prolog_to_json(path(List, 'ok'), JSON_Object);
                prolog_to_json(error('path not found', 'error'), JSON_Object)), 
        reply_json(JSON_Object).