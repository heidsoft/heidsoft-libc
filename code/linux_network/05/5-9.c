#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/rtnetlink.h>
#include <linux/if_arp.h>

int
main() {
    int soc;
    struct sockaddr_nl sa;
    struct {
        struct nlmsghdr nh;
        struct ifinfomsg ifi;
    } request;
    int seq = 100;
    int n;
    char buf[4096];
    struct nlmsghdr* nlhdr;
    /* Netlinkソケットの作成 */
    soc = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    /* sockaddr_nlの準備 */
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = 0; /* kernel */
    sa.nl_groups = 0; /* ユニキャスト */
    /* カーネルへのリクエストメッセージの生成 */
    memset(&request, 0, sizeof(request));
    request.nh.nlmsg_len = sizeof(request);
    request.nh.nlmsg_type = RTM_GETLINK;
    request.nh.nlmsg_flags = NLM_F_ROOT | NLM_F_REQUEST;
    request.nh.nlmsg_pid = 0; /* 0なので宛先はカーネル */
    request.nh.nlmsg_seq = seq; /* 必要に応じて適切にseqを設定してください */
    request.ifi.ifi_family = ARPHRD_ETHER;
    /* カーネルへNetlinkメッセージを送信 */
    n = sendto(soc, (void*)&request, sizeof(request), 0,
               (struct sockaddr*)&sa, sizeof(sa));

    if (n < 0) {
        perror("sendto");
        return 1;
    }

    /* カーネルからの返答を受信する */
    n = recv(soc, buf, sizeof(buf), 0);

    if (n < 0) {
        perror("recvmsg");
        return 1;
    }

    printf("recv\n");

    /* 受信したNetlinkメッセージを解析する */
    for (nlhdr = (struct nlmsghdr*)buf; NLMSG_OK(nlhdr, n);
            nlhdr = NLMSG_NEXT(nlhdr, n)) {
        struct ifinfomsg* ifimsg;
        struct rtattr* rta;
        int rtalist_len;
        /* Netlinkメッセージの長さとタイプを表示 */
        printf("===\n");
        printf("len : %d\n", nlhdr->nlmsg_len);
        printf("type : %d\n", nlhdr->nlmsg_type);

        /* RTM_NEWLINKというメッセージでリンク情報が返ってくる */
        if (nlhdr->nlmsg_type != RTM_NEWLINK) {
            printf("error : %d\n", nlhdr->nlmsg_type);
            continue;
        }

        /* Netlinkメッセージのデータ部分をifinfomsgとして処理 */
        ifimsg = NLMSG_DATA(nlhdr);

        if (ifimsg->ifi_family != AF_UNSPEC && ifimsg->ifi_family != AF_INET6) {
            printf("error family : %d\n", ifimsg->ifi_family);
            continue;
        }

        /* 各種パラメータを表示 */
        printf(" family : %d\n", ifimsg->ifi_family);
        printf(" type : %d\n", ifimsg->ifi_type);
        printf(" index : %d\n", ifimsg->ifi_index);
        printf(" flags : %d\n", ifimsg->ifi_flags);
        printf(" change : %d\n", ifimsg->ifi_change);
        /* ifinfomsgの次に付加されているrtattrメッセージを解析 */
        rtalist_len = nlhdr->nlmsg_len - NLMSG_LENGTH(sizeof(struct ifinfomsg));

        for (rta = IFLA_RTA(ifimsg); RTA_OK(rta, rtalist_len);
                rta = RTA_NEXT(rta, rtalist_len)) {
            printf(" type : %d\n", rta->rta_type);

            switch (rta->rta_type) {
            case IFLA_IFNAME:
                printf(" + %s\n", (char*)RTA_DATA(rta));
                break;

            case IFLA_MTU:
                printf(" + MTU : %d\n", *(int*)RTA_DATA(rta));
                break;

            case IFLA_LINK:
                printf(" + Link Type : %d\n", *(int*)RTA_DATA(rta));
                break;

            case IFLA_ADDRESS:
            case IFLA_BROADCAST: {
                    unsigned char* a = RTA_DATA(rta);

                    if (RTA_PAYLOAD(rta) == 6) {
                        printf(" + %s : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
                               rta->rta_type == IFLA_ADDRESS ? "Address" : "Broadcast",
                               a[0], a[1], a[2], a[3], a[4], a[5]);
                    }
                }
                break;

            case IFLA_STATS: {
                    struct net_device_stats* nds = RTA_DATA(rta);

                    if (RTA_PAYLOAD(rta) != (int)sizeof(struct net_device_stats)) {
                        printf("SOMETHING WRONG!\n");
                        break;
                    }

                    printf(" + stats : rxpkt=%ld, txpkt=%ld\n",
                           nds->rx_packets, nds->tx_packets);
                }
                break;

            default:
                printf(" + other type : %d\n", rta->rta_type);
                break;
            }
        }
    }

    close(soc);
    return 0;
}
