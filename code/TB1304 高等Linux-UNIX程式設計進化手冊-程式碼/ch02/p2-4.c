#include <stdio.h>
//#include <ctype.h>
void skip_whitespace (FILE *stream)
{
    int c;
    do
      c = getc (stream);  /* 無需檢查EOF，因為它不是空白字元且ungetc忽略EOF. */
    while (isspace (c));
    ungetc (c, stream);
}
