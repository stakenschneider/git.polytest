/* ex12_1.l */
    int flag;
%%
^[abc]     |
\n                   { flag = yytext[0] ; ECHO; } 
magic           { switch (flag)
                            {
                                case 'a': printf("<first>"); break;
                                case 'b': printf("<second>"); break;
                                case 'c': printf("<third>"); break;
                                default : ECHO;
                            }
                    }
%%
#include "yy.c"  
