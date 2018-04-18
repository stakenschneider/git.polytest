%{ 
#define YYDEBUG 1 
extern int yydebug; 
%} 
%union
{
    int ival;
    char *text;
}; 
%token <text>MONTH 
%token <ival> NUMBER

%start date 

%% 
date : MONTH NUMBER ',' NUMBER
	{ abs_date( $1, $2, $4 );
	 printf("\n%d.%s.%d\n", $2, $1, $4);}
%% 
