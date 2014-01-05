%receive
%Table = old table
%Play = Place where Player plays {1..9}
%Return = Table after plays

receive(Table,Play,Return):-
	(integer(Play), !,
	process(Table, 'Human', x, Play, Return));
	Return = Table.

%main_loop
process(OldBoard, Turn, Value, TempPos, NewBoard) :-
    passTurn(OldBoard, Turn, Value, Position, TempPos),
    defineSquare(OldBoard, Position, Value, TempBoard),
    change(Value, NewValue),
    change(Turn, NewTurn),
    !,
    (Turn='Human', !,
    process(TempBoard, NewTurn, NewValue,
	     _, NewBoard);
    NewBoard = TempBoard).

%responsible to pass the play (used when its a human)
passTurn(Board, 'Human', _, Return, TempPos) :-
	checkSquare(Return, Board, TempPos).

%responsible to pass the play (used when its the AI)
passTurn(Board, 'Maquina', Value, Return,_) :-
	response(Board, Value, Return).

%defines the value of a square
defineSquare(Board, Position, Value, NewBoard) :-
    converts(Position, SPosition),
    defineValueInLine(Board, SPosition, Value, NewBoard).

%returns the value of a square
returnSquare(Board, Position, Return) :-
    converts(Position, SPosition),
    returnValueInLine(Board, SPosition, Return).

%define first line
defineValueInLine([Line1, Line2, Line3], [1, Column], Value, [Return, Line2, Line3]) :-
	defineValueInColumn(Line1, Column, Value, Return).
%define second line
defineValueInLine([Line1, Line2, Line3], [2, Column], Value, [Line1, Return, Line3]) :-
	defineValueInColumn(Line2, Column, Value, Return).
%define third line
defineValueInLine([Line1, Line2, Line3], [3, Column], Value, [Line1, Line2, Return]) :-
	defineValueInColumn(Line3, Column, Value, Return).

%define first value
defineValueInColumn([_, Column2, Column3], 1, Value, [Value, Column2, Column3]).
%define second value
defineValueInColumn([Column1, _, Column3], 2, Value, [Column1, Value, Column3]).
%define third value
defineValueInColumn([Column1, Column2, _], 3, Value, [Column1, Column2, Value]).

%returns first line
returnValueInLine([Line1, _, _], [1, Column], Return) :- returnValueInColumn(Line1, Column, Return).
%returns second line
returnValueInLine([_, Line2, _], [2, Column], Return) :- returnValueInColumn(Line2, Column, Return).
%returns third line
returnValueInLine([_, _, Line3], [3, Column], Return) :- returnValueInColumn(Line3, Column, Return).

%return first value
returnValueInColumn([Square1, _, _], 1, Square1).
%return second value
returnValueInColumn([_, Square2, _], 2, Square2).
%return third value
returnValueInColumn([_, _, Square3], 3, Square3).

%check if the square isn't filled already
checkSquare(Position, Board, TempPos) :-
 not(alreadyCheckedHouse(Board, TempPos)), Position is TempPos.

%see if the house is checked
alreadyCheckedHouse(Board, Position) :-
    returnSquare(Board, Position, Return),
    check_if_play(Return).

%converts the number in matrix place
converts(Position, [Line, Column]) :-
    Temp is Position - 1,
    Line  is 1 + (Temp // 3),
    Column is 1 + (Temp mod 3).

%check_if_play
check_if_play(x).
check_if_play(o).

% change the value
change(x, o).

% change the value
change(o, x).

% change the value
change('Maquina', 'Human').

% changes the value
change('Human', 'Maquina').

%machine AI implementation

%response from the machine
response(Board, Value, X) :-
    bestMovement(Board, Value, X),
    not(alreadyCheckedHouse(Board, X)).

% machine rules definition
bestMovement(Board, Value, X) :- tryWin(Board, Value, X).
bestMovement(Board, Value, X) :- tryDefend(Board, Value, X).
bestMovement(Board, _, X) :- tryCenter(Board, X).
bestMovement(Board, _, X) :- tryCorner(Board, X).
bestMovement(Board, _, X) :- tryEdges(Board, X).

tryWin(Board, Value, 1) :- riskIt(Board, Value, 1).
tryWin(Board, Value, 2) :- riskIt(Board, Value, 2).
tryWin(Board, Value, 3) :- riskIt(Board, Value, 3).
tryWin(Board, Value, 4) :- riskIt(Board, Value, 4).
tryWin(Board, Value, 5) :- riskIt(Board, Value, 5).
tryWin(Board, Value, 6) :- riskIt(Board, Value, 6).
tryWin(Board, Value, 7) :- riskIt(Board, Value, 7).
tryWin(Board, Value, 8) :- riskIt(Board, Value, 8).
tryWin(Board, Value, 9) :- riskIt(Board, Value, 9).

riskIt(Board, Value, X) :-
    defineSquare(Board, X, Value, TempBoard),
    wonGame(TempBoard).

tryDefend(Board, Value, 1) :- bar(Board, Value, 1).
tryDefend(Board, Value, 2) :- bar(Board, Value, 2).
tryDefend(Board, Value, 3) :- bar(Board, Value, 3).
tryDefend(Board, Value, 4) :- bar(Board, Value, 4).
tryDefend(Board, Value, 5) :- bar(Board, Value, 5).
tryDefend(Board, Value, 6) :- bar(Board, Value, 6).
tryDefend(Board, Value, 7) :- bar(Board, Value, 7).
tryDefend(Board, Value, 8) :- bar(Board, Value, 8).
tryDefend(Board, Value, 9) :- bar(Board, Value, 9).

bar(Board, Value, X) :-
    change(Value, ValueNew),
    defineSquare(Board, X, ValueNew, TempBoard),
    wonGame(TempBoard).

tryCenter(Board, 5) :- returnSquare(Board, 5, 5).

tryCorner(Board, 1) :- returnSquare(Board, 1, 1).
tryCorner(Board, 3) :- returnSquare(Board, 3, 3).
tryCorner(Board, 7) :- returnSquare(Board, 7, 7).
tryCorner(Board, 9) :- returnSquare(Board, 9, 9).

tryEdges(Board, 2) :- returnSquare(Board, 2, 2).
tryEdges(Board, 4) :- returnSquare(Board, 4, 4).
tryEdges(Board, 6) :- returnSquare(Board, 6, 6).
tryEdges(Board, 8) :- returnSquare(Board, 8, 8).

% Check if wins game
%   All victory possibilities in the game
wonGame(Board) :- found(Board, [1, 2, 3]), !.
wonGame(Board) :- found(Board, [4, 5, 6]), !.
wonGame(Board) :- found(Board, [7, 8, 9]), !.
wonGame(Board) :- found(Board, [1, 4, 7]), !.
wonGame(Board) :- found(Board, [2, 5, 8]), !.
wonGame(Board) :- found(Board, [3, 6, 9]), !.
wonGame(Board) :- found(Board, [1, 5, 9]), !.
wonGame(Board) :- found(Board, [3, 5, 7]).

% Checks if found sequence
found(Board, [Square1, Square2, Square3]) :-
    returnSquare(Board, Square1, Return),
    returnSquare(Board, Square2, Return),
    returnSquare(Board, Square3, Return).
