#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

int
main() {
    int sock;
    sock = socket(AF_INET, 4000, 5000);
    write(-1, "hoge", 4);

    if (sock < 0) {
        perror("socket");
        return 1;
    }

    return 0;
}
