#include "ch14.h"
typedef struct Sema{  
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count; 
} sema_t;    //訊號量資料結構
sema_t semaphore = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 1};
#define THREADS 4

void cleanup_mutex(void *arg)  //互斥變數解鎖/清理函數
{
    printf("\tthread %lx: cleanup/unlock_mutex\n", pthread_self());
    pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void AquireSemaphore(sema_t *ps)
{
    pthread_mutex_lock(&ps->lock);
    pthread_cleanup_push(cleanup_mutex, &ps->lock); //建立互斥變數清理器
    while (ps->count == 0)
        pthread_cond_wait(&(ps->cond), &(ps->lock));
    --ps->count; 
    pthread_cleanup_pop(1);   // 釋放互斥變數
}

void ReleaseSemaphore(sema_t *ps)
{
   pthread_mutex_lock(&ps->lock);
   ++ps->count;
   pthread_cond_signal(&ps->cond);
   pthread_mutex_unlock(&ps->lock);
}

void cleanup_sema(void *arg)   // 訊號量清理器
{
   printf("\tthread %lx: cleanup_sema\n",pthread_self());
   ReleaseSemaphore((sema_t *)arg);
}

void * thread_routine(void * arg)  // 執行緒開始函數
{
   int rv;
   AquireSemaphore(&semaphore);    // 獲得訊號量
   pthread_cleanup_push(cleanup_sema, &semaphore);   // 建立訊號量清理器
   printf ("   thread %lx: sleep\n", pthread_self());
   sleep(1);
   printf ("   thread %lx: waken from sleep\n", pthread_self());
   pthread_cleanup_pop(0);        // 出現訊號量清理器 
   ReleaseSemaphore(&semaphore);  // 釋放訊號量
}

int main(int argc, char* argv[])
{
   pthread_t thread_id[THREADS];
   int i, rv, *status;

   for (i=0; i<THREADS; i++) {  // 建立許多個執行緒
      rv = pthread_create (&thread_id[i], NULL, thread_routine, NULL);
      check_error(rv, "Create thread");
   }
   sleep (2);
   for (i=0; i<THREADS; i++) { // 睡眠2秒後再取消它們
      if ((rv = pthread_cancel (thread_id[i]))== ESRCH) // 執行緒可能已經終止
         printf("thread %lx dose not exist\n", thread_id[i]);
      rv = pthread_join(thread_id[i], (void **)&status);
      check_error(rv, "Join thread");
      if (status==PTHREAD_CANCELED)  // 檢視終止原因
         printf("thread %d canceled %lx\n", i, thread_id[i]);
      else 
         printf("thread %d was not canceled\n", i);
   }
   return 0;
}
