#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main() {
    struct addrinfo hints;
    struct addrinfo* res = NULL;
    int sock0, sock;
    struct sockaddr_storage ss;
    socklen_t sslen;
    char buf[1024];
    int n;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    n = getaddrinfo(NULL, "12345", &hints, &res);

    if (n != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(n));
        return 1;
    }

    sock0 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sock0 < 0) {
        perror("socket");
        return 1;
    }

    if (bind(sock0, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(sock0, 5) != 0) {
        perror("listen");
        return 1;
    }

    for (;;) {
        sslen = sizeof(ss);
        sock = accept(sock0, (struct sockaddr*)&ss, &sslen);

        if (sock < 0) {
            perror("accept");
            return 1;
        }

        n = getnameinfo((struct sockaddr*)&ss, sslen,
                        buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);

        if (n != 0) {
            printf("getnameinfo : %s\n", gai_strerror(n));
            return 1;
        }

        printf("connection from [%s]\n", buf);
        n = read(sock, buf, sizeof(buf));
        n = write(sock, buf, n);
        close(sock);
    }

    close(sock0);
    return 0;
}
