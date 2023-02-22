#include "ch09.h"
int set_termios(int desc)
{
    struct termios settings;
    int result;    
    result = tcgetattr(desc, &settings); /* 取得目前終端屬性 */
    if (result < 0) {
        perror("error in tcgetattr"); return 0;
    }
    settings.c_iflag &= ~ISTRIP;    /* 清除ISTRIP */
    settings.c_lflag &= ~(ICANON | ECHO | ISIG);  /* 設定非加工輸入 */
    settings.c_oflag |= OPOST ;                   /* 設定實現定義的輸出處理 */
    result = tcsetattr(desc, TCSANOW, &settings); /* 使新設定立即生效 */
    if (result < 0) {
        perror("error in tcgetattr"); return 0;
    }
    return 1;
}
