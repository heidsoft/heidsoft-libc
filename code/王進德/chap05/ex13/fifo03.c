/* fifo03.c */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

int main(void) {
    int fd;
    int len;
    char buf[PIPE_BUF];
    time_t tp;
    mode_t mode = 0666;
    unlink("test");

    if ((mkfifo("test", mode)) < 0) {
        perror("mkfifo error");
        exit(1);
    }

    printf("fifo03 ���椤.....\n");
    /* �N�r��g�J�W�� test �� FIFO �� */
    fd = open("test", O_WRONLY);

    if (fd < 0) {
        perror("open error");
        exit(1);
    }

    while (1) {
        time(&tp);   // ���o�ثe�ɶ�
        len = sprintf(buf, "fifo03 �ǨӪ��ثe�ɶ��� %s", ctime(&tp));

        if ((write(fd, buf, len + 1)) < 0) {
            perror("write error");
            close(fd);
            exit(1);
        }

        sleep(3);
    }

    close(fd);
    exit(0);
}


