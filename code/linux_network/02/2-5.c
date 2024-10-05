#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main() {
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    /* 0 auto proto */
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    /* internet */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock0, (struct sockaddr*)&addr, sizeof(addr));
    /* TCP listen */
    listen(sock0, 5);
    /* get sock len */
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr*)&client, &len);
    /* 5文字送信 */
    write(sock, "HELLO", 5);
    /* TCP close  */
    close(sock);
    /* listen close */
    close(sock0);
    return 0;
}
