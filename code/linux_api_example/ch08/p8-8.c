#include "ch08.h"
#define BILLION 1000000000L
int i=1000000;
int main(void)
{
   long timediff;
   struct timespec t1, t2;
   if (clock_getres(CLOCK_REALTIME, &t1))   /* 察看實時牆鍾解析度 */
      perror("Failed to get clock resolution");
   else if (t1.tv_sec != 0)
      printf("Clock resolution no better than one second\n");
   else
      printf("Clock resolution: %ld nanoseconds\n", (long)t1.tv_nsec);
   /* 察看連續兩次clock_gettime獲得的時間 */
   if (clock_gettime(CLOCK_REALTIME, &t1)) 
      err_exit("clock_gettime failed");
   if (clock_gettime(CLOCK_REALTIME, &t2)) 
      err_exit("clock_gettime failed");
   printf("t1: tv_sec=%ld,tv_nsec=%ld\n", t1.tv_sec, t1.tv_nsec);
   printf("t2: tv_sec=%ld,tv_nsec=%ld\n", t2.tv_sec, t2.tv_nsec);
   timediff = BILLION*(t2.tv_sec - t1.tv_sec) + t2.tv_nsec - t1.tv_nsec;
   printf("The difference of t2 and t1: %ld nanoseconds\n", timediff);
   /* 檢視單調時間 */
   if (clock_gettime(CLOCK_MONOTONIC,&t1)<0) 
      err_exit("look CLOCK_MONOTONIC failed");
   printf("CLOCK_MONOTONIC time:tv_sec=%ld,tv_nsec=%ld\n"
            ,t1.tv_sec, t1.tv_nsec);
    /* 衍生子執行緒，父/子執行緒分別察看執行緒時間 */
    if (fork())
       while(i--);   /* 父執行緒工作，子執行緒無工作 */
   clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);  /* 檢視執行緒時間 */
   printf("process %d, CLOCK_PROCESS_CPUTIME_ID time: ", getpid());
   printf("tv_sec=%ld,tv_nsec=%ld\n", t1.tv_sec, t1.tv_nsec);
}
