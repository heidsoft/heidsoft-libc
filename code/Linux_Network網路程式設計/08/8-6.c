#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    int sock;
    struct sockaddr_in dest1, dest2;
    char buf[1024];
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    dest1.sin_family = AF_INET;
    dest2.sin_family = AF_INET;
    /* "127.0.0.1"の部分は必要に応じて変更して下さい */
    inet_pton(AF_INET, "127.0.0.1", &dest1.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &dest2.sin_addr.s_addr);
    dest1.sin_port = htons(11111);
    dest2.sin_port = htons(22222);
    snprintf(buf, sizeof(buf), "data to port 11111\n");
    sendto(sock,
           buf, strlen(buf), 0, (struct sockaddr*)&dest1, sizeof(dest1));
    snprintf(buf, sizeof(buf), "data to port 22222\n");
    sendto(sock,
           buf, strlen(buf), 0, (struct sockaddr*)&dest2, sizeof(dest2));
    close(sock);
    return 0;
}
