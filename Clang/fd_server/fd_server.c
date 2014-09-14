#include "fd_server.h"
#include "name_un_socket.h"
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


static void client_alloc(void)
{
    int     i;
    if (client  == NULL)
    {
        client  = malloc(NALLOC * sizeof(Client));
    }
    else
    {
        client = realloc(client, (client_size + NALLOC) * sizeof(Client));
    }
    if (client == NULL)
    {
        err_sys("can't alloc for client array");
    }

    for (i = client_size; i < client_size + NALLOC; i++)
    {
        client[i].fd = -1;
    }

    client_size += NALLOC;
}

int client_add(int fd, uid_t uid)
{
    int     i;
    if (client == NULL)
    {
        client_alloc();
    }
again:
    for (i = 0; i < client_size; i++)
    {
        if (client[i].fd == -1)
        {
            client[i].fd = fd;
            client[i].uid = uid;
            return i;
        }
    }
    client_alloc();
    goto again;
}

void client_del(int fd)
{
    int     i;
    for (i = 0; i < client_size; i++)
    {
        if (client[i].fd == fd)
        {
            client[i].fd = -1;
            return;
        }
    }
    printf("can't find client entry for fd %d",fd);
}

void loop(void)
{
    int         i,n, maxfd,maxi, listenfd, clifd, nread;
    char        buf[MAXLENLINE];
    uid_t       uid;
    fd_set      rset, allset;

    FD_ZERO(&allset);

    if((listenfd = serv_listen(CS_OPEN)))
    {
        //log_sys("serv_listen error");
    }
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    maxi = -1;

    for ( ; ; )
    {
        rset = allset;
        if ((n=select(maxfd+1, &rset, NULL, NULL, NULL)) < 0)
        {
            //log_sys("select error");
        }

        if (FD_ISSET(listenfd, &rset))
        {
            if((clifd = serv_accept(listenfd, &uid)) < 0)
            {
                //log_sys("serv_accept error: %d", clifd);
            }
            i = client_add(clifd, uid);
            FD_SET(clifd, &allset);
            if (clifd > maxfd)
            {
                maxfd = clifd;
            }
            if (i > maxi)
            {
                maxi = i;
            }
            //log_msg("new connection: uid %d, fd %d", uid, clifd);
        }
        else
        {
            for (i = 0; i <= maxi; i++)
            {
                if ((clifd = client[i].fd) < 0)
                {
                    continue;
                }

                if (FD_ISSET(clifd, &rset))
                {
                    if ((nread = read(clifd, buf, MAXLENLINE)) < 0)
                    {
                        //log_sys("read error on fd: %d",clifd);
                    }
                    else if(nread == 0)
                    {
                        //log_msg("closed: uid %d, fd %d",client[i].uid, clifd);
                        client_del(clifd);
                        FD_CLR(clifd,&allset);
                        close(clifd);
                    }
                    else
                    {
                        handle_request(buf, nread, clifd, client[i].uid);
                    }
                }
            }
        }
    }
}


void pollfd_init(pollfd_wrap *pfd_wrap, struct pollfd *default_pfd)
{
    int i = 0;
    struct pollfd *pfd;
    if ((pfd_wrap->pfd = malloc(pfd_wrap->maxfd * sizeof(struct pollfd))) < 0)
    {
        err_sys("malloc error");
    }

    if (default_pfd == NULL)
    {
        default_pfd->fd = 0;
        default_pfd->events = 0;
        default_pfd->revents = 0;
    }

    for (i = 0; i < NALLOC; i++)
    {
        pfd = pfd_wrap->pfd;
        pfd[i].fd = default_pfd->fd;
        pfd[i].events = default_pfd->events;
        pfd[i].revents = default_pfd->revents;
    }
}

//interval grow is NALLOC;
void pollfd_alloc(pollfd_wrap *pfd_wrap, struct pollfd *default_pfd)
{
    int             i;
    int             oldmax = pfd_wrap->maxfd;
    int             newmax = oldmax + NALLOC;
    struct pollfd   *pfd;

    if ((pfd_wrap->pfd = realloc(pfd_wrap->pfd, newmax * sizeof(pfd_wrap->pfd))) == NULL)
    {
        err_sys("realloc error");
    }

    if (default_pfd == NULL)
    {
        default_pfd->fd = 0;
        default_pfd->events = 0;
        default_pfd->revents = 0;
    }

    for (i = oldmax; i < newmax; i++)
    {
        pfd = pfd_wrap->pfd;
        pfd[i].fd = default_pfd->fd;
        pfd[i].events = default_pfd->events;
        pfd[i].revents = default_pfd->revents;
    }
    pfd_wrap->maxfd = newmax;
}


/*
 *
 * return
 * -1 : pfd_wrap is NULL
 * 0  : add success
 * 1  : no more space, call pollfd_alloc() please
*/
int pollfd_add(pollfd_wrap *pfd_wrap, struct pollfd *newpfd)
{
    int             i;
    struct pollfd   *pfd;
    if (pfd_wrap == NULL)
    {
        return -1;
    }
again:
    pfd = pfd_wrap->pfd;
    for (i = 0; i < pfd_wrap->maxfd; i++)
    {
        if (pfd[i].fd == -1)
        {
            pfd[i].fd = newpfd->fd;
            pfd[i].events = newpfd->events;
            pfd[i].revents = newpfd->revents;
            return 0;
        }
    }
    return 1;
}

