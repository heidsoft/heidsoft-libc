#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int
main(int argc, char* argv[]) {
    struct sockaddr_in server;
    int sock;
    char buf[32];
    char* deststr;
    unsigned int** addrptr;
    int n;

    if (argc != 2) {
        printf("Usage : %s dest\n", argv[0]);
        return 1;
    }

    deststr = argv[1];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr(deststr);

    if (server.sin_addr.s_addr == 0xffffffff) {
        struct hostent* host;
        host = gethostbyname(deststr);

        if (host == NULL) {
            return 1;
        }

        addrptr = (unsigned int**)host->h_addr_list;

        while (*addrptr != NULL) {
            server.sin_addr.s_addr = *(*addrptr);

            /* connect()が成功したらloopを抜けます */
            if (connect(sock,
                        (struct sockaddr*)&server,
                        sizeof(server)) == 0) {
                break;
            }

            addrptr++;
        }
    } else {
        /* inet_addr()が成功したとき */
        if (connect(sock,
                    (struct sockaddr*)&server,
                    sizeof(server)) != 0) {
            perror("connect");
            return 1;
        }
    }

    /* 本当はconnect()が成功したかどうかを判断してから次に進むべきです */
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);
    printf("%d, %s\n", n, buf);
    close(sock);
    return 0;
}
