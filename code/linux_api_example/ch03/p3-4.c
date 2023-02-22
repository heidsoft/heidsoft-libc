/* 若value非0，設定FD_CLOEXEC標志,若value為0,清除該標志。成功傳回0，失敗傳回-1*/
#include "ch03.h"
int set_cloexec_flag (int desc, int value)
{
   int oldflags = fcntl (desc, F_GETFD, 0);
   if(oldflags < 0)    /* 若讀標簽失敗，錯誤傳回。*/
       return oldflags;
   /* 設定需要的標志. */
   if(value != 0)
       oldflags |= FD_CLOEXEC;
   else
      oldflags &= ~FD_CLOEXEC;
   return fcntl (desc, F_SETFD, oldflags); 
}
