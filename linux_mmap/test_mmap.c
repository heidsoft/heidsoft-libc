#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    const char *filepath = "example.txt"; // 要映射的文件路径
    int fd = open(filepath, O_RDWR); // 打开文件，可读可写
    if (fd == -1) {
        perror("open"); // 打开文件失败
        exit(EXIT_FAILURE);
    }

    // 获取文件状态信息，包括文件大小
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat"); // 获取文件信息失败
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t length = sb.st_size; // 文件大小
    // 将文件映射到内存
    void *map = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap"); // 映射失败
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 读取文件内容
    printf("File content: %s\n", (char *)map);

    // 修改文件内容
    char *text = "Hello, mmap!";
    memcpy(map, text, strlen(text));

    // 解除映射
    if (munmap(map, length) == -1) {
        perror("munmap"); // 解除映射失败
    }

    close(fd); // 关闭文件
    return 0;
}

