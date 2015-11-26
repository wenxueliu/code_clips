#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include "poll.h"
#include <stdlib.h>
#include "socket.h"
#include <string.h>

int main(char argc, char *argv[])
{
    key_t           key;
    long            qid;
    size_t          nbytes;
    struct mymesg   m;

    if (argc != 3)
    {
        fprintf(stderr,"usage: sendmsg KEY message\n");
        exit(EXIT_FAILURE);
    }

    key = strtol(argv[1], NULL, 0);
    if ((qid = msgget(key, 0)) < 0)
    {
        err_sys("msgget error\n");
    }
    memset(&m, 0, sizeof(m));
    strncpy(m.mtext, argv[2], MAXMSZ-1);
    nbytes  = strlen(m.mtext);
    m.mtype = 1;
    if (msgsnd(qid, &m, nbytes, 0) < 0)
    {
        err_sys("can't send message");
    }
    exit(EXIT_SUCCESS);
}
