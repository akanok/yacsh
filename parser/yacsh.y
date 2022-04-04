%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../yacsh.h"
    #include "../builtins.h"

    void yyerror (char *s);
    int yylex();

    static Command cmd;
%}

%start line
%union {char *word;}
%token <word> WORD
%token NEWLINE NONE


%%
line:       get_words end
            | get_words args_list end
            | end
            ;
    
args_list:  get_words
            | args_list get_words
            ;


get_words: WORD {   //printf("Word: %s\n",$1);
                  if (cmd.argsNum == 0){
                      Command_create(&cmd, $1);
                  } else {
                      Command_addArgument(&cmd, $1);
                  }
                }
       ;


end: NEWLINE {
                if ( Command_builtins(&cmd) == 0 ){
                    execute(cmd.args); 
                }
                Command_free(&cmd);
                return 1;
            }
   ;

%%


void yyerror (char *s) {
    fprintf (stderr, "%s\n", s);
}



