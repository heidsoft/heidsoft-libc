#include "filelock.h"
const char *test_file="/tmp/test_lock";
int main(void)
{
   char *strw = "set write lock on region", *strr = "set read lock on region";
   int fd, bytes_count;
   fd = open(test_file, O_RDWR|O_CREAT, 0666);   /* 開啟檔案 */
   if(fd<0) 
      err_exit("Unable to open file");
   for (bytes_count = 0; bytes_count<100; bytes_count++)  /* 寫入資料 */
      write(fd, "A", 1);              
   if (fork()!=0) {     /* 父執行緒 */
      /* 在區域[10-29]設定共享讀鎖 */
      if (SET_LOCK (fd, F_RDLCK, 10, SEEK_SET, 20)<0)
         fprintf(stderr,"%d %s [10-29] failed\n", getpid(),strr);
      else
         fprintf(stderr,"%d %s [10-29] succeed\n",getpid(),strr);
      /* 在區域[40-49]設定互斥寫鎖 */
      if (SET_LOCK (fd, F_WRLCK, 40, SEEK_SET, 10)<0)
         fprintf(stderr,"%d %s [40-49] failed\n", getpid(),strw);
      else
         fprintf(stderr,"%d %s [40-49] succeed \n",getpid(), strw);
      sleep(3);      /* 睡眠3秒以便子執行緒測試鎖 */
      printf("%d close file\n",getpid());
      close(fd);
      exit(EXIT_SUCCESS);
   } else {         /* 子執行緒 */
      pid_t mypid=getpid(); 
      sleep(1);     /* 讓父執行緒先執行 */
      /* 對區域[10-14]設定讀鎖，與父執行緒設定的讀鎖部分重疊，可成功 */
      if (SET_LOCK (fd, F_RDLCK, 10, SEEK_SET, 5)<0) 
         fprintf(stderr,"%d %s [10-14] failed\n", mypid, strr);
      else 
         fprintf(stderr, "%d %s [10-14] succeed\n", mypid, strr);    
      /* 對區域[15-20]加寫鎖，與父執行緒設定的讀鎖部分重疊，不會成功 */    
      if (SET_LOCK (fd, F_WRLCK, 15,SEEK_SET,6)<0) 
         fprintf(stderr,"%d %s [15-20] failed\n", mypid, strw);
      else 
        fprintf(stderr, "%d %s [15-20]\n", mypid, strw);
      /* 對區域[40-49]加讀鎖，與父執行緒設定的寫鎖重疊，不會成功 */
      if (SET_LOCK (fd, F_RDLCK, 40, SEEK_SET, 10)<0) 
         fprintf(stderr,"%d %s [40-49] failed\n", mypid,strr);
      else 
         fprintf(stderr,"%d %s [40-49] succeed\n", mypid, strr);
      /* 對區域[41-60]加寫鎖並等待，與父執行緒設定的寫鎖重疊，等待父執行緒離開後才會成功 */   
      if (SET_LOCK_W(fd, F_WRLCK, 41, SEEK_SET, 20)<0) 
         fprintf(stderr,"%d %s [41-60] succeed\n", mypid, strw);
      else
         fprintf(stderr,"%d %s [41-60] succeed\n", mypid, strw);
      /* 對區域[0-60]解鎖，將同時釋放區域[10-14]的讀鎖和區域[41-60]的寫鎖 */
      if (un_lock (fd,0,SEEK_SET,69)<0) 
         fprintf(stderr,"%d unlock the region [0-69] failed\n", mypid);
      else 
         fprintf(stderr,"%d unlocked the region [0-69] succeed\n", mypid);
      fprintf(stderr,"Process %d end\n", mypid);    
      close(fd);
      exit(EXIT_SUCCESS);
   }
}
