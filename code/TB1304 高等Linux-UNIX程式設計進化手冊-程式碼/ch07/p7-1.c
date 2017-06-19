#include "ch07.h"
volatile sig_atomic_t usr_interrupt = 0;   /* 當捕捉了SIGUSR1訊號時設定此變數 */
void sig_usr(int sig)                  /* SIGUSR1訊號捕捉函數 */
{
   usr_interrupt = 1;
}
void child_function(void)        /* 子執行緒執行此函數 */
{
   printf("I'm here!  My pid is %d.\n", (int)getpid());
   kill(getppid(), SIGUSR1);     /* 知會父執行緒 */
   /* 繼續執行 */
   puts("Bye, now....");
   exit(EXIT_SUCCESS);
}
int main(void)
{
   pid_t child_id;     
   signal (SIGUSR1, sig_usr);  /* 建立訊號控制碼 */
   child_id = fork ();          /* 建立子執行緒 */
   if (child_id == 0)           /* 子執行緒 */
      child_function();         /* 子執行緒不傳回 */
   while (!usr_interrupt) ;    /* 等待子執行緒傳送訊號 */  
   puts ("That's all!");       /* 收到訊號後繼續執行 */
   return 0;
}

