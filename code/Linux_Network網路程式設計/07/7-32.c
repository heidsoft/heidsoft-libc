#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MYIPCKEY 0xdeadbeef
#define NSEMS 16

int
my_sem_init() {
    int semid;
    unsigned short semun_array[NSEMS];
    struct sembuf sb[NSEMS];
    int i;
    semid = semget(MYIPCKEY, NSEMS, 0600 | IPC_CREAT | IPC_EXCL);

    if (semid >= 0) {
        for (i = 0; i < NSEMS; i++) {
            semun_array[i] = 1;
        }

        if (semctl(semid, NSEMS, SETALL, &semun_array) != 0) {
            perror("semctl");
            return 1;
        }

        printf("[pid:%d] new semaphore set, semid=%d\n", getpid(), semid);

        for (i = 0; i < NSEMS; i++) {
            sb[i].sem_num = i;
            sb[i].sem_op = -1;
            sb[i].sem_flg = SEM_UNDO;
        }

        printf("[pid:%d] before semop()\n", getpid());
        printf("[pid:%d] press enter to start semop()\n", getpid());
        getchar();

        if (semop(semid, sb, NSEMS) != 0) {
            perror("semop");
            return 1;
        }

        printf("[pid:%d] press enter to exit this process\n", getpid());
        getchar();
        exit(0);
    } else {
        if (errno != EEXIST) {
            perror("semget");
            return 1;
        } else {
            struct semid_ds sds;
            semid = semget(MYIPCKEY, NSEMS, 0600);

            if (semid < 0) {
                perror("semget");
                return 1;
            }

            printf("[pid:%d] before semctl()\n", getpid());

            for (;;) {
                if (semctl(semid, 0, IPC_STAT, &sds) != 0) {
                    perror("semctl");
                    return 1;
                }

                if (sds.sem_otime != 0) {
                    break;
                }

                printf("[pid:%d] waiting otime change...\n", getpid());
                sleep(2);
            }

            sb[0].sem_num = 0;
            sb[0].sem_op = -1;
            sb[0].sem_flg = SEM_UNDO;
            printf("[pid:%d] before semop()\n", getpid());

            if (semop(semid, sb, 1) != 0) {
                perror("semop");
                return 1;
            }

            printf("[pid:%d] after semop()\n", getpid());
        }
    }

    return 0;
}

int
main() {
    pid_t pid;
    pid = fork();

    if (my_sem_init() < 0) {
        printf("[pid:%d] my_sem_init() failed\n", getpid());
    }

    return 0;
}
