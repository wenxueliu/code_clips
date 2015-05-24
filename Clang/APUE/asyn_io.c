#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#define  MAXLINE  256
static jmp_buf  env_alrm;
static void alrm_read(int);
static void alrm_write(int);

static void alrm_read(int signo)
{
    printf("read timeout\n");
}

static void alrm_write(int signo)
{
    printf("write timeout\n");
}

static void sig_alrm(int signo)
{
    longjmp(env_alrm,1);
}
//-1 : read 0 byte
//-2 : timeout
int aio_read(int fd, void* line, size_t len, size_t timeout)
{
    int n;
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("signal(SIGALRM) error\n");
    }
    if (setjmp(env_alrm) != 0)
    {
        printf("read timeout\n");
        return -2;
    }
    alarm(timeout);
    if ((n = read(fd, line, len)) < 0)
    {
        perror("read error\n");
        return -1;
    }
    alarm(0);
    return n;
}

int aio_write(int fd, void* line, size_t len, size_t timeout)
{
    int n;
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("signal(SIGALRM) error\n");
    }
    if (setjmp(env_alrm) != 0)
    {
        printf("read timeout\n");
        return -2;
    }
    alarm(timeout);
    if ((n = write(fd, line, len)) < 0)
    {
        perror("write error:");
        return -1;
    }
    alarm(0);
    return n;
}

int main(int argc, char *argv)
{
    char *line[MAXLINE];
    int timeout = 5;
    int n = aio_read(STDIN_FILENO,line,MAXLINE,timeout);
    aio_write(STDOUT_FILENO,line,n,timeout);
    exit(EXIT_SUCCESS);
}
