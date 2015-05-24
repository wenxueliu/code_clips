#include <stdio.h>
#include <errno.h>
#include <signal.h>

void print_mask(const char *str)
{
    sigset_t    sigset;
    int         errno_save;

    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0)
    {
        perror("sigprocmask error\n");
        return ;
    }
    else
    {
        printf("%s", str);
        if (sigismember(&sigset, SIGINT))
        {
            printf("SIGINT\n");
        }
        if (sigismember(&sigset, SIGQUIT))
        {
            printf("SIGQUIT\n");
        }
        if (sigismember(&sigset, SIGUSR1))
        {
            printf("SIGUSR1\n");
        }
        if (sigismember(&sigset, SIGALRM))
        {
            printf("SIGALRM\n");
        }
        /* remain signal can go here*/
    }
    errno = errno_save;
}


