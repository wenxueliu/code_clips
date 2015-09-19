#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "ae.h"
#include "net.h"
#include "zmalloc.h"


static uint64_t time_us();
static int delayed_initial_connect(aeEventLoop *loop, long long id, void *data);
static int connect_socket(aeEventLoop *loop, connection *c);
static void socket_connected(aeEventLoop *loop, int fd, void *data, int mask);
void process_run(connection *cs);
static void socket_readable(aeEventLoop *loop, int fd, void *data, int mask);
static void socket_writeable(aeEventLoop *loop, int fd, void *data, int mask);

static struct sock sock = {
    .connect  = sock_connect,
    .close    = sock_close,
    .read     = sock_read,
    .write    = sock_write,
    .readable = sock_readable
};

typedef struct {
    aeEventLoop *loop;
    uint64_t connections;
    int interval;
    uint64_t stop_at;
    uint64_t complete;
    uint64_t requests;
    uint64_t bytes;
    uint64_t start;
    double throughput;
    uint64_t mean;
    struct hdr_histogram *latency_histogram;
    struct hdr_histogram *u_latency_histogram;
} thread;


int main(int argc, char **argv[]) {
    char *schema  = "http";
    char *host    = "127.0.0.1";
    char *port    = "8000";
    char *service = port ? port : schema;
    char *path    = "/";

    struct addrinfo *addrs, *addr;
    struct addrinfo hints = {
        .ai_family   = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };
    int rc;
    if ((rc = getaddrinfo(host, service, &hints, &addrs)) != 0) {
        const char *msg = gai_strerror(rc);
        fprintf(stderr, "unable to resolve %s:%s %s\n", host, service, msg);
        exit(1);
    }
    for (addr = addrs; addr != NULL; addr = addr->ai_next) {
        int fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (fd == -1) continue;
        rc = connect(fd, addr->ai_addr, addr->ai_addrlen);
        close(fd);
        if (rc == 0) break;
    }
    if (addr == NULL) {
        char *msg = strerror(errno);
        fprintf(stderr, "unable to connect to %s:%s %s\n", host, service, msg);
        exit(1);
    }

    thread *t = zcalloc((size_t)1 * sizeof(thread));
    size_t connections = 10;
    connection *cs = zcalloc(connections * sizeof(connection));
    cs->request = "http request\n";
    cs->length = strlen(cs->request);
    //cs->thread = t;
    cs->addr = addr;
    process_run(cs);


    //pthread_t thread;
    //int ret
    //thread *threads = zcalloc(threads * sizeof(thread));
    //for (i = 0, i < threads; i++) {
    //    thread *t = &threads[i];
    //    if (pthread_create(&t.thread, NULL, &thread_main, t) != 0) {
    //        char *msg = strerror(errno);
    //        fprintf(stderr, "create thread : %s\n", msg);
    //        exit(2);
    //    }
    //}
    //uint64_t start    = time_us();

    //for (i = 0, i < threads; i++) {
    //  thread *t = &threads[i];
    //  pthread_join(t->thread, NULL);
    //}
    return 0;
}

void process_run(connection *cs)
{
    uint64_t start    = time_us();
    int connections = 10;
    aeEventLoop *loop = aeCreateEventLoop(10 + connections * 3);

    aeCreateTimeEvent(loop, 50, delayed_initial_connect, cs, NULL);

    aeMain(loop);
    aeDeleteEventLoop(loop);
}

static uint64_t time_us() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec * 1000000) + t.tv_usec;
}

void dumpStatic(uint64_t start, int complete, int bytes) {
    uint64_t runtime_us = time_us() - start;
    long double runtime_s   = runtime_us / 1000000.0;
    long double req_per_s   = complete   / runtime_s;
    long double bytes_per_s = bytes      / runtime_s;
    printf("Requests/sec: %9.2Lf\n", req_per_s);
}

static int delayed_initial_connect(aeEventLoop *loop, long long id, void *data) {
    connection* c = data;
    connect_socket(loop, c);
    return AE_NOMORE;
}

static int connect_socket(aeEventLoop *aeloop, connection *c) {
    struct addrinfo addr = *c->addr;
    struct aeEventLoop *loop = aeloop;
    int fd, flags;

    fd = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);

    flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    if (connect(fd, addr.ai_addr, addr.ai_addrlen) == -1) {
        if (errno != EINPROGRESS) goto error;
    }

    flags = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags));

    flags = AE_READABLE | AE_WRITABLE;
    if (aeCreateFileEvent(loop, fd, flags, socket_connected, c) == AE_OK) {
        //c->parser.data = c;
        //c->fd = fd;
        return fd;
    }

  error:
    close(fd);
    return -1;
}

static void socket_connected(aeEventLoop *loop, int fd, void *data, int mask) {
    connection *c = data;

    switch (sock.connect(c)) {
        case OK:    break;
        case ERROR: goto error;
        case RETRY: return;
    }

    //http_parser_init(&c->parser, HTTP_RESPONSE);
    c->written = 0;

    aeCreateFileEvent(loop, fd, AE_WRITABLE, socket_writeable, c);
    aeCreateFileEvent(loop, fd, AE_READABLE, socket_readable, c);

    return;

  error:
    //reconnect_socket(c->thread, c);
    printf("socket_connected error\n");
    return;

}

static void socket_readable(aeEventLoop *loop, int fd, void *data, int mask) {
    printf("socket_readable\n");
    connection *c = data;
    size_t n;

    do {
        switch (sock.read(c, &n)) {
            case OK:    break;
            case ERROR: goto error;
            case RETRY: return;
        }
        printf("read-> %s", c->buf);

    } while (n == RECVBUF && sock.readable(c) > 0);

    return;

  error:
    printf("socket_readable error\n");
    return;
    //c->thread->errors.read++;
    //reconnect_socket(c->thread, c);
}

static void socket_writeable(aeEventLoop *loop, int fd, void *data, int mask) {
    printf("socket_writeable\n");
    connection *c = data;

    char  *buf = c->request + c->written;
    size_t len = c->length  - c->written;
    size_t n;

    c->fd = fd;
    switch (sock.write(c, buf, len, &n)) {
        case OK:    break;
        case ERROR: goto error;
        case RETRY: return;
    }

    c->written += n;
    printf("c->writen = %lu, c->length=%lu\n", c->written, c->length);
    if (c->written == c->length) {
        c->written = 0;
        aeDeleteFileEvent(loop, c->fd, AE_WRITABLE);
    }

    return;

  error:
    printf("sock_writeable error\n");
    return;
}
