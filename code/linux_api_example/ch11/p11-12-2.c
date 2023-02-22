#include "p11-12.h"

int main(void) /* 服務程式 */
{
    struct exchange *shm;
    int producer_ok,consumer_ok,i;
    int shmid;
    char readbuf[BUFSIZ];

    /* 建立訊號量consumer和producer */
    consumer_ok = open_semaphore_set(key1, 1);
    producer_ok= open_semaphore_set(key2, 1);
    init_a_semaphore(consumer_ok, 0, 1);  /* 禁止消費 */
    init_a_semaphore(producer_ok, 0, 0);  /* 容許生產 */

    /* 獲得並連線名為"shared"的共享儲存段 */
    shm = (struct exchange *)shminit(ftok("shared",0),&shmid);

    /* 從標准輸入讀資料並寫至共享儲存段 */
    for ( i=0; ; i++ ) {
         /* 讀入資料 */
        semaphore_P(consumer_ok);   /* 等待客戶執行緒釋放共享儲存段 */
        printf("Enter some text:");
        fgets(readbuf,BUFSIZ,stdin);
         /* 填充共享儲存緩沖 */
        shm->seq = i;
        sprintf(shm->buf, "%s",readbuf);
        semaphore_V(producer_ok);  /* 容許客戶執行緒取資料 */
        if (strncmp(readbuf, "end", 3) == 0 )
            break;
    }
    semaphore_P(consumer_ok); /* 等待客戶執行緒消費完畢 */
    /* 移除訊號量 */
    rm_semaphore(producer_ok);
    rm_semaphore(consumer_ok);

    /* 分離並移除共享儲存段 */
    shmdt(shm);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *)NULL);
    exit(0);
}
