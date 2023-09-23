#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

//g++ `pkg-config --cflags --libs glib-2.0`  -o analyze main.cpp -lpcap 
void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    const struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct tcphdr* tcpHeader;
    char sourceIp[INET_ADDRSTRLEN];
    char destIp[INET_ADDRSTRLEN];

    ethernetHeader = (struct ether_header*)packet;
    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {
        ipHeader = (struct ip*)(packet + ETHER_HDR_LEN);
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

        printf("From: %s\n", sourceIp);
        printf("To: %s\n", destIp);

        if (ipHeader->ip_p == IPPROTO_TCP) {
            tcpHeader = (struct tcphdr*)(packet + ETHER_HDR_LEN + ipHeader->ip_hl*4);

            printf("TCP:\n");
            printf("\tSequence Number: %u\n", ntohl(tcpHeader->seq));
            printf("\tAcknowledgement Number: %u\n", ntohl(tcpHeader->ack_seq));
            printf("\tWindow Size: %d\n", ntohs(tcpHeader->window));
            printf("\tFlags: ");
            if (tcpHeader->syn == 1) printf("SYN ");
            if (tcpHeader->ack == 1) printf("ACK ");
            if (tcpHeader->fin == 1) printf("FIN ");
            if (tcpHeader->rst == 1) printf("RST ");
            if (tcpHeader->psh == 1) printf("PSH ");
            if (tcpHeader->urg == 1) printf("URG ");
            printf("\n");
        }
    }
}

int main() {
    pcap_t *descr;
    char errbuf[PCAP_ERRBUF_SIZE];

    descr = pcap_open_offline("test.pcap", errbuf);
    if (descr == NULL) {
        printf("pcap_open_offline() failed: %s\n", errbuf);
        return 1;
    }

    if (pcap_loop(descr, 0, packetHandler, NULL) < 0) {
        printf("pcap_loop() failed: %s\n", pcap_geterr(descr));
        return 1;
    }

    return 0;
}