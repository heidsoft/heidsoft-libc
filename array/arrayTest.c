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
	//结论：++ 与*优先级相同，结合方向为自右向左，等价于*(p++)
	//也就是说先得到p所指向的变量值(*p),再使p=p+1,
	//或者说先取得当前p所指向的变量值在使p指向下一个变量
	return 0;
}
