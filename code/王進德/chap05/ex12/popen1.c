/* popen1.c */
#include <stdio.h>

int main() {
    FILE*  fp;
    char  buf[501];
    int   ch;
    memset(buf, '\0', sizeof(buf));
    printf("Ū���޹D...\n");
    fp = popen("ls -l", "r");

    if (fp != NULL) {
        ch = fread(buf, sizeof(char), 500, fp);

        if (ch > 0) {
            printf("\n�ɮצC��p�U:\n");
            printf("%s\n", buf);
        }

        pclose(fp);
    } else {
        printf("�L�k���� popen.\n");
        return 1;
    }

    printf("\n�g�J�G�i���ɮ�...\n");
    fp = fopen("test.bin", "wb");

    if (fp == NULL) {
        printf("�L�k�}���ɮ�.\n");
        return 1;
    }

    fwrite(buf, sizeof(char), 500, fp);
    fclose(fp);
    printf("\nŪ���G�i���ɮ�.\n");
    memset(buf, '\0', sizeof(buf));
    fp = fopen("test.bin", "rb");

    if (fp == NULL) {
        printf("�L�k�}���ɮ�.\n");
        return 1;
    }

    fread(buf, sizeof(char), 500, fp);
    fclose(fp);
    printf("\n�g�J�޹D...\n\n");
    fp = popen("grep popen", "w");
    fwrite(buf, sizeof(char), 500, fp);
    pclose(fp);
    return 0;
}

