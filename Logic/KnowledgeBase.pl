% Knowledge base

% Specifications for OSX
% launch swi-prolog on terminal
% /opt/local/bin/swipl

% load this file 
% ['/Users/joaocarreira/Desktop/NaaiTek/Logic/KnowledgeBase.pl'].

% connects(Person_A, Person_B, Connection_Strength)
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
connects('Ana', 'Vicky', 3).
connects('Ana', 'Joao', 3).
connects('Joao', 'Vicky', 3).
connects('Joao', 'Simao', 1).
connects('Joao', 'Artur', 2).
connects('Joao', 'Tiago', 2).
connects('Joao', 'Andre', 2).
connects('Joao', 'Diogo', 1).
connects('Andre', 'Simao', 1).
connects('Andre', 'Catarina', 1).
connects('Andre', 'Artur', 2).
connects('Andre', 'Tiago', 2).
connects('Andre', 'Diogo', 1).
connects('Andre', 'Stephanie', 1).
connects('Artur', 'Alcides', 1).
connects('Alcides', 'Nando', 1).
connects('Tiago', 'Marcia', 1).
connects('Tiago', 'Diogo', 1).
connects('Diogo', 'Francisco', 1).
connects('Francisco', 'JoseCid', 1).
connects('Diogo', 'Stephanie', 3).
connects('Stephanie', 'Maria', 3).
connects('Tiago', 'Stephanie', 1).
connects('Simao', 'Artur', 1).
connects('Artur', 'Tiago',2).

