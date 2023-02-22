#include "ch11.h"
int main (void)
{
    pid_t pid;
    int n, mypipe[2];
    char buffer[BUFSIZ+1], some_data[] = "Hello, world!";
    /* 建立管線 */
    if (pipe (mypipe)) 
        err_exit("Pipe failed.\n");
    /* 衍生子執行緒 */ 
    if ((pid = fork())==(pid_t)0) { /* 這是子執行緒 */
        close(mypipe[1]);     /* 子執行緒關閉管線輸出端 */
        n = read(mypipe[0], buffer, BUFSIZ);
        printf("child %d: read %d bytes: %s\n",getpid(),n,buffer);
    } else {   /* 這是父執行緒 */
        close(mypipe[0]);    /* 父執行緒關閉管線輸入端 */
        n = write(mypipe[1], some_data, strlen(some_data));
        printf("parent %d: write %d bytes: %s\n", getpid(), n, some_data);
    }
    exit (EXIT_SUCCESS);
}
