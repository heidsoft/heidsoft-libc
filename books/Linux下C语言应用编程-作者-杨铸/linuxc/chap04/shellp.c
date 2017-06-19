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
	char *pipech;
	int fd[2];

	printf("WoLaoDa# ");
	fflush(stdout);
	while (1) {
		fgets(buf, 100, stdin);
		buf[strlen(buf) - 1] = '\0';
		if ((pipech = strchr(buf, '|')) == NULL) {
			if ((pid = fork()) < 0) {
				perror("fork");
				exit(-1);
			} else if (pid == 0) {
				parseargs(buf);
				execvp(cmdargv[0], cmdargv);
				exit(0);
			}
		} else {
			*pipech = '\0';
			if (pipe(fd) < 0) {
				perror("pipe");
				exit(-1);
			}
			if ((pid = fork()) < 0) {
				perror("fork");
				exit(-1);
			} else if (pid == 0) {
				parseargs(buf);
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) < 0) {
					perror("dup2");
					exit(-1);
				}
				execvp(cmdargv[0], cmdargv);
				exit(0);
			}
			if ((pid = fork()) < 0) {
				perror("fork");
				exit(-1);
			} else if (pid == 0) {
				parseargs(pipech + 1);
				close(fd[1]);
				if (dup2(fd[0], STDIN_FILENO) < 0) {
					perror("dup2");
					exit(-1);
				}
				execvp(cmdargv[0], cmdargv);
				exit(0);
			}
			close(fd[0]);
			close(fd[1]);
		}
		while (wait(NULL) != -1)
			;
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
