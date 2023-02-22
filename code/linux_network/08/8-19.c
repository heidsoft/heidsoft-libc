#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>

int
main() {
    struct sockaddr_in server;
    int sock;
    char buf[65536];
    int n;
    fd_set fds, writefds;
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    /* 127.0.0.1はlocalhost */
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    /* サーバに接続 */
    n = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if (n != 0) {
        perror("connect");
        return 1;
    }

    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);

    while (1) {
        memcpy(&fds, &writefds, sizeof(fds));
        printf("before select\n");
        n = select(sock + 1, NULL, &fds, NULL, NULL);

        if (n <= 0) {
            perror("select");
            break;
        }

        printf("after select\n");

        if (FD_ISSET(sock, &fds)) {
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
