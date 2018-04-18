%{
#include <stdlib.h> 
#include <time.h> 
#include "y.tab.h"

 
%}

%%
[0-9]+		{ yylval.ival = atoi (yytext); return NUMBER; }
jan     	{ yylval.ival = 0; return MONTH; } 
feb     	{ yylval.ival = 1; return MONTH; } 
mar     	{ yylval.ival = 2; return MONTH; } 
apr     	{ yylval.ival = 3; return MONTH; } 
may     	{ yylval.ival = 4; return MONTH; } 
jun     	{ yylval.ival = 5; return MONTH; } 
jul     	{ yylval.ival = 6; return MONTH; } 
aug     	{ yylval.ival = 7; return MONTH; } 
sep    		{ yylval.ival = 8; return MONTH; } 
oct     	{ yylval.ival = 9; return MONTH; } 
nov     	{ yylval.ival = 10; return MONTH; } 
dec     	{ yylval.ival = 11; return MONTH; } 
","     	{ return yytext[0]; }
"-"     	{ return yytext[0]; }
[ \t\n] 	;
.               { return 0; } 
%%

int yywrap() { return 1; } 

long abs_date (const int month, const int day, const int year) { 
  struct tm time_info; 
  time_t time_raw_format; 

  time_info.tm_year = year - 1970; 
  time_info.tm_mon = month; 
  time_info.tm_mday = day; 
  time_info.tm_hour = 0; 
  time_info.tm_min = 0; 
  time_info.tm_sec = 0; 
  time_info.tm_isdst = 0; 

  time_raw_format = mktime(&time_info); 

  return time_raw_format; 
} 
