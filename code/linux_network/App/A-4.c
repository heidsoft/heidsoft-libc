#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define ADDRESS "127.0.0.1"

int
main() {
    struct hostent* host;
    struct in_addr addr;
    /* 文字列で表現されるIPアドレスを32ビットのバイナリ値に変換 */
    addr.s_addr = inet_addr(ADDRESS);
    /* IPアドレスからホスト名を取得 */
    host = gethostbyaddr((const char*)&addr.s_addr,
                         sizeof(addr.s_addr), AF_INET);

    if (host == NULL) {
        /* gethostbyaddrのエラーはperrorではないので注意 */
        herror("gethostbyaddr");
        return 1;
    }

    /* 結果を表示 */
    printf("%s\n", host->h_name);
    return 0;
}
