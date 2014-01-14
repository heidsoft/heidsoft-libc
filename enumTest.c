/******************************** 
	描述:enumTest.c 枚举类型测试 
创建时间:2014/1/14/22:35
	作者:heidsoft
	版本:v1.0
*********************************/

#include <stdio.h>
#include <string.h>  //为了使用strcmp()
#include <stdbool.h> //c99特性

/*声明枚举数据结构*/
enum spectrum {
	red/*红色*/,
	orange/*橙色*/,
	yellow/*黄色*/,
	green/*绿色*/,
	blue/*蓝色*/,
	violet/*紫色*/
};

const char * colors[]={"red","orange","yellow",
					  "green","blue","violet"};
#define LEN 10

int main(void)
{
	char choice[LEN];
	enum spectrum color;
	bool color_is_found = false;
	
	puts("Enter a color (empty line to quit); ");
	//如果键入的字符不为空，且不是输入的换行符
	while(gets(choice)!=NULL && choice[0]!='\0')
	{
		//循环颜色
		for(color=red; color <=violet; color++)
		{
			//如果颜色比较等于0
			if(strcmp(choice,colors[color])==0)
			{
				//那么颜色找到
				color_is_found = true;

				//跳出循环
				break;
			}
		}

		if(color_is_found)
		{
			switch(color)
			{
				case red :    puts(" 你选择的是 red");
						      break;
				case orange : puts(" 你选择的是 orange");
						      break;
				case yellow : puts(" 你选择的是 yellow");
							  break;
				case green :  puts(" 你选择的是 green");
						      break;
				case blue :   puts(" 你选择的是 blue");
						      break;
				case violet : puts(" 你选择的是 violet");
						      break;
			}

		}else{
			printf("我不知道你选择的颜色 %s .\n",choice);
		}
		color_is_found=false;

		puts("Next color, please (empty line to quit); ");
	}

	puts("Goodbye");

	return 0;
}