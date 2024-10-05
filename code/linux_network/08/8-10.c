#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NEVENTS 16

int
main() {
    int sock1, sock2, epfd;
    struct sockaddr_in addr1, addr2;
    struct epoll_event ev, ev_ret[NEVENTS];
    char buf[2048];
    int i;
    int nfds;
    int n;
    /* 受信ソケットを2つ作ります */
    sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    addr1.sin_family = AF_INET;
    addr2.sin_family = AF_INET;
    /* "127.0.0.1"の部分は必要に応じて変更して下さい */
    inet_pton(AF_INET, "127.0.0.1", &addr1.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &addr2.sin_addr.s_addr);
    /* 2つの別々のポートで待つために別のポート番号をそれぞれ設定します */
    addr1.sin_port = htons(11111);
    addr2.sin_port = htons(22222);
    /* 2つの別々のポートで待つようにbindします */
    bind(sock1, (struct sockaddr*)&addr1, sizeof(addr1));
    bind(sock2, (struct sockaddr*)&addr2, sizeof(addr2));
    epfd = epoll_create(NEVENTS);

    if (epfd < 0) {
        perror("epoll_create");
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock1;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock1, &ev) != 0) {
        perror("epoll_ctl");
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock2;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock2, &ev) != 0) {
        perror("epoll_ctl");
        return 1;
    }

    /* このサンプルでは、10秒間データを受信しないとループを抜けます */
    while (1) {
        printf("before epoll_wait\n");
        nfds = epoll_wait(epfd, ev_ret, NEVENTS, 10 * 1000);

        if (nfds < 0) {
            perror("epoll_wait");
            return 1;
        }

        printf("after epoll_wait\n");

        /* タイムアウトの場合にepoll_waittは0を返します */
        if (nfds == 0) {
            /* ループから抜けます */
            printf("timeout\n");
            break;
        }

        for (i = 0; i < nfds; i++) {
            if (ev_ret[i].data.fd == sock1) {
                /* sock1に読み込み可能データがある場合 */
                /* sock1からデータを受信して表示します */
                n = recv(sock1, buf, sizeof(buf), 0);
                write(fileno(stdout), buf, n);
            } else if (ev_ret[i].data.fd == sock2) {
                /* sock2に読み込み可能データがある場合 */
                /* sock2からデータを受信して表示します */
                n = recv(sock2, buf, sizeof(buf), 0);
                write(fileno(stdout), buf, n);
            }
        }
    }

    close(sock1);
    close(sock2);
    return 0;
}
