#include <stdio.h>

/************************
作用：指针数组测试
作者：heidsoft
************************/
int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	
	int a[]={1,2,3,4,5,5,6,7,8,9,0};
	int *p1=a;
	int *p2=&a[0];

	printf("p1=%x\n",p1);//p1存储的是a的地址
	printf("p2=%x\n",p2);//p2存储的是数组a第一个元素的地址
	
	//得到的结论是p1和p2的值是一样的，即他们都指向了同一个地址
printf("=============================\n");
	printf("p1=%d\n",*p1);
	printf("p2=%d\n",*p2);

	//所以*p1与*p2打印出来的值是相等的，都是1

	return 0;
}
