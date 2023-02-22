#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int
main() {
    int sock;
    struct sockaddr_un addr;
    int n;
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/afu_dgram");
    n = sendto(sock, "HOGE\n", 5, 0, (struct sockaddr*)&addr, sizeof(addr));
    printf("send data\n");
    close(sock);
    return 0;
}
