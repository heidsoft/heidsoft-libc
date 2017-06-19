#include <pcap.h>
#include <string.h>
#include <time.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <arpa/inet.h>

void
pcap_handler_func(u_char* user,
                  const struct pcap_pkthdr* h,
                  const u_char* bytes) {
    char timebuf[64];
    char addrstr[64];
    struct ether_header* ethhdr = (struct ether_header*)bytes;
    struct iphdr* ipv4h;
    struct ip6_hdr* ipv6h;

    if (h->caplen < sizeof(struct iphdr) + sizeof(struct ether_header)) {
        return;
    }

    memset(timebuf, 0, sizeof(timebuf));

    if (ctime_r(&h->ts.tv_sec, timebuf) == NULL) {
        return;
    }

    timebuf[strlen(timebuf) - 1] = '\0';
    printf("%s, caplen:%d, len:%d, ", timebuf, h->caplen, h->len);

    switch (ntohs(ethhdr->ether_type)) {
    case ETHERTYPE_IP:
        printf("IPv4 ");
        ipv4h = (struct iphdr*)(bytes + sizeof(struct ether_header));
        inet_ntop(AF_INET, &ipv4h->saddr, addrstr, sizeof(addrstr));
        printf("src[%s]\n", addrstr);
        break;

    case ETHERTYPE_IPV6:
        printf("IPv6 ");
        ipv6h = (struct ip6_hdr*)(bytes + sizeof(struct ether_header));
        inet_ntop(AF_INET6, (void*)&ipv6h->ip6_src, addrstr, sizeof(addrstr));
        printf("src[%s]\n", addrstr);
        break;

    default:
        printf("other\n");
        break;
    }

    return;
}
int
main() {
    pcap_t* p;
    char errbuf[PCAP_ERRBUF_SIZE];
    char cmdstr[] = "tcp";
    struct bpf_program bpfprog;
    p = pcap_open_live("lo", 65536, 1, 10, errbuf);

    if (p == NULL) {
        fprintf(stderr, "%s\n", errbuf);
        return 1;
    }

    if (pcap_compile(p, &bpfprog, cmdstr, 0, 0) < 0) {
        fprintf(stderr, "%s\n", pcap_geterr(p));
        return 1;
    }

    if (pcap_setfilter(p, &bpfprog) < 0) {
        fprintf(stderr, "%s\n", pcap_geterr(p));
        return 1;
    }

    if (pcap_loop(p, -1, pcap_handler_func, NULL) < 0) {
        fprintf(stderr, "%s\n", pcap_geterr(p));
        pcap_close(p);
        return 1;
    }

    pcap_close(p);
    return 0;
}
