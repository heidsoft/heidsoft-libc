#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int
main(int argc, char* argv[]) {
    int shm;
    struct shmid_ds sds;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s shmid\n", argv[0]);
        return 1;
    }

    shm = atoi(argv[1]);

    if (shmctl(shm, IPC_RMID, &sds) != 0) {
        perror("shmctl");
        return 1;
    }

    return 0;
}
