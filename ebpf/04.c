#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <stdint.h>

typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, u64);
} pkt_count SEC(".maps");

SEC("classifier")
int handle_ingress(struct __sk_buff *skb) {
    u16 h_proto;
    u64 zero = 0, *value;

    struct ethhdr *eth = bpf_hdr_pointer_helpers(skb, 0);
    if (!eth)
        return XDP_ABORTED;

    h_proto = bpf_ntohs(eth->h_proto);

    value = bpf_map_lookup_elem(&pkt_count, &h_proto);
    if (value) {
        __sync_fetch_and_add(value, 1);
    } else {
        bpf_map_update_elem(&pkt_count, &h_proto, &zero, BPF_NOEXIST);
    }

    return XDP_PASS;
}
char _license[] SEC("license") = "GPL";
int _version SEC("version") = 1;