#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    struct sockaddr_in server;
    int sock;
    char buf[65536];
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

    while (1) {
        printf("write %d bytes\n", sizeof(buf));
        n = write(sock, buf, sizeof(buf));

        if (n <= 0) {
            printf("write error: %d\n", n);
            break;
        }
    }

    /* socketの終了 */
    close(sock);
    return 0;
}
