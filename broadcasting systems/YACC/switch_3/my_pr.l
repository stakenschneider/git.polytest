%{
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}
      
digit			[0-9]
space			[ \t\n]
L			[_A-Za-z]
%%
{space}			;
switch			{ return SWITCH; }
case			{ return CASE; }
default			{ return DEFAULT; }
break			{ return BREAK; }
{L}({L}|{digit})*	{ yylval.text = strdup(yytext); return MARK; }
{digit}+		{ yylval.ival = atoi(yytext); return NUMBER; }
.			{ return yytext[0]; }

%%
yywrap() { return 1; }

