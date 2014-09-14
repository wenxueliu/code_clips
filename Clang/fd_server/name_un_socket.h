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

void err_quit(const char *str);

void err_sys(const char *str);

int serv_listen(const char *name);

int serv_accept(int listenfd, uid_t *uidptr);

#define CLI_PATH    "/var/tmp"
#define CLI_PERM    "S_IRWXU"

int cli_conn(const char *name);

int send_err(int fd, int errcode, const char *msg);


//#define CONTROLLEN  CMSG_LEN(sizeof(int))
static struct cmsghdr  *cmptr = NULL;

#if defined(SCM_CREDS)
#define CREDSTRUCT      cmsgcred
#define SCM_CREDTYPE    SCM_CREDS
#elif defined(SCM_CREDENTIALS)
#define CREDSTRUCT      ucred
#define SCM_CREDTYPE    SCM_CREDENTIALS
#else
//#error  passing credentials is unsupported!
#endif

#define RIGHTSLEN   CMSG_LEN(sizeof(int))
#define CREDSLEN    CMSG_LEN(sizeof(struct CREDSTRUCT))
#define CONTROLLEN  (RIGHTSLEN + CREDSLEN)

int send_fd(int fd, int fd_to_send);

int recv_fd(int fd, ssize_t (*userfunc)(int, const void *,size_t));
#define MAXLINE     4096
int recv_fd_ex(int fd, uid_t *uidptr, ssize_t (*userfunc)(int, const void *,size_t));
