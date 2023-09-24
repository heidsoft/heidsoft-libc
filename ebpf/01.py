import pybpf
import pcap

# BPF program
prog = pybpfProgram()
load_xdp_bpf(prog, """
        /* define map for storing packet metadata */
        map_t metadata_map = bpf_map(type=BPF_MAP_TYPE_HASH,
                                     key_size=8,
                                     value_size=8,
                                     max_entries=1024,
                                     map_name="metadata_map");

        /* define BPF program */
        int ret = bpf_prog_load(BPF_MODEDEDGE,
                                /* BPF program */
                                "",
                                /* length of BPF program */
                                0,
                                /* program index (ignored) */
                                &prog);

        if (ret < 0) {
            printf("bpf_program_load() failed: %d\n", ret);
            exit(-1);
        }
""")

# BPF map
metadata_map = pybpf.get_map("metadata_map")

# Packet processing function
def process_packet(pkt, caplen):
    print("Received packet (len=%d)" % caplen)
    # BPF map key and value are in the 'ukey' and 'pkt' fields of the BPF_xdp_md structure
    metadata = pybpf.BPF_xdp_md().ukey
    sport = metadata.src_port
    dport = metadata.dst_port
    seq = metadata.seq
    ##print("From %d:%d to %d:%d seq=%d" % (sport, ntohs(tcp->src_port), dport, ntohs(tcp->dst_port), seq))

# Open a pcap capture
with pcap.pcap('enp0s5') as pcap:
    # Attach BPF program to interface
    ret = prog.attach(pcap.dev, pybpf.BPF.XDP_MODE)
    if ret < 0:
        print("bpf_prog_attach() failed: %d" % ret)
        exit(-1)

    # Process packets
    for event in pcap:
        process_packet(event, event.len)