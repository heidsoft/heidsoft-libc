#include "ch12.h"
#define DATA1   "Fine,thanks."
#define DATA2   "Hello,how are you?"
int main(void)
{
    int sockets[2], child;
    char buf[1024];
    /* 建立套接字偶對 */
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) 
        err_exit("socketpair error");
    /* 建立子執行緒*/
    if ((child = fork()) == -1) 
        err_exit("fork error ");
    if (child != 0) {       /* 這是父執行緒 */
        close(sockets[0]);  /* 關閉子執行緒的套接字 */
        /* 讀來自子執行緒的訊息 */
        if (read(sockets[1], buf, sizeof(buf)) < 0) 
            err_exit("reading socket error");
        printf("parent %d received request: %s\n", getpid(), buf);
        /* 向子執行緒寫訊息 */
        if (write(sockets[1], DATA1, sizeof(DATA1)) < 0) 
            err_exit("writing socket error");
        close(sockets[1]);    /* 通訊結束 */
    } else {     /* 這是子執行緒 */
        close(sockets[1]);     /* 關閉父執行緒的套接字端 */
        /* 傳送訊息給父執行緒 */
        if (write(sockets[0], DATA2, sizeof(DATA2)) < 0) 
            err_exit("writing socket error");
        /* 讀來自父執行緒的訊息 */
        if (read(sockets[0], buf, sizeof(buf)) < 0)
            err_exit("reading socket error");
        printf("child process %d received answer: %s\n", getpid(),buf);
        close(sockets[0]);     /* 通訊結束 */
    }
}
