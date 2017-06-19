#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
    struct addrinfo hints, *res;
    int err;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    err = getaddrinfo(NULL, "12345", &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    /* ソケットの作成 */
    sock0 = socket(res->ai_family, res->ai_socktype, 0);

    if (sock0 < 0) {
        perror("socket");
        return 1;
    }

    if (bind(sock0, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    freeaddrinfo(res); /* addrinfo構造体を解放 */
    /* TCPクライアントからの接続要求を待てる状態にする */
    listen(sock0, 5);
    /* TCPクライアントからの接続要求を受け付ける */
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr*)&client, &len);
    /* 5文字送信 */
    write(sock, "HELLO", 5);
    /* TCPセッションの終了 */
    close(sock);
    /* listen するsocketの終了 */
    close(sock0);
    return 0;
}

