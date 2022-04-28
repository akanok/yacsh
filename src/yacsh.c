#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "./parser/y.tab.h"

extern int yy_scan_string(char * str);
extern int yylex_destroy();

/**
 * green	"\033[01;32m"
 * red		"\033[0;31m"
 * orange	"\033[01;33m"
 * blue		"\e[0;34m"
 **/
const char* const PATH_COLOR = "\e[36;1m";
const char* const SHELL_PROMPT_COLOR = "\e[35;1m" ;
const char* const SHELL_PROMPT = "yacsh ";
const char* const SHELL_PROMPT_SEPARATOR = " :: ";

const int MAX_HISTORY_LINE_NUMBER = 10;
const char* const HISTORY_FILE_NAME = "/.yacsh_history";
char* history_path;


char *prompt_colorizer(char *path){
	const char* close = "\e[0m";
	int len = 2*strlen(SHELL_PROMPT_COLOR) + strlen(SHELL_PROMPT) + strlen(PATH_COLOR) + strlen(path) + 3*strlen(close) + strlen(SHELL_PROMPT_SEPARATOR);
	char *prompt = malloc(len*sizeof(char));
	strncpy(prompt,SHELL_PROMPT_COLOR,strlen(SHELL_PROMPT_COLOR)+1);
	strncat(prompt,SHELL_PROMPT,strlen(SHELL_PROMPT)+1);
	strncat(prompt,close,strlen(close)+1);
	strncat(prompt,PATH_COLOR,strlen(PATH_COLOR)+1);
	strncat(prompt,path,strlen(path)+1);
	strncat(prompt,close,strlen(close)+1);
	strncat(prompt,SHELL_PROMPT_COLOR,strlen(SHELL_PROMPT_COLOR)+1);
	strncat(prompt,SHELL_PROMPT_SEPARATOR,strlen(SHELL_PROMPT_SEPARATOR)+1);
	strncat(prompt,close,strlen(close)+1);
	return prompt;
}

char *prompt(){
	char cwd[PATH_MAX];
	char *cdir = basename( getcwd(cwd, sizeof(cwd)) );
	char *colorized_path = prompt_colorizer(cdir);
	return colorized_path;
}

void save_history(char* line){
	FILE *history_file = fopen(history_path,"a");
	if ( history_file == NULL){
		fprintf(stderr, "[Error] could not open '%s' for saving history's entry: %s\n",history_path, strerror(errno));
		exit(1);
	}
	fprintf(history_file,"%s\n",line);
	fclose(history_file);

	if ( history_length >= MAX_HISTORY_LINE_NUMBER ){
		history_truncate_file(history_path,MAX_HISTORY_LINE_NUMBER/2);
	}
}


int main (void) {
	char *home = getenv("HOME");

	history_path = calloc(1, (strlen(home)+strlen(HISTORY_FILE_NAME)+1) * sizeof(char) );
	strncat(history_path,home,strlen(home)+1);
	strncat(history_path,HISTORY_FILE_NAME,strlen(HISTORY_FILE_NAME)+1);

	using_history();
	read_history(history_path);

	while(1){
		char *prompt_line = prompt();
		char *line = readline(prompt_line);
		free(prompt_line);

		if( strlen(line) > 0 ){
			char *input = (char *) malloc(strlen(line) + 2);
			strcpy(input,line);
			strncat(input,"\n",3);

			yy_scan_string(input);
			yyparse();
			yylex_destroy();

			add_history(line);
			save_history(line);
			free(input);
		}
		free(line);
		
	}

	return 0;
}



