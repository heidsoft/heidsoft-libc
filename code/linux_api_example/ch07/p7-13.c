#include "ch07.h"
#ifdef _POSIX_REALTIME_SIGNALS
#define SIG_STOP_CHLD SIGRTMIN
void rt_handler (int signo,siginfo_t *info,void *extra)
{
   int intval = info->si_value.sival_int;
   printf("Signal %d, value %d recieved from parent %d\n",signo,intval,
         info->si_pid);
   if (!intval)
     _exit(0); 
}
int main(void)
{
   pid_t pid;
   sigset_t set, oldset;
   if ((pid=fork())==0){  /* 子執行緒 */
      struct sigaction action;
      if (sigaction(SIGINT, &action,NULL)==-1){
         perror("sigusr:sigaction");
        _exit(1);
      }
      sigemptyset(&set);
      sigaddset(&set, SIG_STOP_CHLD);
      sigaddset(&set, SIG_STOP_CHLD+1);
      sigaddset(&set, SIGINT);
      sigprocmask(SIG_BLOCK, &set, &oldset);
      action.sa_mask=set;
      action.sa_flags=SA_SIGINFO;
      action.sa_sigaction=rt_handler;
      if ((sigaction(SIG_STOP_CHLD, &action,NULL)==-1)
             || (sigaction(SIG_STOP_CHLD+1, &action,NULL)==-1) 
             || (sigaction(SIGINT, &action,NULL)==-1)) {
         perror("sigaction");
         _exit(1);
      }                                               
      while(1)
         sigsuspend(&oldset);
  } else {  /* 父執行緒 */
     union sigval sval;
     sval.sival_int=0;
     int stat;
     sleep(2);      /* 讓父執行緒先執行 */
     sigqueue(pid, SIG_STOP_CHLD+1, sval); /* 傳送實時訊號*/ 
     sval.sival_int=1;
     sigqueue(pid, SIG_STOP_CHLD, sval);
     sval.sival_int=2;
     sigqueue (pid, SIGINT, sval);         /* 傳送普通訊號*/
     sval.sival_int=3;
     sigqueue (pid, SIGINT, sval);
     sval.sival_int=4;
     sigqueue(pid, SIG_STOP_CHLD, sval);  /* 傳送實時訊號*/
     pid = wait(&stat);
     printf("child exit status =%d\n",WEXITSTATUS(stat));
     _exit(0);
  } 
}
#endif 
