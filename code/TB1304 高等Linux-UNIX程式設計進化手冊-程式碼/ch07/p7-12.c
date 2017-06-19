#include "ch07.h"
volatile pid_t pkid;
sig_atomic_t killed=0;
sigjmp_buf again;
void sig_handler(int sig, siginfo_t *sip, void *extra)
{
    printf ("%d got a signal: %d, ", getpid(), sig);
    fflush(stdout); 
    psignal(sig, "");
    if (sip->si_code == SI_USER){
        printf("\tsi_code %d, signal was sened by kill(), sender is %d\n",
                 sip->si_code, sip->si_pid);
        return;
    }
    printf ("\tsi_code %d, ", sip->si_code);
    switch (sig){  /* 對不同的訊號，根據si_code的值對訊號原因進行不同的判別 */
    case SIGFPE: 
       if (sip->si_code==FPE_INTDIV)
          printf ("integer divided by zero");
       else if (sip->si_code==FPE_FLTDIV)
          printf ("floating point divided by zero");
       else /* … 其它情形 */
          printf ("overflow or something else");
       printf (" at address %x\n", sip->si_addr);
       siglongjmp(again, 1); /* 傳回時略過出錯點 */
       break;
    case SIGSEGV: 
       if (sip->si_code == SEGV_MAPERR)
           printf ("unmapped address:");
       else
           printf ("unprivileged access address:");
       printf("%x\n", sip->si_addr);
       exit(0);   /* 這是最後傳送的訊號，程式從這裡終止執行 */
    case SIGCHLD:
       if (sip->si_code == CLD_KILLED)
           printf("\tChild %d was killed\n",sip->si_pid);
       else 
           printf("\tChild exited with exit_status: %d\n",sip->si_status);
       killed = 1;         /* 設定訊號處理完畢標志 */
       break;
    default:
       printf("others\n");
       exit(0);
   }
}
void (*fun)(int)=NULL;   /* 懸空的函數指標 */
int a, b=0;
int main(void)
{
   struct sigaction sa;
   /* 設定訊號控制碼 */
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;         /* 使用帶三個參數的訊號控制碼 */
   sa.sa_sigaction = sig_handler;
   sigaction(SIGCHLD, &sa, 0);
   sigaction(SIGFPE, &sa, 0);
   sigaction(SIGSEGV, &sa, 0);
   /* 建立一個子執行緒,然後kill它 */ 
   if (pkid = fork()){     /* 父執行緒 */
       printf("I am father,My pid=%d. \n",getpid());
       printf("I forked and killed a child whose pid is %d\n", pkid);
       kill(pkid,SIGTERM); 
       while (!killed);              /* 等待直到子執行緒離開 */
       kill(getpid(),SIGFPE);
       if (sigsetjmp(again,1))      /* 設定非局部控制轉移環境 */
          goto L1;
       a=a/b;            /* 浮點例外後將從訊號控制碼傳回到上一敘述 */ 
L1:
      (*fun)(a);        /* 空指標將導致函數呼叫產生段錯, 訊號控制碼不傳回 */
   } else               /* 子執行緒 */
       while(1);        /* 子執行緒等待被kill */
}
