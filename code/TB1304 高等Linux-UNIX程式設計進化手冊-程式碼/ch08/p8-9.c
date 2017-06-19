#include "ch08.h"
#define OUR_CLOCK CLOCK_REALTIME
timer_t mytimer;
void timer_intr(int signo, siginfo_t *extra, void *cruft) // 定時器到期訊號控制碼
{
   int noverflow;
   if (signo==SIGINT) /* 使用者輸入的CRTL/C訊號，終止程式執行 */
      exit(1);  
   /* 檢視是否有逾時 */
   if (noverflow = timer_getoverrun(*(timer_t *)extra->si_value.sival_ptr))
      printf("timer has overflowerd--error\n");
   printf("timer expiration\n");
   return;
}
main(int argc, char **argv)
{
   int c;
   struct itimerspec i;
   struct timespec resolution;
   struct sigaction sa;
   sigset_t allsigs;
   struct sigevent timer_event;
   /* 首先確保存在所希望的時鍾 */
   if (clock_getres(OUR_CLOCK, &resolution)<0)
      err_exit("clock_getres");
   printf("Clock resolution %d sec %d nsec\n", 
             resolution.tv_sec, resolution.tv_nsec);
   /* 設定定時器到期訊號知會模式 */
   timer_event.sigev_notify=SIGEV_SIGNAL; /* 產生訊號 */
   timer_event.sigev_signo=SIGRTMIN;       /* 要傳送的訊號 */
   timer_event.sigev_value.sival_ptr=(void *)&mytimer; /* 攜帶定時器ID指標 */
   /* 建立定時器 */
   if (timer_create(OUR_CLOCK, &timer_event, &mytimer)<0) 
      err_exit("timer_create");
   /* 捕捉定時器到期訊號 */
   sigemptyset(&sa.sa_mask);
   sigaddset(&sa.sa_mask,SIGRTMIN);  /* 控制碼函數中屏蔽SIGRTMIN訊號 */
   sa.sa_flags=SA_SIGINFO;             /* 實時訊號 */
   sa.sa_sigaction=timer_intr;
   sigaction(SIGRTMIN, &sa, NULL);
   /* 捕捉Ctr-c訊號 */
   sigaction(SIGINT, &sa, NULL);
   /* 確定定時時間並設定定時器 */
   i.it_interval.tv_sec=0;
   i.it_interval.tv_nsec=resolution.tv_nsec*10; /* 定時間隔時間為解析度的10倍*/
   i.it_value=i.it_interval;
   if (timer_settime(mytimer, 0, &i, NULL)<0)
      err_exit("timer_settime");
   sigemptyset(&allsigs);
   while(1){    /* 透過輸入CRTL/C終止程式執行 */
         sigsuspend(&allsigs);    // 等待定時器到期訊號
   }
}
