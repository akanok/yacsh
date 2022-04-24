#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


typedef struct Command {
	int argsNum;		// #ofActualArgs
	int allocatedArgs;	// #ofPreallocatedArgs
	char **args;
} Command;



void execute(char *args[]){
	pid_t pid;
	pid = fork();
	 
	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		execvp(args[0],args);
		perror("yacsh: Command not found: ");
	}
	else {
		int returnStatus;
		waitpid(pid,&returnStatus,0); // wait for child to terminate
		if (returnStatus != 0){
			fprintf(stderr, "Process %d terminated with error code %d (Command: %s)\n", pid, returnStatus,args[0]);
		}
	}

}


void Command_create(Command *cmd, char *command){
	cmd->allocatedArgs = 4;
	cmd->argsNum++;

	cmd->args = (char **) calloc( 4, sizeof(char *) ); //[ [char*] [char*] [char*] [char*] ]
	if (cmd->args == NULL){ fprintf(stderr,"Calloc failed to allocate space for new Command\n"); }

	cmd->args[0] = (char *) calloc( 1, strlen(command)+1 );
	if (cmd->args[0] == NULL){ fprintf(stderr,"Calloc failed to allocate space for Command %s\n",command); }

	strncpy( cmd->args[0], command, strlen(command)+1 );
	// +1 because strlen() do not calculate the null-byte (end of string)
}

void Command_addArgument(Command *cmd, char *arg){
	if (cmd->allocatedArgs == cmd->argsNum){
		char **tmp = (char**) realloc(cmd->args, 2*(cmd->allocatedArgs)*sizeof(char *) );
		if (tmp == NULL ){
			fprintf(stderr,"Realloc failed to reallocate space for Command %s\n",cmd->args[0]);
		} else { cmd->args = tmp; }

		cmd->allocatedArgs *= 2;
	}

	int i = cmd->argsNum;
	cmd->args[i] = (char *) calloc( 1, strlen(arg)+1 );
	if (cmd->args[i] == NULL){ fprintf(stderr,"Calloc failed to allocate space for arg %s of command %s\n", arg, cmd->args[0]); }
	strncpy( cmd->args[i], arg, strlen(arg)+1 );
	cmd->argsNum++;
}

void Command_free(Command *cmd){

	for (int i=0; i<(cmd->allocatedArgs); i++){
		free(cmd->args[i]);
		cmd->args[i] = NULL;
	}

	free(cmd->args);
	cmd->args = NULL;
	cmd->argsNum = 0;
	cmd->allocatedArgs = 0;
}


