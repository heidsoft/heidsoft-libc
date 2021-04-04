/* simplepipe.c
 * parent read chars from stdin, then send them to child through pipe,
 * child receive chars from pipe ,then display on stdout
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFSIZE 1024
int main(void) {
	int pid, fd[2], n;
	char buf[BUFSIZE];
	if (pipe(fd) < 0)
		{ printf("pipe error\n"); exit(1); }
	if ((pid = fork()) < 0)
		{ printf("fork failure\n"); exit(1); }
	else if (pid == 0) {  //child
		close(fd[1]);
		while((n = read(fd[0], buf, BUFSIZE))>0) {
			write(STDOUT_FILENO, "receive from parent:  ", 21);
			write(STDOUT_FILENO, buf, n);
		}
		if (n < 0)
			{ printf("read error\n"); exit(1); }
		close(fd[0]);
		printf("exit child\n");
		exit(0);
	}
	close(fd[0]);
	while((n=read(STDIN_FILENO, buf, BUFSIZE)) >0)
	  write(fd[1], buf, n);
	if (n < 0)
		{ printf("read error\n"); exit(1); }
	close(fd[1]);
	printf("exit parent\n");
	return 0;
}
