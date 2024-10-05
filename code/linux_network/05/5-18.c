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
    struct ifaddrmsg* ifahdr;
    struct rtattr* rta_ipaddr;
    soc = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = 0; /* kernel */
    sa.nl_groups = 0;
    memset(buf, 0, sizeof(buf));
    nlhdr = (struct nlmsghdr*)buf;
    ifahdr = NLMSG_DATA(nlhdr);
    nlhdr->nlmsg_type = RTM_NEWADDR;
    nlhdr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
    nlhdr->nlmsg_pid = 0;
    nlhdr->nlmsg_seq = seq;
    ifahdr->ifa_family = AF_INET;
    ifahdr->ifa_prefixlen = 8;
    ifahdr->ifa_scope = RT_SCOPE_HOST;
    ifahdr->ifa_index = if_nametoindex("lo");
    ifahdr->ifa_flags = IFA_F_PERMANENT;
    nlhdr->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
    rta_ipaddr = (void*)((char*)nlhdr + nlhdr->nlmsg_len);
    rta_ipaddr->rta_type = IFA_LOCAL;
    *(u_int32_t*)RTA_DATA(rta_ipaddr) = htonl(0x7f00007f);  // 127.0.0.127
    rta_ipaddr->rta_len = RTA_LENGTH(sizeof(u_int32_t));
    nlhdr->nlmsg_len += rta_ipaddr->rta_len;
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
