#include "ch11.h"
union semun {  /* 說明semun結構 */
   int val;
   struct semid_ds *buf;
   unsigned short *array;
};
/* 移除特殊的訊號量集合 */
#define rm_semaphore(sid) semctl(sid, 0, IPC_RMID,0)
/* 給特殊的訊號量賦初值 */
void init_a_semaphore( int sid, int semnum, int initval)
{
   union semun semopts;    
   semopts.val = initval;
   semctl( sid, semnum, SETVAL, semopts);
}
/* 給特殊的訊號量集合賦初值 */
void init_all_semaphore( int sid, int val_array[])
{
   union semun semopts; 
   semopts.array = val_array;
   semctl( sid, 0, SETALL, semopts);
}
/* 改變訊號量集合的存取權限,權限參數必須是形如"660"的字串 */
int changemode(int sid, char *mode)
{
   int rc;
   union semun semopts;    
   struct semid_ds mysemds;
   semopts.buf = &mysemds;    /* 使semopts.buf 指向我們自己定義的緩沖區 */
   /* 獲得內定資料結構的目前值 */
   if ((rc = semctl(sid, 0, IPC_STAT, semopts)) == -1)
      return -1;
   sscanf(mode, "%o", &semopts.buf->sem_perm.mode); /* 改變存取權限 */
   /* 更新內定資料結構 */
   return(semctl(sid, 0, IPC_SET, semopts));
}
