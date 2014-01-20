#include <stdio.h>

/******************************
作用:返回指针的函数测试案例
作者:heidsoft
联系:heidsoftg@gmail.com
*******************************/

char *name[7]={"Monday","Tuesday","Wednesday","Thursday"
				"Friday","Saturday","Sunday"};
char *message="wrong input";

char *week(int day)
{
	if(day<0 || day>7)
		return message;
	else
		return name[day-1];
}

int main(int argc, char *argv[])
{
	
	int day;
	char *p;
	printf("Hello, world  day=%d,*p=%s\n",day,p);

	printf("Input a number of a week:\n");
	scanf("%d",&day);

	p=week(day);
	printf("%s\n",p);

	return 0;
}
