#include "p11-12.h"

int main(void) /* 客戶程式 */
{
    struct exchange *shm;
    int shmid, producer_ok,consumer_ok,i;

    /* 開啟訊號量consumer和producer */
    consumer_ok = open_semaphore_set(key1, 1);
    //init_a_semaphore(consumer_ok, 0, 0);
    /* 建立訊號量producer */
    producer_ok = open_semaphore_set(key2, 1);
    //init_a_semaphore(producer_ok, 0, 0);

    /* 獲得並連線名為"shared"的共享儲存段 */
    shm = (struct exchange *)shminit(ftok("shared",0),&shmid);

    /* 從共享儲存段讀服務執行緒所寫的資料，並輸出它們*/
    while(1){
       semaphore_P(producer_ok);   /* 等待資料產生完畢 */
       /* 處理資料，遇到"end"結束循環 */
       printf("%d recieived:Sequence=%d,data=%s",getpid(), shm->seq, shm->buf);
       if (strncmp(shm->buf, "end", 3) == 0)
          break;
       semaphore_V(consumer_ok);  /* 讓服務執行緒產生資料 */        
    }
    /* 分離共享儲存段 */
    shmdt(shm);
    semaphore_V(consumer_ok);  /* 讓服務執行緒產生資料 */        
    exit(0);
}
