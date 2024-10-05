#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/rtnetlink.h>

int
main() {
    int soc;
    struct sockaddr_nl sa;
    int n;
    char buf[4096];
    struct nlmsghdr* nlhdr;
    char ipaddrstr[48];
    soc = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = 0; /* kernel */
    sa.nl_groups = RTMGRP_IPV4_IFADDR;

    if (bind(soc, (struct sockaddr*)&sa, sizeof(sa)) != 0) {
        perror("bind");
        return 1;
    }

    while (1) {
        printf("before recv()\n");
        n = recv(soc, buf, sizeof(buf), 0);

        if (n < 0) {
            perror("recv");
            return 1;
        }

        printf("after recv()\n");

        for (nlhdr = (struct nlmsghdr*)buf; NLMSG_OK(nlhdr, n);
                nlhdr = NLMSG_NEXT(nlhdr, n)) {
            struct ifaddrmsg* addrmsg;
            struct rtattr* rta;
            int n;
            printf("====\n");
            printf("len : %d\n", nlhdr->nlmsg_len);
            printf("type : %d\n", nlhdr->nlmsg_type);

            if (nlhdr->nlmsg_type != RTM_NEWADDR
                    && nlhdr->nlmsg_type != RTM_DELADDR) {
                continue;
            }

            addrmsg = NLMSG_DATA(nlhdr);
            printf(" family : %d\n", addrmsg->ifa_family);
            printf(" prefixlen : %d\n", addrmsg->ifa_prefixlen);
            printf(" flags : %d\n", addrmsg->ifa_flags);
            printf(" scope : %d\n", addrmsg->ifa_scope);
            printf(" index : %d\n", addrmsg->ifa_index);
            n = nlhdr->nlmsg_len - NLMSG_LENGTH(sizeof(struct ifaddrmsg));

            for (rta = IFA_RTA(addrmsg); RTA_OK(rta, n); rta = RTA_NEXT(rta, n)) {
                struct in_addr* ina;
                printf(" type=%d, len=%d\n", rta->rta_type, rta->rta_len);

                switch (rta->rta_type) {
                case IFA_ADDRESS:
                case IFA_BROADCAST:
                case IFA_LOCAL:
                    ina = RTA_DATA(rta);
                    inet_ntop(addrmsg->ifa_family, ina, ipaddrstr, sizeof(ipaddrstr));
                    printf(" + %s\n", ipaddrstr);
                    break;

                case IFA_LABEL:
                    printf(" + %s\n", (char*)RTA_DATA(rta));
                    break;

                default:
                    printf(" + other type\n");
                    break;
                }
            }
        }
    }

    close(soc);
    return 0;
}
