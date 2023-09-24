from bcc import BPF

prog = """
#include <net/inet_sock.h>
#include <bcc/proto.h>

#define ETH_HLEN 14

struct ip_t {
    u8 ver_ihl;
    u8 tos;
    u16 tot_len;
    u16 id;
    u16 frag_off;
    u8 ttl;
    u8 protocol;
    u16 check;
    u32 saddr;
    u32 daddr;
};

struct tcp_t {
    u16 source;
    u16 dest;
    u32 seq;
    u32 ack_seq;
};

#define HTTP_PORT 80

int http_filter(struct pt_regs *ctx, struct sock *sk) {
    u16 dport = 0;
    bpf_probe_read(&dport, sizeof(dport), &(inet_sk(sk)->inet_dport));
    if (dport == htons(HTTP_PORT)) {
        bpf_trace_printk("HTTP request\\n");
    }

    return 0;
}
"""

b = BPF(text=prog)
b.attach_kprobe(event="tcp_v4_connect", fn_name="http_filter")

print("Tracing HTTP requests... Hit Ctrl-C to end.")
b.trace_print()