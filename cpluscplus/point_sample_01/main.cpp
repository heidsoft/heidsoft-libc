/**
 *
 * 赋值语句具有右结合性
 *
 */

#include <string>
int main(void){
    int ival; int *pval;

    //ival = pval =0;//error

    pval = &ival;

    std::string s1,s2;

    s1 = s2 ="OK";
 
    return 0;
}
