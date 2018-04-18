/* ex5.l0 */
    int len[40];
    int i;
%%
        {
            for( i = 0; i < 40; i++ )
                    len[i] = 0;
        }
[a-z]+         len[yyleng]++;
.    |    \n     ;
%%
yywrap()
{
    for( i = 0; i < 40; i++ )
            if( len[i] > 0 ) printf( "%5d%10d\n", i, len[i] );
    return( 1 );
}
main()
{
    while( yylex() );
} 
