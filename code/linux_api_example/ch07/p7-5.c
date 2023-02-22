#include "ch07.h"
volatile sig_atomic_t flag = 0;   /* 該標志在 SIGALRM 訊號控制碼中被設定. */
int main (void)
{
    sigset_t block_alarm;
    /* 起始訊號集合 */
    sigemptyset (&block_alarm);
    sigaddset (&block_alarm, SIGPROF);   /* 要阻塞的訊號是SIGALRM */
     ...
    while (1){
        sigprocmask (SIG_BLOCK, &block_alarm, NULL);    /* 設定訊號屏蔽 */
        if (flag){   /* 檢查訊號到達否,若到達則清除標志flag  */         
            ACTIONS-IF-ARRIVED
            flag = 0;
        }
        sigprocmask (SIG_UNBLOCK, &block_alarm, NULL);  /* 解除訊號屏蔽 */   
        ...
    }
}

