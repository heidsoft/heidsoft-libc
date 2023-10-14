#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 65536

int main() {
    int sockfd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    // 创建原始套接字
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    // 设置套接字选项
    int enable = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable)) < 0) {
        perror("Failed to set socket option");
        exit(1);
    }

    // 接收和处理IP数据包
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int length = recv(sockfd, buffer, sizeof(buffer), 0);
        if (length < 0) {
            perror("Failed to receive packet");
            continue;
        }

        // 解析IP头部
        struct iphdr *ip_header = (struct iphdr *)(buffer);
        char source_ip[INET_ADDRSTRLEN];
        char dest_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->saddr), source_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip_header->daddr), dest_ip, INET_ADDRSTRLEN);

        printf("Received IP packet from %s to %s\n", source_ip, dest_ip);

        // 模拟路由查找和转发过程
        // ... 进行路由查找和转发操作 ...

        // 发送IP数据包
      if (send(sockfd, buffer, length, 0) < 0) {
            perror("Failed to send packet");
            exit(1);
        }
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}