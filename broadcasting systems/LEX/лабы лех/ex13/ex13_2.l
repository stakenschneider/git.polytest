/* ex13_2.l */
%S AA BB CC 
%{
#define YY_USER_ACTION { printf("(%d)", YYSTATE); }
%}
%%
^[abc]                        { ECHO; BEGIN( yytext[0]-'a'+1 ); }
\n                               { ECHO; BEGIN( 0 ); }
<AA>magic               printf( "<first>" );
<BB>magic               printf( "<second>" );
<CC>magic               printf( "<third>" );
%%
#include "yy.c"  
