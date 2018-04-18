%union
{
    int ival;
    long lval;
}; 
%token <ival> MONTH NUMBER
%type <lval> date
%start between 

%% 
date : MONTH NUMBER ',' NUMBER
	{ $$ = abs_date( $1, $2, $4 );}
between : date '-' date
		{ printf("DAYS: %ld\n", ($1 - $3)/(3600L * 24L) ); }
%% 
