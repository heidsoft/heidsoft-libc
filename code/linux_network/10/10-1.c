#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>

int
main() {
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int pid, cpid;
    char buf[1024];
    int n;
    int status;
    /* ソケットの作成 (1) */
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    /* ソケットの設定 */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock0, (struct sockaddr*)&addr, sizeof(addr));
    /* TCPクライアントからの接続要求を待てる状態にする */
    listen(sock0, 5);
    /* (1) 終わり */

    /* (2) */
    for (;;) {
        /* TCPクライアントからの接続要求を受け付ける */
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr*)&client, &len);
        /* (3) */
        pid = fork();

        if (pid == 0) {
            /* child process */
            n = read(sock, buf, sizeof(buf));

            if (n < 0) {
                perror("read");
                return 1;
            }

            write(sock, buf, n);
            /* TCPセッションの終了 */
            close(sock);
            return 0;
        } else {
            /* (4) */
            while ((cpid = waitpid(-1, &status, WNOHANG)) > 0);

            if (cpid < 0 && errno != ECHILD) {
                perror("waitpid");
                return 1;
            }
        }
    }

    /* (5) */
    /* listen するsocketの終了 */
    close(sock0);
    return 0;
}
