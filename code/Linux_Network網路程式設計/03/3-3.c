#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main() {
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in senderinfo;
    socklen_t addrlen;
    char buf[2048];
    int n;
    /* (1) */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    /* (2) */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    /* (3) */
    addrlen = sizeof(senderinfo);
    n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                 (struct sockaddr*)&senderinfo, &addrlen);
    write(fileno(stdout), buf, n);
    /* (4) */
    close(sock);
    return 0;
}
