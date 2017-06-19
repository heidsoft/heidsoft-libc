#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
int
main() {
    int sock;
    int n;
    char buf[2048];
    struct iphdr* ip;
    struct sockaddr_ll saddr;
    sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));

    if (sock < 0) {
        perror("socket");
        return 1;
    }

    /* bind()用のパラメータ設定 */
    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_protocol = htons(ETH_P_ALL);
    saddr.sll_ifindex = if_nametoindex("lo");

    if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
        perror("bind");
        return 1;
    }

    n = recv(sock, buf, sizeof(buf), 0);

    if (n < 1) {
        perror("recv");
        return 0;
    }

    ip = (struct iphdr*)buf;
    printf("IP version  : %d\n", ip->version);
    printf("protocol    : %d\n", ip->protocol);
    printf("source addr : %.8x\n", ip->saddr);
    printf("dest addr   : %.8x\n", ip->daddr);
    return 0;
}
