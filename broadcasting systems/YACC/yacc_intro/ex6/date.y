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
between : date 
	    { $<lval>$ = $1 / (3600L * 24L); } 
	  '-' 
	  date
	    { $4 = $4/(3600L * 24L);
              printf( "%ld\n", $<lval>2 - $4 ); }
%% 
