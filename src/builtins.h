#pragma once

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include "./yacsh.h"


static char lastdir[PATH_MAX];

extern char **environ;

int cd(Command *cmd){
    char curdir[PATH_MAX];
    getcwd(curdir, sizeof(curdir));


    if (cmd->args[2] != NULL){
      fprintf(stderr,"Wrong options!\n");
      fprintf(stderr,"Usage: cd [dir]\n\tcd -h\tTo view more options\n");
      return 1;
    }
    

    if (cmd->args[1] == NULL) {
        Command_addArgument(cmd, getenv("HOME"));
    } else if ( !strncmp(cmd->args[1],"-h",3) || !strncmp(cmd->args[1],"--help",7) ){
        printf("Usage: cd [DIR]\n\n\tChange the current directory to DIR. The default DIR is the value of the HOME shell variable\n\nOptions:\n\n\tcd -\t\tGo to previos directory\n\n\tcd -h  --help\t\tShow this help\n\n");
        return 0;
    } else if ( !strncmp(cmd->args[1],"-",2) ){
        if (*lastdir == '\0'){
          fprintf(stderr, "cd: no previous directory\n");
          return 1;
        }

        free(cmd->args[1]);
        cmd->args[1] = (char *) calloc(1, strlen(lastdir)+1);
        if (cmd->args[1] == NULL){ fprintf(stderr,"cd: Calloc failed to allocate space for last dir %s\n", curdir); }
        strncpy(cmd->args[1], lastdir, strlen(lastdir)+1 );
    }


    if (chdir(cmd->args[1]) == -1) {
        fprintf(stderr, "cd: %s: %s\n", strerror(errno), cmd->args[1]);
        return 1;
    }

    strncpy(lastdir, curdir, strlen(curdir)+1 );
    return 0;
}



void set(Command *cmd){
	char **s = environ;

    for (; *s; s++) {
      printf("%s\n", *s);
    }
}




int Command_builtins(Command *cmd){

	char *command = cmd->args[0];

	if ( !strncmp(command, "exit", 5) ){
		exit(0);
	} else if ( !strncmp(command, "cd", 3) ){
		cd(cmd);
		return 1;
	} else if ( !strncmp(command, "set", 4) ){
		set(cmd);
		return 1;
	} else if ( !strncmp(command, "unset", 6) ){
		//unset(cmd); //putenv()
		return 1;
	}

	return 0; // This command is not a builtin
}


