#include "ch07.h"
#include "xmalloc.c"
int make_stack_overflow(void)
{
    int a[100000];
    static int n;
    a[100000-1] = make_stack_overflow();   	/* 遞歸呼叫直到堆疊溢位 */
    return n++;
}
void set_ss_stack(stack_t *stk, stack_t *ostk)   /* 設定替代訊號堆疊 */
{
   if ((stk->ss_sp = (void *)xmalloc(SIGSTKSZ)) == NULL) /* 為替代訊號堆疊分配空間 */
      perror("   xmalloc error");
   stk->ss_size = SIGSTKSZ; 
   stk->ss_flags = SS_ONSTACK;
   if (sigaltstack(stk, ostk) < 0)
      perror("   sigaltstack fail");
   return;
}
void check_ss_flags(char *str)    /* 檢視替代訊號堆疊狀態 */
{
   stack_t ostk;
   printf("%s\n",str);
   if (sigaltstack(NULL, &ostk) < 0)
      perror(" sigaltstack exam fail.");
   if (ostk.ss_flags&SS_ONSTACK)
      printf("   flags=SS_ONSTACK \n");
   else if(ostk.ss_flags&SS_DISABLE)
      printf("   flags=SS_DISABLE \n");
   else 
      printf("   flags=%d\n",ostk.ss_flags);
   return;
}
void sig_segv(int signo)          	/* SIGSEGV訊號控制碼 */
{
    stack_t sigstk,ostk;
    check_ss_flags("in sig_segv:");
    printf("We catched SIGSEGV signal! \n");
    exit(0);
}
void sig_usr1(int signo)          	/* SIGUSR1訊號控制碼 */
{
    stack_t sigstk,ostk;
    check_ss_flags("in sig_usr1:");
    set_ss_stack(&sigstk, &ostk);     	/* 設定替代訊號堆疊,這一次將失敗 */
    return;
}
int main(void)
{
    struct sigaction act;
    stack_t sigstk,ostk;
     /* 安裝訊號控制碼並在替代訊號堆疊上執行它們 */
    act.sa_flags = 0;
    act.sa_flags |= SA_ONSTACK;         
    sigemptyset(&act.sa_mask);
    act.sa_handler = sig_segv;
    if (sigaction(SIGSEGV,&act,NULL) <0 )
   	   perror("SIGSEGV error");
    act.sa_handler = sig_usr1;
    if (sigaction(SIGUSR1,&act,NULL) < 0)
       perror("SIGUSR1 error");
    set_ss_stack(&sigstk, &ostk); 	/* 設定替代訊號堆疊 */
    check_ss_flags("before kill in main"); /* 檢視替代訊號堆疊 */
    kill(getpid(), SIGUSR1); 	/* 給自己傳送SIGUSR1訊號 */  
    check_ss_flags("after kill in main");         
    make_stack_overflow();	/* 造成堆疊空間溢位 */ 
    return;
}
