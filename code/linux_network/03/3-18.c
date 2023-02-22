#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void
print_my_port_num(int sock) {
    struct sockaddr_in s;
    socklen_t sz = sizeof(s);
    getsockname(sock, (struct sockaddr*)&s, &sz);
    printf("%d\n", ntohs(s.sin_port));
}
int
main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in senderinfo;
    socklen_t senderinfolen;
    int n;
    char buf[2048];

    if (argc != 2) {
        fprintf(stderr, "Usage : %s dstipaddr\n", argv[0]);
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
    n = sendto(sock, "HELLO", 5, 0, (struct sockaddr*)&addr, sizeof(addr));

    if (n < 1) {
        perror("sendto");
        return 1;
    }

    print_my_port_num(sock);
    memset(buf, 0, sizeof(buf));
    senderinfolen = sizeof(senderinfo);
    recvfrom(sock, buf, sizeof(buf), 0,
             (struct sockaddr*)&senderinfo, &senderinfolen);
    printf("%s\n", buf);
    close(sock);
    return 0;
}
