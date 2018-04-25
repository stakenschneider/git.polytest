%{
    #include "y.tab.h"
%}

%%
"do"    {return DO;}
"while"        {return WHILE;}
"=="    {return EQUAL;}
"!="    {return IEQUAL;}
">="    {return LEEQ;}
"<="    {return GREQ;}

[a-zA-Z][0-9a-zA-Z]*   { yylval.cval = strdup(yytext); return NAME; }
[0-9]+                   { yylval.cval = strdup(yytext); return NUM;}
"("    |
")"    |
"{"    |
"}"    |
";"    |
"+"    |
"-"    |
">" |
"<" |
"="    {return yytext[0];}
[ \n\t]    ;
%%
int yywrap() {return 1; }
