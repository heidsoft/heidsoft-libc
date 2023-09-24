#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;

struct bpf_map_def SEC("maps") pkt_count = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u32),
    .value_size = sizeof(u64),
    .max_entries = 1024,
};

SEC("filter")
int  handle_ingress(struct __sk_buff *skb) {
    struct ethhdr *eth = (struct ethhdr *)(skb->data);
    u32 proto = eth->h_proto;
    u64 zero = 0, *value;

    value = bpf_map_lookup_elem(&pkt_count, &proto);
    if (value) {
        __sync_fetch_and_add(value, 1);
    } else {
        bpf_map_update_elem(&pkt_count, &proto, &zero, BPF_NOEXIST);
    }

    return BPF_RET;
}

char _license[] SEC("license") = "GPL";
int _version SEC("version") = 1;