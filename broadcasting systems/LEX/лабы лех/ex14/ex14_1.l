/* ex14_1.l0 */
    int s = 0, h = 0;
%%
she                  s++;
he                    h++;
(.\n)                 ;
%%
yywrap()
{
    printf( "she: %d times, he: %d times\n", s, h );
    return( 1 );
}
main()
{
    while( yylex() );
} 
