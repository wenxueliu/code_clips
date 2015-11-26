#include <stdio.h>
#include <pthread.h>


int killed = 0;
int cnt = 0;
pthread_t tid = 0;

void init(void) __attribute__((constructor));
void deinit(void) __attribute__((destructor));


void init(void) {
    printf("kick.so init\n");
}

void * run(void * arg) {

    while (!killed && cnt < 10) {
        sleep(1);
        if (!killed) {
            printf("sleep %d\n", ++cnt);
        }
    }

}

int func(void * arg) {
    pthread_create(&tid, NULL, run, arg);
} 

/**
 * the thread must stop running, before dlclose() return, or core
 */
void deinit(void) {

    killed = 1;
    if (tid) {
        printf("kick.so deinit\n");
        pthread_join(tid, NULL);
        tid = 0;
    }
}

