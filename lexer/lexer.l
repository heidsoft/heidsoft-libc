%{
#include <stdio.h>
%}

%%

[0-9]+    {
            printf("A number: %s\n", yytext);
          }

[a-zA-Z]+ {
            printf("A word: %s\n", yytext);
          }

.|\n       {  }

%%

int main(void) {
  yylex();
  return 0;
}