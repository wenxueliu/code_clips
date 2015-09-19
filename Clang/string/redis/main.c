
#include <stdio.h>
#include "sds.h"

int main(int argc, char **argv)
{
    sds str = sdsnewlen("sds-test", 8);
    printf("str : %s\n", str);

    sds str1 = sdsnewlen(NULL, 8);
    printf("str1 : %s\n", str1);

    sds str2 = sdsnew("sds-test2");
    printf("str2 : %s\n", str);
}
