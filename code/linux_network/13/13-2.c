#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    int sock;
    struct sockaddr_in6 addr;
    int hops = 10;
    sock = socket(AF_INET6, SOCK_DGRAM, 0);

    if (setsockopt(sock,
                   IPPROTO_IPV6,
                   IPV6_MULTICAST_HOPS,
                   (char*)&hops,
                   sizeof(hops)) != 0) {
        perror("setsockopt");
        return 1;
    }

    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(12345);
    inet_pton(AF_INET6, "ff12::1111:2222", &addr.sin6_addr);
    sendto(sock, "HELLO", 5, 0, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    return 0;
}
