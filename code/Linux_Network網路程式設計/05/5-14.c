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
    struct {
        struct nlmsghdr nh;
        struct ifaddrmsg ifa;
    } request;
    int seq = 100;
    int n;
    char buf[4096];
    struct nlmsghdr* nlhdr;
    soc = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = 0; /* kernel */
    sa.nl_groups = 0;
    request.nh.nlmsg_len = sizeof(request);
    request.nh.nlmsg_type = RTM_GETADDR;
    request.nh.nlmsg_flags = NLM_F_ROOT | NLM_F_REQUEST;
    request.nh.nlmsg_pid = 0;
    seq++;
    request.nh.nlmsg_seq = seq;
    request.ifa.ifa_family = AF_INET;
    n = sendto(soc, (void*)&request, sizeof(request), 0,
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
        struct ifaddrmsg* addrmsg;
        struct rtattr* rta;
        int n;
        printf("====\n");
        printf("len : %d\n", nlhdr->nlmsg_len);
        printf("type : %d\n", nlhdr->nlmsg_type);

        if (nlhdr->nlmsg_type != RTM_NEWADDR) {
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
            printf(" type=%d, len=%d\n", rta->rta_type, rta->rta_len);

            switch (rta->rta_type) {
            case IFA_ADDRESS:
            case IFA_BROADCAST:
            case IFA_LOCAL: {
                    char str[128];
                    char* addrptr = RTA_DATA(rta);
                    inet_ntop(addrmsg->ifa_family, addrptr, str, sizeof(str));
                    printf(" + %s\n", str);
                }
                break;

            case IFA_LABEL:
                printf(" + %s\n", (char*)RTA_DATA(rta));
                break;

            default:
                printf(" other type\n");
                break;
            }
        }
    }

    close(soc);
    return 0;
}
