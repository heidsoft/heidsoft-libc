#include <stdio.h>

/********************************
作用:以不同的方式，输出数组的值
作者:heidsoft
联系:heidsoftg@gmail.com
********************************/
int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	int a[10],*p=NULL,i;

	//给数组初始化
	for(i=0;i<10;i++)
		a[i]=i;
	printf("\n");

	//方法一
	for(i=0;i<10;i++)
		printf("a=%d",*(a+i));
	printf("\n");
printf("======================================\n");
	//方法二
	for(p=a;p<a+10 ; )
		printf("%d",*p++);

	return 0;
}
