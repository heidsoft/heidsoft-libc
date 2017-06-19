#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    int sock;
    struct sockaddr_in addr;
    int ttl = 10;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (setsockopt(sock,
                   IPPROTO_IP,
                   IP_MULTICAST_TTL,
                   (char*)&ttl,
                   sizeof(ttl)) != 0) {
        perror("setsockopt");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_pton(AF_INET, "239.192.1.2", &addr.sin_addr.s_addr);
    sendto(sock, "HELLO", 5, 0, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    return 0;
}
