//#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stddef.h>
#include <time.h>

#define QLEN    10
#define STALE   30

void err_quit(const char *str)
{
    printf("%s : %s %d\n",str,__FILE__,__LINE__);
    exit(EXIT_FAILURE);
}

void err_sys(const char *str)
{
    printf("%s : %s %d\n",str,__FILE__,__LINE__);
}

int serv_listen(const char *name)
{
    int                 fd,len, err, rval;
    struct sockaddr_un  un;
    if (strlen(name) >= sizeof(un.sun_path)){
        errno = ENAMETOOLONG;
        return -1;
    }

    if ((fd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
    {
        return -2;
    }
    unlink(name);
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path);
    if (bind(fd, (struct sockaddr*)&un, len) < 0)
    {
        rval = -3;
        goto errout;
    }

    if (listen(fd,QLEN) < 0)
    {
        rval = -4;
        goto errout;
    }

    return fd;

errout:
    err = errno;
    close(fd);
    errno = err;
    return rval;
}

int serv_accept(int listenfd, uid_t *uidptr)
{
    int                 clifd,err, rval;
    socklen_t           len;
    time_t              staletime;
    struct sockaddr_un  un;
    struct stat         statbuf;
    char                *name;
    if ((name = malloc(sizeof(un.sun_path) + 1)) == NULL)
    {
        return EXIT_FAILURE;
    }
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr*)&un, &len)) < 0)
    {
        free(name);
        return -2;
    }

    len -= offsetof(struct sockaddr_un, sun_path);
    memcpy(name, un.sun_path,len);
    name[len] = 0;
    if (stat(name, &statbuf) < 0)
    {
        rval = -3;
        goto errout;
    }
#ifdef S_ISSOCK
    if (S_ISSOCK(statbuf.st_mode) == 0)
    {
        rval = -4;
        goto errout;
    }
#endif
    if ((statbuf.st_mode & (S_IRWXO | S_IRWXG)) ||
        (statbuf.st_mode & S_IRWXU) != S_IRWXU)
    {
        rval = -5;
        goto errout;
    }

    staletime = time(NULL) - STALE;
    if (statbuf.st_atime < staletime ||
        statbuf.st_ctime < staletime ||
        statbuf.st_mtime < staletime)
    {
        rval = -6;
        goto errout;
    }

    if (uidptr != NULL)
    {
        *uidptr = statbuf.st_uid;
    }
    unlink(name);
    free(name);
    return clifd;
errout:
    err = errno;
    close(clifd);
    free(name);
    errno = err;
    return rval;
}

#define CLI_PATH    "/var/tmp"
#define CLI_PERM    "S_IRWXU"

