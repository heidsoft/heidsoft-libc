#include "ch11.h"
#define BUFSIZE 128
struct my_msg {/* 消息缓冲区结构类型定义 */
 long int my_msg_type;
 char text[BUFSIZE];
};

