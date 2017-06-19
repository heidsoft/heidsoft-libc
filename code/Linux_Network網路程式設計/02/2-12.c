#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ポート番号とbindされたアドレスを表示する関数 */
void
print_my_port_num(int sock) {
    char buf[48];
    struct sockaddr_in s;
    socklen_t sz;
    sz = sizeof(s);

    /* ソケットの「名前」を取得、getsockname()はChapter6参照 */
    if (getsockname(sock, (struct sockaddr*)&s, &sz) != 0) {
        perror("getsockname");
        return;
    }

    /* bindされているIPアドレスを文字列へ変換 */
    inet_ntop(AF_INET, &s.sin_addr, buf, sizeof(buf));
    /* 結果を表示 */
    printf("%s : %d\n", buf, ntohs(s.sin_port));
}

int
main() {
    int s0, sock;
    struct sockaddr_in peer;
    socklen_t peerlen;
    /* ソケットを作成していきなりlisten()する */
    s0 = socket(AF_INET, SOCK_STREAM, 0);

    if (listen(s0, 5) != 0) {
        perror("listen");
        return 1;
    }

    /* listen()すると自動的に未使用ポートを割り当てられることを確認 */
    print_my_port_num(s0);
    /* TCPコネクションを受付 */
    peerlen = sizeof(peer);
    sock = accept(s0, (struct sockaddr*)&peer, &peerlen);

    if (sock < 0) {
        perror("accept");
        return 1;
    }

    /* 相手に文字列を送信して終了 */
    write(sock, "HOGE\n", 5);
    close(sock);
    close(s0);
    return 0;
}
