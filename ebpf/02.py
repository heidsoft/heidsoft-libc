from bcc import BPF

prog = """
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/in.h>

#define HTTP_PORT 80

int http_filter(struct __sk_buff *skb) {
    struct ethhdr *eth = bpf_hdr_pointer_helpers(skb, 0);
    struct iphdr *ip = (struct iphdr *)(eth + 1);
    struct tcphdr *tcp = (struct tcphdr *)(ip + 1);
    if (ip->protocol == IPPROTO_TCP && tcp->dest == htons(HTTP_PORT)) {
        bpf_trace_printk("HTTP request\\n");
    }
    return 0;
}
"""

b = BPF(text=prog)
b.attach_kprobe(event="tcp_v4_connect", fn_name="http_filter")

print("Tracing HTTP requests... Hit Ctrl-C to end.")
b.trace_print()