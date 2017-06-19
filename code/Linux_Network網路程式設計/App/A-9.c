#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    struct in_addr inaddr;
    char* str;
    inaddr.s_addr = inet_addr("127.0.0.1");
    printf("0x%x\n", inaddr.s_addr);
    str = inet_ntoa(inaddr);
    printf("%s\n", str);
    return 0;
}
