#include <stdio.h>

enum states { before = 0, inside = 1, after = 2 };

struct branch {
    enum states new_state:2;
    int should_putchar:1;
};

struct branch the_table[3][3] = {
    /*             ' '          '\n'         others      */
    /* before */ { {before, 0}, {before, 1}, {inside, 1} },
    /* inside */ { {after,  0}, {before, 1}, {inside, 1} },
    /* after  */ { {after,  0}, {before, 1}, {after,  0} }
};

void step (enum states *state, int c)
{
    int idx2 = (c == ' ') ? 0 : (c == '\n') ? 1 : 2;
    struct branch *b = & the_table[*state][idx2];
 
    *state = b->new_state;
    if(b->should_putchar) putchar(c);
}

int main ()
{
    int c;
    enum states state = before;
 
    while((c = getchar()) != EOF)
        step(&state, c);
    return 0;
}
