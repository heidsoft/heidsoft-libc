%{
#include <stdio.h>
%}

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*
%%
{DIGIT}+             { printf("NUMBER: %s\n", yytext); }
{ID}                 { printf("IDENTIFIER: %s\n", yytext); }
"+"                   { printf("PLUS\n"); }
"-"                   { printf("MINUS\n"); }
"*"                   { printf("MULTIPLY\n"); }
"/"                   { printf("DIVIDE\n"); }
[ \t\n]+              ; // 忽略空白字符
.                     { printf("UNKNOWN: %s\n", yytext); }
%%
int main(int argc, char **argv) {
    yylex();
    return 0;
}
