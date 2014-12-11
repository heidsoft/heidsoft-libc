#include <stdio.h>


/******************************
作用:用指针做函数参数的测试案例
作者:heidsoft
联系:heidsoftg@gmail.com
******************************/
void change(int i,int *p)
{
	i++;
	if(p!=NULL)
		(*p)++;
}

int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	int a=5,b=10;

	change(a,&b);
	printf("a=%d b=%d\n",a,b);
	return 0;
}
