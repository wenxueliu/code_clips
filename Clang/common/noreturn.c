//name: noreturn.c  测试__attribute__((noreturn))
#include <stdio.h>
#include <stdlib.h>
void myexit();

int test(int) __attribute__((noreturn));
void myexit() {
    exit(1);
}

int test(int n) {
    if ( n > 0 ) {
        myexit();
        printf("exit no return \n");
        /* 程序不可能到达这里 */
    } else {
        return 0;
    }
}

int main(int argc, char **argv) {
    test(2);
    return 0;
}

