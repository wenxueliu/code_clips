#ifndef FD_SOCKET_H_
#define FD_SOCKET_H_

#include "socket.h"
#include <fcntl.h>
#define CL_OPEN     "open"
#define MAXARGC     50
#define WHITE       " \t\n"
extern char     errmsg[];
extern int      oflag;
extern char     *pathname;

int     cli_args(int, char **);
void    handle_request(char *, int, int);
int     buf_args(char *, int(*optfunc)(int, char**));

#endif
