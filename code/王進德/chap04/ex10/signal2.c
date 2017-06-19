/* signal2.c */
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static int alarm_flag = 0;

typedef struct student {
    char name[20];
    int  score;
} student;

void show(student s);

void set_alarm(int sig) {
    student s1;
    printf("��J�m�W: ");
    scanf("%s", s1.name);
    printf("��J���Z: ");
    scanf("%d", &s1.score);
    show(s1);
    alarm_flag = 1;
}

int main() {
    pid_t pid;
    pid = fork();
    int   status;

    switch (pid) {
    case -1:
        perror("fork error");
        exit(1);

    case 0:
        /* child */
        sleep(5);
        kill(getppid(), SIGALRM);
        exit(0);
    }

    /* parent process */
    printf("���� alarm �T��....\n");
    (void) signal(SIGALRM, set_alarm);
    waitpid(pid, &status, 0);
    //pause();

    if (alarm_flag) {
        printf("�t�α����� alarm �T��\n");
    }

    exit(0);
}

void show(student s) {
    printf("�ǥ� %s �����Z�� %d\n", s.name, s.score);
}

