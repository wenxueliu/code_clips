/*
 * INSPIRE BY: http://www.alexonlinux.com/pthread-mutex-vs-pthread-spinlock
 *
 * NOTE:
 * 1. mutex better than spinlock with setting thread affinity
 * 2. spinlock better than mutex without setting thread affinity
 *
 * $ lscpu
 * Architecture:          x86_64
 * CPU op-mode(s):        32-bit, 64-bit
 * Byte Order:            Little Endian
 * CPU(s):                4
 * On-line CPU(s) list:   0-3
 * Thread(s) per core:    2
 * Core(s) per socket:    2
 * Socket(s):             1
 * NUMA node(s):          1
 * Vendor ID:             GenuineIntel
 * CPU family:            6
 * Model:                 37
 * Stepping:              2
 * CPU MHz:               1199.000
 * BogoMIPS:              4787.97
 * Virtualisation:        VT-x
 * L1d cache:             32K
 * L1i cache:             32K
 * L2 cache:              256K
 * L3 cache:              3072K
 * NUMA node0 CPU(s):     0-3
 *
 * $ g++ -Wall -pthread main.cc
 * $ ./a.out 1
 * Consumer TID 7424
 * Result - 0.808538
 *
 * $ ./a.out 2
 * Consumer TID 7211
 * Consumer TID 7212
 * Result - 2.655054
 *
 * $ ./a.out 4
 * Consumer TID 7252
 * Consumer TID 7251
 * Consumer TID 7253
 * Consumer TID 7254
 * Result - 7.430128
 *
 * $ ./a.out 8
 * Consumer TID 7350
 * Consumer TID 7353
 * Consumer TID 7351
 * Consumer TID 7352
 * Consumer TID 7349
 * Consumer TID 7355
 * Consumer TID 7356
 * Consumer TID 7354
 * Result - 6.776495
 *
 * $ g++ -DUSE_THREAD_AFFINITY -Wall -pthread main.cc
 *
 * $ ./a.out 1
 * Consumer TID 5387
 * Result - 0.817537
 *
 * $ ./a.out 2
 * Consumer TID 5417
 * Consumer TID 5418
 * Result - 2.863826
 *
 * $ ./a.out 4
 * Consumer TID 5457
 * Consumer TID 5458
 * Consumer TID 5460
 * Consumer TID 5459
 * Result - 2.348186
 *
 * $ ./a.out 8
 * Consumer TID 5510
 * Consumer TID 5515
 * Consumer TID 5516
 * Consumer TID 5517
 * Consumer TID 5511
 * Consumer TID 5512
 * Consumer TID 5513
 * Consumer TID 5514
 * Result - 2.379262
 *
 * $ g++ -DUSE_SPINLOCK -Wall -pthread main.cc
 *
 * ./a.out 1
 * Consumer TID 8243
 * Result - 0.756641
 *
 * $ ./a.out 2
 * Consumer TID 8282
 * Consumer TID 8283
 * Result - 1.918107
 *
 * $ ./a.out 4
 * Consumer TID 8313
 * Consumer TID 8314
 * Consumer TID 8315
 * Consumer TID 8316
 * Result - 2.597788
 *
 * $ ./a.out 8
 * Consumer TID 8369
 * Consumer TID 8367
 * Consumer TID 8366
 * Consumer TID 8368
 * Consumer TID 8365
 * Consumer TID 8370
 * Consumer TID 8371
 * Consumer TID 8372
 * Result - 5.510854
 *
 * $ g++ -DUSE_SPINLOCK -DUSE_THREAD_AFFINITY -Wall -pthread main.cc
 *
 * $ ./a.out 1
 * Consumer TID 6215
 * Result - 0.729303
 *
 * $ ./a.out 2
 * Consumer TID 6403
 * Consumer TID 6404
 * Result - 1.059514
 *
 * $ ./a.out 4
 * Consumer TID 6560
 * Consumer TID 6561
 * Consumer TID 6562
 * Consumer TID 6563
 * Result - 2.158060
 *
 * $ ./a.out 8
 * Consumer TID 6700
 * Consumer TID 6699
 * Consumer TID 6701
 * Consumer TID 6702
 * Consumer TID 6703
 * Consumer TID 6704
 * Consumer TID 6705
 * Consumer TID 6706
 * Result - 4.213965
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <limits.h>
#include <string.h>

#include <list>

#define LOOPS 20000000

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;

void thread_set_cpu_affinity();
void thread_get_cpu_affinity();
pid_t gettid();

typedef struct {
    pid_t     id;
    pthread_t thread;
    int       times;
} thread;

pid_t gettid() { return syscall( __NR_gettid ); }

void thread_set_cpu_affinity(int cores) {
    pthread_t thread ;
    thread = pthread_self();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(gettid()%cores, &cpuset);
    int ret = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (ret != 0) {
        handle_error_en(ret, "pthread_getaffinity_np");
    }
}

void thread_get_cpu_affinity() {
    pthread_t thread ;
    thread = pthread_self();
    cpu_set_t cpuset;
	int ret = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (ret != 0) {
        handle_error_en(ret, "pthread_getaffinity_np");
    }
	for (int cpu_index = 0; cpu_index < CPU_SETSIZE; cpu_index++) {
	    if (CPU_ISSET(cpu_index, &cpuset)) {
		    printf("thread:%d cpu:%d\n", gettid(), cpu_index);
        }
    }
}

list<int> the_list;

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex;
#endif


void *consumer(void *ptr) {
    thread *cur_thread = (thread *)ptr;
    int i;



#ifdef USE_THREAD_AFFINITY
    thread_set_cpu_affinity(4);
    thread_get_cpu_affinity();
#else
    printf("thread %lu\n", (unsigned long)gettid());
#endif

    while (1) {
#ifdef USE_SPINLOCK
        pthread_spin_lock(&spinlock);
#else
        pthread_mutex_lock(&mutex);
#endif

        if (the_list.empty()) {
#ifdef USE_SPINLOCK
            pthread_spin_unlock(&spinlock);
#else
            pthread_mutex_unlock(&mutex);
#endif
            break;
        }

        cur_thread->times++;
        cur_thread->id = gettid();
        i = the_list.front();
        the_list.pop_front();

#ifdef USE_SPINLOCK
        pthread_spin_unlock(&spinlock);
#else
        pthread_mutex_unlock(&mutex);
#endif
    }

    return NULL;
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

    int thread_num = 2;
    if (argc == 1) {
        thread_num = 2;
    } else if (argc == 2) {
        thread_num = get_vaild_int(argv[1]);
    } else {
        printf("Usage: %s [thread_num]", argv[0]);
    }

    thread *threads = (thread *)malloc(thread_num * sizeof(threads));
    pthread_t thrs[thread_num];
    struct timeval tv1, tv2;

#ifdef USE_SPINLOCK
    pthread_spin_init(&spinlock, 0);
#else
    pthread_mutex_init(&mutex, NULL);
#endif

    // Creating the list content...
    for (int i = 0; i < LOOPS; i++) {
        the_list.push_back(i);
    }

    // Measuring time before starting the threads...
    gettimeofday(&tv1, NULL);

    for (int t = 0; t < thread_num; t++) {
        threads->thread = thrs[t];
        threads->times  = 0;
        pthread_create(&thrs[t], NULL, consumer, &threads[t]);
    }

    for (int t = 0; t < thread_num; t++) {
        pthread_join(thrs[t], NULL);
    }

    // Measuring time after threads finished...
    gettimeofday(&tv2, NULL);

    if (tv1.tv_usec > tv2.tv_usec)
    {
        tv2.tv_sec--;
        tv2.tv_usec += 1000000;
    }
    printf("Result - %ld.%06ld\n", tv2.tv_sec - tv1.tv_sec,
        tv2.tv_usec - tv1.tv_usec);

    for (int t = 0; t < thread_num; t++) {
        printf("thread->id:%d, thread->times:%d\n", threads[t].id, threads[t].times);
    }

#ifdef USE_SPINLOCK
    pthread_spin_destroy(&spinlock);
#else
    pthread_mutex_destroy(&mutex);
#endif

    return 0;
}
