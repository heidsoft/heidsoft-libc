/* mymath.c */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymath.h"

int main() {
    int a, b;
    srand(time(NULL));
    printf("��J�@�Ӿ��: ");
    scanf("%d", &a);
    printf("��� %d �����謰 %d\n", a, fun2(a));
    b = fun3();
    printf("�H���Ʋ��;��o�쪺�Ƭ� %d\n", b);
    return 0;
}
