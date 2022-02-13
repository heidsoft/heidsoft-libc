// valgrind -q --tool=memcheck --leak-check=yes  ./uninitialized
// jake.liu
// 2021-05-16
// gcc -g3 -o uninitialized uninitialized.c 
#include <stdio.h>
#include <string.h>
//use strcmp function ,but not include header file, so complie after,out like error
//[root@localhost test]# gcc -g3 -o uninitialized uninitialized.c 
//uninitialized.c: In function ‘main’:
//uninitialized.c:14:9: warning: implicit declaration of function ‘strcmp’ [-Wimplicit-function-declaration]
//   14 |     if(!strcmp(a, "a")) {
//     |         ^~~~~~
int
main(int argc, char **argv)
{
    char a[2];

    a[0] = 'a';
    a[1] = '\0';
    if(!strcmp(a, "a")) {
        puts("a is \"a\"");
    }

    return 0;
}
