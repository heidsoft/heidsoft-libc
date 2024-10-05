#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

int
main(int argc, char* argv[]) {
    int err;
    int sock;
    char buf[32];
    char* deststr;
    struct addrinfo hints, *res0, *res;
    int n;

    if (argc != 2) {
        printf("Usage : %s dest\n", argv[0]);
        return 1;
    }

    deststr = argv[1];
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = PF_UNSPEC;

    if ((err = getaddrinfo(deststr, "http", &hints, &res0)) != 0) {
        printf("ERROR : %s\n", gai_strerror(errno));
        return 1;
    }

    for (res = res0; res != NULL; res = res->ai_next) {
        printf("%d\n", res->ai_family);
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (sock < 0) {
            continue;
        }

        if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
            close(sock);
            continue;
        }

        break;
    }

    freeaddrinfo(res0);

    if (res == NULL) {
        /* 有効な接続ができなかった */
        fprintf(stderr, "failed\\n");
        return 1;
    }

    /* HTTPで「/」をリクエストする文字列を生成 */
    snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");
    /* HTTPリクエスト送信 */
    n = write(sock, buf, (int)strlen(buf));

    if (n < 0) {
        perror("write");
        return 1;
    }

    /* サーバからのHTTPメッセージ受信 */
    while (n > 0) {
        n = read(sock, buf, sizeof(buf));

        if (n < 0) {
            perror("read");
            return 1;
        }

        /* 受信結果を標準出力へ表示(ファイルディスクリプタ1は標準出力) */
        write(fileno(stdout), buf, n);
    }

    close(sock);
    return 0;
}
