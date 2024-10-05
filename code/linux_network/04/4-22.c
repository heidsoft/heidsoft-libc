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
    ifr.ifr_mtu = 1400;

    if (ioctl(fd, SIOCSIFMTU, &ifr) != 0) {
        perror("ioctl");
        return 1;
    }

    close(fd);
    return 0;
}
