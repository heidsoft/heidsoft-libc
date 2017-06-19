#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int
main() {
    int shm;
    char* ptr;
    shm = shmget(IPC_PRIVATE, 128, IPC_CREAT | 0600);

    if (shm < 0) {
        perror("shmget");
        return 1;
    }

    ptr = shmat(shm, NULL, 0);

    if ((int)ptr == -1) {
        perror("shmat");
        return 1;
    }

    strcpy(ptr, "HOGE");
    shmdt(ptr);
    return 0;
}