void loop2(void)
{
    int             i, listenfd, clifd, nread;
    char            buf[MAXLENLINE];
    uid_t           uid;
    struct pollfd   *pollfd = NULL;
    int             numfd = 1;
    pollfd_wrap     *pfd_wrap;

    struct pollfd *default_pfd;
    default_pfd->fd = -1;
    default_pfd->events = POLLIN;
    default_pfd->events = 0;

    pfd_wrap->pfd   = default_pfd;
    pfd_wrap->maxfd = NALLOC;

    pollfd_init(pfd_wrap, default_pfd);

    pollfd = pfd_wrap->pfd;
    int maxfd = pfd_wrap->maxfd;

    if ((listenfd = serv_listen(CS_OPEN)) < 0)
    {
        //log_sys("serv_listen error");
    }

    client_add(listenfd, 0);
    pollfd[0].fd = listenfd;

    for (;;)
    {
        if (poll(pollfd, numfd, -1) < 0)
        {
            //log_sys("poll error");
        }

        if (pollfd[0].revents & POLLIN)
        {
            if ((clifd = serv_accept(listenfd, &uid)) < 0)
            {
                //log_sys("serv_accept error: %d", clifd);
            }
            client_add(clifd,uid);

            if (numfd == pfd_wrap->maxfd)
            {
                default_pfd->fd = -1;
                pollfd_alloc(pfd_wrap, default_pfd);
            }
            else
            {
                default_pfd->fd = clifd;
                pollfd_add(pfd_wrap, default_pfd);
            }
            pollfd = pfd_wrap->pfd;

            pollfd[numfd].fd = clifd;
            pollfd[numfd].events = POLLIN;
            pollfd[numfd].revents = 0;
            numfd++;
            //log_msg("new connection: uid %d, fd %d, uid, clifd");
        }

        for (i = 1; i < numfd; i++)
        {
            if (pollfd[i].revents & POLLHUP)
            {
                goto hungup;
            }
            else if(pollfd[i].revents & POLLIN)
            {
                if ((nread = read(pollfd[i].fd, buf, MAXLENLINE)) < 0)
                {
                    //log_sys("read error on fd %d",pollfd[i].fd);
                }
                else if(nread == 0)
                {
        hungup:
                    //log_msg("closed: fd %d", pollfd[i].fd);
                    client_del(pollfd[i].fd);
                    close(pollfd[i].fd);
                    //pack the pollfd
                    //TODO there is a drawback, if you allocate
                    //many pollfds, it cannot be released if you
                    //needn't them;
                    if (i < (numfd-1))
                    {
                        pollfd[i].fd = pollfd[numfd-1].fd;
                        pollfd[i].events = pollfd[numfd-1].events;
                        pollfd[i].revents = pollfd[numfd-1].revents;
                        i--;
                    }
                    numfd--;
                }
                else
                {
                    handle_request(buf, nread, pollfd[i].fd,client[i].uid);
                }
            }
        }
    }
}

void handle_request(char *buf, int nread, int clifd, uid_t uid)
{
    char *errmsg = NULL;
    errmsg = malloc(MAXLENLINE * sizeof(char));
    memset(errmsg, 0, MAXLENLINE);

    int     newfd;
    if (buf[nread-1] != 0)
    {
        snprintf(errmsg, MAXLENLINE-1,
                "request from uid %d not null terminated: %*.*s\n",
                uid, nread, nread, buf);
        send_err(clifd, -1, errmsg);
        return;
    }
    //log_msg("request: %s, from uid %d", buf,uid);

    client_args *cli_args = NULL;
    if (parse_args(buf, &cli_args) < 0)
    {
        send_err(clifd, -1, errmsg);
        //log_msg(errmsg);
        return;
    }

    open_fd *op_fd = NULL;
    if (get_cli_args(cli_args, &op_fd) < 0)
    {
        send_err(clifd, -1, errmsg);
        //log_msg(errmsg);
        return;
    }

    if ((newfd = open(op_fd->path, op_fd->oflag)) < 0)
    {
        snprintf(errmsg, MAXLENLINE-1, "can't open %s: %s\n", op_fd->path,strerror(errno));
        send_err(clifd, -1, errmsg);
        //log_msg(errmsg);
        return;
    }

    if (send_fd(clifd, newfd) < 0)
    {
        //log_sys("send_fd error");
    }
    //log_msg("send fd %d over fd %d for %s", newfd, clifd, op_fd->path);
    close(newfd);
}

//int parse_args(char *buf, int(*optfunc)(int, char**))
int parse_args(char *buf, client_args **cli_args)
{
    *cli_args = malloc(sizeof(client_args));
    int j = 0;
    for (j = 0; j < MAXLENARG; j++)
    {
        (*cli_args)->argv[j] = malloc(MAXLENARG * sizeof(char));
        memset((*cli_args)->argv[j], 0,MAXLENARG * sizeof(char));
    }
    memset(cli_args, 0, sizeof(cli_args));

    char    *ptr;
    char    **argv = (*cli_args)->argv;
    int     argc = (*cli_args)->argc;

    if (strtok(buf, WHITE) == NULL)
    {
        return -1;
    }
    argv[argc = 0] = buf;
    while((ptr = strtok(NULL, WHITE)) != NULL)
    {
        if (++argc >= MAXARGC - 1)
        {
            return -1;
        }
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;
    return 0;
}

int get_cli_args(client_args *args, open_fd **fd)
{
    *fd = malloc(sizeof(open_fd));
    memset(fd,0,sizeof(open_fd));

    char *errmsg = NULL;
    errmsg = malloc(MAXLENLINE * sizeof(char));
    memset(errmsg, 0, MAXLENLINE * sizeof(char));

    int argc = args->argc;
    char **argv = args->argv;
    if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
    {
        strcpy(errmsg,"usage: <pathname> <oflag>\n");
        return -1;
    }
    (*fd)->path = argv[1];
    (*fd)->oflag = atoi(argv[2]);
    return 0;
}
