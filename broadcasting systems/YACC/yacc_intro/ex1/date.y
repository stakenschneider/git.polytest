%{ 
#define YYDEBUG 1 
extern int yydebug; 
%} 

%token NUMBER MONTH 

%start date 

%% 
date : MONTH NUMBER NUMBER 
%% 
