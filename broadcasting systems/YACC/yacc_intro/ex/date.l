%{
#include <stdlib.h> 
#include <time.h> 
#include "y.tab.h"

#define YYSTYPE int
extern YYSTYPE yylval; 
%}

%%
[0-9]+		{ yylval = atoi (yytext); return NUMBER; }
jan   		{ yylval = 0; return MONTH; }
feb     	{ yylval = 1; return MONTH; }
mar     	{ yylval = 2; return MONTH; }
apr     	{ yylval = 3; return MONTH; }
may     	{ yylval = 4; return MONTH; }
jun     	{ yylval = 5; return MONTH; }
jul     	{ yylval = 6; return MONTH; }
aug     	{ yylval = 7; return MONTH; }
sep     	{ yylval = 8; return MONTH; }
oct     	{ yylval = 9; return MONTH; }
nov     	{ yylval = 10; return MONTH; }
dec     	{ yylval = 11; return MONTH; }
","     	{ return yytext[0]; }
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
