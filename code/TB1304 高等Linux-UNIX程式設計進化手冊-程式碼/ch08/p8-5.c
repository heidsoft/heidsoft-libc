#include "ch08.h"
#define  TICK  (double)sysconf(_SC_CLK_TCK)
void work(int);
int main(void)
{
   clock_t start, stop;
   struct tms buf;
   int i;
   pid_t pid;
   start = times(&buf);
   if (!(pid = fork())) {
      work(8);  /* 子執行緒程式碼 */
      exit(0);
   }
   /* 父執行緒程式碼 */
   work(10);
   pid = wait(NULL);   /* 等待子執行緒  */
   stop = times(&buf);
   printf ("  wall time: %lf (seconds), total %ld (ticks)\n",
       (double)(stop-start)/TICK, stop-start);
   printf ("  user time: %lf\n system time: %lf\n ",
       (double)(buf.tms_utime + buf.tms_cutime) / TICK,
       (double)(buf.tms_stime + buf.tms_cstime) / TICK);
   printf (" child: user time-- %lf, system time-- %lf \n",
       (double)(buf.tms_cutime) / TICK, (double)(buf.tms_cstime) / TICK);
   printf ("parent: user time-- %lf, system time-- %lf\n",
       (double)(buf.tms_utime) / TICK, (double)(buf.tms_stime) / TICK);
   printf ("process cpu time: %lf, _SC_CLK_TCK=%ld\n",
       (double)(buf.tms_utime + buf.tms_stime) / TICK, sysconf(_SC_CLK_TCK));
   return 0;
}
void work(int k)       /* 類比工作 */
{
   FILE *f;
   int i;
   double x=4.5;
   f = tmpfile();
   for(i=0; i < k*100000; i++)
      fprintf(f, "Do some output\n");
   for (i=0; i<k*100000; i++)
      x=log(x*x + 3.21);
}
