%{
#include "date_4_2_tab.h"
%}

%%
[0-9]+         { return NUMBER; }
jan             |
feb             |
...
nov            |
dec             { return MONTH; }
[ \t\n]         ;
.                 { return 0; }
%%

int yywrap() { return 1; } 
