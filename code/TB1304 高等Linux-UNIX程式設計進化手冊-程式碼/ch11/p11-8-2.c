#include "ch11.h"
#define SHMSZ     27
int main(void)
{
    int shmid;
    key_t key;
    char *shm, *s;
    /* 獲得名為“785”的共享儲存段 */
    key = 785; 
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) 
        err_exit("shmget");
    /* 連線該共享儲存段 */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) 
        err_exit("shmat");
    /* 從共享儲存段讀服務執行緒所寫的資料，並輸出它們*/
    for (s = shm; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');
    exit(0);
}
