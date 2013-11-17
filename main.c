#include <stdio.h>
/*
 1、有1、2、3、4个数字，能组成都少个互不相同且无重复数字的三位数？都是多少？
 */
int main(void)
{
    int i,j,k;
    int flag=0;
    printf("\n");
    for(i=1;i<5;i++){
        for(j=1;j<5;j++){
            for(k=1;k<5;k++){
                if(i!=k&&i!=j&&j!=k){
                    //确保i/j/k三位互不相同
                    flag++;
                    printf("%d,%d,%d\n",i,j,k);
                }
            }
        }
    }
    printf("flag=%d\n",flag);
    return 0;
}

int test_love()
{
    printf("*");
}

