% Tag Semantics 
%
%
% tag('C#', ['CSharp', 'CS']).
% C# is related to CSharp and CS.

tag('C#', ['CSharp', 'CS']).
tag('CSharp', ['C#', 'CS']).
tag('CS', ['CSharp', 'C#']).

tag('phone', ['smartphone', 'dummyphone']).
tag('smartphone', ['WP', 'phone']).
tag('dummyphone', ['Android', 'phone']).

tag('animal', ['dog','cat']).
tag('cat',['animal']).
tag('dog',['animal']).

tag('family',[]).

tag('WP', ['smartphone', 'phone']).
tag('Android', ['dummyphone', 'phone']).