/* myarray.c */
#include <stdio.h>
#include "myarray.h"

int main() {
    int score[5];
    int i;
    int ans1;
    double ans2;
    printf("�п�J���ӤH�����Z:\n");

    for (i = 0; i < 5; i++) {
        scanf("%d", &score[i]);
    }

    ans1 = mysum(score);
    printf("���ӤH�����Z�`�M��: %d ��\n", ans1);
    ans2 = myavg(score);
    printf("���ӤH���������Z�� %.2lf ��\n", ans2);
    return 0;
}

