%receive
%Table = old table
%Play = Place where Player plays {1..9}
%Return = Table after plays

receive(Table,Play,Return):-
	(integer(Play), !,
	process(Table, 'Humano', 'X', Play, Return));
	Return = Table.

%main_loop
process(TabuleiroAntigo, Turno, Valor, TempPos, TabuleiroNovo) :-
    passTurn(TabuleiroAntigo, Turno, Valor, Posicao, TempPos),
    defineSquare(TabuleiroAntigo, Posicao, Valor, TempTabuleiro),
    change(Valor, NovoValor),
    change(Turno, NovoTurno),
    !,
    (Turno='Humano', !,
    process(TempTabuleiro, NovoTurno, NovoValor,
	     _, TabuleiroNovo);
    TabuleiroNovo = TempTabuleiro).

%responsible to pass the play (used when its a human)
passTurn(Tabuleiro, 'Humano', _, Retorno, TempPos) :-
	checkSquare(Retorno, Tabuleiro, TempPos).

%responsible to pass the play (used when its the AI)
passTurn(Tabuleiro, 'Maquina', Valor, Retorno,_) :-
	response(Tabuleiro, Valor, Retorno).

%defines the value of a square
defineSquare(Tabuleiro, Posicao, Valor, NovoTabuleiro) :-
    converts(Posicao, SPosicao),
    defineValueInLine(Tabuleiro, SPosicao, Valor, NovoTabuleiro).

%returns the value of a square
returnSquare(Tabuleiro, Posicao, Retorno) :-
    converts(Posicao, SPosicao),
    returnValueInLine(Tabuleiro, SPosicao, Retorno).

%define first line
defineValueInLine([Linha1, Linha2, Linha3], [1, Coluna], Valor, [Retorno, Linha2, Linha3]) :-
	defineValueInColumn(Linha1, Coluna, Valor, Retorno).
%define second line
defineValueInLine([Linha1, Linha2, Linha3], [2, Coluna], Valor, [Linha1, Retorno, Linha3]) :-
	defineValueInColumn(Linha2, Coluna, Valor, Retorno).
%define third line
defineValueInLine([Linha1, Linha2, Linha3], [3, Coluna], Valor, [Linha1, Linha2, Retorno]) :-
	defineValueInColumn(Linha3, Coluna, Valor, Retorno).

%define first value
defineValueInColumn([_, Coluna2, Coluna3], 1, Valor, [Valor, Coluna2, Coluna3]).
%define second value
defineValueInColumn([Coluna1, _, Coluna3], 2, Valor, [Coluna1, Valor, Coluna3]).
%define third value
defineValueInColumn([Coluna1, Coluna2, _], 3, Valor, [Coluna1, Coluna2, Valor]).

%returns first line
returnValueInLine([Linha1, _, _], [1, Coluna], Retorno) :- returnValueInColumn(Linha1, Coluna, Retorno).
%returns second line
returnValueInLine([_, Linha2, _], [2, Coluna], Retorno) :- returnValueInColumn(Linha2, Coluna, Retorno).
%returns third line
returnValueInLine([_, _, Linha3], [3, Coluna], Retorno) :- returnValueInColumn(Linha3, Coluna, Retorno).

%return first value
returnValueInColumn([Casa1, _, _], 1, Casa1).
%return second value
returnValueInColumn([_, Casa2, _], 2, Casa2).
%return third value
returnValueInColumn([_, _, Casa3], 3, Casa3).

%check if the square isn't filled already
checkSquare(Posicao, Tabuleiro, TempPos) :-
 not(alreadyCheckedHouse(Tabuleiro, TempPos)), Posicao is TempPos.

%see if the house is checked
alreadyCheckedHouse(Tabuleiro, Posicao) :-
    returnSquare(Tabuleiro, Posicao, Retorno),
    check_if_play(Retorno).

