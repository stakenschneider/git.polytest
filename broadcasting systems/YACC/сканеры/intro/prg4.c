#include <stdio.h>

enum states { before, inside, after };

void step (enum states *state, int c)
{
    if(c == '\n') {
        putchar('\n');
        *state = before;
    } else
    switch(*state) {
        case before:
            if(c != ' ') {
                putchar(c);
                *state = inside;
            }
            break;
        case inside:
            if(c == ' ') {
                *state = after;
            } else {
                putchar(c);
            }
            break;
        case after:
            break;
    }
} 

int main ()
{
    int c;
    enum states state = before;

    while((c = getchar()) != EOF) {
        step(&state, c);
    }
    return 0;
}
