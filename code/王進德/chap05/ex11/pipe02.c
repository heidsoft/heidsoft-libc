/* pipe02.c */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char* argv[]) {
    int fd[2];
    int fdin;
    char buf[PIPE_BUF];
    int pid, len;

    // �إ� Pipe
    if ((pipe(fd)) < 0) {
        perror("pipe error");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    // �l�{��,�������g�J�Ϊ� Pipe,�AŪ�� Pipe
    if (pid == 0) {
        close(fd[1]);

        while ((len = read(fd[0], buf, PIPE_BUF)) > 0) {
            buf[len] = '\0';
            printf("Read %d bytes..\n", len);
            printf("%s\n", buf);
        }

        close(fd[0]);
    }

    // ���{��,������Ū���Ϊ� Pipe,�A�g�J Pipe
    if (pid > 0) {
        close(fd[0]);
        fdin = open("pipe01.c", O_RDONLY);

        if (fdin < 0) {
            perror("open error");
            exit(1);
        }

        while ((len = read(fdin, buf, PIPE_BUF)) > 0) {
            write(fd[1], buf, len);
        }

        close(fdin);
        close(fd[1]);
    }

    waitpid(pid, NULL, 0);
    exit(0);
}







