#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./parser/y.tab.h"

extern int yy_scan_string(char * str);
extern int yylex_destroy();

static char* const path_color = "\e[36;1m";
static char* const shell_prompt_color = "\e[35;1m" ;
static char* const shell_prompt = "yacsh ";
static char* const shell_prompt_separator = " ::";

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
	strncpy(prompt,shell_prompt_color,strlen(shell_prompt_color)+1);
	strncat(prompt,shell_prompt,strlen(shell_prompt)+1);
	strncat(prompt,close,strlen(close)+1);
	strncat(prompt,path_color,strlen(path_color)+1);
	strncat(prompt,path,strlen(path)+1);
	strncat(prompt,close,strlen(close)+1);
	strncat(prompt,shell_prompt_color,strlen(shell_prompt_color)+1);
	strncat(prompt,shell_prompt_separator,strlen(shell_prompt_separator)+1);
	strncat(prompt,close,strlen(close)+1);
	return prompt;
}

void prompt(){
	char cwd[PATH_MAX];
	char *cdir = basename( getcwd(cwd, sizeof(cwd)) );
	char *colorized_path = prompt_colorizer(cdir);
	fprintf(stderr, "%s",colorized_path);
	free(colorized_path);
}



int main (void) {

	using_history();

	int k = 1;
	while(1){
		prompt();
		char *line = readline(" ");
		
		char *input = malloc(strlen(line) + 2);
		strcpy(input,line);
		strncat(input,"\n",3);
		free(line);

		yy_scan_string(input);
		yyparse();
		yylex_destroy();

		add_history(input);
		free(input);

		k++;
		register HIST_ENTRY **list;
		list = history_list();
		for (int i=0; i<k; i++){
			printf("%s\n", list[i]->line);
		}
		
		// *history_get_history_state()
		
	}



	//int result = 0;
	//do{
	//	prompt();
	//	result = yyparse();
	//} while(result!=0);
	return 0;
}


/**
 * void printStringAsHex(char str[]){
	for (int i=0; i<strlen(str); i++){
		printf("%x ",str[i]);
	}
}
*/


