#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MTEXTSIZE 10

int
main(int argc, char* argv[]) {
    int msgid;
    struct msgbuf {
        long mtype;
        char mtext[MTEXTSIZE];
    } mbuf;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s msgid\n", argv[0]);
        return 1;
    }

    msgid = atoi(argv[1]);
    mbuf.mtype = 777;
    memset(&mbuf.mtext, 0, MTEXTSIZE);
    mbuf.mtext[0] = 'B';

    if (msgsnd(msgid, &mbuf, MTEXTSIZE, 0) != 0) {
        perror("msgsnd");
        return 1;
    }

    return 0;
}
