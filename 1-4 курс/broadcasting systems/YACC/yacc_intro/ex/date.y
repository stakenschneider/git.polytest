

%token NUMBER MONTH 

%start date 

%% 
date : MONTH NUMBER ',' NUMBER
	{int r=abs_date( $1, $2, $4 ); 
	printf("%i\n\n", (char)r);}
%% 
