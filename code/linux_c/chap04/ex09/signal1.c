/* signal1.c */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

struct product {
    char      str[100];
    double    price;
};

struct product p1;

void mess(int sig) {
    printf("�T���N��: %d\n", sig);
    printf("�~�W�� %s\n", p1.str);
    printf("����� %.2lf\n", p1.price);
    printf("�t�|���� %.2lf\n", p1.price * 1.05);
    (void) signal(SIGINT, SIG_DFL);
}

int main() {
    (void) signal(SIGINT, mess);
    printf("��J�~�W: ");
    scanf("%s", &p1.str);
    printf("��J���: ");
    scanf("%lf", &p1.price);

    while (1) {
        printf("Press CTRL-C\n");
        sleep(2);
    }

    exit(0);
}
