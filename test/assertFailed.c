// 断言测试
// jake.liu
// 2021-05-16
// gcc -g3 -o segmentationFault segmentationFault.c
// gdb segmentationFault
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
main(int argc, char **argv)
{
    int x;

    x = 3;

    assert(x+x == 4);

    return 0;
}
