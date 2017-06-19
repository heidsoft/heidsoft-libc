#include "ch14.h"
int count;
int i=-1;             // 取消點標志
volatile int val=0;   // volatile修飾以防編譯器對它進行改善

void *thread_routine(void *arg)   // 執行緒開始函數 
{
   int state = PTHREAD_CANCEL_DISABLE;
   int oldstate, j;
   for (count=1;;count++) {
      for(j=0;j<1000;j++)   // 類比工作
         val=(val*j)/count;
      if (count%1024 ==0){
         i=1;
         pthread_testcancel(); // // 每隔2048個迭代回應一次取消請求
      } else if ((count-100)%2048 == 0) {
         /* 進入不可取消工作區 */
         pthread_setcancelstate(state, &oldstate); // 不容許被取消
         for(j=0;j<1000000;j++)
            val=(val*j)/count;
         pthread_setcancelstate(oldstate, &state);  // 還原以前的可取消狀態
         i=0;      
         pthread_testcancel(); // 之後及時回應可能出現的懸掛取消請求
      } 
   }
}
int main(void)
{
   pthread_t tid;
   int rv;
   void * result;
   rv=pthread_create(&tid, NULL, thread_routine, NULL);
   check_error(rv,"Create thread");
   sleep(3);
   rv=pthread_cancel(tid);              // 向執行緒發出取消請求
   check_error(rv, "cancel thread");
   rv=pthread_join(tid, &result);
   if (result == PTHREAD_CANCELED)
      printf("thread %lx was canceled at itertion %d,i=%d\n", tid,count,i);
   else 
      printf("Thread %lx was not canceled. count=%d,i=%d\n",tid, count, i);
   return 0;
}
