#include <stdio.h>

int y_or_n_ques(const char *question)
{
    fputs (question, stdout);
    while (1){
        int c, answer;
         /* 寫一空格分開問題與回答 */
        fputc (' ', stdout);
         /* 讀此行中的第一個字元.它應當是回答字元，但也可能不是. */
        c = tolower (fgetc (stdin));
        answer = c;
         /* 忽略此行的其余字元. */
        while (c != '\n' && c != EOF)
            c = fgetc (stdin);
         /* 若果是回答字元，響應回答. */
        if (answer == 'y')
            return 1;
        if (answer == 'n')
            return 0;
        /* 非回答字元，請求合法回答. */
        fputs ("Please answer y or n:", stdout);
    }
}
