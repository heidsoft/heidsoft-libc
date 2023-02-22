#include "ch07.h"
#define SIGRT  SIGRTMIN
int main(void)
{
   int numsigs=0;
   sigset_t set;
   siginfo_t info;
   union sigval val;
   struct timespec timeout;
   sigfillset(&set);
   sigprocmask(SIG_SETMASK, &set, NULL);
   if (fork()==0) {  // 子執行緒a
      int mysig, i=0;
      pid_t parent =getppid();
      printf("child will signal parent %d\n",parent);
      for (mysig=SIGRTMIN; mysig<SIGRTMAX+1; mysig++) {
         val.sival_int=i++;
         if (sigqueue(parent, mysig, val)<0)   // 向父執行緒傳送排隊訊號
            perror("sigqueue");
      }
      exit(1);
   }
   // 父執行緒
   sleep(2); /* let child done */
   printf("Parent sigwait for child to queue signals \n");
   timeout.tv_sec=0;
   timeout.tv_nsec=2;
   while (1) {
      int sig;
      sig=sigtimedwait(&set,&info,&timeout);  // 等待訊號
      if (sig<0 && (errno==EAGAIN)){
         printf("Main: done after %d signals.\n",numsigs); 
         exit(0);
      }
      printf("Main woke up by signal %d\n",sig);
      if (info.si_code != SI_QUEUE)
         printf ("Main received signo=%d. It was not send by sigqueue\n",sig);
      else 
         printf("signo=%d,pid=%d,uid=%d,val=%d\n",info.si_signo,
                 info.si_pid, info.si_uid,info.si_value.sival_int);
      numsigs++;
   } 
}
