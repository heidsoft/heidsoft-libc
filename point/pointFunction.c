#include <stdio.h>

/*********************************
作用:函数指针测试案例
作者:heidsoft
联系:heidsoftg@gmail
**********************************/
#define GET_MAX 0
#define GET_MIN 1

//获取最大的
int get_max(int i, int j)
{
	return i>j?i:j;
}

//获取最小的
int get_min(int i, int j)
{
	return i>j?j:i;
}

//比较大小
int compare(int i, int j, int flag)
{
	int ret;
	int (*p)(int,int);//定义函数指针
	if(flag==GET_MAX)
		p=get_max; //这个地方只能给函数的名字，而不能带参数
	else
		p=get_min; //不能对指向函数的指针做任何运算,如p++,p--,p+n,p-n
	ret=p(i,j);

	return ret;
}
int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	
	int i=5,j=10,ret;
	ret=compare(i,j,GET_MAX);
	printf("The Max is %d\n",ret);
	return 0;
}

/**********************************************************
结论:
int *f(int i,int j); //返回指针的函数，他是一个函数的声明。
int (*p)(int i,int j); //指向函数的指针，他定义了一个指针。
***********************************************************/