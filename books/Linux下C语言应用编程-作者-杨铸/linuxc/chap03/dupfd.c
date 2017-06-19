#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	int destfd;
	if ((destfd = open("/tmp/yangzhu.txt", O_RDWR | O_CREAT, 0666)) < 0)
		{ perror("open"); exit(-1); }
	if (dup2(STDOUT_FILENO, destfd) < 0)
		{ perror("dup2"); exit(-1); }
	write(destfd, "test for dup2\n", 14);
	return 0;
}
	
