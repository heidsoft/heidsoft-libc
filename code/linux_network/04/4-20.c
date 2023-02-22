#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>

int
main() {
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    /* ethernetである */
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    /* 変更するハードウェアアドレス値 */
    ifr.ifr_hwaddr.sa_data[0] = 0xAA;
    ifr.ifr_hwaddr.sa_data[1] = 0xBB;
    ifr.ifr_hwaddr.sa_data[2] = 0xCC;
    ifr.ifr_hwaddr.sa_data[3] = 0xDD;
    ifr.ifr_hwaddr.sa_data[4] = 0xEE;
    ifr.ifr_hwaddr.sa_data[5] = 0xFF;
    /* eth0を変更 */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    /* ハードウェアアドレス変更 */
    if (ioctl(fd, SIOCSIFHWADDR, &ifr) != 0) {
        /* 失敗したら理由を表示 */
        perror("ioctl");
        return 1;
    }

    close(fd);
    return 0;
}
