%{
#define     YYDEBUG    0
%} 
%token TYPE
%token WHILE
%token IDENTIFIER CONSTANT
%token JNZ JZ JA JB

%start unit
%%
declare : TYPE IDENTIFIER ';' {newVarDeclaration($1, $2);}
	| TYPE IDENTIFIER '[' CONSTANT ']' ';' {newArrDeclaration($1, $2, $4);};

declarations : declarations declare 
		| declare;


jump: 	JNZ {setConditionCheck(1);}
	| JZ {setConditionCheck(2);}
	| JA {setConditionCheck(3);}
	| JB {setConditionCheck(4);};

condition : IDENTIFIER jump {setConditionVar($1)};



assignment: IDENTIFIER '=' CONSTANT ';' {newAssignment($1, $3);}
	| IDENTIFIER '=' IDENTIFIER ';' {newAssignment($1, $3);} 
	| IDENTIFIER '=' IDENTIFIER '[' CONSTANT ']' ';' {newArrAssignment($1, $3, $5);};

statement: assignment 
	| statement assignment;

cycle:  {generateLable();} 
	WHILE '(' condition ')'
	'{' {genBeginCycleComment();}
	body '}'	{genEndCycleComment(); genCondition();} ;


expressions: statement
	| cycle
	| statement cycle;

body:	expressions
	| body expressions;


unit :  body
	| declarations body

%%
