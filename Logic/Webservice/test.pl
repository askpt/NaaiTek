:- use_module(library(http/thread_httpd)).
:- use_module(library(http/http_dispatch)).

:- http_handler(root(hello_lapr5), say_hi, []).		

server(Port) :-						
        http_server(http_dispatch, [port(Port)]).

say_hi(_Request) :-					
        format('Content-type: text/plain~n~n'),
        format('Hello LAPR5!~n').