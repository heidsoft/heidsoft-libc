#include <stdio.h>

/**************************
作用:指针测试
作者:heidsoft
***************************/
int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	char *pc;
	int *pi,p;
	double *pd=NULL;
	int a =100,b=200;
	int *p1 = &a,*p2=&b;

	printf("%d,%d\n",a,b);
	printf("%d,%d\n",*p1,*p2);
	printf("%x,%x\n",&a,&b);
	printf("%p,%p\n",&a,&b);
	printf("%x,%x\n",p1,p2);
	printf("%p,%p\n",p1,p2);

printf("==========================================\n");
	*p1=*p1*3;//p1指向的值乘以3
	printf("a=%d\n",a);
	printf("*p1=%d\n\n",*p1);
	
	p1=&b;//将b的地址赋值给p1
	printf("p1=%x\n",p1);
	printf("p2=%x\n\n",p2);
	 
	return 0;
}
