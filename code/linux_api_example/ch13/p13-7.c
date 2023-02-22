#include "ch13.h"
#define SIZE 100
int a[SIZE],b[SIZE];
int max;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void max_fun(int *arg)    /* 計算max(max, arg[]) */
{
   int * ap=arg;
   int rv, i, l_max;
   for (i=1; i<SIZE; i++){ // 找出陣列ap中的最大值
      if (ap[i] > l_max) 
         l_max = ap[i];
   }
   /* 計算max(L_max, max),需互斥動作 */
   rv = pthread_mutex_lock(&mtx);      // 獲得互斥變數
   check_error(rv, "mutex_lock ");
   if (l_max > max)
      max = l_max;
   rv = pthread_mutex_unlock(&mtx);   // 釋放互斥變數
   check_error(rv, "mutex_unlock ");
   pthread_exit(NULL);       
}

int main(void)
{
   pthread_t tid1,tid2;    // 執行緒標誌
   int i, rv;
   
   for (i=1; i<SIZE; i++){ // 產生陣列元素的資料
      a[i]=rand();
      b[i]=rand();
   }
   /* 建立2個執行緒執行函數max_fun()分別處理陣列a和b */
   rv = pthread_create(&tid1, NULL, (void*(*)())max_fun, a);
   check_error(rv, "pthread_create: tid1");
   rv = pthread_create(&tid2, NULL, (void*(*)())max_fun, b);
   check_error(rv, "pthread_create: tid2");
   /* 等待2個執行緒結束 */
   pthread_join(tid1, NULL);
   pthread_join(tid2, NULL);
   printf ("max value = %d\n", max);
   exit(0);
}
