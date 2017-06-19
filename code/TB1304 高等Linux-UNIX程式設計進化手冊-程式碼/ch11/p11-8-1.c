#include "ch11.h"
#define SHMSZ     27
int main(void)
{
    char c, *shm, *s;
    int shmid;
    /* 建立名為"785"的共享儲存段 */
    if ((shmid = shmget(785, SHMSZ, IPC_CREAT|0666)) < 0) 
        err_exit("shmget");
    /* 連線共享儲存段至執行緒的資料空間 */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) 
        err_exit("shmat");
    /* 寫一串字母至共享儲存給另一個執行緒 */
    s = shm;
    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = '\0';
    exit(0);
}
