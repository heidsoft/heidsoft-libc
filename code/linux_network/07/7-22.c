#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int
main(int argc, char* argv[]) {
    int shm;
    char* ptr;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s shmid\n", argv[0]);
        return 1;
    }

    shm = atoi(argv[1]);
    ptr = shmat(shm, NULL, 0);

    if ((int)ptr == -1) {
        perror("shmat");
        return 1;
    }

    printf("string from shared memory : %s\n", ptr);
    shmdt(ptr);
    return 0;
}
