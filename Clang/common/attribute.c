//attribute.c
#include <stdio.h>
#include <stdarg.h>

void myprint(const char *format,...) __attribute__((format(printf,1,2)));

void myprint(const char *format,...) {
    va_list args;

    printf("call myprint\n");
    va_start(args, format);
    //TODO
    va_end(args);
}
void test()
{
    myprint("i=%d/n",6);
    myprint("%d,%d/n",1,2);
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
