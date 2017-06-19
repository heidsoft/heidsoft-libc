#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main(int argc, char* argv[]) {
    int sock;
    int n;
    int err;
    struct addrinfo hints, *res;

    if (argc != 2) {
        fprintf(stderr, "usage : %s myipv4addr\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    err = getaddrinfo(argv[1], NULL, &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    sock = socket(res->ai_family, res->ai_socktype, 0);

    if (sock < 0) {
        perror("socket");
        return 1;
    }

    if (bind(sock, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    freeaddrinfo(res);
    printf("SSM SOURCE : %s\n", argv[1]);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    err = getaddrinfo("232.1.2.3", "12345", &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s＼n", gai_strerror(err));
        return 1;
    }

    n = sendto(sock, "HELLO", 5, 0, res->ai_addr, res->ai_addrlen);

    if (n < 1) {
        perror("sendto");
        return 1;
    }

    close(sock);
    freeaddrinfo(res);
    return 0;
}
