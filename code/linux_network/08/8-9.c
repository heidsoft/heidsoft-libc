#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define NEVENTS 16

int
main() {
    struct sockaddr_in server;
    struct epoll_event ev, ev_ret[NEVENTS];
    int sock, epfd;
    char buf[65536];
    int nfds;
    int n;
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    /* "127.0.0.1"の部分は必要に応じて変更して下さい */
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    /* サーバに接続 */
    n = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if (n != 0) {
        perror("connect");
        return 1;
    }

    epfd = epoll_create(NEVENTS);

    if (epfd < 0) {
        perror("epoll_create");
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLOUT;
    ev.data.fd = sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0) {
        perror("epoll_ctl");
        return 1;
    }

    while (1) {
        printf("before epoll_wait\n");
        nfds = epoll_wait(epfd, ev_ret, NEVENTS, -1);

        if (nfds < 0) {
            perror("epoll_wait");
            return 1;
        }

        printf("after epoll_wait\n");

        if (ev_ret[0].data.fd == sock) {
            printf("write %d bytes\n", sizeof(buf));
            n = write(sock, buf, sizeof(buf));

            if (n <= 0) {
                printf("write error: %d\n", n);
                break;
            }
        }
    }

    /* socketの終了 */
    close(sock);
    return 0;
}

