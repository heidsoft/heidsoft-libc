#include "ch13.h"
extern struct job *create_job();            //建立一個作業
extern void job_enqueue(struct job *ptr); //向作業佇列加入一個作業
typedef struct job_sync_data {
   int job_count;            //作業數
   pthread_mutex_t mtx;     //用於保護job_count
   pthread_cond_t cond;     //用於知會job_count的改變
} job_sync_data;
extern job_sync_data job_sync;
void master_thread()
{
   struct job *new_job;
   int rv;
   for (; ;) {
      if ((new_job = create_job())== NULL)  // 建立下一個作業
         pthread_exit((void *)NULL);         // 沒有進一步的工作則終止自己
      /* 獲得相連的互斥變數 */
      rv=pthread_mutex_lock(&job_sync.mtx); 
      check_error(rv, "mutex lock");
      /* 加入新作業至作業佇列 */
      job_enqueue(new_job);
      job_sync.job_count++;
      /* 向條件變數發訊號喚醒一個執行緒 */
      rv = pthread_cond_signal(&job_sync.cond);
      check_error(rv,"cond_signal");  
      /* 釋放相連的互斥變數 */
      rv=pthread_mutex_unlock(&job_sync.mtx);
      check_error(rv,"mtx_unlock");
   }
}
