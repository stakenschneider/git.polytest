%union {
char* cval;
};

%{
#include <stdlib.h>
#include <string.h>
#define    YYDEBUG    1
extern int yydebug;
int count_do = 0;
int count_doA = 0;
%} 

%token  DO WHILE
%token	<cval>NAME
%token	<cval>NUM
%token  <cval>EQUAL
%token  <cval>IEQUAL
%token  <cval>LEEQ
%token  <cval>GREQ
%start    op

%%
op: ex | ex op;

ex:    start_do body start_while cond;

start_do : DO {count_do++;
    count_doA++;
    if (count_do == 1)
    {
        count_do = count_doA;
    }
    printf("D%d:\n", count_do);};

ind : NAME {$<cval>$ = $<cval>1;};
iii : NAME {$<cval>$ = $<cval>1;};
iii : NUM {$<cval>$ = $<cval>1;};

body: '{'exp'}'
exp : exp ex;
exp : ex;
exp : exp exp2;
exp : exp2;
exp2 : ind '=' iii ';'{ printf("    mov %s, %s\n", $<cval>1,  $<cval>3);};

start_while :   WHILE

cond : '('cond_b')'

cond_b: iii '<' iii { printf("    mov ax, %s\n", $<cval>1);
                    printf("    cmp ax, %s\n", $<cval>3);
                    printf("    jge D%d\n", count_do);
                    count_do--;};

cond_b: iii '>' iii { printf("    mov ax, %s\n", $<cval>1);
                    printf("    cmp ax, %s\n", $<cval>3);
                    printf("    jle D%d\n", count_do);
                    count_do--;};

cond_b: iii EQUAL iii { printf("    mov ax, %s\n", $<cval>1);
                    printf("    cmp ax, %s\n", $<cval>3);
                    printf("    jne D%d\n", count_do);
                    count_do--;};

cond_b: iii IEQUAL iii { printf("    mov ax, %s\n", $<cval>1);
                    printf("    cmp ax, %s\n", $<cval>3);
                    printf("    je D%d\n", count_do);
                    count_do--;};

cond_b: iii LEEQ iii { printf("    mov ax, %s\n", $<cval>1);
                    printf("    cmp ax, %s\n", $<cval>3);
                    printf("    jg D%d\n", count_do);
                    count_do--;};

cond_b: iii GREQ iii { printf("    mov ax, %s\n", $<cval>1);
                    printf("    cmp ax, %s\n", $<cval>3);
                    printf("    jl D%d\n", count_do);
                    count_do--;};

%%
