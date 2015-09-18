#!/bin/sh
rm Micro
bison -d parser.y
flex Micro.lex
gcc -o Micro Micro.c parser.tab.c lex.yy.c -lfl


