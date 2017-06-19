#include "ch03.h"
char bufl[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";
int main (void)
{
    int fd;
    if ((fd = open("file.hole",O_WRONLY|O_CREAT/*|O_APPEND*/, 0644)) < 0)
        err_exit("creat error" );
    if (write (fd, bufl, 10) != 10)
        err_exit("buf1 write error");
	    /* 檔案目前位置為10，現在將它移到40 */
    if (lseek(fd, 40, SEEK_SET) == -1)
        err_exit("lseek error");
	    /* 檔案位置現在為40 */
    if (write (fd, buf2, 10) != 10)
       err_exit("buf2 write error");
	    /* 檔案位置現在為50 */
    exit(0);
}
