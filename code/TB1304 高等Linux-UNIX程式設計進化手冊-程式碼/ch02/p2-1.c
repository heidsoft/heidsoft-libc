#include "ch02.h"
int main(void)
{
    FILE *stream;
    char buf[80];
    printf("open and creat test_file\n");
    if ((stream = fopen("test_file","w")) == NULL )
        err_exit("open() failed");
    printf("write string to test_file\n");
    fputs("Hello world",stream);   /* 輸出字串至終端 */
    if ((stream = freopen("test_file","r",stream)) == NULL )
        err_exit("open() failed");
    printf("read string from test_file\n");
    fgets(buf, sizeof(buf), stream);  /* 從終端讀入一行字串 */
    printf("the string is \"%s\"\n", buf);
    fclose(stream);
}
