#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_KEY 12345
#define SEM_KEY 54321
#define SHM_SIZE 1024

int main() {
    int shmid, semid;
    char *shmaddr;
    struct sembuf sem_op;

    // 获取共享内存
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // 连接到共享内存
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // 获取信号量
    semid = semget(SEM_KEY, 1, 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    // 等待信号量
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;  // 减少信号量的值
    sem_op.sem_flg = 0;
    if (semop(semid, &sem_op, 1) < 0) {
        perror("semop");
        exit(1);
    }

    // 读取共享内存中的数据
    printf("Data from shared memory: %s\n", shmaddr);

    // 断开共享内存
    shmdt(shmaddr);

    // 删除共享内存和信号量
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}

