#include <sys/sem.h>
int open_semaphore_set( key_t keyval, int numsems )
{
   int sid;
   if (!numsems)  // 我們要求不論建立還是獲得訊號量都必須指明集合中訊號量的個數
      return(-1);
   if((sid = semget( keyval, numsems, IPC_CREAT | 0660 )) == -1)
      return(-1);
   else
      return(sid);
}
