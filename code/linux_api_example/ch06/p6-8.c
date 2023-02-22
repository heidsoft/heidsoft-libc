#include "ch06.h"
#include "p6-3.c"  //pr_exit()
int main()
{
   int status;
	   printf("running ps with system\n");
	   if ((status = system("ps -af"))<0)
      err_exit("system() error");
   pr_exit(status, -1);      /* 傳遞一個假的PID */
	   exit(EXIT_SUCCESS);
}
