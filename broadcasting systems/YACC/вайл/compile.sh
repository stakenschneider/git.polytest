#!/bin/bash
flex -w $1.l
bison -d $1.y
gcc -o syntax lex.yy.c $1.tab.c zz.c
./syntax <text_in.txt >text_out.txt
