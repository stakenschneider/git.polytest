#include <stdio.h> 
extern int yydebug; 
main() { 
  yydebug = 1; 
  return yyparse(); 
} 
yyerror(char * s) { 
  fprintf( stderr, "%s\n", s); 
}

