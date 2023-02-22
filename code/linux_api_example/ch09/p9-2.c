#include "ch09.h"
int main(void)
{
    FILE * output;
    int tty_out,std_out;
    output = fopen("/dev/tty","w"); 
    if(!output) {
        fprintf(stderr, "unable to open /dev/tty\n"); exit(1);
    }
    fprintf(output, "This line is output to /dev/tty \n");
fprintf(stdout, "This line is output to stdout \n");
    tty_out=fileno(output);
    std_out=fileno(stdout);
    fprintf(output, "output: %s\n",
             isatty(tty_out) ? ttyname(tty_out) : "not a tty");
    fprintf(output, "stdout: %s\n",
             isatty(std_out) ? ttyname(std_out) : "not a tty");
}
