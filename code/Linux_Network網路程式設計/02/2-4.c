#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int
main() {
    int sock;
    printf("fileno(stdin) = %d\n", fileno(stdin));
    close(0);
    /* sock will be zero, and it is not an error! */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    printf("sock=%d\n", sock);
    return 0;
}
