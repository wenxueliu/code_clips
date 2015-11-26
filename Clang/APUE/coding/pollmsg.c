#include "socket.h"


void *helper(void *arg)
{
    int                 n;
    struct mymesg       m;
    struct threadinfo   *tip = arg;

    for (;;){
        memset(&m,0,sizeof(m));
        if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)
        {
            err_sys("msgrecv error");
        }
        if ((write(tip->fd, m.mtext, n)) < 0)
        {
            err_sys("write error");
        }
    }
}

int main()
{
    int                 i,n,err;
    int                 fd[2];
    int                 qid[NQ];
    struct pollfd       pfd[NQ];
    struct threadinfo   ti[NQ];
    pthread_t           tid[NQ];
    char                buf[MAXMSZ];

    for (i = 0; i < NQ; i++)
    {
        if ((qid[i] = msgget((KEY+i), IPC_CREAT|0666)) < 0)
        {
            err_sys("msgget error");
        }
        printf("queue ID %d is %d\n",i, qid[i]);

        if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)
        {
            err_sys("socketpair error");
        }

        pfd[i].fd = fd[0];
        pfd[i].events = POLLIN;
        ti[i].qid = qid[i];
        ti[i].fd = fd[1];

        if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)
        {
            err_sys("pthread_create error");
        }

        for (;;)
        {
            if (poll(pfd, NQ, -1) < 0)
            {
                err_sys("poll err");
            }
            for (i = 0; i < NQ; i++)
            {
                if (pfd[i].revents & POLLIN)
                {
                    if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)
                    {
                        err_sys("read err");
                    }
                    buf[n] = 0;
                    printf("queue id %d, message %s\n",qid[i],buf);
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}
