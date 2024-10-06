%{
#include <stdio.h>
%}

%%
[0-9]+      { printf("NUMBER: %s\n", yytext); }
[\t]+       ;  // 忽略制表符
"+"         { printf("PLUS\n"); }
"-"         { printf("MINUS\n"); }
"*"         { printf("MULTIPLY\n"); }
"/"         { printf("DIVIDE\n"); }
\n          ;  // 忽略换行符
.           { printf("UNKNOWN: %s\n", yytext); }
%%

int main() {
    yylex();
    return 0;
}
