#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <string.h>
#include "../bin/y.tab.h"




static char* const path_color = "\e[36;1m";
static char* const shell_prompt_color = "\e[35;1m" ;
static char* const shell_prompt = "yacsh ";
static char* const shell_prompt_separator = " :: ";

/**
 * green	"\033[01;32m"
 * red		"\033[0;31m"
 * orange	"\033[01;33m"
 * blue		"\e[0;34m"
 **/


char *prompt_colorizer(char *path){
	char* close = "\e[0m";
	int len = 2*strlen(shell_prompt_color) + strlen(shell_prompt) + strlen(path_color) + strlen(path) + 3*strlen(close) + strlen(shell_prompt_separator);
	char *prompt = malloc(len*sizeof(char));
	strcpy(prompt,shell_prompt_color);
	strcat(prompt,shell_prompt);
	strcat(prompt,close);
	strcat(prompt,path_color);
	strcat(prompt,path);
	strcat(prompt,close);
	strcat(prompt,shell_prompt_color);
	strcat(prompt,shell_prompt_separator);
	strcat(prompt,close);
	return prompt;
}

void prompt(){
	char cwd[PATH_MAX];
	char *cdir = basename( getcwd(cwd, sizeof(cwd)) );
	char *colorized_path = prompt_colorizer(cdir);
	printf("%s",colorized_path);
	free(colorized_path);
}



int main (void) {
	int result = 0;
	do{
		prompt();
		result = yyparse();
	} while(result!=0);
	return 0;
}



