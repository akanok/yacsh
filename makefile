
CFLAGS = -Wall -Wextra

compile: yacsh

run:
	./bin/yacsh
	

yacsh: yacc lex ./src/yacsh.c check_bin_folder
	cc ./src/yacsh.c ./src/parser/y.tab.c ./src/parser/lex.yy.c -o ./bin/yacsh -lreadline $(CFLAGS)

lex: ./parser/yacsh.l ./src/parser/y.tab.h
	lex -t ./parser/yacsh.l > ./src/parser/lex.yy.c

yacc: ./parser/yacsh.y ./src/yacsh.h ./src/builtins.h
	yacc ./parser/yacsh.y --header=./src/parser/y.tab.h  -o ./src/parser/y.tab.c


check_bin_folder:
	@if ! [[ -d "./bin" ]]; then\
		mkdir ./bin;\
	fi

clean: ./bin
	@if [[ -f "./bin/yacsh" ]]; then\
		rm ./bin/yacsh;\
	fi

clean_all: clean ./src/parser/y.tab.c ./src/parser/y.tab.h ./src/parser/lex.yy.c
	rm ./src/parser/y.tab.c ./src/parser/y.tab.h ./src/parser/lex.yy.c