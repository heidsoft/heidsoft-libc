#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

int
main(int argc, char* argv[]) {
    char* service = "12345";
    struct addrinfo hints, *res0, *res;
    int err;
    int sock;
    int fd;
    char buf[65536];
    int n, ret;

    if (argc != 3) {
        fprintf(stderr, "Usage : %s hostname filename\n", argv[0]);
        return 1;
    }

    fd = open(argv[2], O_RDONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = PF_UNSPEC;

    if ((err = getaddrinfo(argv[1], service, &hints, &res0)) != 0) {
        printf("error %d : %s\n", err, gai_strerror(err));
        return 1;
    }

    for (res = res0; res != NULL; res = res->ai_next) {
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
        printf("failed\n");
        return 1;
    }

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        ret = write(sock, buf, n);

        if (ret < 1) {
            perror("write");
            break;
        }
    }

    close(sock);
    return 0;
}
