#include <stdio.h>

/****************************
作用:指针数组的使用
作者:heidsoft
联系:heidsoftg@gmail.com

指针数组定义的形式：
类型名 *数组名[数组长度];
例如
int *p[10];
*****************************/
int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	int a[5]={1,2,3,4,5};
	int *p[5],i;
	int **pp=p;

	for(i=0;i<5;i++)
		p[i]=&a[i];

	for(i=0;i<5;i++)
		printf("%d ",*p[i]);
	printf("\n");

	
	for(i=0;i<5;i++)
		printf("%d ",**pp++);
	printf("\n");
**pp=p;
	for(i=0;i<5;i++,pp++)
		printf("%d ",**pp);
	printf("\n");
	

	return 0;
}
