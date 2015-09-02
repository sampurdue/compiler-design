#!/bin/sh


flex Micro.lex
gcc lex.yy.c -o Micro -lfl
