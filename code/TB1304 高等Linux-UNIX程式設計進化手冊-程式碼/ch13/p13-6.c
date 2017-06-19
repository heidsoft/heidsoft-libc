#include "ch13.h"
#include <sys/shm.h>
#include "xmalloc.c"
/* 建立一個動態分配空間、具有定制屬性的互斥變數 */
pthread_mutex_t *mutex_customized (int shared, int type, int key)
{
   int rv;
   key_t shmid;
   pthread_mutex_t *mtx;
   pthread_mutexattr_t mtxattr;  // 互斥變數屬性物件
   /* 起始化互斥變數屬性物件 */
   rv = pthread_mutexattr_init (&mtxattr);
   check_error(rv, "mutexattr_init ");
   /* 設定共享屬性 */
   rv = pthread_mutexattr_setpshared(&mtxattr, shared);
   check_error(rv, " mutexattr_setpshared ");
   /* 設定型態屬性，在有的系統上需要明確定義特征測試巨集_GNU_SOUCE */
   rv = pthread_mutexattr_settype (&mtxattr, type);
   check_error(rv, "mutexattr_settype");
   /* 為互斥變數分配共享儲存空間 */
   if (shared == PTHREAD_PROCESS_PRIVATE)
      mtx = (pthread_mutex_t *)xmalloc(sizeof(pthread_mutex_t));     
   else {  // 執行緒共享互斥變數分配在共享儲存中
      shmid = shmget(key, sizeof(pthread_mutex_t), 0666|IPC_CREAT);
      if (shmid == -1) 
          err_exit("shmget error");
      mtx = (pthread_mutex_t *)shmat(shmid, (char *)0, 0);
      if ((int)mtx == -1) 
         err_exit("shmget error");
   }
   /* 用屬性物件起始化mtx */
   rv = pthread_mutex_init(mtx, &mtxattr);
   check_error(rv, "mutex_init ");
   /* 立即銷毀不再需要的屬性物件 */
   rv=pthread_mutexattr_destroy(&mtxattr);
   check_error(rv, "mutexattr destroy ");
   return (mtx);
} 
#define KEY 8125
pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;  // 靜態起始化的執行緒私有互斥變數
pthread_mutex_t mtx2;       // 動態起始化的執行緒私有互斥變數
pthread_mutex_t *mtx3;      // 執行緒共享互斥變數
int main(void)
{
   int rv;
   // ...
   rv = pthread_mutex_init (&mtx2, NULL); // 起始化mtx2為系統預設互斥變數
   /*起始化mtx3為定制的執行緒共享遞歸互斥變數*/
   mtx3 = mutex_customized(PTHREAD_PROCESS_SHARED,
                            PTHREAD_MUTEX_RECURSIVE,KEY);
   // ...套用程式的其它程式碼
   /* 銷毀mtx2 */
   rv = pthread_mutex_destroy(&mtx2);
   check_error(rv, "mutex_destroy: mtx2");
   pthread_exit(NULL);
}
