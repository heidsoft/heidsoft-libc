#include "ch03.h"
/* 如果value非0，设置desc的O_NONBLOCK标志,否则清除该标志。成功返回0，失败返回-1.*/
int set_nonblock_flag (int desc, int value)
{
   int oldflags = fcntl(desc, F_GETFL, 0);
   if(oldflags == -1)    /* 若获取标签失败，返回错误指示. */
       return -1;
    /* 形成并设置新标签 */
   if(value != 0)
       oldflags |= O_NONBLOCK;
   else
       oldflags &= ~O_NONBLOCK;
   return fcntl(desc, F_SETFL, oldflags); 
}

