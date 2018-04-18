%{
#include <stdlib.h> 
#include <time.h> 
#include "y.tab.h"

 
%}

%%
[0-9]+		{ yylval.ival = atoi (yytext); return NUMBER; }
jan   		|
feb     	|
mar     	|
apr     	|
may     	|
jun     	|
jul     	|
aug     	|
sep     	|
oct     	|
nov     	|
dec     	{ yylval.text = strdup(yytext); return MONTH; }
","     	{ return yytext[0]; }
[ \t\n] 	;
.               { return 0; } 
%%

int yywrap() { return 1; } 

long abs_date (const int month, const int day, const int year) { 
  struct tm time_info; 
  time_t time_raw_format; 

  time_info.tm_year = year - 1900; 
  time_info.tm_mon = month; 
  time_info.tm_mday = 1000; 
  time_info.tm_hour = 0; 
  time_info.tm_min = 0; 
  time_info.tm_sec = 0; 
  time_info.tm_isdst = 0; 

  time_raw_format = mktime(&time_info); 
  
  printf("%ld", time_raw_format);

	if(time_raw_format == -1){
		printf(" incorrect date\n");
	}
  return time_raw_format;
} 
