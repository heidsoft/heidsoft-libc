#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int parseargs(char * cmdline);
char *cmdargv[20] = {0};
int main(void)
{
	pid_t pid;
	char buf[100];
	int retval;
	printf("WoLaoDa# ");
	fflush(stdout);
	while (1) {
		fgets(buf, 100, stdin);
		buf[strlen(buf) - 1] = '\0';
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			parseargs(buf);
			execvp(cmdargv[0], cmdargv);
			exit(0);
		}
		wait(&retval);
		printf("WoLaoDa# ");
		fflush(stdout);
	}
}

int parseargs(char * cmdline)
{
	char *head, *tail, *tmp;
	int i;
	head = tail = cmdline;
	for( ; *tail == ' '; tail++)
		;
	head = tail;
	for (i = 0; *tail != '\0'; i++) {
		cmdargv[i] = head;
		for( ; (*tail != ' ') && (*tail != '\0'); tail++)
			;
		if (*tail == '\0')
			continue;
		*tail++ = '\0';
		for( ; *tail == ' '; tail++)
			;
		head = tail;
	}
	cmdargv[i] = '\0';
	return i;
}
