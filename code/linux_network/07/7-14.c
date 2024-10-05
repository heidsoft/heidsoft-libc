#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>

int
main() {
    int sv[2];
    pid_t pid;
    char buf[128];
    memset(buf, 0, sizeof(buf));

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(sv[1]);
        read(sv[0], buf, sizeof(buf));
        printf("child process : data from parent process [%s]\n", buf);
        exit(0);
    } else {
        int status;
        close(sv[0]);
        write(sv[1], "HOGE", 4);
        printf("parent process : child process id %d\n", pid);
        wait(&status);
    }

    return 0;
}
