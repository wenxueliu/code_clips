#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void pr_exit(int status)
{
    if (WIFEXITED(status))
    {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("abnormal termination, signal number = %d%s\n",
            WTERMSIG(status),

#ifdef WCOREDUMP
        WCOREDUMP(status) ? "  (core file generated)" : "");
#else
        "");
#endif
    }

    else if (WIFSTOPPED(status))
    {
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}

int system(const char *cmdstring)
{
    pid_t   pid;
    int     status;

    if (cmdstring == NULL)
    {
        return 1;
    }

    if ((pid = fork()) < 0)
    {
        status = -1;
    }
    else if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", cmdstring, (char*)0);
    }
    else
    {
        while (waitpid(pid, &status, 0) < 0)
        {
            if (errno != EINTR)
            {
                status = -1;
                break;
            }
        }
    }
}

int main(void)
{
    int status;
    if ((status = system("date")) < 0)
    {
        perror("system() error");
    }

    pr_exit(status);

    if ((status = system("nosuchcommand")) < 0)
    {
        perror("system() error");
    }

    pr_exit(status);

    exit(EXIT_SUCCESS);
}
