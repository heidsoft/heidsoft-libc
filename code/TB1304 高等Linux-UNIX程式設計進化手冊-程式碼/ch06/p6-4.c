#include "ch06.h"
#include "p6-3.c"  //pr_exit()

int zero=0;

int main(void)
{
   pid_t pid;
   int status;

   if ((pid = fork()) == 0)  /* 衍生子執行緒1 */
      exit(0);               /* 子執行緒1標準離開 */
   if ((pid = fork()) == 0)  /* 衍生子執行緒2 */
      abort();      /* 子執行緒2產生SIGABRT訊號離開 */
   if ((pid = fork()) == 0) {   /* 衍生子執行緒3 */
      status/=zero;   /* 子執行緒3因0作除數產生SIGFPE訊號離開 */
      exit(0);
   }
   while ((pid = wait(&status)) >= 0) /* 等待子執行緒  */
      pr_exit(status, pid);
   perror("wait over");
   exit(EXIT_SUCCESS);
}
