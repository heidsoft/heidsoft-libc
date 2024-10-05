#include "ch05.h"
static jmp_buf jmpbuffer;
int i1;
static void f1(int i, int j, int k) 
{
   printf ("     in f1 (): i1=%d, i2=%d, i3=%d\n", i, j, k);
   longjmp(jmpbuffer,1);
}
int main(void)
{
   volatile int i2;
   int i3;
   i1=2; i2=3; i3=4;
   if (setjmp (jmpbuffer)!=0) {
      printf("after longjmp: i1=%d, i2=%d, i3=%d\n", i1, i2, i3);
      exit(0);
   }
   i1=6; i2=7; i3=8;
   f1(i1, i2, i3);
}
