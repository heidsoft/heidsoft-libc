#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>

#define NEVENTS 16

void
sigusr1_handler(int sig) {
    write(fileno(stdout), "signal called\n", 14);
}

int
main() {
    int nfds;
    int epfd;
    /* SIGUSR1のシグナルハンドラを設定 */
    signal(SIGUSR1, sigusr1_handler);
    epfd = epoll_create(NEVENTS);

    if (epfd < 0) {
        perror("epoll_create");
        return 1;
    }

    printf("before epoll_wait\n");
    /* SIGUSR1が来るまでepoll_waitはblockし続けます */
    nfds = epoll_wait(epfd, NULL, NEVENTS, -1);
    printf("after epoll_wait : %d\n", nfds);
    /* errnoがEINTRになっているのを確認してください */
    printf("%d\n", errno);
    perror("perror after epoll_wait");
    return 0;
}
