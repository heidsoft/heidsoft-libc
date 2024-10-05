#include "ch10.h"

int input_timeout(int filedes, unsigned int seconds)
{
    fd_set set;
    struct timeval timeout;
    int n;
    char buf[1024];

    FD_ZERO(&set);             /* 起始檔案描述字集合 */
    FD_SET(filedes, &set);
    timeout.tv_sec = seconds;  /* 起始timeout資料結構 */
    timeout.tv_usec = 0;

    /* select時間到期傳回0；輸入就緒傳回1；出現錯誤傳回-1 */
again:
    if ((n = select(FD_SETSIZE, &set, NULL, NULL, &timeout)) < 0) { //出現錯誤
        if (errno = EINTR) 
            goto again;
    }
    else if (n > 0) {   //輸入就緒
        n = read(filedes,buf,sizeof(buf));
        printf("read data:  %s\n",buf);
    }
    else  //時間到期
        printf("tiomeout for select\n");
    return n;
}

int main (void)
{
    fprintf(stderr, "select returned %d.\n",input_timeout(STDIN_FILENO, 5));
    return 0;
}
