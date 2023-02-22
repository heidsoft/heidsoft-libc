#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    struct sockaddr_in server;
    int sock;
    char buf[32];
    int n;
    /* ソケットの作成 */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    /* 127.0.0.1はlocalhost */
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    /* サーバに接続 */
    connect(sock, (struct sockaddr*)&server, sizeof(server));
    n = write(sock, "HOGE＼n", 6);
    shutdown(sock, SHUT_WR);
    n = read(sock, buf, sizeof(buf));

    if (n == 0) {
        printf("closed by peer＼n");
    }

    /* socketの終了 */
    close(sock);
    return 0;
}
