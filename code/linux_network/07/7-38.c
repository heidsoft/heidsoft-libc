#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MTEXTSIZE 10

int
main(int argc, char* argv[]) {
    int msgid, msgtype;
    struct msgbuf {
        long mtype;
        char mtext[MTEXTSIZE];
    } mbuf;

    if (argc != 3) {
        fprintf(stderr, "Usage : %s msgid msgtype\n", argv[0]);
        return 1;
    }

    msgid = atoi(argv[1]);
    msgtype = atoi(argv[2]);

    if (msgrcv(msgid, &mbuf, MTEXTSIZE, msgtype, 0) < 0) {
        perror("msgrcv");
        return 1;
    }

    printf("%c\n", mbuf.mtext[0]);
    return 0;
}
