#include <stdio.h>

/*******************************
作用:测试指向字符串的指针
作者:heidsoft
联系:heidsoftg@gmail.com
*******************************/
int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	
	char a[]="Linux C Program",b[20],c[20];
	int i;

	for(i=0;*(a+i);i++)
		*(b+i)=*(a+i);//赋值
	*(b+i)="\0";
	
	char *p1,*p2;//没有初始
	p1=a;
	p2=c;
	
	printf("p1=%s\n",*p1);
	printf("p2=%s\n",*p2);

	for(; *p1!='\0';p1++,p2++)
		*p2=*p1;//对数据进行赋值
	*p2="\0";


	printf("c=%s\n",a);
	printf("c=%s\n",b);
	printf("c=%s\n",c);
	return 0;
}
