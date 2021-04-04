/* rwfifo.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <signal.h>

void sig_pipe(int signo)
{
	printf("I'm in sig_pipe\n");
	exit(-1);
}

int main(void)
{
	int fd;
	int len;
	char buf[PIPE_BUF];
	time_t tp;
	signal(SIGPIPE, sig_pipe);
	printf("I am %d\n", getpid());
	if((fd = open("fifo1", O_WRONLY)) < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	printf("between open and write\n");
	while(1) {
		time(&tp);
		len = sprintf(buf, "wrfifo %d sends %s", getpid(), ctime(&tp));
		
		if((write(fd,buf,len+1)) < 0) {
			perror("write");
			close(fd);
			exit(EXIT_FAILURE);
		}
		sleep(3);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}

