#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int
show_current(int n) {
    char buf[256];

    if (getcwd(buf, sizeof(buf)) == NULL) {
        return 1;
    }

    printf("current working directory (%d) : %s＼n", n, buf);
    return 0;
}

int
main() {
    if (show_current(1) != 0) {
        return 1;
    }

    if (chroot(".") != 0) {
        perror("chroot");
        return 1;
    }

    if (show_current(2) != 0) {
        return 1;
    }

    if (mkdir("hoge", 0755) != 0) {
        perror("mkdir");
        return 1;
    }

    if (show_current(3) != 0) {
        return 1;
    }

    if (chdir("..") != 0) {
        perror("chdir ..");
        return 1;
    }

    if (show_current(4) != 0) {
        return 1;
    }

    return 0;
}
