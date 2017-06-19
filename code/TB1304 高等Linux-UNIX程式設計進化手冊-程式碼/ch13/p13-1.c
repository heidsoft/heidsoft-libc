#include "ch13.h"
#define NUM_THREADS 4 
#define VECL 4*10000000
double a[VECL], b[VECL], sum;
struct args {         // dotsum使用的資料
   pthread_t tid;       // 執行緒ID
   int l_sum;           // 局部點積
   int index;               // 從0開始的執行緒編號
} tharg[NUM_THREADS];     // 每個執行緒對應一個元素
void *dotsum(void *arg)   // 求部分點積的執行緒開始函數
{
   struct args *ap = (struct args *)arg;
   int  i, start, end;
   double l_sum = 0.0;                           // 局部點積
   start = ap->index * (VECL/NUM_THREADS);   // 向量開始點 
   end   = start + (VECL/NUM_THREADS);        // 向量結束點 
   printf("thread %i do from index %d to %d\n",ap->index, start,end-1);
   for (i = start; i < end; i++) 
      l_sum += (a[i]*b[i]);
   ap->l_sum = l_sum;                           // 儲存計算結果 
   return;
}
int main(void)
{
   int  i;
   assert(VECL%4==0);                  // 只有滿足此條件程式才正確 
   for (i=0; i<VECL; i++)
      a[i]=b[i]=1.0;
   for(i=1; i<NUM_THREADS; i++) {  // 建立NUM_THREADS-1個執行緒平行執行dotsum()函數 
      tharg[i].index = i;
      pthread_create(&tharg[i].tid, NULL, dotsum, (void *)&tharg[i]);
   }
   tharg[0].index = 0;
   dotsum((void *)&tharg[0]);      /* 主軸程參與計算 */
   for(i=1; i<NUM_THREADS; i++)   /* 等待其它執行緒完成計算 */
      pthread_join(tharg[i].tid, (void **)NULL);
   /* 累加每個執行緒的部分和並輸出結果 */
   sum=0.0;
   for (i=0; i<NUM_THREADS; i++)
      sum += tharg[i].l_sum;
   printf ("Sum =  %lf \n", sum);
   pthread_exit(NULL);
}
