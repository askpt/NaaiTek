% Knowledge base

% Specifications for OSX
% launch swi-prolog on terminal
% /opt/local/bin/swipl

% load this file 
% ['/Users/joaocarreira/Desktop/NaaiTek/Logic/KnowledgeBase.pl'].

% connects(Person_A, Person_B, Connection_Strength, [tags])
% this rule establises a connection between two people, with a strength ranging from 1 to 3
%
%
% examples:
% John and Peter, as brothers
% connects(John, Peter, 3)
%
% Jack and Beth as friends
% connects(Jack, Beth, 2)
%
% Mario and Susie as work colleagues
% connects(Mario, Susie, 1)


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% KnowledgeBase bootstrap
connects('Ana', 'Vicky', 3, ['dog']).
connects('Ana', 'Joao', 3, ['family']).
connects('Joao', 'Vicky', 3, ['animal']).
connects('Joao', 'Simao', 1, ['C#']).
connects('Joao', 'Artur', 2, ['CSharp']).
connects('Joao', 'Tiago', 2, ['CS']).
connects('Joao', 'Andre', 2, ['phone']).
connects('Joao', 'Diogo', 1, ['cat']).
connects('Andre', 'Simao', 1, ['smartphone','CS']).
connects('Andre', 'Catarina', 1, ['WP']).
connects('Andre', 'Artur', 2, ['animal']).
connects('Andre', 'Tiago', 2, ['Android']).
connects('Andre', 'Diogo', 1, ['CS']).
connects('Andre', 'Stephanie', 1, ['family']).
connects('Artur', 'Alcides', 1, ['cat']).
connects('Alcides', 'Nando', 1, ['family']).
connects('Tiago', 'Marcia', 1, ['family']).
connects('Tiago', 'Diogo', 1, ['C#']).
connects('Diogo', 'Francisco', 1, ['family']).
connects('Francisco', 'JoseCid', 1, ['dummyphone']).
connects('Diogo', 'Stephanie', 3, ['phone']).
connects('Stephanie', 'Maria', 3, ['family']).
connects('Tiago', 'Stephanie', 1, ['dog']).
connects('Simao', 'Artur', 1, ['WP']).
connects('Artur', 'Tiago',2, ['CS']).

