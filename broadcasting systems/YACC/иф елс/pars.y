%union {
char* cval;
};
%{ 

#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
int count = 1;
int ifcount = 1;
int ifcount2 =1;
int stack[100];
int  sp = 0;
%} 

%token  IF ELSE EQUAL GREQ IEQUAL LEEQ
%token	<cval>VARNAME
%token	<cval>VARVALUE
%start	op 


%%
op		:	ex | ex op;
ex		:	start_if cond body | start_else cond2 body

start_if	:	IF
start_else	:	ELSE;

cond2		:	{printf("else\n"); push(ifcount); ifcount++;};

cond		:	'('cond_body')'				{push(ifcount); ifcount++;};

cond_body	:	iii '>' iii
			{ printf("T%d: > %s %s\n", count, $<cval>1, $<cval>3); count++;	
			printf("T%d: JZ L%d\n", count, ifcount); count++;};
cond_body	:	iii '<' iii
			{ printf("T%d: < %s %s\n", count, $<cval>1, $<cval>3); count++;	
			printf("T%d: JZ L%d\n", count, ifcount); count++;};
cond_body	:	iii EQUAL iii
			{ printf("T%d: == %s %s\n", count, $<cval>1, $<cval>3); count++; 
			printf("T%d: JZ L%d\n", count, ifcount); count++;};
cond_body	:	iii IEQUAL iii
			{ printf("T%d: != %s %s\n", count, $<cval>1, $<cval>3); count++; 
			printf("T%d: JZ L%d\n", count, ifcount); count++;};
cond_body	:	iii LEEQ iii
			{ printf("T%d: <= %s %s\n", count, $<cval>1, $<cval>3); count++; 
			printf("T%d: JZ L%d\n", count, ifcount); count++;};
cond_body	:	iii GREQ iii
			{ printf("T%d: >= %s %s\n", count, $<cval>1, $<cval>3); count++; 
			printf("T%d: JZ L%d\n", count, ifcount); count++;};
ind		:	VARNAME
			{$<cval>$ = $<cval>1;};
iii		:	VARNAME
			{$<cval>$ = $<cval>1;};
iii		:	VARVALUE 
			{$<cval>$ = $<cval>1;};
body		:	'{' assign1 '}' {printf("L%d:\n", pop());};
assign1		:	assign1 ex;
assign1		:	ex;
assign1		:	assign1 assign;
assign1		:	assign;
assign		:	ind '=' iii ';'
			{printf("T%d: =  %s %s\n", count, $<cval>1, $<cval>3); count++;};



%%

int push (int i)
{
if (sp>100)
{
printf ("ERROR: stack overflow");
return 1;
}
stack[sp++] = i;
return 0;
}
int pop (void)
{
if (sp==0)
{
printf ("ERROR: empty stack");
return 2;
}
return stack[--sp];
}


