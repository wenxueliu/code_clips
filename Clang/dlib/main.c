#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

typedef int (*Handler)(void *);
const static char LIB_PATH[] = "./libtick.so";

int main() {

    char c;
    int i;
    char * error;
    Handler handler;
    void *dl_handle = NULL;
    dl_handle = dlopen(LIB_PATH, RTLD_LAZY);

    if (dl_handle == NULL) {
        fprintf(stderr, "open %s error\n", LIB_PATH);
        return 1;
    }

    handler = dlsym( dl_handle, "func");
    error = dlerror();
    if (error != NULL) {
        printf( "!!! %s\n", error );
        return;
    }

    handler(NULL);

    while (scanf("%c", &c) > 0) {

        if (isspace(c)) {
            continue;
        }

        printf("%c\n", c);
        switch(c) {
            case 'q':
                goto end;
            default:
                break;
        }
    }

end:

    dlclose(dl_handle);
    printf("closed dl (%s)\n", LIB_PATH);


    getchar();
    getchar();
    return 0;
}

