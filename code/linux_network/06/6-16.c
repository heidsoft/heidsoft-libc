#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main() {
    int sock;
    struct sockaddr_storage addr;
    socklen_t slen;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    slen = sizeof(addr);
    printf("%d\n", slen);
    getsockname(sock, (struct sockaddr*)&addr, &slen);
    printf("%d\n", slen);
    return 0;
}
