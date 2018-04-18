%{
#define YYDEBUG 1
#define YYSTYPE int
#include <stdio.h>
%}

%token NUM
%start __list

%%
__list: _list           { printf ("\n Lenght is %d\n", $1); }

_list:  /* empty list */        { $$ = 0; /* lenght is 0 */ }
        | list                            /* not empty */
        ;

list:   NUM             { $$ = 1; }     /* size is 1 */
        | NUM ',' list  { $$ = $1 + 1; } /* total size is a size of
                                           sublist + 1 */
        ;
%%
