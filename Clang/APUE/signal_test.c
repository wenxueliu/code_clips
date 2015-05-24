#include <stdio.h>
#include <signal.h>
#include <unistd.h>

typedef void    Sigfunc(int);   /* for signal handlers */ 
static void     sig_quit(int);

Sigfunc * signal(int signo, Sigfunc *func)
{
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM)
    {
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
        act.sa_flags |= SA_RESTART;
    }
    if (sigaction(signo, &act, &oact) < 0)
    {
        return (SIG_ERR);
    }
    return (oact.sa_handler);
}

int pending()
{
    sigset_t    newmask, oldmask, pendmask;
    if (signal(SIGQUIT,sig_quit) < 0)
    {
        printf("can't catch SIGQUIT\n");
        return (-1);
    }
    //
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        perror("SIG_BLOCK error\n");
    }

    sleep(5);

    if (sigpending(&pendmask) < 0)
    {
        printf("sigpending error\n");
        return -1;
    }

    if (sigismember(&pendmask, SIGQUIT) < 0)
    {
        printf("\n SIGQUIT pending\n");
    }

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        printf("sigprocmask error\n");
        return -1;
    }
    printf("SIGQUIT unblocked\n");
    sleep(5);

}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) < 0)
    {
        perror("signal can not set SIGQUIT\n");
    }
}

int main(int argc, char *argv[])
{
    return pending();
}
