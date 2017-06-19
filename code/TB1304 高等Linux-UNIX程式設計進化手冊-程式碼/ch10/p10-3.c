#include "filelock.h"
int main(void)
{
    int fd;
    off_t fdpos;
    if((fd = open("tempfile", O_RDWR|O_CREAT, 0666)) < 0)    /* 開啟檔案 */
       err_exit("open error");
    /* 對檔案尾及其後的擴充部分置寫鎖 */
    if (SET_LOCK(fd, F_WRLCK, 0, SEEK_END, 0) == -1) 
       printf("%d set write lock failed\n", getpid());
    else
       printf("%d set write lock success\n", getpid());
    /* 定位檔案位置於目前檔案尾，並寫入8個字元 */
    fdpos = lseek(fd, 0L, SEEK_END);
    write(fd, "8 bytes.", 8);   
    /* 企圖釋放前面設定的寫鎖，但實際並未釋放，因為檔案尾的位置已後移 */
    if (un_lock(fd, 0L, SEEK_END, 0) == -1) 
       err_exit("UN_LOCK error")
    else
       printf("%d un_lock(fd,0,SEEK_END,0) success\n", getpid());
    /* 讓子執行緒在該區域設定讀鎖驗證父執行緒是否已正確釋放其寫鎖 */
    if (vfork()== 0){   /* 子執行緒 */
        /* 對整個檔案置寫鎖，不能設定表明父執行緒有鎖區域未釋放 */
       if (SET_LOCK(fd, F_WRLCK, 0, SEEK_SET, 0) == -1)
          printf("%d: there are some locks on the file\n",getpid());
       else
          printf("%d: no any locks on the file\n",getpid());
       exit(EXIT_SUCCESS);
    }
    /* 父執行緒 */
    sleep(5);       /* 確保父執行緒後離開 */
    exit(EXIT_SUCCESS);
}
