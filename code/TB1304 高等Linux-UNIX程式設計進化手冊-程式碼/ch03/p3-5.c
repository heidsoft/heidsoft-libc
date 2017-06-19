#include "ch03.h"
int display_file_status_flags(int fd)
{
   int accmode, val;
   if ((val = fcntl(fd, F_GETFL, 0) ) < 0)
       err_exit(“fcntl error”);
/* 截取文件访问方式标志 */
accmode = val & O_ACCMODE;
/* 打印出所有设置了的标志 */
   If (accmode == O_RDONLY)
       printf(“read only”);
   else if (accmode == O_WRONLY)
       printf(“write only”);
   else if (accmode == O_RDWR)
       printf(“read write”);
   else {
       printf(“unknown access mode\n”);
	  exit(1);
   }
   if (val & O_APPEND)
       printf(“, append”);
   if (val & O_NONBLOCK)
       printf(“, nonblocking”);
#if !defined(POSIX_SOURCE) && defined (O_SYNC)
   if (val & O_SYNC)
       printf(“, synchronous writes”);
#endif
   putchar(‘\n‘ );
   exit(0);
}
