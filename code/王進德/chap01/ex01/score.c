/* score.c */
#include <stdio.h>
int main() {
    int score;
    printf("��J���Z: ");
    scanf("%d", &score);

    if (score >= 90) {
        printf("��\n");
    } else if (score >= 80) {
        printf("�A\n");
    } else if (score >= 70) {
        printf("��\n");
    } else if (score >= 60) {
        printf("�B\n");
    } else {
        printf("���ή�\n");
    }
}
