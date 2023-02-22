#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>

int
main() {
    struct ifconf ifc;
    struct ifreq* ifr;
    int fd;
    int nifs, i;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifc.ifc_len = 0;
    ifc.ifc_ifcu.ifcu_buf = NULL;
    ioctl(fd, SIOCGIFCONF, &ifc);
    printf("%d\n", ifc.ifc_len);

    if (ifc.ifc_len == 0) {
        return 1;
    }

    ifc.ifc_ifcu.ifcu_buf = (void*)malloc(ifc.ifc_len);
    ioctl(fd, SIOCGIFCONF, &ifc);
    /* kernelから返ってきた数を計算 */
    nifs = ifc.ifc_len / sizeof(struct ifreq);
    ifr = (struct ifreq*)ifc.ifc_ifcu.ifcu_buf;

    /* すべてのインターフェース名を表示 */
    for (i = 0; i < nifs; i++) {
        printf("%s\n", ifr[i].ifr_name);
    }

    close(fd);
    return 0;
}
