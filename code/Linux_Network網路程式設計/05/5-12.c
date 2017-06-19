#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
    struct rtattr* rta_mtu;
    soc = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = 0; /* kernel */
    sa.nl_groups = 0;
    memset(buf, 0, sizeof(buf));
    nlhdr = (struct nlmsghdr*)buf;
    ifihdr = NLMSG_DATA(nlhdr);
    nlhdr->nlmsg_type = RTM_SETLINK;
    nlhdr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
    nlhdr->nlmsg_pid = 0;
    seq++;
    nlhdr->nlmsg_seq = seq;
    ifihdr->ifi_family = ARPHRD_ETHER;
    ifihdr->ifi_type = ARPHRD_LOOPBACK;
    ifihdr->ifi_index = if_nametoindex("lo");
    ifihdr->ifi_change = 0;
    nlhdr->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    rta_mtu = (void*)((char*)nlhdr + nlhdr->nlmsg_len);
    rta_mtu->rta_type = IFLA_MTU;
    *(int*)RTA_DATA(rta_mtu) = 17000;
    rta_mtu->rta_len = RTA_LENGTH(sizeof(int));
    nlhdr->nlmsg_len += rta_mtu->rta_len;
    n = sendto(soc, (void*)nlhdr, nlhdr->nlmsg_len, 0,
               (struct sockaddr*)&sa, sizeof(sa));

    if (n < 0) {
        perror("sendto");
        return 1;
    }

    printf("recv\n");
    n = recv(soc, buf, sizeof(buf), 0);

    if (n < 0) {
        perror("recvmsg");
        return 1;
    }

    for (nlhdr = (struct nlmsghdr*)buf; NLMSG_OK(nlhdr, n);
            nlhdr = NLMSG_NEXT(nlhdr, n)) {
        printf("len : %d\n", nlhdr->nlmsg_len);
        printf("type : %d\n", nlhdr->nlmsg_type);

        if (nlhdr->nlmsg_type == NLMSG_ERROR) {
            struct nlmsgerr* errmsg;
            errmsg = NLMSG_DATA(nlhdr);
            printf("%d, %s\n", errmsg->error, strerror(-errmsg->error));
        }
    }

    close(soc);
    return 0;
}
