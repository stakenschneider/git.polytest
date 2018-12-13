#include <stdio.h>

int main ()
{
    int c;
 
    do {
        c = getchar();
        while(c == ' ')
            c = getchar();
        while(c != EOF && c != ' ' && c != '\n') {
            putchar(c);
            c = getchar();
        }
        putchar('\n');
        while(c != EOF && c != '\n')
            c = getchar();
    } while(c != EOF);

    return 0;
}
