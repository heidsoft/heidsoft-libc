#include "ch13.h"
typedef struct job_sync_data {
   int job_count;            //作業數
   pthread_mutex_t mtx;     //用於保護job_count
   pthread_cond_t cond;     //用於知會job_count的改變
} job_sync_data; 
job_sync_data job_sync = {
              0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
struct job {
   int job_id;
   void * work_ptr;
};
extern struct job *job_dequeue();          //從作業佇列中分析一個作業
extern void procee_job(struct job *ptr);  //處理一個作業
void worker_thread()
{
   struct job *curr_job;
   int rv;
   for(; ;){
      /* 獲得條件變數相連的互斥變數 */
      rv=pthread_mutex_lock(&job_sync.mtx);  
      check_error(rv, "mutex lock");
      /* 測試有否作業，若果沒有則等待。條件變數等待會釋放相連的互斥變數以便其它執行緒
         能夠放置作業於佇列之中。從條件變數等待傳回之前會重新獲得該互斥變數 */
      while (job_sync.job_count == 0)
         pthread_cond_wait(&job_sync.cond, &job_sync.mtx);
      curr_job=job_dequeue(); // 得到作業
      if (curr_job != NULL) 
         job_sync.job_count--; 
      // 釋放相連的互斥變數
      rv=pthread_mutex_unlock(&job_sync.mtx);
      check_error(rv, "mtx_unlock failed");
      // 處理作業
      procee_job(curr_job); 
   }
}
