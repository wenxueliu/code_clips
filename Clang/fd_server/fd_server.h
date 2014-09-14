#ifndef _FD_SERVER_H_
#define _FD_SERVER_H_

#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdio.h>
#include <unistd.h>

#define MAXLENPATH 256
#define MAXLENLINE 4096
#define CS_OPEN "/tmp/opend.socket"
#define CL_OPEN  "open"

#define NALLOC  10
#define MAXARGC     50
#define MAXLENARG  256
#define WHITE       " \t\n"

typedef struct{
    int     fd;
    uid_t   uid;
} Client;

extern Client   *client;
extern int      client_size;
extern int      debug;
//extern char     errmsg[];
//extern int      oflag;
//extern char     *pathname;

typedef struct {
    struct pollfd   *pfd;
    int             maxfd;
} pollfd_wrap;

typedef struct {
    char    *path;
    int     oflag;
} open_fd;

typedef struct {
    char    *argv[MAXARGC];
    int     argc;
} client_args;

int client_add(int fd, uid_t uid);
void client_del(int fd);
void loop(void);

void pollfd_alloc(pollfd_wrap *pfd_wrap, struct pollfd *default_pfd);
int pollfd_add(pollfd_wrap *pfd_wrap, struct pollfd *newpfd);
void loop2(void);

void handle_request(char *buf, int nread, int clifd, uid_t uid);

int cli_arg(int argc, char **argv);
int buf_args(char *buf, int(*optfunc)(int, char**));
int parse_args(char *buf, client_args **args);
int get_cli_args(client_args *args, open_fd **fd);

void log_sys(const char *,...);
void log_msg(const char *,...);
void log_quit(const char *,...);
#endif
