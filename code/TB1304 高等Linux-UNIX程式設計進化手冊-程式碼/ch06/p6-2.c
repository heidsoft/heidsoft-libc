#include "ch06.h"
char *env_init[]={"USER=unknow","PATH=/tmp",NULL};
char *path = "/home/zkj/book/ch06/echoall";
int main(void)
{
   pid_t pid;
   if ((pid=vfork()) < 0)  
      err_exit ("vfork error");
   else if (pid == 0){ /* 子執行緒 */
      if (execle(path, "echoall", "arg1", "ARG2", (char *)0, env_init) < 0)
         err_exit("execle error");
      }
   else { 
      if ((pid=vfork()) < 0)  
         err_exit("vfork error");
      else if (pid == 0){ /* 子執行緒 */
         if (execlp("./echoall", "echoall", "only one arg", (char *)0)<0)
            err_exit("execle error");
      }
   
      exit(EXIT_SUCCESS);
   }
}
