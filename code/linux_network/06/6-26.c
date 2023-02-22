#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    char ipv6str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &in6addr_loopback, ipv6str, sizeof(ipv6str));
    printf("%s\n", ipv6str);
    return 0;
}
