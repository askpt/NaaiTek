%receive
%Table = old table
%Play = Place where Player plays
%Return = Table after plays

receive(Table,Play,Return):-
	(integer(Play),
	process(Table, 'Human', 'X', Play, Return));
	Return = Table.

%check_if_game_over
process(OldBoard, _, _, _, NewBoard) :-
    gameover(OldBoard), !,
    NewBoard = OldBoard.

%main_loop
process(TabuleiroAntigo, Turno, Valor, TempPos, TabuleiroNovo) :-
    passaVez(TabuleiroAntigo, Turno, Valor, Posicao, TempPos),	     defineCasa(TabuleiroAntigo, Posicao, Valor, TempTabuleiro),      trocar(Valor, NovoValor),
    trocar(Turno, NovoTurno),
    (Turno='Humano',
    processa(TempTabuleiro, NovoTurno, NovoValor,
	     _, TabuleiroNovo);
    receive(TempTabuleiro, _,_)
    ).
