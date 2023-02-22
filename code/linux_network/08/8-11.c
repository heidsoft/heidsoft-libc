#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <arpa/inet.h>

#define NEVENTS 16

int
main() {
    struct sockaddr_in server;
    int sock, epfd;
    char buf[32];
    int nfds, n;
    int val;
    struct epoll_event ev, ev_ret[NEVENTS];
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    /* 192.168.0.0は利用されていないIPアドレス */
    inet_pton(AF_INET, "192.168.0.0", &server.sin_addr.s_addr);
    epfd = epoll_create(NEVENTS);

    if (epfd < 0) {
        perror("epoll_create");
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0) {
        perror("epoll_ctl");
        return 1;
    }

    val = 1;
    ioctl(sock, FIONBIO, &val);
    /* サーバに接続 */
    n = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if (n != 0) {
        if (errno == EINPROGRESS) {
            printf("before epoll_wait\n");
            nfds = epoll_wait(epfd, ev_ret, NEVENTS, 1000);

            if (nfds < 0) {
                perror("epoll_wait");
                return 1;
            } else if (nfds == 0) {
                printf("timedout\n");
                /* socketの終了 */
                close(sock);
                return 1;
            }

            printf("after epoll_wait : nfds=%d\n", nfds);
        } else {
            perror("connect");
            return 1;
        }
    }

    /* サーバからデータを受信 */
    n = read(sock, buf, sizeof(buf));
    write(fileno(stdout), buf, n);
    /* socketの終了 */
    close(sock);
    return 0;
}
