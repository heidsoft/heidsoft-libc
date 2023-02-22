#include "ch14.h"
#define OUTPUTSCHEDPARAM(policy, priority)   printf(         \
                     "Thread %lx: Policy=%s, priority=%d\n", \
                     pthread_self(),                       \
                     (policy==SCHED_FIFO?"FIFO"            \
                       :(policy==SCHED_RR?"RR"             \
                       :(policy==SCHED_OTHER?"OTHER"       \
                       :"unknown"))), priority)
#include "p13-13.c"
#include "p14-6.c"

barrier_t barrier;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
 
void *t_routine(void *n)
{
   int rv, mypolicy, thread_no = (int)n;
   struct sched_param myparam;
   barrier_wait(&barrier);             // 柵欄等齊所有執行緒到達，程式13-13
   rv = pthread_mutex_lock(&mutex);   // 開始競爭互斥變數 
   check_error(rv, "mutex_lock");
   printf("Thread %d :",thread_no);
   /* 列印出執行緒的分派策略和優先級 */
   rv = pthread_getschedparam(pthread_self(),
                                   &mypolicy, &myparam);
   check_error(rv, "getschedparam");
   OUTPUTSCHEDPARAM(mypolicy, myparam.sched_priority);
   rv = pthread_mutex_unlock(&mutex);  // 釋放互斥變數
   check_error(rv, "mutex_unlock");
}

int main(void)
{
   int rv, priority,  policy;
   struct sched_param param;
   struct sched_thread t_param;

   /* 起始化一個5執行緒同步的柵欄 */
   rv = barrier_init(&barrier, 5);    // 程式13-13 
   check_error(rv, "barrier_init");

   /* 輸出起始執行緒的分派策略和優先級 */
   rv = pthread_getschedparam(pthread_self(), &policy, &param);
   check_error(rv,"main: getschedparam");
   OUTPUTSCHEDPARAM(policy, param.sched_priority);

   /*  建立4個不同分派策略和優先級的執行緒  */
   t_param.thread_func = t_routine;
   t_param.policy = SCHED_FIFO;
   t_param.num = 1;
   t_param.priority = sched_get_priority_min(SCHED_FIFO); 
   create_exlicit_sched_threads(&t_param); 

   t_param.num = 2;
   t_param.priority +=2;
   create_exlicit_sched_threads(&t_param);

   t_param.policy = SCHED_RR;
   t_param.num=3;
   t_param.priority = sched_get_priority_min(SCHED_RR);
   create_exlicit_sched_threads(&t_param);

   t_param.num=4;
   t_param.priority = sched_get_priority_max(SCHED_RR);
   create_exlicit_sched_threads(&t_param);

  /* 改變自己的分派策略和優先級 */
   if (policy == SCHED_RR) {  // 分派策略是我們需要的策略，只提升優先級
      rv = pthread_setschedprio(pthread_self(),t_param.priority+6);
      check_error(rv, "getschedparam");
   } else {                       // 否則改變策略和優先級 
      param.sched_priority += 10;
      rv = pthread_setschedparam(pthread_self(),SCHED_RR, &param);
      check_error(rv, "getschedparam");
   }
   t_routine(0);     // 參與執行緒競爭
   pthread_exit((void *)NULL);
}

