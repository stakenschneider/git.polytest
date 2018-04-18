%{
#define     YYDEBUG     0
extern int yydebug; 
%}

%token     NUMBER     MONTH

%start     date

%%
date     :     MONTH NUMBER ',' NUMBER
                                { print_date( $1, $2, $4 ); } 
	|     MONTH NUMBER ';' NUMBER
                                { print_date( $1, $2, $4 ); } 
%% 

