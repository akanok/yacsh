%{
    #include "./y.tab.h"
    void yyerror (char *s);
    int yylex();
%}


%%
[ \t]                           {}

[ \t]*\n                        {return NEWLINE;}

[-a-zA-Z0-9_\/\.=:@?$`]+        {yylval.word = yytext; return WORD;}

('.*'|\".*\")                   {yylval.word = yytext; return WORD;}

.                               {yyerror ("unrecognised character");}
%%


int yywrap(){
    return 1;
}
