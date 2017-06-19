#include "ch07.h"
struct two_words {int a, b;}data;     
void handler(int signum)
{
    printf ("%d,%d\n", data.a, data.b);
    alarm (1);
}
int main (void)
{
    static struct two_words zero = {0, 0}, one = {1, 1};
    signal (SIGALRM, handler);
    data = zero;
    alarm (1);
    while (1){          
       data = zero;
       data = one;
    }
}
