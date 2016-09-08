//test2.c
#include <stdio.h>

void pass_ref(int *i) {
    *i = 4;
}

void pass_value(int i) {
    i = 4;
}

int main(int argc, char* argv) {
    int val = 0;
    int ref = 0;
    pass_value(val);
    pass_ref(&ref);
    printf("val=%d, ref=%d\n", val, ref);
}
