/* ex6.l */
%{
#define limit 5
%}
%%
[A-Z][a-z]*         if( yyleng > limit ) yyless( limit );
[a-z]*                 ECHO;
%%
#include "yy.c" 
