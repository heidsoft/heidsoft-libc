#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main(int argc, char* argv[]) {
    int sock;
    int n;
    int err;
    struct addrinfo hints, *res;
    struct group_source_req gsr;
    struct group_req group;
    char buf[2048];

    if (argc != 2) {
        fprintf(stderr, "usage : %s blocksrcipv4addr\n", argv[0]);
        return 1;
    }

    /* bind()するためのソケットを作成 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(NULL, "12345", &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    sock = socket(res->ai_family, res->ai_socktype, 0);

    if (sock < 0) {
        perror("socket");
        return 1;
    }

    if (bind(sock, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    freeaddrinfo(res); /* 最初のgetaddrinfo()結果はもう利用しないので破棄 */
    /* マルチキャストグループ情報を設定 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    err = getaddrinfo("239.192.1.2", NULL, &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    memset(&group, 0, sizeof(group));
    group.gr_interface = 0;
    memcpy(&group.gr_group, res->ai_addr, res->ai_addrlen);

    /* 239.192.1.2へJOIN */
    if (setsockopt(sock, IPPROTO_IP, MCAST_JOIN_GROUP, (char*)&group,
                   sizeof(group)) != 0) {
        perror("setsockopt");
        return 1;
    }

    memset(&gsr, 0, sizeof(gsr));
    /* gsr_interfaceは利用されないので設定せずにそのまま */
    memcpy(&gsr.gsr_group, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res); /* getaddrinfo()の結果は必要ないので破棄 */
    /* ここからがMCAST_BLOCK_SOURCEのための処理 */
    printf("SOURCE to block : %s\n", argv[1]);
    /* ユーザが入力した「ブロックしたい送信元」に関しての設定 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    err = getaddrinfo(argv[1], NULL, &hints, &res);

    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    memcpy(&gsr.gsr_source, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    /* setsockopt()でマルチキャストフィルタを設定 */
    if (setsockopt(sock, IPPROTO_IP, MCAST_BLOCK_SOURCE, (char*)&gsr,
                   sizeof(gsr)) != 0) {
        perror("setsockopt");
        return 1;
    }

    /* 受信してみる */
    n = recv(sock, buf, sizeof(buf), 0);

    if (n < 1) {
        perror("recv");
        return 1;
    }

    /* 受信できたら表示。設定した送信元からのパケットは受け取らないはず */
    printf("%s\n", buf);
    close(sock);
    return 0;
}

