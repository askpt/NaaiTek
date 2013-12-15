% prolog server
:- use_module(library(http/thread_httpd)).
:- use_module(library(http/http_dispatch)).

% supporting json
:- use_module(library(http/json)).
:- use_module(library(http/json_convert)).
:- use_module(library(http/http_json)).


:- http_handler(root(give_me_json), json_test, []).		


server(Port) :-						
        http_server(http_dispatch, [port(Port)]).


% creating a json object
:- json_object path(path:list).

test :-
  prolog_to_json(circle(coord(3, 5)), JSON_Object),
  format(user_output, '~w', JSON_Object).


%handle(Request) :-
        %http_read_json(Request, JSONIn),
        %json_to_prolog(JSONIn, PrologIn),
        %<compute>(PrologIn, PrologOut),		% application body
        %prolog_to_json(PrologOut, JSONOut),
        %prolog_to_json(circle(coord(3, 5)), JSON_Object),
        %reply_json(JSON_Object).

json_test(_Request) :-					
        prolog_to_json(path(['Vicky', 'Ana', 'Joao', 'Andre', 'Diogo', 'Francisco', 'JoseCid']), JSON_Object),
        reply_json(JSON_Object).