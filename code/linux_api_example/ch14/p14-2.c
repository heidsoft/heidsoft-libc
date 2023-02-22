#include "ch14.h"
pthread_key_t key1, key2;
/* 使用執行緒專有資料的工作函數 */
void do_work(void)
{
   int num;
   char *vp;
   num = (int)pthread_getspecific(key1); /* 獲得執行緒專有資料，這是一個常量 */
   vp = (char *)pthread_getspecific(key2);  /* 獲得執行緒專有資料，這是一個指標 */
   sprintf(vp,"\tThread %d is at work now, key2's value:%x ", num, vp);
}
/* 執行緒開始函數，負責設定執行緒專有資料鍵之值 */
void start_func(int thread_num)
{
   char *buf, *vp;
   pthread_setspecific(key1, (void *)thread_num);
   buf = (char *)malloc(100);
   pthread_setspecific(key2, (void *)buf);
   do_work();
   vp = (char *)pthread_getspecific(key2);
   printf("Thread %d's thread specific data is :\n %s\n", thread_num, vp);
   if (thread_num !=0 ) // 起始執行緒傳回，其它執行緒終止
     pthread_exit(NULL);
}
/* 執行緒專有資料鍵相連的解構函數，負責線上程終止時釋放分配的儲存空間 */
void destructor(void *value)
{
   free(value);
   printf("memory %x released by destructor\n",value);
}
int main(int argc,char *argv[])
{
   int rv;
   pthread_t tid;
   /* 建立執行緒專有資料鍵key1和key2 */
   rv = pthread_key_create(&key1, NULL);
   check_error(rv, "Create key1");
   rv = pthread_key_create(&key2, destructor);
   check_error(rv, "Create key2");
   /* 建立一個執行緒做star_func的工作,其執行緒編號為1 */
   rv = pthread_create(&tid, NULL, (void *(*)())start_func, (void *)1);
   check_error(rv, "Thread create");
   /* 自己也參與工作，其執行緒編號為0 */
   start_func(0);
   /* 等待執行緒終止 */
   rv = pthread_join(tid, NULL);
   check_error(rv, "Thread join ");
   rv = pthread_key_delete(key1);
   check_error(rv, "key1 delete");
   pthread_exit(NULL);
}

