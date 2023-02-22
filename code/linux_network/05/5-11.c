#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

int
main() {
    int soc;
    struct sockaddr_nl sa;
    char buf[4096];
    int n;
    int seq = 100;
    struct nlmsghdr* nlhdr;
    struct ifinfomsg* ifihdr;
    soc = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = 0; /* kernel */
    sa.nl_groups = 0;
    memset(buf, 0, sizeof(buf));
    nlhdr = (struct nlmsghdr*)buf;
    ifihdr = NLMSG_DATA(nlhdr);
    nlhdr->nlmsg_type = RTM_SETLINK; /* (1) */
    nlhdr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK; /* (2) */
    nlhdr->nlmsg_pid = 0;
    seq++;
    nlhdr->nlmsg_seq = seq;
    /* (3) */
    ifihdr->ifi_family = AF_UNSPEC;
    ifihdr->ifi_type = ARPHRD_LOOPBACK; /* lo is loopback */
    ifihdr->ifi_index = if_nametoindex("lo");
    /* (4) */
    /* "UP & RUNNING"にするには、ifi_flagsを以下に変更 */
    /*
    ifihdr->ifi_flags = IFF_UP | IFF_RUNNING;
    */
    ifihdr->ifi_flags = 0;
    ifihdr->ifi_change = IFF_UP | IFF_RUNNING;
    nlhdr->nlmsg_len = sizeof(struct nlmsghdr) + NLMSG_LENGTH(sizeof(struct ifinfomsg));
    n = sendto(soc, (void*)nlhdr, nlhdr->nlmsg_len, 0, (struct sockaddr*)&sa,
               sizeof(sa));

    if (n < 0) {
        perror("sendto");
        return 1;
    }

    n = recv(soc, buf, sizeof(buf), 0);

    if (n < 0) {
        perror("recvmsg");
        return 1;
    }

    printf("recv\n");

    /* (5) */
    for (nlhdr = (struct nlmsghdr*)buf; NLMSG_OK(nlhdr, n);
            nlhdr = NLMSG_NEXT(nlhdr, n)) {
        printf("len : %d\n", nlhdr->nlmsg_len);
        printf("type : %d\n", nlhdr->nlmsg_type);

        if (nlhdr->nlmsg_type == NLMSG_ERROR) {
            struct nlmsgerr* errmsg;
            /* (6) */
            errmsg = NLMSG_DATA(nlhdr);
            printf("%d, %s\n", errmsg->error, strerror(-errmsg->error));
        }
    }

    close(soc);
    return 0;
}
