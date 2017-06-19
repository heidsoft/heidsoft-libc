#include "ch11.h"
int main(void)
{
    int pid, fd[2],len;
    char buffer[PIPE_BUF];
    FILE *stream; 
    if ( pipe(fd) != 0) 
        err_exit("pipe creation failed");
    if ((pid = fork())== 0) { /* 子執行緒 */
        close(fd[1]);            /* 不使用管線的寫端 */
        dup2(fd[0],0);           /* 關閉stdin,重新導向管線的輸入端至stdin */
        close(fd[0]);            /* 關閉這個不再有用的描述字 */
        /* 用cat過濾輸出 */
        if (execl("/bin/cat","cat", NULL/*"-n"*/, NULL) == -1)
            err_exit("Unable to run cat"); 
    } else {                      /* 父執行緒 */
        close(fd[0]);            /* 不使用管線的輸入端 */
        printf("you can type a line and the line will echoed:\n");
        dup2(fd[1],1);           /* 關閉stdout,重新導向管線的輸出端至stdout */
        close(fd[1]);
        while (gets(buffer) != NULL)
           puts(buffer);         /* 寫至管線*/
    }
 } 
