#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>

int
main() {
    int sock;
    struct sockaddr_in server;
    struct timeval tv;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /* 接続先指定用構造体の準備 */
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    /* 192.168.0.0は利用されていないIPアドレス */
    inet_pton(AF_INET, "192.168.0.0", &server.sin_addr.s_addr);
    /* タイムアウトを1秒に設定。tv_secが秒、tv_usecがマイクロ秒 */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
    connect(sock, (struct sockaddr*)&server, sizeof(server));
    perror("connect");
    close(sock);
    return 0;
}
