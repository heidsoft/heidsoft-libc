#include "ch13.h"
#include <sys/mman.h>
#define STACK_GROWS_DOWN
#undef  STACK_GROWS_UP
void Hello(void)
{
   int array[512];
   printf("Hello, &array=%d\n", &array);
   Hello(); 
}
/* 申請執行緒堆疊空間並建立堆疊溢位保護區。堆疊位址對齊在頁邊界上，堆疊大小擴充為頁的倍數 */
void *thread_stack_alloc(size_t size)   
{
   int pagesize;
   void *mem, *guard_addr, *stack_addr;
   if (size < PTHREAD_STACK_MIN){
     fprintf(stderr, " thread_stack_alloc: SIZE less than PTHREAD_STACK_MIN\n");
     exit(1);
   }
   /* 計算堆疊大小為頁的整倍數 */
   pagesize = sysconf(_SC_PAGESIZE);
   if(size % pagesize != 0)   
      size = ((size + pagesize) / pagesize) * pagesize;
   /* 申請對齊在頁邊界上的儲存空間，其中包括作為堆疊溢位區的額外一頁 */
   mem = (void *)valloc(size + pagesize);
   /* 建立溢位保護區 */
#ifdef STACK_GROWS_UP
   guard_addr = mem + size;
   stack_addr = mem;
#else
   guard_addr = mem;
   stack_addr = mem + pagesize;
#endif
   if (mprotect(guard_addr, pagesize, PROT_NONE))
      err_exit("mprotect");
   printf("stack area=(%d,%d)\n", stack_addr, stack_addr+size-1);
   printf("gaurded area=(%d,%d)\n", guard_addr, guard_addr+pagesize-1);
   return stack_addr;
}
int main (void)
{
   pthread_t thread;
   int rv;
   pthread_attr_t attr;
   size_t stacksize, guardsize;
   char *stackaddr;
   pthread_attr_init(&attr);      /* 起始化執行緒屬性物件*/
   /* 申請用作執行緒堆疊的空間 */
   stackaddr = (void *)thread_stack_alloc(PTHREAD_STACK_MIN); 
   /* 設定執行緒屬性物件中的堆疊位址和大小 */
   rv = pthread_attr_setstack(&attr, stackaddr, PTHREAD_STACK_MIN); 
   check_error(rv, "pthread_setstack()");
   /* 檢視堆疊屬性值 */   
   pthread_attr_getstack(&attr, (void *)&stackaddr, &stacksize);
   pthread_attr_getguardsize(&attr, &guardsize);
   printf("stack attributes: stackaddr=%d, stacksize=%d, guardsize=%d\n", 
           stackaddr, stacksize, guardsize);
   /* 建立執行緒 */
   rv = pthread_create(&thread, &attr, (void *(*)())Hello, (void *)NULL);
   check_error(rv, "pthread_create()");
   pthread_attr_destroy(&attr);   /* 及時銷毀執行緒屬性物件，避免再次使用 */
   check_error(rv, "pthread_attr_destroy()");
   /* ... 後繼程式程式碼 ... */
   pthread_exit(NULL);
}
