/* ex12_1.l */
    int flag;
%%
^a                 { flag = 'a'; ECHO; }
^b                 { flag = 'b'; ECHO; }
^c                 { flag = 'c'; ECHO; }
\n                 { flag = 0 ; ECHO; }
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
