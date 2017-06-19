//#include "ch14.h"
struct sched_thread {
   int num;
   int policy;
   int priority;
   void * (*thread_func)();
};

pthread_t create_exlicit_sched_threads(struct sched_thread *arg)
{
   pthread_t tid;
   pthread_attr_t attr;
   struct sched_param param;
   int rv, scope;

   assert(arg->policy==SCHED_FIFO||arg->policy==SCHED_RR);
   /* 起始化執行緒屬性物件 */
   rv = pthread_attr_init(&attr);
   check_error(rv, "attr init");
   /* 設定執行緒屬性物件中的執行緒分派繼承屬性為非繼承 */
   rv = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
   check_error(rv, "attr_setinheritsched");
   /* 設定其競爭範圍為系統範圍 */
   rv = pthread_attr_getscope(&attr, &scope);
   check_error(rv, "attr_ getscope");
   if (scope != PTHREAD_SCOPE_SYSTEM){
      rv = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
      check_error(rv, "attr_ setscope");
   }
   /* 建立一個指定分派屬性的執行緒 */
   rv = pthread_attr_setschedpolicy(&attr, arg->policy);    // 設定分派策略
   check_error(rv, "create_exlicit_sched_thread:setschedpolicy");
   param.sched_priority = arg->priority;                   // 設定分派優先級
   rv = pthread_attr_setschedparam(&attr, &param);      
   check_error(rv, "create_exlicit_sched_thread:setschedparam");
   rv = pthread_create(&tid, &attr, arg->thread_func, (void *)arg->num);
   check_error(rv, "create thread 1");
   /* 銷毀執行緒屬性物件 */
   rv = pthread_attr_destroy(&attr);
   check_error(rv, "attr destroy");
   return tid;
}
