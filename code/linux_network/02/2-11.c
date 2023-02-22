#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

int
main() {
    int sock;
    sock = socket(3000, 4000, 5000);

    if (sock < 0) {
        close(fileno(stdout));
        printf("%d\n", errno);
        perror("socket");
        return 1;
    }

    return 0;
}
