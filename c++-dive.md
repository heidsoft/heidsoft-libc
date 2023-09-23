# 变量规范
```
char ch;	/*所有的字符变量均以ch开始*/
byte b;		/*所有的字节均冠以b*/
long l;		/*所有的长字均冠以l*/
```
# 指针规范
```
char* pch;	/*指向ch的指针以p开始*/
byte* pb;	/*同理*/
long* pl;	/*同理*/
void* pv;	/*特意显用的空指针*/
char** ppch;	/*指向字符指针的指针*/
byte** ppb;	/*指向字节指针的指针*/
```
# 函数规范
```
char* strcpy(char* strTo,char* strFrom)	/*原型*/
size_t sizeNew,sizeOld;			/*原型*/
void* malloc(size_t size);		/*原型*/
void* realloc(void* pv,size_t sizeNew); /*原型*/
```
# 值返回规范
```
ch=chLastKeyPressed;	/*由变量得一字符*/
ch=chInputBuffer[];	/*由数组得一字符*/
ch=chReadKeyboard;	/*由函数得一字符*/
```

