#include "ch07.h"
static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjmp=0;
static int sigalrm_appear=0;
void pr_mask(const char *str)
{
   sigset_t sigset;
   int  i;
   sigprocmask(0, NULL, &sigset);
   printf(" %s\n",str);
   for(i = 1; i < NSIG; i++)
      if(sigismember(&sigset,i)) 
		 psignal(i,"---");
}
static void sig_usr1(int signo)
{
   time_t startime;
   if(canjmp == 0)
      return;                          /* 不期望的訊號,忽略*/
   pr_mask("starting sig_usr1: ");
   alarm(3);                           /* 3秒之後產生SIGALRM訊號 */
   while(!sigalrm_appear);           /* 等待SIGALRM */
   pr_mask("finishing sig_usr1:");
   canjmp = 0;
   siglongjmp(jmpbuf,1);            /* 跳回到main,不傳回 */
}
static void sig_alrm(int signo)
{
   pr_mask("in sig_alrm: ");
   sigalrm_appear = 1;
   return;
}
int main(void)
{
   signal(SIGUSR1,sig_usr1);
   signal(SIGALRM,sig_alrm);
   pr_mask("starting main:");
   if (sigsetjmp(jmpbuf,1)){
      pr_mask("ending main: ");
      exit(EXIT_SUCCESS);
   }
   canjmp = 1;     /* 標志sigsetjmp()已被呼叫過*/
   for (;;)             
      pause();      /* 等待訊號 */
}
