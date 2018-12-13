%{
#include "y.tab.h"
%}

%%
[0-9]+         { return NUMBER; }
jan             |
feb             |
nov            	|
dec             { return MONTH; }
","     	{return yytext[0];}
";"     	{return yytext[0];}
[ \t\n]         ;
.                 { return 0; }
%%

int yywrap() { return 1; } 
