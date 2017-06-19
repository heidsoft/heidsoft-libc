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
    struct addrinfo hints, *res0, *res;
    int n;
    int err;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s dst\n", argv[0]);
        return 1;
    }

    /* IPアドレス表記＋ホスト名両方に対応 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; /* IPv4/IPv6両方に対応 */
    hints.ai_socktype = SOCK_DGRAM;
    err = getaddrinfo(argv[1], "12345", &hints, &res0);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    for (res = res0; res != NULL; res = res->ai_next) {
        sock = socket(res->ai_family, res->ai_socktype, 0);

        if (sock < 0) {
            perror("socket");
            return 1;
        }

        n = sendto(sock, "HELLO", 5, 0, res->ai_addr, res->ai_addrlen);

        if (n < 1) {
            perror("sendto");
        }

        close(sock);

        if (n > 0) {
            break;
        }
    }

    freeaddrinfo(res0);
    return 0;
}