%converts the number in matrix place
converts(Posicao, [Linha, Coluna]) :-
    Temp is Posicao - 1,
    Linha  is 1 + (Temp // 3),
    Coluna is 1 + (Temp mod 3).

%check_if_play
check_if_play('X').
check_if_play('O').

% change the value
change('X', 'O').

% change the value
change('O', 'X').

% change the value
change('Maquina', 'Humano').

% changes the value
change('Humano', 'Maquina').

%machine AI implementation

%response from the machine
response(Tabuleiro, Valor, X) :-
    bestMovement(Tabuleiro, Valor, X),
    not(alreadyCheckedHouse(Tabuleiro, X)).

% machine rules definition
bestMovement(Tabuleiro, Valor, X) :- tryWin(Tabuleiro, Valor, X).
bestMovement(Tabuleiro, Valor, X) :- tryDefend(Tabuleiro, Valor, X).
bestMovement(Tabuleiro, _, X) :- tryCenter(Tabuleiro, X).
bestMovement(Tabuleiro, _, X) :- tryCorner(Tabuleiro, X).
bestMovement(Tabuleiro, _, X) :- tryEdges(Tabuleiro, X).

tryWin(Tabuleiro, Valor, 1) :- riskIt(Tabuleiro, Valor, 1).
tryWin(Tabuleiro, Valor, 2) :- riskIt(Tabuleiro, Valor, 2).
tryWin(Tabuleiro, Valor, 3) :- riskIt(Tabuleiro, Valor, 3).
tryWin(Tabuleiro, Valor, 4) :- riskIt(Tabuleiro, Valor, 4).
tryWin(Tabuleiro, Valor, 5) :- riskIt(Tabuleiro, Valor, 5).
tryWin(Tabuleiro, Valor, 6) :- riskIt(Tabuleiro, Valor, 6).
tryWin(Tabuleiro, Valor, 7) :- riskIt(Tabuleiro, Valor, 7).
tryWin(Tabuleiro, Valor, 8) :- riskIt(Tabuleiro, Valor, 8).
tryWin(Tabuleiro, Valor, 9) :- riskIt(Tabuleiro, Valor, 9).

riskIt(Tabuleiro, Valor, X) :-
    defineSquare(Tabuleiro, X, Valor, TempTabuleiro),
    wonGame(TempTabuleiro).

tryDefend(Tabuleiro, Valor, 1) :- bar(Tabuleiro, Valor, 1).
tryDefend(Tabuleiro, Valor, 2) :- bar(Tabuleiro, Valor, 2).
tryDefend(Tabuleiro, Valor, 3) :- bar(Tabuleiro, Valor, 3).
tryDefend(Tabuleiro, Valor, 4) :- bar(Tabuleiro, Valor, 4).
tryDefend(Tabuleiro, Valor, 5) :- bar(Tabuleiro, Valor, 5).
tryDefend(Tabuleiro, Valor, 6) :- bar(Tabuleiro, Valor, 6).
tryDefend(Tabuleiro, Valor, 7) :- bar(Tabuleiro, Valor, 7).
tryDefend(Tabuleiro, Valor, 8):- bar(Tabuleiro, Valor, 8).
tryDefend(Tabuleiro, Valor, 9) :- bar(Tabuleiro, Valor, 9).

bar(Tabuleiro, Valor, X) :-
    change(Valor, ValorNovo),
    defineSquare(Tabuleiro, X, ValorNovo, TempTabuleiro),
    wonGame(TempTabuleiro).

tryCenter(Tabuleiro, 5) :- returnSquare(Tabuleiro, 5, '5').

tryCorner(Tabuleiro, 1) :- returnSquare(Tabuleiro, 1, '1').
tryCorner(Tabuleiro, 3) :- returnSquare(Tabuleiro, 3, '3').
tryCorner(Tabuleiro, 7) :- returnSquare(Tabuleiro, 7, '7').
tryCorner(Tabuleiro, 9) :- returnSquare(Tabuleiro, 9, '9').

tryEdges(Tabuleiro, 2) :- returnSquare(Tabuleiro, 2, '2').
tryEdges(Tabuleiro, 4) :- returnSquare(Tabuleiro, 4, '4').
tryEdges(Tabuleiro, 6) :- returnSquare(Tabuleiro, 6, '6').
tryEdges(Tabuleiro, 8) :- returnSquare(Tabuleiro, 8, '8').

% Check if wins game
%   All victory possibilities in the game
wonGame(Tabuleiro) :- found(Tabuleiro, [1, 2, 3]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [4, 5, 6]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [7, 8, 9]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [1, 4, 7]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [2, 5, 8]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [3, 6, 9]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [1, 5, 9]), !.
wonGame(Tabuleiro) :- found(Tabuleiro, [3, 5, 7]).

% Checks if found sequence
found(Tabuleiro, [Casa1, Casa2, Casa3]) :-
    returnSquare(Tabuleiro, Casa1, Retorno),
    returnSquare(Tabuleiro, Casa2, Retorno),
    returnSquare(Tabuleiro, Casa3, Retorno).
