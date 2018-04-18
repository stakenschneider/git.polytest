	int s = 0, h = 0, k = 0, t = 0, q = 0, b = 0, y = 0;

%%
go       s++;
goto     h++;
too      k++;
on       t++;

toon     q++; 
gon      b++;
goton    y++;
.|\n   ;
%%

#ifndef yywrap
int yywrap() { return( 1 ); }
#endif

main() 
{ 
    while( yylex() ); 
    if (q != 0 || b != 0 || y != 0) 
    {printf("error");} else
    printf( "go: %d times\ngoto: %d times\ntoo: %d times\non: %d times\n", s, h, k, t );
}