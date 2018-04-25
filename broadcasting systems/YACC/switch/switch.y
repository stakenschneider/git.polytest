%union {
char* cval;
};
%{ 
#define    YYDEBUG    1
extern int yydebug;
#include <stdlib.h> 
#include <string.h>
int count_case=1;
int count_switch=0;
int cnt = 1;
%} 

%token  SWITCH CASE BREAK
%token	<cval>VARNAME
%token	<cval>VARVALUE
%start	op

%%
op		:	ex | ex op;
ex		:	start_switch cond body; 		
start_switch	:	SWITCH					{count_switch++;};	
cond		:	'('exp')';
iii		:	VARNAME
			{$<cval>$ = $<cval>1;};
iii		:	VARVALUE 
			{$<cval>$ = $<cval>1;};
exp		: 	iii { printf("    mov ax, %s\n", $<cval>1);};
exp		:	exp '+' iii { printf("    add ax, %s\n", $<cval>3);};
exp		:	exp '-' iii { printf("    sub ax, %s\n", $<cval>3);};
exp		:	exp '*' iii { printf("    mul ax, %s\n", $<cval>3);};
exp		:	exp '/' iii { printf("    div ax, %s\n", $<cval>3);};

body		:	'{' op2 '}'				{cnt=0; count_switch--; count_case--; 
								 if(count_switch==0) printf("end:");};

op2		:	ex2 | ex2 op2;
ex2		:	start_case cond2 assign1 start_break ';';

start_case	:	CASE;
cond2		: 	iii':'		{printf("L%d%d:\n",count_switch,count_case);
					 printf("    mov bx, %s\n    cmp ax, bx\n", $<cval>1);
					 printf("    jne L%d%d\n", count_switch, count_case+1);
					 count_case++;};
assign1		:	assign1 ex;
assign1		:	ex;
assign1		:	assign1 assign;
assign1		:	assign;
assign		:	iii '=' iii ';' {printf("    mov %s, %s\n", $<cval>1, $<cval>3);};

start_break	:	BREAK 		{	if(cnt==0) cnt=1;
						else {
							printf("L%d%d:\n",count_switch,count_case); 
							printf("    jmp end\n");
						}
					};
%%

