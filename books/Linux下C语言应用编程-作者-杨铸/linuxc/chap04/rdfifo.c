/* rdfifo.c*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int main(void)
{
	int fd, fd2;
	int len;
	char buf[PIPE_BUF] = "abc\n";
	mode_t mode=0666;
	if (mkfifo("fifo1",mode) <0)
//	if((mkfifo("fifo1",mode) <0) && (errno != EEXIST))
		{ printf("mkfifo error\n"); exit(1); }
	if ((fd = open("fifo1", O_RDONLY)) < 0)
//	if ((fd = open("fifo1", O_RDONLY | O_NONBLOCK)) < 0)
		{ printf("open error\n"); exit(1); }
/*
	printf("between open r and open w\n");
	if ((fd2 = open("fifo1", O_WRONLY)) < 0)
		err_sys("open for write err\n");
*/
//	printf("between open w and read\n"); 
	while((len = read(fd, buf, PIPE_BUF-1)) > 0)
	//while(((len = read(fd, buf, PIPE_BUF-1)) > 0) || errno == EAGAIN)
	{
//		sleep(1);
	  printf("rdfifo read: %s", buf);
	}
	close(fd);
	if (len == 0)
		printf("read end\n");
//	close(fd2);
	exit(EXIT_SUCCESS);
}

