#include "ch11.h"
void write_data (FILE * stream)
{
    int i;
    for (i = 0; i<50; i++){        /* 輸出50個整數*/
       fprintf (stream, "line %2d\n", i);
       if (ferror (stream)) 
           err_exit("Output to stream failed.\n");
    }
}
int main (void)
{
    FILE *output;
    /* 建立執行more的執行緒分頁顯示輸入,並建立通向它的輸入管線 */
    output = popen ("more -10", "w");
    if (!output) 
        err_exit("Could not run more.\n"); 
    write_data (output);
    /* 等待popen建立的執行緒終止並關閉與它相連的流 */
    pclose (output);
    return EXIT_SUCCESS;
}
