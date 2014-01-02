%receive
%Table = old table
%Play = Place where Player plays
%Return = Table after plays

receive(Table,Play,Return):-
	(integer(Play),
	processa(Table, 'Humano', 'X', Play, Return));
	Return = Table.


% Retorna o valor de uma casa.
%   Tabuleiro - tabuleiro.
%   Posicao   - posicao.
%   Retorno   - valor da posicao.
retornaCasa(Tabuleiro, Posicao, Retorno) :-
    converte(Posicao, SPosicao),                    % converte a posicao de entrada para sua representacao no tabuleiro.
    retorna_valor_na_linha(Tabuleiro, SPosicao, Retorno).       % retorna o valor da casa.

% Converte a posicao de entrada em sua representacao no tabuleiro.
%   Posicao     -  posicao de 1 .. 9.
%   [Linha, Coluna] -  representacoa da posicao em uma matriz 3x3.
converte(Posicao, [Linha, Coluna]) :-
    Temp is Posicao - 1,            % diminui a posicao em 1.
    Linha  is 1 + (Temp // 3),          % temp // 3 retorna a linha.
    Coluna is 1 + (Temp mod 3).         % temp mod 3 retorna a coluna.

retorna_valor_na_linha([Linha1, Linha2, Linha3], [1, Coluna], Retorno) :- retorna_valor_na_coluna(Linha1, Coluna, Retorno). % sempre retorna a primeira linha em array de 3 linhas.
retorna_valor_na_linha([Linha1, Linha2, Linha3], [2, Coluna], Retorno) :- retorna_valor_na_coluna(Linha2, Coluna, Retorno). % sempre retorna a segunda linha em array de 3 linhas.
retorna_valor_na_linha([Linha1, Linha2, Linha3], [3, Coluna], Retorno) :- retorna_valor_na_coluna(Linha3, Coluna, Retorno). % sempre retorna a terceira linha em array de 3 linhas.

retorna_valor_na_coluna([Casa1, Casa2, Casa3], 1, Casa1). % sempre o primeiro valor em um array de 3 caracteres.
retorna_valor_na_coluna([Casa1, Casa2, Casa3], 2, Casa2). % sempre o segundo valor em um array de 3 caracteres.
retorna_valor_na_coluna([Casa1, Casa2, Casa3], 3, Casa3). % sempre o terceiro valor em um array de 3 caracteres.

% Define o valor de uma casa.
%   Tabuleiro       - tabuleiro.
%   Posicao         - posicao.
%   Valor               - novo valor.
%   NovoTabuleiro       - novo tabuleiro com os novos valores definidos.
defineCasa(Tabuleiro, Posicao, Valor, NovoTabuleiro) :-
    converte(Posicao, SPosicao),
    define_valor_na_linha(Tabuleiro, SPosicao, Valor, NovoTabuleiro).

define_valor_na_linha([Linha1, Linha2, Linha3], [1, Coluna], Valor, [Retorno, Linha2, Linha3]) :- define_valor_na_coluna(Linha1, Coluna, Valor, Retorno). % sempre define a primeira linha em array de 3 linhas.
define_valor_na_linha([Linha1, Linha2, Linha3], [2, Coluna], Valor, [Linha1, Retorno, Linha3]) :- define_valor_na_coluna(Linha2, Coluna, Valor, Retorno). % sempre define a segunda linha em array de 3 linhas.
define_valor_na_linha([Linha1, Linha2, Linha3], [3, Coluna], Valor, [Linha1, Linha2, Retorno]) :- define_valor_na_coluna(Linha3, Coluna, Valor, Retorno). % sempre define a terceira linha em array de 3 linhas.

define_valor_na_coluna([Coluna1, Coluna2, Coluna3], 1, Valor, [Valor, Coluna2, Coluna3]). % sempre o primeiro valor em um array de 3 caracteres.
define_valor_na_coluna([Coluna1, Coluna2, Coluna3], 2, Valor, [Coluna1, Valor, Coluna3]). % sempre o segundo valor em um array de 3 caracteres.
define_valor_na_coluna([Coluna1, Coluna2, Coluna3], 3, Valor, [Coluna1, Coluna2, Valor]). % sempre o terceiro valor em um array de 3 caracteres.

% fim da implementacao do tabuleiro.

% implementacao da logica do jogo

% Responsavel pelo loop principal do jogo.
% checa se � gameover se n�o faz a proxima jogada acontecer.
% � chamada recursivamente.
% recebe um tabuleiro e retorna um novo tabuleiro.
%   TabuleiroAntigo - tabuleiro antes.
%   TabuleiroNovo   - tabuleiro novo.
%
processa(TabuleiroAntigo, _, _, _, TabuleiroNovo) :-
    %%gameover(TabuleiroAntigo), !,       % checa se � gameover.
    TabuleiroNovo = TabuleiroAntigo.        % se o tabuleiro anterior ao processamento � igual ao novo.

% Responsavel pelo loop principal do jogo.
% checa se � gameover se n�o faz a proxima jogada acontecer.
% � chamada recursivamente.
% recebe um tabuleiro e retorna um novo tabuleiro.
%   TabuleiroAntigo - tabuleiro antes.
%   TabuleiroNovo   - tabuleiro novo.
%   Turno       - Humano ou Maquina.
%   Valor       - valor Humano - x Maquina - 0.
processa(TabuleiroAntigo, Turno, Valor, TempPos, TabuleiroNovo) :-
    passaVez(TabuleiroAntigo, Turno, Valor, Posicao, TempPos),           % passa a vez para o proximo jogador.
    defineCasa(TabuleiroAntigo, Posicao, Valor, TempTabuleiro),     % define o valor na casa.
    trocar(Valor, NovoValor),                       % troca o valor.
    trocar(Turno, NovoTurno),                       % troca o jogador.
    (Turno='Humano', processa(TempTabuleiro, NovoTurno, NovoValor, _, TabuleiroNovo)   % proximo processamento.
    ;
    receive(TempTabuleiro, _,_)).
    %check if two turns have passed, if so, send the new board to TabuleiroNovo

% Troca o Valor.
%
trocar('X', 'O').

% Troca o Valor.
%
trocar('O', 'X').

% Troca o Valor.
%
trocar('Maquina', 'Humano').

% Troca o Valor.
%
trocar('Humano', 'Maquina').

% Resposanvel por identifcar de quem � a vez. Se for a vez do Humano
% chama o pegaMovimentoValido. Se for a vez da maquina chama resposta.
%   Tabuleiro   - tabuleiro do jogo.
%   'Humano'    - jogador.
%   Retorno     - posicao jogada.
%
passaVez(Tabuleiro, 'Humano', _, Retorno, TempPos) :- pegaMovimentoValido(Retorno, Tabuleiro, TempPos).


% Resposanvel por identifcar de quem � a vez. Se for a vez do Humano
% chama o pegaMovimentoValido. Se for a vez da maquina chama resposta.
%   Tabuleiro   - tabuleiro do jogo.
%   'Maquina'   - jogador.
%   Retorno     - posicao jogada.
%
passaVez(Tabuleiro, 'Maquina', Valor, Retorno,_) :-
    resposta(Tabuleiro, Valor, Retorno).                % chama o metodo que computa a jogada da maquina.

% Ler a entrada do jogador Humano e verifica se � uma jogada valida.
%   Posicao     - posicao.
%   Tabuleiro   - tabuleiro do jogo.
pegaMovimentoValido(Posicao, Tabuleiro, TempPos) :-
 not(casaJaMarcada(Tabuleiro, TempPos)), Posicao is TempPos.

% fim da implementacao da logica do jogo.

% implementacao da ia da maquina

% definicoes: (a ordem � significatne)
%    tenta ganhar.
%    tenta bloquiar a vitoria do oponente.
%    tenta pegar o centro.
%    tenta pegar as quinas, na ordem [1,3,7,9]
%    tenta pegar as bordas, na ordem [2,4,6,8]


% responsavel por processar a jogada da maquina.
%   Tabuleiro - tabuleiro do jogo.
%   Valor     - 'O'
%   X     - usado para escrever a jogada na tela.
resposta(Tabuleiro, Valor, X) :-
    melhorMovimento(Tabuleiro, Valor, X),   % decide qual o melhor movimento obdecendo as ordens definidas.
    not(casaJaMarcada(Tabuleiro, X)).    % verifica se a casa ja n�o esta marcada
%%    print('melhorMovimento='),print(X),nl.  % ecreve o movimento.

% definicoes das regras da maquina.
%
melhorMovimento(Tabuleiro, Valor, X) :- tentarGanhar(Tabuleiro, Valor, X), print('tentarGanhar='),print(X),nl.
melhorMovimento(Tabuleiro, Valor, X) :- tentardDefender(Tabuleiro, Valor, X), print('tentarDefender='),print(X),nl.
melhorMovimento(Tabuleiro, _, X) :- tentarCentro(Tabuleiro, X), print('tentarCentro='),print(X),nl.
melhorMovimento(Tabuleiro, _, X) :- tentarQuina(Tabuleiro, X), print('tentarQuina='),print(X),nl.
melhorMovimento(Tabuleiro, _, X) :- tentarBordas(Tabuleiro, X), print('tentarBordas='),print(X),nl.

tentarGanhar(Tabuleiro, Valor, 1) :- arrisca(Tabuleiro, Valor, 1).
tentarGanhar(Tabuleiro, Valor, 2) :- arrisca(Tabuleiro, Valor, 2).
tentarGanhar(Tabuleiro, Valor, 3) :- arrisca(Tabuleiro, Valor, 3).
tentarGanhar(Tabuleiro, Valor, 4) :- arrisca(Tabuleiro, Valor, 4).
tentarGanhar(Tabuleiro, Valor, 5) :- arrisca(Tabuleiro, Valor, 5).
tentarGanhar(Tabuleiro, Valor, 6) :- arrisca(Tabuleiro, Valor, 6).
tentarGanhar(Tabuleiro, Valor, 7) :- arrisca(Tabuleiro, Valor, 7).
tentarGanhar(Tabuleiro, Valor, 8) :- arrisca(Tabuleiro, Valor, 8).
tentarGanhar(Tabuleiro, Valor, 9) :- arrisca(Tabuleiro, Valor, 9).

arrisca(Tabuleiro, Valor, X) :-
    defineCasa(Tabuleiro, X, Valor, TempTabuleiro),
    fezLinha(TempTabuleiro).

tentardDefender(Tabuleiro, Valor, 1) :- barra(Tabuleiro, Valor, 1).
tentardDefender(Tabuleiro, Valor, 2) :- barra(Tabuleiro, Valor, 2).
tentardDefender(Tabuleiro, Valor, 3) :- barra(Tabuleiro, Valor, 3).
tentardDefender(Tabuleiro, Valor, 4) :- barra(Tabuleiro, Valor, 4).
tentardDefender(Tabuleiro, Valor, 5) :- barra(Tabuleiro, Valor, 5).
tentardDefender(Tabuleiro, Valor, 6) :- barra(Tabuleiro, Valor, 6).
tentardDefender(Tabuleiro, Valor, 7) :- barra(Tabuleiro, Valor, 7).
tentardDefender(Tabuleiro, Valor, 8):- barra(Tabuleiro, Valor, 8).
tentardDefender(Tabuleiro, Valor, 9) :- barra(Tabuleiro, Valor, 9).

barra(Tabuleiro, Valor, X) :-
    trocar(Valor, ValorNovo),
    defineCasa(Tabuleiro, X, ValorNovo, TempTabuleiro),
    fezLinha(TempTabuleiro).

tentarCentro(Tabuleiro, 5) :- retornaCasa(Tabuleiro, 5, '5').

tentarQuina(Tabuleiro, 1) :- retornaCasa(Tabuleiro, 1, '1').
tentarQuina(Tabuleiro, 3) :- retornaCasa(Tabuleiro, 3, '3').
tentarQuina(Tabuleiro, 7) :- retornaCasa(Tabuleiro, 7, '7').
tentarQuina(Tabuleiro, 9) :- retornaCasa(Tabuleiro, 9, '9').

tentarBordas(Tabuleiro, 2) :- retornaCasa(Tabuleiro, 2, '2').
tentarBordas(Tabuleiro, 4) :- retornaCasa(Tabuleiro, 4, '4').
tentarBordas(Tabuleiro, 6) :- retornaCasa(Tabuleiro, 6, '6').
tentarBordas(Tabuleiro, 8) :- retornaCasa(Tabuleiro, 8, '8').

% Verifica se o jogo acabou.
%   Alguem fez linha ou o Tabuleiro esta cheio.
%%gameover(Tabuleiro) :- tabuleiroCheio(Tabuleiro), !.
%%gameover(Tabuleiro) :- fezLinha(Tabuleiro).

% Vericia se alguem fez linha.
%   Todas as possibilidades de vitoria no jogo.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [1, 2, 3]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [4, 5, 6]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [7, 8, 9]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [1, 4, 7]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [2, 5, 8]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [3, 6, 9]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [1, 5, 9]), !.
fezLinha(Tabuleiro) :- achou(Tabuleiro, [3, 5, 7]).

