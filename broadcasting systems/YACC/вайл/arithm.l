D			[0-9]
L			[a-zA-Z_]

%{ 

#define YYSTYPE char*
extern YYSTYPE yylval;

#include "arithm.tab.h"  
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
//#include "polish.h"

extern FILE *declarations;
extern FILE *code;
int temp_cell_number = 0;
int lable_number = 0;
int condition_type;
char *condition_var;
%} 

%%
char			{ yylval = strdup(yytext); return TYPE; }
int			{ yylval = strdup(yytext); return TYPE; }
while			{ return WHILE; }

{L}({L}|{D})*		{ yylval = strdup(yytext); return IDENTIFIER; }
{D}+			{ yylval = strdup(yytext); return CONSTANT; }

";"		{ return yytext[0]; }
"{"		{ return yytext[0]; }
"}"		{ return yytext[0]; }
"="		{ return yytext[0]; }
"("		{ return yytext[0]; }
")"		{ return yytext[0]; }
"["		{ return yytext[0]; }
"]"		{ return yytext[0]; }
"-"		{ return yytext[0]; }
"+"		{ return yytext[0]; }
"*"		{ return yytext[0]; }
"/"		{ return yytext[0]; }

"!="[ ]*"0" 	{ return JNZ; }
"="[ ]*"0" 	{ return JZ; }
">"[ ]*"0" 	{ return JA; }
"<"[ ]*"0" 	{ return JB; }

[ \t\n\r]+		;
.			{return 0;}

%%

int yywrap(void)
{
	return(1);
}

/*
assignment: IDENTIFIER '=' CONSTANT ';' {newAssignment($1, $3);}
	| IDENTIFIER '=' IDENTIFIER ';' {newAssignment($1, $3);}
	| IDENTIFIER '=' IDENTIFIER '[' CONSTANT ']' {newArrAssignment($1, $3, $5);};
*/
//стек операторов
static int sp_operators = 0;
char operators[100];
//стек операндов
static int sp_operands = 0;
char operands[100][100];
//стек меток циклов
static int sp_lables = 0;
char lables[35][100];

void executeOperator(char operator);

//операции работы со стеком операторов
void push_operator(char operator){
	if(sp_operators < 100){
		operators[sp_operators++] = operator;
	}else{
		printf("Stack overflow. Too much operators.\n");
		exit(1);
	}		
} 

char pop_operator(void){
	if(sp_operators > 0){
		return operators[--sp_operators];
	}else{
		printf("Stack underflow. Too few operators.\n");
		exit(1);
	}
}


//операции работы со стеком операндов
void push_operand(char *operand){
	if(sp_operands < 100){
		//operands[sp_operands++] = operand;
		sprintf(operands[sp_operands++], "%s", operand);
	}else{
		printf("Stack overflow. Too much operands.\n");
		exit(1);
	}		
} 

char *pop_operand(void){
	if(sp_operands > 0){
		return operands[--sp_operands];
	}else{
		printf("Stack underflow. Too few operands.\n");
		exit(1);
	}
}

//операции работы со стеком меток
void push_lable(char *lable){
	if(sp_lables < 100){
		sprintf(lables[sp_lables++], "%s", lable);
	}else{
		printf("Stack overflow. Too much lables.\n");
		exit(1);
	}		
} 

char *pop_lable(void){
	if(sp_lables > 0){
		return lables[--sp_lables];
	}else{
		printf("Stack underflow. Too few lables.\n");
		exit(1);
	}
}


//пришла открывающая скобка
void openBracket(void){
	printf("pushing: (\n");
	push_operator('(');
}

//пришла закрывающая скобка
void closeBracket(void){
	char c;
	printf("close bracket.\n");
	c = pop_operator();
	printf("poping: %c\n", c);
	while(c != '('){
		executeOperator(c);
		c = pop_operator();
		printf("poping: %c\n", c);
	}
}

//приоритет операций
int priority(char op){
	switch(op){
		case '*':
		case '/':
		return 2;
		
		case '+':
		case '-':
		return 1;
	}
	return 0;
}

//пришел новый оператор
void newOperator(char operator){	
	char c;
	int noneed = 0;
	if(sp_operators > 0){
		c = pop_operator();
		printf("poping: %c\n", c);
		while(priority(c) >= priority(operator)){
			executeOperator(c);
			if(sp_operators > 0){
				noneed = 0;
				c = pop_operator();
				printf("poping: %c\n", c);
			}else{
				noneed = 1;				
				break;						
			}
		}
		//возвращаем в стек последний оператор, мы его вытащили, чтобы проверить приоритет
		if(noneed != 1){
			printf("pushing operator: %c\n", c);
			push_operator(c);
		}
	}
	//кладе в стек новый оператор
	printf("pushing operator: %c\n", operator);
	push_operator(operator);
}

