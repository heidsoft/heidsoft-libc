#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
/*
 * チェックサムを計算する関数です。
 * ICMPヘッダのチェックサムフィールドを埋めるために利用します。
 * IPヘッダなどでも全く同じ計算を利用するので、
 * IPヘッダのチェックサム計算用としても利用できます。
 */
unsigned short
checksum(unsigned short* buf, int bufsz) {
    unsigned long sum = 0;

    while (bufsz > 1) {
        sum += *buf;
        buf++;
        bufsz -= 2;
    }

    if (bufsz == 1) {
        sum += *(unsigned char*)buf;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}
/* main 文はここからです。*/
int
main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_ll addr;
    int n;
    char buf[2000];
    struct icmphdr* icmphdrptr;
    struct iphdr* iphdrptr;

    if (argc != 4) {
        printf("usage : %s srcaddr dstaddr ifname\n", argv[0]);
        return 1;
    }

    /* AF_PACKETソケットを作成します */
    sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));

    if (sock < 0) {
        perror("socket");
        return 1;
    }

    /* IPヘッダとICMPヘッダのポインタをセットします */
    iphdrptr = (struct iphdr*)buf;
    icmphdrptr = (struct icmphdr*)(buf + sizeof(struct iphdr));
    memset(&buf, 0, sizeof(buf));
    /* IPヘッダ情報を設定しています */
    iphdrptr->ttl = 32; // TTLは32
    iphdrptr->protocol = IPPROTO_ICMP; // ICMP
    iphdrptr->ihl = sizeof(struct iphdr) / 4;  // IP header length
    iphdrptr->version = 4; // IP version 4
    iphdrptr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
    iphdrptr->id = random(); // このサンプルではsrandom()をしてないので注意
    inet_pton(AF_INET, argv[1], &iphdrptr->saddr); // source address
    inet_pton(AF_INET, argv[2], &iphdrptr->daddr); // destination address
    /* IPヘッダのチェックサムを計算します */
    iphdrptr->check = checksum((unsigned short*)iphdrptr, sizeof(struct
                               iphdr));
    /* ICMPヘッダを用意します */
    icmphdrptr->type = ICMP_ECHO;
    icmphdrptr->code = 0;
    icmphdrptr->checksum = 0;
    icmphdrptr->un.echo.id = 0;
    icmphdrptr->un.echo.sequence = 0;
    /* ICMPヘッダのチェックサムを計算します */
    icmphdrptr->checksum = checksum((unsigned short*)icmphdrptr, sizeof(struct
                                    icmphdr));
    /* 宛先用のsockaddr_llを設定します */
    memset(&addr, 0, sizeof(addr));
    /* 送信用のsockaddr_llを設定しています */
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_IP); //IPv4です。IPv6はETH_P_IPV6です
    addr.sll_ifindex = if_nametoindex(argv[3]);
    addr.sll_halen = IFHWADDRLEN; // IFHWADDRLEN => 6
    memset(&addr.sll_addr, 0xff, IFHWADDRLEN); // 面倒なのでbroadcast
    /* ICMPヘッダだけのICMPパケットを送信します */
    /* ICMPデータ部分はプログラムを簡潔にするために省いています */
    n = sendto(sock,
               (char*)iphdrptr, sizeof(struct iphdr) + sizeof(struct icmphdr),
               0, (struct sockaddr*)&addr, sizeof(addr));

    if (n < 1) {
        perror("sendto");
        return 1;
    }

    /* ICMP送信部分はここまでです*/
    /* ここから下はICMP ECHO REPLY受信部分になります */
    memset(buf, 0, sizeof(buf));
    /* 相手ホストからのICMP ECHO REPLYを待ちます */
    n = recv(sock, buf, sizeof(buf), 0);

    if (n < 1) {
        perror("recv");
        return 1;
    }

    /* 受信データからIPヘッダ部分へのポインタを取得します */
    iphdrptr = (struct iphdr*)buf;
    /*
     * 本当はIPヘッダを調べて
     * パケットがICMPパケットかどうか調べるべきです
     */
    /* 受信データからICMPヘッダ部分へのポインタを取得します */
    icmphdrptr = (struct icmphdr*)(buf + (iphdrptr->ihl * 4));

    /* ICMPヘッダからICMPの種類を特定します */
    if (icmphdrptr->type == ICMP_ECHOREPLY) {
        printf("received ICMP ECHO REPLY\n");
    } else {
        printf("received ICMP %d\n", icmphdrptr->type);
    }

    /* 終了 */
    close(sock);
    return 0;
}
