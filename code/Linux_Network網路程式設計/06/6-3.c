#include <stdio.h>
#include <sys/types.h>

int
main() {
#ifdef __BYTE_ORDER
# if __BYTE_ORDER == __BIG_ENDIAN
    char* endianstr = "big endian\n";
# elif __BYTE_ORDER == __LITTLE_ENDIAN
    char* endianstr = "little endian\n";
# else
    unknown endian, gcc will fail here.
# endif
#else // __BYTE_ORDER not defined
    error !
#endif
    printf("%s\n", endianstr);
    return 0;
}
