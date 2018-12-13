/* ex8.l */
/* @Zoom -> mooZ */
    int i;
%%
\@[A-Za-z]+         {
                                for(i=1; i<yyleng; i++)
                                        unput(yytext[i]);
                             }
%%
#include "yy.c" 
