#include <stdio.h>

#include <sys/uio.h>
#include <string.h>
#include "name_un_socket.h"
#include "fd.h"


int csopen(char *name, int oflag)
{
    int             len;
    char            buf[2];
    struct iovec    iov[3];
    static int      csfd = -1;

    if (csfd < 0)
    {
        if ((csfd = cli_conn(CS_OPEN)) < 0)
        {
            err_sys("cli_conn error");
            return -1;
        }
    }
    sprintf(buf, " %d", oflag);
    iov[0].iov_base     = CL_OPEN " ";
    iov[0].iov_len      = strlen(CL_OPEN) + 1;
    iov[1].iov_base     = name;
    iov[1].iov_len      = strlen(name);
    iov[2].iov_base     = buf;
    iov[2].iov_len      = strlen(buf) + 1;
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
    if (writev(csfd, &iov[0], 3) != len)
    {
        err_sys("write error");
        return -1;
    }

    return recv_fd(csfd, write);

}
