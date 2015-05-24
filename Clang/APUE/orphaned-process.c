#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>


static void sig_hup(int signo)
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void pr_ids(char *name)
{
    printf("%s: pid = %ld, ppid = %d, pgrp = %ld, tpgrp = %ld\n",
        name, (long)getpid(), (long)getpid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

int main(void)
{
    char     c;
    pid_t   pid;

    pr_ids("parent");
    if ((pid = fork()) < 0)
    {
        perror("fork error");
    }
    else if (pid > 0)
    {
        sleep(5);
    }
    else
    {
        pr_ids("child");
        signal(SIGHUP, sig_hup);
        kill(getpid(), SIGTSTP);
        pr_ids("child");
        if (read(STDIN_FILENO, &c, 1) != 1)
        {
            printf("read error %d on controlling TTY\n", errno);
        }
    }
    exit(EXIT_SUCCESS);
}


