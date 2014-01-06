%%Hangman Game in SWI Prolog
% This top-level predicate runs the game.  It prints a
% welcome message, picks a phrase, and calls getGuess.

% Ans = Answer
% AnsList = AnswerList

hangman(Category):-
    getPhrase(Category,Ans),
    !,
    write('Welcome to hangman.'),
    nl,
    name(Ans,AnsList),
    makeBlanks(AnsList, BlankList),
    getGuess(AnsList,BlankList).

% Possible categories
category(nature).
category(physics).
category(web_sites).
category(programming_language).
category(user_added).


% Randomly returns a phrase from the list of possibilities.

getPhrase(Category,Ans):-
	category(Category),
	findall(Word,phrases(Category,Word),L),
	length(L, X),
	R is random(X),
	N is R+1,
	getNth(L, N, Ans), !.

% Asks the user for a letter guess.  Starts by writing the
% current "display phrase" with blanks, then asks for a guess and
% calls process on the guess.

getGuess(AnsList, BlankList):-
    name(BlankName, BlankList),
    write(BlankName),
    nl,
    write('Enter your guess, followed by a period and return.'),
    nl,
    read(Guess),
    !,
    name(Guess, [GuessName]),
    processGuess(AnsList,BlankList,GuessName).

% Process guess takes a list of codes representing the answer, a list of codes representing the current
% "display phrase" with blanks in it, and the code of the letter that was just guessed.  If the guess
% was right, call substitute to put the letter in the display phrase and check for a win.  Otherwise, just
% get another guess from the user.

processGuess(AnsList,BlankList,GuessName):-
    member(GuessName,AnsList),
    !,
    write('Correct!'),
    nl,
    substitute(AnsList, BlankList, GuessName, NewBlanks),
    checkWin(AnsList,NewBlanks).

processGuess(AnsList, BlankList,_):-
    write('Nope!'),
    nl,
    getGuess(AnsList, BlankList).

% Check to see if the phrase is guessed.  If so, write 'You win' and if not, go back and get another guess.

checkWin(AnsList, BlankList):-
    name(Ans, AnsList),
    name(BlankName, BlankList),
    BlankName = Ans,
    !,
    write('You win!').

checkWin(AnsList, BlankList):-
    !,
    getGuess(AnsList, BlankList).


% getNth(L,N,E) should be true when E is the Nth element of the list L. N will always
% be at least 1.
%
getNth([H|_],1,H).

getNth([_|T],N,E):-
    N1 is N-1,
    getNth(T,N1,E1),
    E=E1.

% makeBlanks(AnsList, BlankList) should take an answer phrase, which is a list
% of character codes that represent the answer phrase, and return a list
% where all codes but the '_' turn into the code for '*'.  The underscores
% need to remain to show where the words start and end.  Please note that
% both input and output lists for this predicate are lists of character codes.
% You can test your code with a query like this:
% testMakeBlanks:- name('csc_is_awesome', List), makeBlanks(List, BlankList), name(Towrite, BlankList), write(Towrite).

makeBlanks(AnsCodes, BlankCodes) :-
  maplist(answer_blank, AnsCodes, BlankCodes).

answer_blank(Ans, Blank) :-
  Ans == 0'_ -> Blank = Ans ; Blank = 0'* .

% substitute(AnsList, BlankList, GuessName, NewBlanks) Takes character code lists AnsList and BlankList,
% and GuessName, which is the character code for the guessed letter.  The NewBlanks should again be a
% character code list, which puts all the guesses into the display word and keeps the *'s and _'s otherwise.
% For example, if the answer is 'csc_is_awesome' and the display is 'c*c_**_*******' and the guess is 's', the
% new display should be 'csc_*s_***s***'.
% You can test your predicate with a query like this:
% testSubstitute:- name('csc_is_awesome', AnsList), name('c*c_**_*******', BlankList), name('s',[GuessName]), substitute(AnsList, BlankList, GuessName, NewBlanks),
%    name(Towrite, NewBlanks), write(Towrite).

% Also, since the predicate doesn't deal directly with character codes, this should also work:
% substitute(['c','s','c'],['c','*','c'],'s',L).  L should be ['c','s','c'].

substitute(AnsCodes, BlankCodes, GuessName, NewBlanks) :-
     maplist(place_guess(GuessName), AnsCodes, BlankCodes, NewBlanks).

place_guess(Guess, Ans, Blank, Display) :-
    Guess == Ans -> Display = Ans ; Display = Blank.


%% check if the guess belongs to the sentence
% to be called from the C++
checkIfBelongs(Guess, Ans, NewSentence):-
	name(Ans, AnsList),
	makeBlanks(AnsList, BlankList),
	name(Guess, [GuessName]),
	belongs(AnsList, BlankList, GuessName, NewSentence),
	!.

%% belongs
% returns the sentence
% if its member, returns the 'blanks' with the places filled
% if not returns the same 'blanks' list
belongs(AnsList, BlankList, GuessName, BlankName):-
    member(GuessName,AnsList),
    !,
    substitute(AnsList, BlankList, GuessName, BlankListTmp),
    name(BlankName,BlankListTmp).

belongs(_, BlankList, _, BlankName):-
	name(BlankName, BlankList).
