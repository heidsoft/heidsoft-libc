#include "ch07.h"
int main(void)
{
    sigset_t base_mask, waiting_mask, oldmask;
    int sig;
     /* 阻塞使用者的中斷. */
    sigemptyset (&base_mask);
    sigaddset (&base_mask, SIGQUIT);
    sigaddset (&base_mask, SIGINT);
    if (sigprocmask (SIG_BLOCK, &base_mask, &oldmask)<0 )
       err_exit("SIG_BLOCK error");
    printf(" I have blocked SIGQUIT and SIGINT, and\n you can try "
           "to send these two signals and wait for my reply.\n");
    sleep(10);
     /* 過一會兒後,檢視是否有懸掛訊號. */
    printf("Let me see if there is any pending signal.\n");
    sigpending (&waiting_mask);
    for ( sig=1;sig<NSIG;sig++)
       if (sigismember (&waiting_mask, sig)){ 
          psignal(sig,"There is a pending signal" );
	       /* 對該訊號做出回應... */
       }
     /* 還原訊號屏蔽,這將放開SIGINT和SIGTSTP */
    if (sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
       err_exit("SIG_SETMASK error");
    /* 若果有訊號,下面的程式碼不會執行 */
    printf("Ther is no suspending signal. BYE!\n");	
    exit(EXIT_SUCCESS);
}
