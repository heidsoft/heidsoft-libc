#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>

#define NEVENTS 16

int soc[2];

void
processA() {
    sleep(3);
    printf("processA : send message\n");
    write(soc[0], "HOGE", 4);
    return;
}

void
processB() {
    int epfd;
    struct epoll_event ev, ev_ret[NEVENTS];
    int nfds;
    int i;
    char buf[128];
    epfd = epoll_create(NEVENTS);

    if (epfd < 0) {
        perror("epoll_create");
        return;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = soc[1];

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, soc[1], &ev) != 0) {
        perror("epoll_ctl");
        return;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fileno(stdin);

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fileno(stdin), &ev) != 0) {
        perror("epoll_ctl");
        return;
    }

    while (1) {
        printf("before epoll_wait\n");
        nfds = epoll_wait(epfd, ev_ret, NEVENTS, -1);

        if (nfds < 0) {
            perror("epoll_wait");
            return;
        }

        printf("after epoll_wait\n");

        for (i = 0; i < nfds; i++) {
            if (ev_ret[i].data.fd == soc[1]) {
                printf("processB: break message from socketpair\n");
                goto outofloop;
            } else if (ev_ret[i].data.fd == fileno(stdin)) {
                read(fileno(stdin), buf, sizeof(buf));
                printf("processB: input from stdin\n");
            }
        }
    }

outofloop:
    printf("processB: outside of loop\n");
    return;
}

int
main() {
    int ret;
    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, soc);

    if (ret != 0) {
        perror("socketpair");
        return 1;
    }

    if (fork() == 0) {
        processA();
    } else {
        processB();
    }

    return 0;
}
