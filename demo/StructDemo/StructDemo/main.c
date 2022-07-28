//
//  main.c
//  StructDemo
//
//  Created by Jake liu on 2022/2/20.
//

#include <stdio.h>

/*
 结构体练习
 */
struct string{
    int length;
    char *data;
};

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    struct string s;
    s.length = 4;
    s.data = "this string is a lot longer than you think.";
    puts(s.data);
    return 0;
}
