#include "namedPoint.h"
#include <stdio.h>

int main(int ac ,char** av){
    struct NamedPoint* origin = makeNamedPoint(0.0,0.0,"origin");
    struct NamedPoint* upperRight = makeNamedPoint(1.0,1.0,"upperRight");
    printf("origin...\n");
    printf(origin->name);
    return 0;
}
