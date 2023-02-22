#include "ch13.h"
#include "err_exit.h"
struct alarmtag {
   time_t    the_time;              // SIGALRM訊號接收時間
   int       got_alarm;             // SIGALRM訊號接收標志
} alarmtime;
void *signal_thread(void *);     // 專門處理程式終止訊號的執行緒
void *work_thread(void *);       // 工作執行緒
void alarm_handler(int signal)  // SIGALRM訊號捕捉函數
{
//   printf("catch ALARM ,%d\n",pthread_self());
   time (&alarmtime.the_time);        /* 取目前原始時間  */
   alarmtime.got_alarm = 1;            /* 設定訊號到及格志 */
}

int main (int argc, char *argv[])
{
   pthread_t  sig_tid, work_tid;     
   int rv; 
   sigset_t sig_set;
   struct sigaction sa;
   /* 設定SIGALRM訊號的動作 */
   sa.sa_handler = alarm_handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART; 
   if (sigaction(SIGALRM, &sa, NULL) == -1)   
      err_exit("sigaction failed");             // 訊號動作設定出錯 
   /* 屏蔽所有訊號，內含SIGALRM,SIGINT */
   pthread_sigmask(SIG_SETMASK, NULL, &sig_set);
   sigaddset(&sig_set, SIGINT);
   sigaddset(&sig_set, SIGUSR1); 
   rv = pthread_sigmask (SIG_BLOCK, &sig_set, NULL);
   check_error(rv, "pthread_sigmask");
   /* 建立訊號處理執行緒，它繼承設定的訊號屏蔽 */
   rv = pthread_create (&sig_tid, NULL, signal_thread, NULL);
   check_error(rv, "pthread_create");
   /* 變更訊號屏蔽，放開SIGALRM訊號 */
   sigemptyset(&sig_set);
   sigaddset (&sig_set, SIGALRM);
   rv = pthread_sigmask (SIG_UNBLOCK, &sig_set, NULL);
   check_error(rv, "pthread_sigmask");
   /* 建立工作執行緒，它繼承新設定的訊號屏蔽 */
   rv = pthread_create (&work_tid, NULL, work_thread, NULL);
   check_error(rv, "pthread_create");
   /* 等待執行緒終止 */
   rv = pthread_join(work_tid, NULL);
   check_error(rv, "pthread_join");
   rv = pthread_kill(sig_tid, SIGUSR1);
   rv = pthread_join(sig_tid, NULL);
   check_error(rv, "pthread_join");
   printf("Good Bye\n");
   exit(0);
}

void *work_thread(void *arg)  /* 工作執行緒 */
{
   int i=0;
   alarmtime.got_alarm=0;
   printf("Work_thread: I am runing, my tid is: %lu\n",pthread_self());
   alarm(5);                         // 設定一秒的定時，這是傳送給執行緒的訊號
   printf("Work_thread: I set a alarm and go to sleep now.\n zzz~~~\n");
   while (!alarmtime.got_alarm);   // 執行直到被鬧鍾打斷
//      for (i=0;i<10000000;i++)
//         for(j=0;j<100000;j++)
//           ;   // 每隔2000個迭代輸出一段訊息
//         printf(" ~~~zzz ");
//   }
   printf("Work_thread:The alam waken me up at %s", ctime(&alarmtime.the_time));
   printf("Work_thread:I finished.\n");
   pthread_exit(NULL);
}
void *signal_thread (void *arg)   /* 專門處理訊號的執行緒 */
{
   int   sig, rv;
   sigset_t   wait_set, sig_set;   
   printf("Signal_thread:I am running, my tid is %lu\n", pthread_self());    
   /* 等待特殊的訊號 */
   sigemptyset(&wait_set);
   sigaddset (&wait_set, SIGUSR1);  
   sigaddset (&wait_set, SIGINT);  
   rv = sigwait (&wait_set, &sig);
   check_error(rv, "sigwait");
   switch (sig) {
   case SIGINT:     /* 處理 SIGTERM  */
      // ...  結束之前的清場處理程式碼
      /* 放開訊號屏蔽，並以預設動作再次產生該訊號使自己終止，進一步導致執行緒終止. */
      printf ("Signal_thread:I received SIGINT:%d\n", sig);
      sigemptyset(&sig_set);
      sigaddset (&sig_set, sig); 
      rv = pthread_sigmask(SIG_UNBLOCK, &sig_set, NULL);
      signal(sig, SIG_DFL);   // 程式7-3
      raise (sig);              // 傳送給自己的同步訊號
      pthread_exit(NULL);      // 應當不會執行
   case SIGUSR1:
      printf ("Signal_thread:I received SIGUSR1 and finished.\n");
      pthread_exit(NULL); 
   default:         /* 其它訊號，不應當出現 */
      fprintf (stderr, "Unexpected signal %d ", sig);
      exit(0); 
   }
}
