#include "ch12.h"
#include "p12-8.c"   // socket_connect.c, 程式12-8
int main(int argc, char **argv)
{
    int connfd, n, result;
    char buf[256];
    connfd = socket_connect(NULL, "2003");    /* 與服務建立連線 */
    write(connfd, argv[1], sizeof(argv[1]));  /* 向服務傳送資料 */
    n = read(connfd, buf, sizeof(buf));        /* 讀服務的回送資料 */
    buf[n] = 0;  /* 終止符 */
    printf("string from server = %s\n", buf);
    close(connfd);
    exit(0);
}
