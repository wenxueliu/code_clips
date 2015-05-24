#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>

static void my_alram(int signo)
{
    struct  passwd      *rootptr;
    printf("signal hander\n");
    int ret = alarm(2);
    printf("alarm(2) return %d\n",ret);
}

int main(int argc, char **argv)
{
    struct passwd       *ptr;
    //alarm(1);
    //sleep(2);
    signal(SIGALRM,my_alram);
    int ret = alarm(4);
    printf("alarm(4) return %d\n",ret);
    ret = alarm(2);
    printf("alarm(2) return %d\n",ret);
    ret = alarm(6);
    printf("alarm(6) return %d\n",ret);
    ret = alarm(0);
    printf("alarm(0) return %d\n",ret);
    sleep(2);
    alarm(2);
    for(;;)
    {
        pause();
        printf("wait for signal\n");
    }
}
