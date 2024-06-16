#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#define SHM_KEY 12345
#define SEM_KEY 54321
#define SHM_SIZE 1024

int main() {
    int shmid, semid;
    char *shmaddr;
    struct sembuf sem_op;

    // 创建共享内存
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
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

    // 创建信号量
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    // 初始化信号量
    if (semctl(semid, 0, SETVAL, 1) < 0) {
        perror("semctl");
        exit(1);
    }

    // 写入数据到共享内存
    strcpy(shmaddr, "Hello from writer");

    // 设置信号量，释放资源
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;  // 增加信号量的值
    sem_op.sem_flg = 0;
    if (semop(semid, &sem_op, 1) < 0) {
        perror("semop");
        exit(1);
    }

    // 断开共享内存
    shmdt(shmaddr);

    return 0;
}

