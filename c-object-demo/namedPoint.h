
/***********************************
数据结构定义
************************************/
struct NamedPoint;

/************************************
函数定义
*************************************/
struct NamedPoint* makeNamedPoint(double x, double y, char* name);
void setName(struct NamedPoint* np, char* name);
char* getName(struct NamedPoint* np);
