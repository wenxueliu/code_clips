/**
 * This is a echo server using Redis ae Event library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include "anet.h"
#include "ae.h"

#define IP_STR_LEN  INET6_ADDRSTRLEN
#define  BUF_SIZE  1024

static const int MAX_FD = 65536;
static const int BACK_LOG = 128;
static char server_err[ANET_ERR_LEN];


typedef struct EchoClient {

    char ip[IP_STR_LEN];
    int port;

    char buf[BUF_SIZE]; /*The buffer for received and to send data*/
    int pos; /*The first filled position in buf*/
    int size; /*received data in buffer*/

    pthread_mutex_t mutex;

} EchoClient;

void init_client(EchoClient * client) {
    client->port = 0;
    client->pos = 0;
    client->size = 0;
    pthread_mutex_init(&client->mutex, NULL);
}

void destroy_client(EchoClient * client) {
    pthread_mutex_destroy(&client->mutex);
}

void close_client(aeEventLoop * loop, EchoClient * client, int fd) {
    destroy_client(client);
    aeDeleteFileEvent(loop, fd, AE_WRITABLE | AE_READABLE);
    close(fd);
}

/**
 * Event loop has found the fd for write is avalibable
 * */
void onWritePrepared(aeEventLoop * loop, int fd, void * privdata, int mask) {

    EchoClient * client = (EchoClient *)privdata;
    pthread_mutex_lock(&client->mutex);
    int nwrite = write(fd, client->buf+client->pos, client->size);
    if (nwrite <= 0) {
        close_client(loop, client, fd);
    }
    printf("SEND->(%s,%d): %-*s\n", client->ip, client->port, client->size, client->buf + client->pos);
    client->pos += nwrite;
    client->size -= nwrite;
    pthread_mutex_unlock(&client->mutex);

    if (client->size == 0) {
        aeDeleteFileEvent(loop, fd, AE_WRITABLE);
    }
}

/**
 * Event loop has found the fd for read is avalibable
 * when read data from fd to client->buf, then add aeCreateFileEvent for write
 */
void onReadPrepared(aeEventLoop * loop, int fd, void * privdata, int mask) {

    EchoClient * client = (EchoClient *)privdata;

    pthread_mutex_lock(&client->mutex);

    int nread = read(fd, client->buf + client->pos, BUF_SIZE-client->pos);

    if (nread <= 0) {
        close_client(loop, client, fd);
        return;
    }

    client->size += nread;

    printf("RECV<-(%s,%d): %-*s\n", client->ip, client->port, client->size, client->buf+client->pos);
    pthread_mutex_unlock(&client->mutex);

    aeCreateFileEvent(loop, fd, AE_WRITABLE, onWritePrepared, client);

}

/**
 * When a new client connect to this server, acceptTcpHandler() method will be invoked.
 */
void acceptTcpHandler(aeEventLoop * loop, int fd, void *privdata, int mask) {
    int cfd;

    EchoClient * client = malloc(sizeof(EchoClient));

    if (client == NULL) {
        fprintf(stderr, "malloc failed!");
        return;
    }

    init_client(client);

    /*accept the new client*/
    cfd = anetTcpAccept(server_err, fd, client->ip, sizeof(client->ip), &client->port);
    if (cfd == ANET_ERR) {
        fprintf(stderr, "Accept client error\n");
        return;
    }

    printf("new client :%s:%d\n", client->ip, client->port);

    /*add the new cfd to epoll*/
    if (aeCreateFileEvent(loop, cfd, AE_READABLE, onReadPrepared, client) == AE_ERR) {
         close(fd);
         destroy_client(client);
         free(client);
         client = NULL;
    }
}

int main() {
   int fd = 0;
   aeEventLoop * loop = NULL;

   loop = aeCreateEventLoop(MAX_FD);
   fd = anetTcpServer(server_err, 8000, NULL, BACK_LOG);

   if (fd > 0) {
       if (aeCreateFileEvent(loop, fd, AE_READABLE, acceptTcpHandler, NULL) == AE_ERR) {
            fprintf(stderr, "CreateFileEvent error!\n");
            return EXIT_FAILURE;
       }
   } else {
       fprintf(stderr, "anetTcpServer error");
       return EXIT_FAILURE;
   }

    aeMain(loop);
    aeDeleteEventLoop(loop);

    return 0;
}

