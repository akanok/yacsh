#!/bin/bash
cd ./bin/
rm lex.yy.c yacsh y.tab.c y.tab.h
yacc -d ../src/yacsh.y 
lex ../src/yacsh.l 
cc ../src/yacsh.c y.tab.c lex.yy.c -o yacsh # -Wall -Wextra
./yacsh
