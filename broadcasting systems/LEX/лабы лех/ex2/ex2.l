/* ex2.l */
    int lineno = 0;
%%
\n     lineno++;
.       ;
%%
yywrap()                /* !!! */
{
        printf( "%d lines\n", lineno );
        return( 1 );
}
main()
{
    while( yylex() );
} 
