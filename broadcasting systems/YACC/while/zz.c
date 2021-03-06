#include <stdio.h> 
int yydebug; 
FILE *declarations;
FILE *code;

int main(void) { 
	int status;
	char buffer;
  	size_t result = 1;

	declarations = fopen("declarations.asm", "w");
	code = fopen("code.asm", "w");
	if((code==NULL) || (declarations==NULL)){
		printf("Can't open files.\n");
		return 1;
	}
	yydebug = 1; 

  	status = yyparse(); 
	fclose(code);
	code = fopen("code.asm", "r");
	//копирование файла code в declarations
	while(result > 0){
		result = fread(&buffer, 1, 1, code);
		if(result > 0)
			fwrite(&buffer, 1, 1, declarations);
	}

	fclose(declarations);
	fclose(code);	
	return status;
} 

yyerror(char * s) { 
	fprintf( stderr, "%s\n", s); 
}
