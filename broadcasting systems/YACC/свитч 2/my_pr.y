%{
int yydebug = 1;
#include <stdio.h>
%}

%union
{
	int ival;
	char *text;
};
%token	NUMBER SWITCH CASE BREAK DEFAULT MARK
%type	<ival>expr list

%start	_switch

%left	'+' '-'
%left	'*' '/'

%%
_switch:	SWITCH '(' list ')' '{' _case _default '}'	{ printf("exit: "); }
	;

list:		
	|	list expr		{ printf("\tpop ax\n");
					  printf("\tjmp ax\n"); }
	;

expr:		MARK			{ printf("\tmov ax,%s\n", $<text>1); 
					  printf("\tpush ax\n"); }	
	|	NUMBER			{ printf("\tmov ax,%d\n", $<ival>1);  
					  printf("\tpush ax\n"); }
	|	'(' { /*printf("\t(\n");*/ } expr ')'	{ /*printf("\t)\n");*/ }
	|	expr '+' expr		{ printf("\tpop cx\n");
					  printf("\tpop ax\n");
					  printf("\tadd ax,cx\n"); 
					  printf("\tpush ax\n"); }
	|	expr '-' expr		{ printf("\tpop cx\n");
					  printf("\tpop ax\n");
					  printf("\tsub ax,cx\n"); 
					  printf("\tpush ax\n"); }
	|	expr '*' expr		{ printf("\tpop bx\n");
					  printf("\tpop ax\n");
					  printf("\tmul ax\n"); 
					  printf("\tpush ax\n"); }
	|	expr '/' expr		{ printf("\tpop bx\n");
					  printf("\tpop ax\n");
					  printf("\tdiv bx\n"); 
					  printf("\tpush bx\n"); }
	;

_default:
	|	DEFAULT { printf("def:\n"); } ':' in_case	{  }
	;

_case:	
	|	_case __case		{  }
	;
__case:		CASE NUMBER { printf("case_%d:\n", $<ival>2); } ':' in_case	{  }
	;

in_case:
	|	in_case line		{  }
	;

line:		MARK '=' MARK ';'	{ printf("\tmov %s,%s\n", $<text>1, $<text>3); }
	|	MARK '=' NUMBER ';'	{ printf("\tmov %s,%d\n", $<text>1, $<ival>3); }
	|	BREAK ';'		{ printf("\tjmp exit\n"); }
	;

%%
