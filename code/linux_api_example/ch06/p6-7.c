#include "ch06.h"
int main (void)
{
   pid_t pid;
   if ((pid = fork()) < 0)
      err_exit("fork error");
   else if (pid == 0) {	            /* 第一個子執行緒 */
      if ((pid = fork()) < 0) 	    /* 再次衍生一個子執行緒 */
         err_exit("fork error ");
      else if (pid > 0) {
         printf("child exit\n");
         exit(EXIT_SUCCESS);	      /*  終止第一次衍生的子執行緒 */
      }
      /*  這是第二個子執行緒.當父執行緒執行exit後,它被init繼承. */
      sleep(2);                 /* 在此做需子執行緒做的工作  */
      printf("grandchild's parent pid = %d\n", getppid ());
      exit(EXIT_SUCCESS);
   }
   if (waitpid(pid, NULL, 0) != pid) 	    /*  等待第一個子執行緒  */
      err_exit("waitpid error");
   printf ("parent exit\n");
   exit(EXIT_SUCCESS);
}
