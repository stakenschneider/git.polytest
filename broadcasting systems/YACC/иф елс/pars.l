%{ 
#include "y.tab.h" 
%} 

%% 
"if"		{return IF;}
"else"		{return ELSE;}
"=="		{return EQUAL;} 
"!="		{return IEQUAL;} 
"<="		{return LEEQ;} 
">="		{return GREQ;} 
([a-zA-Z][0-9a-zA-Z]*)("["[0-9]*"]")?   { yylval.cval = strdup(yytext); return VARNAME; } 
[0-9]+"."?[0-9]*			{ yylval.cval = strdup(yytext);  return VARVALUE;}  
"("	|
")"	| 
"{"	| 
"}"	| 
";"	|
"="	|
">"	| 
"<"	 {return yytext[0];} 
[ \n\t]	; 
%% 
int yywrap() {return 1; }

