#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int
main() {
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in senderinfo;
    socklen_t addrlen;
    char buf[2048];
    char senderstr[16];
    int n;
    /* AF_INET+SOCK_DGRAMなので、IPv4のUDPソケット */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    /* 待ち受けポート番号を12345にするためにbind()を行う */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    while (1) {
        memset(buf, 0, sizeof(buf));
        /* recvfrom()を利用してUDPソケットからデータを受信 */
        addrlen = sizeof(senderinfo);
        n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                     (struct sockaddr*)&senderinfo, &addrlen);
        /* 送信元に関する情報を表示 */
        inet_ntop(AF_INET, &senderinfo.sin_addr, senderstr, sizeof(senderstr));
        printf("recvfrom : %s, port=%d\n", senderstr, ntohs(senderinfo.sin_port));
        /* UDPで返信 */
        sendto(sock, buf, n, 0, (struct sockaddr*)&senderinfo, addrlen);
        /* 送信元に関する情報をもう一度表示 */
        printf("sent data to : %s, port=%d\n", senderstr,
               ntohs(senderinfo.sin_port));
    }

    /* このサンプルコードはここへは到達しません */
    close(sock);
    return 0;
}
