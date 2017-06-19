#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main() {
    struct in_addr inaddr1, inaddr2;
    char* ptr1, *ptr2;
    /* test 1 */
    inaddr1.s_addr = inet_addr("127.0.0.1");
    inaddr2.s_addr = inet_addr("255.255.255.255");
    printf("%s, %s¥n", inet_ntoa(inaddr1), inet_ntoa(inaddr2));
    /* test 2 */
    ptr1 = inet_ntoa(inaddr1);
    ptr2 = inet_ntoa(inaddr2);
    printf("%s, %s\n", ptr1, ptr2);
    return 0;
}
