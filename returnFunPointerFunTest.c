#include <stdio.h>
/*****************************
作用:测试返回函数指针的函数
作者:heidsoft
联系:heidsoft@gmail.com
******************************/
int get_big(int i, int j)
{
	return i>j?i:j;
}

int (*get_function(int a))(int , int)
{
	printf("the number is %d\n",a);
	
	return get_big;
}

int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	int i=5,j=10,max;

	int (*p)(int,int);//定义一个返回指针函数的函数

	max=p(i,j);

	printf("The MAX is %d\n",max);

	return 0;
}
