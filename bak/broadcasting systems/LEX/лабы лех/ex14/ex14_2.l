/* ex14_2.l0 */
    int s = 0, h = 0;
%%
she                      { s++; REJECT; }
he                        { h++; REJECT; }
(.\n)                     ;
%%
yywrap()
{
    printf( "\nshe: %d times, he: %d times\n", s, h );
    return( 1 );
}
main()
{
    while( yylex() );
} 
