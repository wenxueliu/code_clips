#ifndef  _SOCKET_H_
#define  _SOCKET_H_

#define _POSIX_C_SOURCE 200809L

#if defined(SOLARIS)		/* Solaris 10 */
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 700
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <poll.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>		/* for SIG_ERR */
#include <time.h>


#define NQ          3
#define MAXMSZ      512
#define KEY         0x123
#define MAXLINE     4096


struct addrinfo {
    int		ai_flags;
    int		ai_family;
    int		ai_socktype;
    int		ai_protocol;
    socklen_t	ai_addrlen;
    struct sockaddr *ai_addr;
    char	       *ai_canonname;
    struct addrinfo *ai_next;
};

int initserver(int type, const struct sockaddr *addr,
        socklen_t alen, int qlen)
{
    int fd, err;
    int reuse = 1;
    if ((fd = socket(addr->sa_family, type, 0)) < 0)
    {
        return -1;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
    {
        goto errout;
    }
    if (bind(fd, addr, alen < 0))
    {
        goto errout;
    }
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        if (listen(fd, qlen) < 0)
        {
            goto errout;
        }
    }
    return fd;
errout:
    err = errno;
    close(fd);
    errno = err;
    return -1;
}

void err_sys(const char *str)
{
    printf("%s : %s %d\n",str,__FILE__,__LINE__);
}

void err_quit(const char *str)
{
    printf("%s : %s %d\n",str,__FILE__,__LINE__);
    exit(EXIT_FAILURE);
}

struct threadinfo{
    int qid;
    int fd;
};

struct mymesg{
    long mtype;
    char mtext[MAXMSZ];
};

//struct pollfd {
//    int   fd;	 /* file descriptor */
//    short events;	 /* requested events */
//    short revents;	 /* returned events */
//};

//struct sockaddr_un{
//    sa_family_t     sun_family;
//    char            sun_path[108];
//};
#endif
