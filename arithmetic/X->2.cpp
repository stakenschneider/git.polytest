#include <stdio.h>
void convert(const int numeric, int notation) //функция перевода чисел (число , основание системы в аргументах)
{
    if(numeric)
    {
        convert(numeric/notation,notation); //рекурсивная ф-ция
        printf("%d" , numeric%notation );
    }
}

int main()
{
    convert( 4 , 5 );
    printf(" ");

    convert( 100 , 10 );
    printf(" ");
    
    convert( 10 , 3 );
    printf(" ");
    
    convert( 2 , 2 );
    printf(" ");
}
