%union {
    char* cval;
};

%{
    #include <stdlib.h>
    #include <string.h>
    #define    YYDEBUG    1
    extern int yydebug;
    int count = 0;
    int count2 = 1;
        int m = 0;
    %}

%token  FOR
%token  <cval>NAME
%token  <cval>NUM
%token  <cval>EQUAL
%token  <cval>IEQUAL
%token  <cval>LEEQ
%token  <cval>GREQ
%token  INC
%token  DEC
%start    op

%%
op: ex | ex op;

ex:    for conditional body;

for    :    FOR        {count++;};

conditional        :    '('cond')'

cond : exp1 ';'  exp2  ';' exp3

ind_t      :    NUM {$<cval>$ = $<cval>1;};
ind        :    NAME {$<cval>$ = $<cval>1;};
iii        :    NAME {$<cval>$ = $<cval>1;};
iii        :    NUM {$<cval>$ = $<cval>1;};

exp1        :  ind '=' iii {
    printf("    mov %s, %s\n", $<cval>1, $<cval>3);
    printf("F%d:\n", count);
};


exp2: iii '<' iii { printf("    mov ax, %s\n", $<cval>1);
    printf("    cmp ax, %s\n", $<cval>3);
    printf("    jge D%d\n", count2);};

exp2: iii '>' iii { printf("    mov ax, %s\n", $<cval>1);
    printf("    cmp ax, %s\n", $<cval>3);
    printf("    jle D%d\n", count2);};

exp2: iii EQUAL iii { printf("    mov ax, %s\n", $<cval>1);
    printf("    cmp ax, %s\n", $<cval>3);
    printf("    jne D%d\n", count2);
};

exp2: iii IEQUAL iii { printf("    mov ax, %s\n", $<cval>1);
    printf("    cmp ax, %s\n", $<cval>3);
    printf("    je D%d\n", count2);};

exp2: iii LEEQ iii { printf("    mov ax, %s\n", $<cval>1);
    printf("    cmp ax, %s\n", $<cval>3);
    printf("    jg D%d\n", count2);};

exp2: iii GREQ iii { printf("    mov ax, %s\n", $<cval>1);
    printf("    cmp ax, %s\n", $<cval>3);
    printf("    jl D%d\n", count2);};


exp3 : ind INC  {
    printf("D%d\n", count2);
    count2++;
    printf("    add %s, 1\n", $<cval>1);};
exp3 : iii DEC  {
    printf("D%d\n", count2);
    count2++;
    printf("    sub %s, 1\n", $<cval>1);};

body : '{'bexp'}'
bexp : bexp ex;
bexp : ex;
bexp : bexp const;
bexp : const;
const: ind '=' iii ';'{
    printf("    mov %s, %s\n", $<cval>1, $<cval>3);
    printf("    loop F%d\n", count);
};

const        :   ind '=' iii '[' ind_t ']' ';'{
    printf("    lea %s, %s\n", $<cval>1,$<cval>3);
    printf("    mov cx, len\n");
    printf("    xor ax, ax\n");
    printf("M%d:\n     inc bx\n", m);
    printf("    loop M%d\n", m);
    m++;
};

%%
