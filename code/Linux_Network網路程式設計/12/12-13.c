#include <pcap.h>
#include <string.h>
#include <time.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
int
main() {
    pcap_t* p;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_dumper_t* pdump;
    p = pcap_open_live("lo", 65536, 1, 10, errbuf);

    if (p == NULL) {
        fprintf(stderr, "%s\n", errbuf);
        return 1;
    }

    if ((pdump = pcap_dump_open(p, "./dump.dat")) == NULL) {
        fprintf(stderr, "%s\n", pcap_geterr(p));
        pcap_close(p);
        return 1;
    }

    if (pcap_loop(p, 10, pcap_dump, (u_char*)pdump) < 0) {
        fprintf(stderr, "%s\n", pcap_geterr(p));
        pcap_dump_close(pdump);
        pcap_close(p);
        return 1;
    }

    pcap_dump_close(pdump);
    pcap_close(p);
    return 0;
}
