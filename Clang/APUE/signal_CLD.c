#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>


static void sig_cld(int);

int main(int argc, char**argv)
{
    pid_t   pid;
    if (signal(SIGCLD, sig_cld) == SIG_ERR)
    {
        perror("signal error\n");
    }
    if ((pid = fork()) < 0)
    {
        perror("fork error\n");
    }
    else if (pid == 0)
    {
        sleep(2);
        _exit(0);
    }
    pause();
    _exit(0);
}

static void sig_cld(int signo)
{
    pid_t       pid;
    int         status;
    printf("SIGCLD received\n");
    if (signal(SIGCLD, sig_cld) == SIG_ERR)
    {
        perror("signal error");
    }
    if ((pid = wait(&status)) < 0)
    {
        perror("wait error");
    }

    printf("pid = %d\n", pid);
}
