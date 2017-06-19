/* waitpid1.c */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void disp0();
void disp1();
void disp2();
int  mymax(int, int);

int main(void) {
    pid_t child;
    int   status, ans;
    void (*fun[3])();
    int (*pm)(int x, int y);
    int   num1, num2, num3, max_num;
    fun[0] = disp0;
    fun[1] = disp1;
    fun[2] = disp2;
    pm = mymax;
    printf("�O�_�ƻs�{��? (1:Yes, 2:No) ");
    scanf("%d", &ans);

    if (ans == 1) {
        child = fork();

        if (child == -1) {
            perror("fork error");
            exit(1);
        }
    }

    if (child == 0) {
        puts("�o�O�l�{��...");
        printf("\t �l�{�� pid = %d\n", getpid());
        printf("\t �l�{�� ppid = %d\n", getppid());
        printf("�n�ʶR���@�ز��~?(0:�q�� 1:�q�� 2:���T)  ");
        scanf("%d", &num3);

        if (num3 >= 0 && num3 <= 2) {
            (*fun[num3])();
        }

        exit(0);
    } else {
        waitpid(child, &status, 0);
        puts("�o�O���{��...");
        printf("\t ���{�� pid= %d\n", getpid());
        printf("\t ���{�� ppid = %d\n", getppid());
        printf("\t �l�{�����}�����A: %d\n", WIFEXITED(status));
        printf("��J��1�Ӽƭ�: ");
        scanf("%d", &num1);
        printf("��J��2�Ӽƭ�: ");
        scanf("%d", &num2);
        max_num = (*pm)(num1, num2);
        printf("�̤j�Ȭ� %d\n", max_num);
        exit(0);
    }
}

int mymax(int x, int y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

void disp0() {
    printf("�z�ʶR�F�q��\n");
}

void disp1() {
    printf("�z�ʶR�F�q��\n");
}

void disp2() {
    printf("�z�ʶR�F���T\n");
}