int cli_conn(const char *name)
{
    int                 fd, len, err, rval;
    struct sockaddr_un  un, sun;
    int                 do_unlink = 0;

    if (strlen(name) >= sizeof(un.sun_path))
    {
        errno = ENAMETOOLONG;
        return -1;
    }

    if (fd = socket(AF_UNIX, SOCK_STREAM,0) < 0)
    {
        return -1;
    }

    memset(&un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path, "%s05ld", CLI_PATH, (long)getpid());
    len = offsetof(struct sockaddr_un, sun_path);
    unlink(un.sun_path);

    if (bind(fd, (struct sockaddr*)&un, len) < 0)
    {
        rval = -2;
        goto errout;
    }

    if (chmod(un.sun_path, CLI_PERM) < 0)
    {
        rval = -3;
        do_unlink = 1;
        goto errout;
    }

    memset(&sun, 0, sizeof(sun));
    strcpy(sun.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if (connect(fd, (struct sockaddr *)&sun, len) < 0){
        rval = -4;
        do_unlink = -1;
        goto errout;
    }
    return fd;
errout:
    err = errno;
    close(fd);
    if (do_unlink)
    {
        unlink(un.sun_path);
    }
    errno = err;
    return rval;
}

int send_err(int fd, int errcode, const char *msg)
{
    int     n;
    if ((n = strlen(msg)) > 0)
    {
        if (write(fd, msg, n) != n)
        {
            return -1;
        }
    }
    if (errcode >= 0)
    {
        errcode = -1;
    }
    if (send_fd(fd, errcode) < 0)
    {
        return -1;
    }
    return 0;
}


//#define CONTROLLEN  CMSG_LEN(sizeof(int))
static struct cmsghdr  *cmptr = NULL;

#if defined(SCM_CREDS)
#define CREDSTRUCT      cmsgcred
#define SCM_CREDTYPE    SCM_CREDS
#elif defined(SCM_CREDENTIALS)
#define CREDSTRUCT      ucred
#define SCM_CREDTYPE    SCM_CREDENTIALS
#else
#error  passing credentials is unsupported!
#endif

#define RIGHTSLEN   CMSG_LEN(sizeof(int))
#define CREDSLEN    CMSG_LEN(sizeof(struct CREDSTRUCT))
#define CONTROLLEN  (RIGHTSLEN + CREDSLEN)

int send_fd(int fd, int fd_to_send)
{
    struct CREDSTRUCT   *credp;
    struct cmsghdr      *cmp;
    struct iovec        iov[1];
    struct msghdr       msg;
    char                buf[2];

    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    msg.msg_flags   = 0;
    if (fd_to_send < 0)
    {
        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        buf[1] = -fd_to_send;
        if (buf[1] == 0)
        {
            buf[1] = 1;
        }
    }
    else{
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
        {
            return -1;
        }
        msg.msg_control    = cmptr;
        msg.msg_controllen = CONTROLLEN;
        cmp = cmptr;
        cmp->cmsg_level  = SOL_SOCKET;
        cmp->cmsg_type   = SCM_RIGHTS;
        cmp->cmsg_len    = RIGHTSLEN;
        credp = (struct CREDSTRUCT *)CMSG_DATA(cmp);
#if defined(SCM_CREDENTIALS)
        credp->uid      = geteuid();
        credp->gid      = geteuid();
        credp->pid      = getpid()
#endif
        buf[1] = 0;
    }
    buf[0] = 0;
    if (sendmsg(fd, &msg, 0) != 2)
    {
        return -1;
    }
    return 0;
}

#define MAXLINE     4096
int recv_fd(int fd, uid_t *uidptr, ssize_t (*userfunc)(int, const void *, size_t))
{
    struct cmsghdr      *cmp;
    struct CREDSTRUCT   *credp;
    int                 newfd = -1;
    int                 nr;
    char                *ptr;
    char                buf[MAXLINE];
    struct iovec        iov[1];
    struct msghdr       msg;
    int                 nr;
    int                 newfd = -1;
    int                 status = -1;
#if defined(CREDOPT)
    const int           on = 1;
    if (setsockopt(fd, SOL_SOCKET, CREDOPT, &on, sizeof(int)) < 0)
    {
        err_sys("setsockopt error");
        return -1;
    }
#endif
    for (;;){
        iov[0].iov_base = buf;
        iov[0].iov_len  = sizeof(buf);
        msg.msg_iov     = iov;
        msg.msg_iovlen  = 1;
        msg.msg_name    = NULL;
        msg.msg_namelen = 0;
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
        {
            return -1;
        }
        msg.msg_control     = cmptr;
        msg.msg_controllen  = CONTROLLEN;
        if ((nr = recvmsg(fd, &msg, 0)) < 0)
        {
            err_sys("recvmsg error");
            return -1;
        }
        else if (nr == 0)
        {
            err_sys("connection closed by server");
            return -1;
        }
        for (ptr = buf; ptr < &buf[nr];)
        {
            if (*ptr++ == 0)
            {
                if (ptr != &buf[nr-1])
                {
                    err_sys("message format error");
                }
                status = *ptr & 0xFF;
                if (status == 0)
                {
                    if (msg.msg_controllen != CONTROLLEN)
                    {
                        err_sys("status = 0 buf no fd");
                    }
                    for (cmp = CMSG_FIRSTHDR(&msg);
                        cmp != NULL; cmp = CMSG_NXTHDR(&msg, cmp)){
                        if (cmp->cmsg_level != SOL_SOCKET)
                        {
                            continue;
                        }
                        switch(cmp->cmsg_type){
                        case SCM_RIGHTS:
                            newfd = *(int *)CMSG_DATA(cmp);
                            break;
                        case SCM_CREDTYPE:
                            credp = (struct CREDSTRUCT *)CMSG_DATA(cmp);
                            *uidptr = credp->CR_UID;
                        }
                    }
                }
                else
                {
                    newfd = -status;
                }
                nr = -2;
            }
        }
        if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)
        {
            return -1;
        }
        if (status >= 0)
        {
            return newfd;
        }
    }
}
