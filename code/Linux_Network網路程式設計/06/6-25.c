#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    char ipv4str[INET_ADDRSTRLEN];
    struct in_addr addr4;
    addr4.s_addr = ntohl(INADDR_LOOPBACK);
    inet_ntop(AF_INET, &addr4, ipv4str, sizeof(ipv4str));
    printf("%s\n", ipv4str);
    return 0;
}
