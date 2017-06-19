#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NSEMS 16

int
main(int argc, char* argv[]) {
    int semid;
    struct sembuf sb;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s semid\n", argv[0]);
        return 1;
    }

    semid = atoi(argv[1]);
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    printf("before semop()\n");

    if (semop(semid, &sb, 1) != 0) {
        perror("semop");
        return 1;
    }

    printf("after semop()\n");
    printf("press enter to exit\n");
    getchar();
    return 0;
}
