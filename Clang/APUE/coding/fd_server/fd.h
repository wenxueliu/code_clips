#include <stdio.h>
#include <unistd.h>

#define MAXLINE 4096
#define CS_OPEN "/tmp/opend.socket"
#define CL_OPEN  "open"

extern int      debug;
extern char     errmsg[];
extern int      oflag;
extern char     *pathname;

typedef struct{
    int     fd;
    uid_t   uid;
} Client;

extern Client   *client;
extern int      client_size;

