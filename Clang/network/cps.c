#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <limits.h>
#include <arpa/inet.h>
#include <assert.h>
#include <time.h>

#define BUFLEN 10240

typedef struct {
    pthread_t thread;
    char *host;
    char *port;
    int connections;
    uint64_t start_time;
    uint64_t end_time;
    int *fds;
} thread;

uint64_t get_ms() {
    struct timeval tv;

    //*  Gettimeofday is slow on some systems. Moreover, it's not necessarily
    //    monotonic. Thus, it's used as a last resort mechanism. */
    int rc = gettimeofday (&tv, NULL);
    assert(rc==0);
    return tv.tv_sec * (uint64_t) 1000 + tv.tv_usec / 1000;
}

typedef struct {
    char *host;
    char *port;
    int connections;
    int thread_num;
} config;


void *thread_main(void *arg) {
    thread *thread = arg;

    //struct addrinfo hints, *result;
    //struct addrinfo *rp;
	//memset(&result, 0, sizeof(result));
	//memset(&hints, 0, sizeof(hints));
    //hints.ai_flags    = 0;
    //hints.ai_family   = AF_INET;
    //hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = 0;

    //int s;
    //if ((s = getaddrinfo(thread->host, thread->port, &hints, &result)) != 0) {
    //    char *msg = strerror(errno);
    //    printf("getaddrinfo %s %s\n error: %s", thread->host, thread->port, gai_strerror(s));
    //    exit(EXIT_FAILURE);
    //}


    thread->start_time = get_ms();

    int con_index = 0;
    for (; con_index < thread->connections; con_index++) {
        //for (rp = result; rp != NULL; rp = result->ai_next) {
        //    int fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        //    int flags = fcntl(fd, F_GETFL, 0);
        //    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

        //    if (connect(fd, rp->ai_addr, rp->ai_addrlen) == -1) {
        //        char *msg = strerror(errno);
        //        fprintf(stderr, "connect  error");
        //        exit(EXIT_FAILURE);
        //    }
        //    thread->fds[con_index] = fd;
        //}

        //printf("connect to host=%s port=%s\n", thread->host, thread->port);
	    char buf[BUFLEN];
	    struct sockaddr_in addr;
	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(atoi(thread->port));
	    strncpy(buf, thread->host, strlen(thread->host));
	    inet_pton(AF_INET, buf, &addr.sin_addr);

        int fd;
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            char *msg = strerror(errno);
            //fprintf(stderr, "createt socket error: %d %s\n", errno, msg);
        }
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        int ret = connect(fd, (struct sockaddr*) &addr, sizeof(addr));
        if (errno == EINPROGRESS) {
            //printf("connection ...\n");
            continue;
        } else {
            char *msg = strerror(errno);
            //fprintf(stderr, "connect  error: %s\n", msg);
            exit(EXIT_FAILURE);
        }
        thread->fds[con_index] = fd;
    }

    thread->end_time = get_ms();
    //setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags));
}

int get_vaild_int(const char *str_num) {
    /*
     * perfer strtol to atoi, see man atoi
     */
    //return atoi(str_num);
    char *enstr;
    int vaild_int = strtol(str_num, &enstr, 10);

    errno = 0;
    if ((errno != 0 && vaild_int == 0) ||
        (errno == ERANGE && (vaild_int == LONG_MAX || vaild_int == LONG_MIN))) {
        fprintf(stderr, "str to int error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (enstr == str_num) {
	   fprintf(stderr, "No digits were found\n");
	   exit(EXIT_FAILURE);
    }
    return vaild_int;
}

int main(int argc, char *argv[]) {
    config *cfg = malloc(sizeof(config));
    printf("argc=%d\n", argc);
    memset(cfg, 0, sizeof(cfg));
    if (argc == 3) {
        cfg->host = argv[1];
        cfg->port = argv[2];
        cfg->connections = 10;
        cfg->thread_num = 1;
    } else if (argc == 4) {
        cfg->host = argv[1];
        cfg->port = argv[2];
        const char *con = argv[3];
        printf("arg3=%lu\n con=%lu", strlen(argv[3]), strlen(con));
        cfg->connections = get_vaild_int(con);
        cfg->thread_num = 1;
    } else if (argc == 5) {
        cfg->host = argv[1];
        cfg->port = argv[2];
        const char *con = argv[3];
        const char *num = argv[4];

        printf("arg3=%lu arg4=%lu con=%lu\n", strlen(argv[3]), strlen(argv[4]), strlen(con));
        cfg->connections = get_vaild_int(con);
        cfg->thread_num  = get_vaild_int(num);
    } else {
        printf("Usage: %s host port [connections] [thread_num]\n", argv[0]);
        return 1;
    }

    //printf("host=%s port=%s connections=%"PRIu64" thread_num=%"PRIu64" \n", cfg->host, cfg->port, cfg->connections, cfg->thread_num);
    printf("host=%s port=%s connections=%d thread_num=%d \n", cfg->host, cfg->port, cfg->connections, cfg->thread_num);
    thread *threads = (thread*)malloc(sizeof(thread) * cfg->thread_num);
    uint64_t connections = cfg->connections / cfg->thread_num;

    for (uint64_t i = 0; i < cfg->thread_num; i++) {
        thread *t = &threads[i];
        t->connections = connections;
        t->host        = cfg->host;
        t->port        = cfg->port;
        t->fds = malloc(sizeof(int*) * t->connections);
        if (pthread_create(&t->thread, NULL, &thread_main, t)) {
            char *msg = strerror(errno);
            printf("unable to create thread %"PRIu64": %s\n", i, msg);
            exit(EXIT_FAILURE);
        }
    }

    for (uint64_t i = 0; i < cfg->thread_num; i++) {
        thread *t = &threads[i];
        printf("thread %"PRIu64": from %"PRIu64" to %"PRIu64" taken time %"PRIu64" ms\n", i, t->start_time, t->end_time, t->end_time - t->start_time);
    }

    sleep(20);
    for (uint64_t i = 0; i < cfg->thread_num; i++) {
        thread *t = &threads[i];
        for (uint64_t j = 0; j < t->connections; j++) {
            close(t->fds[j]);
        }
    }

    for (uint64_t i = 0; i < cfg->thread_num; i++) {
        thread *t = &threads[i];
        pthread_join(t->thread, NULL);
    }
    free(cfg);
    free(threads);
    return 0;
}
