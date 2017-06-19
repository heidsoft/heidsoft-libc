#include "ch02.h"

int main(int argc, char* argv[])
{
    char buf[80];
    float r,s;
    float pi = 3.141596;
    if (argc != 2){
        printf("Usage: %s radius\n",argv[0]);
        exit(1);
    }
    sscanf(argv[1],"%f",&r);   /* 從指令行中讀入半徑 */
    s = pi*r*r;
    sprintf(buf,"The surface of a circle with radius %f is %f\n",r,s);
    puts(buf);
    exit(0);
}
