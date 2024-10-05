#include <linux/icmp.h>
#include <linux/tcp.h>
#include <netinet/in.h>
#include <bpf/libbpf.h>
#include <pcap.h>
#include <stdio.h>
#include <string.h>

#define MAX_PACKET_LEN 65535

static struct bpf_program fp;
static struct bpf_map *map;

static int setup_bpf() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap;
    struct bpf_program fprog;
    struct pcap_pkthdr header;
    u_char *pkt_data;
    int res;

    pcap = pcap_open_live("en0", MAX_PACKET_LEN, 1, 1000, errbuf);
    if (pcap == NULL) {
        fprintf(stderr, "pcap_open_live() failed: %s\n", errbuf);
        return -1;
    }

    bpf_u_int32 net, mask;
    if (pcap_lookupnet("en0", &net, &mask, errbuf) == -1) {
        fprintf(stderr, "pcap_lookupnet() failed: %s\n", errbuf);
        pcap_close(pcap);
        return -1;
    }

    /* define map for storing packet metadata */
    map = bpf_map_create(BPF_MAP_TYPE_HASH, sizeof(u64), sizeof(struct bpf Packet_metadata),
                         1024, "metadata_map");
    if (map == NULL) {
        fprintf(stderr, "bpf_map_create() failed\n");
        pcap_close(pcap);
        return -1;
    }

    /* define BPF program */
    char filter[] = "tcp port 80";
    if (bpf_program_load(BPF_MODEDEDGE, filter, strlen(filter), &fp) != 0) {
        fprintf(stderr, "bpf_program_load() failed\n");
        pcap_close(pcap);
        return -1;
    }

    /* attach BPF program to network interface */
    res = bpf_program_attach(&fp, pcap, 0);
    if (res < 0) {
        fprintf(stderr, "bpf_program_attach() failed: %d\n", res);
        pcap_close(pcap);
        return -1;
    }

    return 0;
}

static void process_packet(u_char *args, const struct pcap_pkthdr *header,
                           const u_char *pkt_data) {
    struct bpf_packet_metadata *metadata;
    u_int16_t sport, dport;
    u_int32_t seq;
    struct tcp_header *tcp;

    metadata = (struct bpf_packet_metadata *) args;
    sport = ntohs(metadata->src_port);
    dport = ntohs(metadata->dst_port);
    seq = ntohl(metadata->seq);

    tcp = (struct tcp_header *) (pkt_data + sizeof(struct eth_header) +
                                sizeof(struct ipv4_header));
    printf("Received packet (len=%d) from %d:%d to %d:%d seq=%d\n",
           header->caplen, sport, ntohs(tcp->src_port), dport, ntohs(tcp->dst_port), seq);

    bpf_map_update_key(map, (void *) &metadata->ukey, pkt_data, header->caplen);
}

int main(int argc, char *argv[]) {
    if (setup_bpf() < 0) {
        return -1;
    }

    while (1) {
        u_char *pkt_data;
        int pkt_len;
        struct pcap_pkthdr header;

        pkt_len = pcap_next(pcap, &pkt_data, &header);
        if (pkt_len > 0) {
            struct bpf_packet_metadata *metadata;

            metadata = (struct bpf_packet_metadata *) pkt_data;
            bpf_probe_read(&metadata->src_port, sizeof(metadata->src_port), (char *) &metadata->ukey.src_port);
            bpf_probe_read(&metadata->dst_port, sizeof(metadata->dst_port), (char *) &metadata->ukey.dst_port);
            bpf_probe_read(&metadata->seq, sizeof(metadata->seq), (char *) &metadata->ukey.seq);

            process_packet((u_char *) metadata, &header, pkt_data);
        }
    }

    return 0;
}