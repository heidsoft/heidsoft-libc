#include "ch12.h"

int make_un_socket(int type, const char *filename)
{
    struct sockaddr_un name;
    int sock;
    socklen_t size;

    /* 建立套接字 */
    sock = socket(AF_UNIX, type, 0);
    if (sock < 0) 
        err_exit("socket");
    /* 命名套接字 */
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, filename);
    /* 位址的大小是檔名開始的偏移與其長度之和加1（用於終止的空位元組）*/
    size = sizeof(struct sockaddr_un) + 1;
    if (bind (sock, (struct sockaddr *)&name, size) < 0)
        err_exit("bind");
    return sock;
}