//пришел новый операнд
void newOperand(char *operand){
	printf("pushing operand: %s\n", operand);
	push_operand(operand);
}

//исполнение оператора
void executeOperator(char operator){
	char *first_operand, *second_operand;
	char temp_cell[27];
	char number[17];
	second_operand = pop_operand();
	first_operand = pop_operand();
	printf("Executing %s %c %s\n", first_operand, operator, second_operand);
	//MOV eax, operand1
	fprintf(code, "\tMOV eax, %s\n", first_operand);
	switch(operator){
		//MUL eax, operand2
		case '*':
			fprintf(code, "\tMUL eax, %s\n", second_operand);
			break;
		//DIV eax, operand2
		case '/':
			fprintf(code, "\tDIV eax, %s\n", second_operand);
			break;
		//ADD eax, operand2
		case '+':
			fprintf(code, "\tADD eax, %s\n", second_operand);
			break;
		//SUB eax, operand2
		case '-':
			fprintf(code, "\tSUB eax, %s\n", second_operand);
			break;
	}
	//Ti dw ?
	sprintf(temp_cell, "tempcell%d", temp_cell_number);
	temp_cell_number ++;
	fprintf(declarations, "%s dw ?\n", temp_cell);
	//MOV Ti, eax
	fprintf(code, "\tMOV %s, eax\n", temp_cell); 
	//кладем результат в стек - имя ячейки временно памяти
	push_operand(temp_cell);
}

//установка типа условия
void setConditionCheck(int type){
	condition_type = type;
}

//установка переменной для проверки
void setConditionVar(char *var){
	condition_var = var;
}

//генерация кода проверки на 0
void genCondition(){
	fprintf(code, "\tMOV eax, %s\n", condition_var);
	fprintf(code, "\tTEST eax\n");
	
	switch(condition_type){
		case 1:
			fprintf(code, "\tJNZ %s\n", pop_lable());
			break;
		case 2:
			fprintf(code, "\tJZ %s\n", pop_lable());
			break;
		case 3:
			fprintf(code, "\tJA %s\n", pop_lable());
			break;
		case 4:
			fprintf(code, "\tJB %s\n", pop_lable());
			break;
	}
	 
	//запись комментариев	
	fprintf(code, "\t;condition end\n");
	fprintf(declarations, ";temporary memory cells - end\n");
}

//генерация комментария
void genConditionComment(){
	fprintf(code, "\n\t;condition\n");
	fprintf(declarations, "\n;generated temporary memory cells\n");
}


//объявление переменной
void newVarDeclaration(char *type, char *name){
	if(strcmp(type, "char") == 0){
		fprintf(declarations, "%s db ?\n", name);
	}else if(strcmp(type, "int") == 0){
		fprintf(declarations, "%s dd ?\n", name);
	}else{
		printf("Wrong type specifier %s\n", type);
	}
}

//объявление массива
void newArrDeclaration(char *type, char *name, char *lenght){
	if(strcmp(type, "char") == 0){
		fprintf(declarations, "%s db %s dup ?\n", name, lenght);
	}else if(strcmp(type, "int") == 0){
		fprintf(declarations, "%s dd %s dup ?\n", name, lenght);
	}else{
		printf("Wrong type specifier %s\n", type);
	}
}

//запись метки цикла
void generateLable(void){
	char lable[35];	
	sprintf(lable, "compiled_lable%d", lable_number);
	push_lable(lable);
	fprintf(code, ":%s\n", lable);
	lable_number ++;
}

//новое присваивание
void newAssignment(char *to, char *from){
	//MOV to, from	
	fprintf(code, "\tMOV %s, %s\n", to, from);
}

//присваивание элемента массива
void newArrAssignment(char *to, char *from, char *index){
	//MOV to, from[index]
	fprintf(code, "\tMOV %s, %s[%s]\n", to, from, index);
}

//запись комментария о начале тела цикла
void genBeginCycleComment(void){
	fprintf(code, "\t;cycle body\n");
}

//запись комментария о конце тела цикла
void genEndCycleComment(void){
	fprintf(code, "\t;cycle body - end\n");
}