% verifcia se axou a sequencia.
%   Tabuleiro - tabuleiro.
%   [Casa1, Casa2, Casa3] - valor para encontrar.
achou(Tabuleiro, [Casa1, Casa2, Casa3]) :-
    retornaCasa(Tabuleiro, Casa1, Retorno), % verifica se achou o valor1.
    retornaCasa(Tabuleiro, Casa2, Retorno), % verifica se achou o valor2.
    retornaCasa(Tabuleiro, Casa3, Retorno). % verifica se achou o valor3.

% Verifica se o todo tabuleiro ja esta marcado.
%   Tabuleiro - tabuleiro.
tabuleiroCheio(Tabuleiro) :-
    casaJaMarcada(Tabuleiro, 1),
    casaJaMarcada(Tabuleiro, 2),
    casaJaMarcada(Tabuleiro, 3),
    casaJaMarcada(Tabuleiro, 4),
    casaJaMarcada(Tabuleiro, 5),
    casaJaMarcada(Tabuleiro, 6),
    casaJaMarcada(Tabuleiro, 7),
    casaJaMarcada(Tabuleiro, 8),
    casaJaMarcada(Tabuleiro, 9).

% Verifica se j� ha uma jogada na casa.
%   Tabuleiro - tabuleiro.
%   Posicao   - posicao.
casaJaMarcada(Tabuleiro, Posicao) :-
    retornaCasa(Tabuleiro, Posicao, Retorno),   % retorna o valor da casa.
    e_uma_jogada(Retorno).          % verifica se o valor da casa � uma jogada.

% Verifica se � uma jogada
%   '0' ou 'X'.
e_uma_jogada('X').
e_uma_jogada('O').
