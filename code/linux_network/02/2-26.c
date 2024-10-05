#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main() {
    int sock0;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int yes = 1;
    struct addrinfo* res, hints;
    int err;
    char buf[2048];
    int n;
    char inbuf[2048];
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    err = getaddrinfo(NULL, "12345", &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    sock0 = socket(res->ai_family, res->ai_socktype, 0);

    if (sock0 < 0) {
        perror("socket");
        return 1;
    }

    setsockopt(sock0,
               SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    if (bind(sock0, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(sock0, 5) != 0) {
        perror("listen");
        return 1;
    }

    // 応答用HTTPメッセージ作成
    snprintf(buf, sizeof(buf),
             "HTTP/1.0 200 OK\r\n"
             "Content-Length: 20\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "HELLO\r\n");

    while (1) {
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr*)&client, &len);

        if (sock < 0) {
            perror("accept");
            break;
        }

        n = read(sock, inbuf, sizeof(inbuf));
        // 本来ならばクライアントからの要求内容をパースすべきです
        write(fileno(stdout), inbuf, n);
        // 相手が何をいおうとダミーHTTPメッセージ送信
        write(sock, buf, (int)strlen(buf));
        close(sock);
    }

    close(sock0);
    return 0;
}
