#include "socket.h"
#include "name_un_socket.h"
#include <sys/uio.h>

#define CL_OPEN  "open"
extern char     errmsg[];
extern int      oflag;
extern char     *pathname;

int     cli_args(int, char **);
void    handle_request(char *, int, int);

int csopen(char *name, int oflag)
{
    pid_t           pid;
    int             len;
    char            buf[0];
    struct iovec    iov[3];
    static int      fd[2] = {-1, -1};

    if (fd[0] < 0)
    {
        if (fd_pipe(fd) < 0)
        {
            err_sys("fd_pipe error");
            return -1;
        }
        if ((pid = fork()) < 0)
        {
            err_sys("fork error");
            return -1;
        }
        else if (pid == 0)
        {
            close(fd[0]);
            if (fd[1] != STDIN_FILENO &&
                dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)
            {
                err_quit("dup2 error to stdin");
            }
            if (fd[1] != STDOUT_FILENO && 
                dup2(fd[1],STDOUT_FILENO) != STDOUT_FILENO)
            {
                err_quit("dup2 error to stdin");
            }
            if (execl("./opend", "opend", (char *)0) < 0)
            {
                err_quit("execl error");
            }
        }
        close(fd[1]);
    }

    sprintf(buf, " %d", oflag);
    iov[0].iov_base = CL_OPEN " ";
    iov[0].iov_len  = strlen(CL_OPEN) + 1;
    iov[1].iov_base = name;
    iov[1].iov_len  = strlen(name);
    iov[2].iov_base = buf;
    iov[2].iov_len  = strlen(buf) + 1;
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
    if (writev(fd[0], &iov[0], 3) != len)
    {
        err_sys("writev error");
        return -1;
    }
    return recv_fd(fd[0], write);
}
