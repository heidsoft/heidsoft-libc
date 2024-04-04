%{
#include <stdio.h>
%}

%%

"/*"    { /* 识别注释开始 */
    char c;
    do {
        do {
            c = input();
        } while (c != '*' && c != 0);
        c = input();
    } while (c != '/' && c != 0);
}

"//".*  /* 识别单行注释 */

[0-9]+  { /* 识别整数 */
    printf("INTEGER: %s\n", yytext);
    return INTEGER;
}

[a-zA-Z_][a-zA-Z_0-9]*  { /* 识别标识符 */
    printf("IDENTIFIER: %s\n", yytext);
    return IDENTIFIER;
}

[ \t\n]+  ; /* 忽略空白字符 */

.       { /* 识别其他字符 */
    printf("UNKNOWN: %s\n", yytext);
    return UNKNOWN;
}

%%

int main() {
    yylex();
    return 0;
}