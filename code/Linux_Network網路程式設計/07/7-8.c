#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int flag = 0;

void
sigusr1_handler(int sig) {
    write(1, "signal called\n", 14);
    flag = 1;
}

int
main() {
    printf("pid=%d\n", getpid());
    signal(SIGUSR1, sigusr1_handler);

    for (;;) {
        printf("waiting for signal\n");
        sleep(5);

        if (flag != 0) {
            printf("break loop\n");
            break;
        }
    }

    return 0;
}
