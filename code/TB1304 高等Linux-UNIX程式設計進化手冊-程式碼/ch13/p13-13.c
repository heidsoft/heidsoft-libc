#include "ch13.h"
typedef struct barrier_struct {  //barrier資料結構
   int             valid;           //合法起始化標志
   pthread_cond_t   cv;             //條件變數
   pthread_mutex_t  mtx;            //相連的互斥變數
   int   predicate;                 //條件述詞,相同的柵欄同步點具有相同的值
   int   barrier_val;               //要等待的執行緒個數
   int   blocked_threads;           //已到達的執行緒個數
} barrier_t;                             
/* 用於保護barrier起始化的互斥變數 */
pthread_mutex_t barrier_init_mutex = PTHREAD_MUTEX_INITIALIZER;
#define BARRIER_VALID 546731      //barrier起始化標志
/* barrier b起始化函數 */
int barrier_init(barrier_t *b, int val)
{
   int rv;
   /* 申請barrier_init_mutex,使得每次只容許一個執行緒進行起始化 */
   if ((rv=pthread_mutex_lock(&barrier_init_mutex))!=0) 
      return (rv);   //上鎖出錯
   if (b->valid == BARRIER_VALID) { // 已進行過起始化，此次為重新起始化
      /* 獲得用於柵欄和條件變數的互斥變數 */
      if ((rv=pthread_mutex_lock(&b->mtx))!=0){ 
         pthread_mutex_unlock(&barrier_init_mutex); 
         return(rv);
      }
      if (b->blocked_threads !=0 ){// 該barrier還有需等待的執行緒，傳回錯誤
         pthread_mutex_unlock(&b->mtx);
         pthread_mutex_unlock(&barrier_init_mutex);
         return(EBUSY);
      }
      /* 重設barrier計數值後傳回 */
      b->barrier_val=val;
      if ((rv=pthread_mutex_unlock(&b->mtx))!=0) {
         pthread_mutex_unlock(&barrier_init_mutex);
         return(rv);
      }
   }else {  // 第一次起始化
      if ((rv=pthread_mutex_init(&b->mtx, NULL))!=0)
         return (rv); 
      if ((rv=pthread_cond_init(&b->cv, NULL)) !=0) {
         pthread_mutex_unlock(&barrier_init_mutex);
         return (rv);
      }
      b->barrier_val=val;
      b->blocked_threads =0;
      b->predicate =0;
      b->valid = BARRIER_VALID;
   }
   if ((rv=pthread_mutex_unlock(&barrier_init_mutex))!=0) // 釋放鎖，然後傳回
      return (rv);
   return (0);
}  
  
/* 柵欄等待函數 */
int barrier_wait(barrier_t *b)
{
   int rv, predicate;
   if (b->valid != BARRIER_VALID) // 檢查柵欄的合法性
      return(EINVAL);             // 柵欄未起始化錯誤
   if ((rv=pthread_mutex_lock(&b->mtx)) != 0) //申請用於柵欄和條件變數的互斥變數
      return (rv);     // 上鎖出錯
   predicate = b->predicate;  // 儲存本次同步述詞值
   b->blocked_threads++;       // 增加到達執行緒計數
   if (b->blocked_threads == b->barrier_val) {  // 這是最後到達的執行緒
      /* 為下次同步重設柵欄值 */
      b->predicate += 1;                     
      b->blocked_threads = 0; 
      /* 喚醒所有受阻的執行緒 */
      if ((rv=pthread_cond_broadcast(&b->cv))!= 0) { 
          pthread_mutex_unlock(&b->mtx); // 廣播出錯,釋放互斥變數後傳回
          return(rv);  
      }
   }else{              // 這是先到達的執行緒
      /* 等待，直到所有執行緒都已到達 */
      while (b->predicate == predicate){
         rv = pthread_cond_wait(&b->cv, &b->mtx);
         if ((rv!=0)&&(rv!=EINTR)){
            pthread_mutex_unlock(&b->mtx);
            return(rv);  // 條件等待出錯,釋放互斥變數後傳回
         }
      }
   }
   /* 柵欄同步結束，釋放用於柵欄和條件變數的互斥變數 */
   if ((rv=pthread_mutex_unlock(&b->mtx))!=0)
      return(rv);   // 釋放鎖出錯
   return(0);  // 標準傳回
}
