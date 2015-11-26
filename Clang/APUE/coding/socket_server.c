#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define     BUFLEN  128
#define     QLEN    10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

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
    int fd;
    int err = 0;
    if ((fd = socket(addr->sa_family, type, 0)) < 0)
    {
        return -1;
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

void serve(int sockfd)
{
    int     clfd;
    FILE    *fp;
    char    buf[BUFLEN];
    set_cloexec(sockfd);
    for (;;)
    {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0)
        {
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        set_cloexec(clfd);
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            sprintf(buf, "error: %s\n", strerror(errno));
            send(clfd, buf, strlen(buf), 0);
        }
        else
        {
            while (fgets(buf, BUFLEN, fp) != NULL)
            {
                send(clfd, buf, strlen(buf), 0);
            }
            pclose(fp);
        }
        close(clfd);
    }
}

void server2(int sockfd)
{
    int     clfd, status;
    pid_t   pid;
    set_cloexec(sockfd);
    for(;;)
    {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0){
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if ((pid = fork()) < 0)
        {
            syslog(LOG_ERR, "ruptimed: fork error %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if (pid == 0){
            if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
                    dup2(clfd,STDERR_FILENO) != STDERR_FILENO)
            {
                syslog(LOG_ERR,"ruptimed: unexcepted error");
                exit(EXIT_FAILURE);
            }
            close(clfd);
            execl("/usr/bin/uptime", "uptime", (char *)0);
            syslog(LOG_ERR, "ruptimed: unexcepted return from exec: %s", strerror(errno));
        }
        else
        {
            close(clfd);
            waitpid(pid, &status, 0);
        }
    }
}


int main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int             sockfd, err,n;
    char            *host;

    if (argc != 1)
    {
        printf("usage: ruptimed");
        exit(EXIT_FAILURE);
    }

    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
    {
        n = HOST_NAME_MAX;
    }

    if ((host = malloc(n)) == NULL)
    {
        printf("malloc error");
        exit(EXIT_FAILURE);
    }

    if (gethostname(host, n) < 0)
    {
        printf("gethostname error");
        exit(EXIT_FAILURE);
    }

    daemonize("ruptimed");
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    {
        syslog(LOG_ERR, "ruptimed: getaddrinfo error %s",gai_strerror(err));
        exit(EXIT_FAILURE);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        if (sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen,QLEN) >= 0)
        {
            serve(sockfd);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_FAILURE);
}
