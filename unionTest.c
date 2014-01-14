/******************************** 
	描述:unionTest.c 联合类型测试 
创建时间:2014/1/14/22:35
	作者:heidsoft
	版本:v1.0
*********************************/

union MyType
{
	int digit;//整数
	double bigfl; //大浮点数
	char letter;  //字符
};

union MyType fit;//myType类型的联合变量
union MyType sava[10]; //10个联合变量的数组
union MyType *pu; //指向MyType类型的联合指针

union MyType valA;
valA.letter = 'B';
union MyType valB=valA;//把一个联合初始化为另一个联合
union MyType valC={88};//初始化联合的digit成员
union MyType valD={.bigfl=102.2};//限定初始化项目

//如果使用联合
fit.digit=23;//把23存储在fit中，使用2个字节
fit.bigfl=2.5;//清楚23,存储2.0, 使用8个字节
fit.letter  = 'h' ;// 清楚2.5,存储‘h’,使用1个字节

/*
	联合在同一个时间只能存储一个值，即使有足够的空间，
	也不能同时保存一个char类型和一个int类型
*/

//如果指针
pu=&fit;
int x = pu->.digit; //相当于x=fit.digit;

//声明用户
struct user
{
	int id;			//用户id
	char name[10];  //用户名称
	int age;        //用户年龄
	char sex[3];    //用户性别
};

//声明right(权限)
struct right
{
	int id;
	int level;//权限级别
};

union data
{
	struct user dUser;
	struct right dRight;
}

struct car_data
{
	char make[10];
	int status; /*0=私有，1=租赁*/
	union data owerInfo;
};
