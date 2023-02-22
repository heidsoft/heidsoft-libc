#include <stdio.h>
#include <sys/types.h>

int
main() {
    u_int32_t i32 = 0x12345678;
    u_int8_t* ch_i32 = (u_int8_t*)&i32;
    u_int16_t i16 = 0x1234;
    u_int8_t* ch_i16 = (u_int8_t*)&i16;
    printf("%.2x %.2x %.2x %.2x\n",
           ch_i32[0], ch_i32[1], ch_i32[2], ch_i32[3]);
    printf("%.2x %.2x\n", ch_i16[0], ch_i16[1]);
    return 1;
}
