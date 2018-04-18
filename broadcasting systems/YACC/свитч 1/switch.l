%{
#include "y.tab.h"
%}
%% 
"switch"	{return SWITCH;}
"case"		{return CASE;}
"break"		{return BREAK;}
[a-zA-Z][0-9a-zA-Z]*   { yylval.cval = strdup(yytext); return VARNAME; } 
[0-9]+			{ yylval.cval = strdup(yytext);  return VARVALUE;}  
"("	|
")"	| 
"{"	| 
"}"	| 
";"	|
":"	|
"+"	|
"-"	|
"*"	|
"/"	|
"="	{return yytext[0];} 
[ \n\t]	; 
%% 
int yywrap() {return 1; }
