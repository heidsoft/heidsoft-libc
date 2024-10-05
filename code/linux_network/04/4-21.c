#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

int
main() {
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFMTU, &ifr) != 0) {
        perror("ioctl");
        return 1;
    }

    close(fd);
    /* 結果を表示 */
    printf("%d\n", ifr.ifr_mtu);
    return 0;
}
