#include "ch10.h"
#define NBUFFERS 2            /* 緩沖區個數 */
#define BUFFERSIZE 2048       /* 緩沖區大小 */
#define BLOCKSIZE  1024       /* 每次讀入的資料區塊大小 */
#define SIG_AIO_WRITE (SIGRTMIN+5)

typedef enum {BUFFER_FREE=1,BUFFER_FILLING,BUFFER_WRITING} BUFFLAG;
typedef struct {
   BUFFLAG state;              /* 緩沖區狀態 */
   int fillpt;                 /* 緩沖區中最後一個資料位置 */
   struct aiocb acb;           /* 異步輸出控制塊 */
   char buffer [BUFFERSIZE];   /* 資料緩沖區 */
} buffer_t;
static buffer_t buf_list[NBUFFERS];
static sigset_t aio_completion_signal;
static volatile int sigcnt = 0, total = 0;
static off_t seek_ptr;

/* 異步輸出完成訊號控制碼 */
void aio_ok(int signo, siginfo_t *info, void *ignored)
{
   buffer_t *donebuf;
   ssize_t rval;
   if ((signo != SIG_AIO_WRITE)||info->si_code!=SI_ASYNCIO)
      return; // 非AIO完成訊號   
   else
      printf("AIO write completed\n");
/* 根據info獲得異步I/O關聯的訊息。 */
   donebuf = (buffer_t *)info->si_value.sival_ptr;
   if ((aio_error((struct aiocb *) &donebuf->acb) != EINPROGRESS))
      rval=aio_return((struct aiocb *) &donebuf->acb);
/* 累計寫出的位元組數和訊號發生次數，並標示緩沖區已空*/
   total += rval; sigcnt++;
   donebuf->state = BUFFER_FREE;
   donebuf->fillpt = 0;
   return;
}
buffer_t *find_free_buf (void) /* 獲得一個閒置的緩沖區 */
{
   int i;
   sigset_t prevmask;
   sigprocmask(SIG_BLOCK, &aio_completion_signal, &prevmask);
   while (1) {
      for (i=0; i<NBUFFERS; i++) {
         if (buf_list[i].state == BUFFER_FREE)
            break;
      }
      if (i == NBUFFERS) 
         sigsuspend(&prevmask);  /* 沒有閒置緩沖區，等待某個異步輸出完成後重試。*/
      else
         break;
   }
   buf_list[i].state = BUFFER_FILLING; /* 找到了閒置緩沖區，標志它正被使用 */
   buf_list[i].fillpt = 0;
   sigprocmask(SIG_SETMASK, &prevmask, NULL);
   return (&buf_list[i]);
}
void flush_filled_buf(buffer_t *full_guy) /* 寫出緩沖區中的資料 */
{
   /* 設定 AIOCB */
   full_guy->acb.aio_offset = seek_ptr;
   seek_ptr += full_guy->fillpt;
   full_guy->acb.aio_buf = full_guy->buffer;
   full_guy->acb.aio_nbytes = full_guy->fillpt;
   full_guy->acb.aio_reqprio = 0;
   full_guy->acb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
   full_guy->acb.aio_sigevent.sigev_signo = SIG_AIO_WRITE;
   full_guy->acb. aio_sigevent.sigev_value.sival_ptr = (void *)full_guy;
   full_guy->state = BUFFER_WRITING; /* 標志緩沖區狀態為正在寫出*/  
   if (aio_write(&full_guy->acb) < 0) /* 發出異步輸出請求 */
      perror ( "aio_write" ) ;
   return;
}
main(int argc, char **argv)
{
   int  i, in_file, out_file;
   buffer_t *currentbuf;
   struct sigaction sig_act;
   int chunksize;
   if (argc != 4) {/* 檢查參數個數 */
      fprintf(stderr, "Usage: %s input-file output-file buf-size-in-Kb\n",
               argv[0]);
      exit(0);
   }
   /* 開啟輸入和輸出檔案 */
   if ((in_file = open(argv[1], O_RDONLY)) == -1) 
      err_exit(argv[1]);
   if ((out_file = open(argv[2], O_WRONLY|O_CREAT, 0777)) == -1)
      err_exit(argv[2]);
   chunksize=atol(argv[3])*BLOCKSIZE;
    /* 起始緩沖區狀態並分配一個複製檔案的緩沖區 */
   for (i=0; i<NBUFFERS; i++){
      buf_list[i].acb.aio_fildes = out_file;
      buf_list[i].state = BUFFER_FREE;
      buf_list[i].fillpt = 0;
   }
   currentbuf = find_free_buf();
	   /* 建立異步I/O完成訊號控制碼 */
   sigemptyset(&aio_completion_signal);
   sigaddset(&aio_completion_signal, SIG_AIO_WRITE);
   sigemptyset(&sig_act.sa_mask);                  
   sig_act.sa_flags = SA_SIGINFO;
   sig_act.sa_sigaction = aio_ok;
   if (sigaction (SIG_AIO_WRITE, &sig_act, 0) < 0)
      perror("sigaction");
	   /* 從in_file複製檔案到out_file */
   while (1) {
      int nbytes;
again:
      /* 讀一塊訊息至緩沖區 */
      nbytes = read(in_file, currentbuf->buffer+currentbuf->fillpt, 
                      chunksize);
      if (nbytes>0) {
         currentbuf->fillpt += nbytes;
         if (currentbuf->fillpt == BUFFERSIZE) {
            /* 寫出已滿的緩沖區並獲得一個新緩沖區 */
            flush_filled_buf (currentbuf);
            currentbuf = find_free_buf() ;
         }
      } else if (nbytes == 0) { /* 到達檔案尾 */
         flush_filled_buf (currentbuf);
         break;
      } else {                  
         if (errno == EINTR) goto again;  /* 被訊號中斷，重新讀 */
         else err_exit("read");
      }
   }
   for (i=0; i<NBUFFERS; i++){  /* 等待所有異步輸出完成 */
      if (buf_list[i].state == BUFFER_WRITING){
         struct aiocb *acbp[1];
         acbp[0] = &buf_list[i].acb;
         aio_suspend(acbp, 1, NULL);
      }
   }
   close(in_file);
   close(out_file);
   printf("Copied: %d bytes, %d signals taken\n", total, sigcnt);
}
