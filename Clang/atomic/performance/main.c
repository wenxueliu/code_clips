/*******************************************************************************

  Copyright(c) 2008-2014

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".


  Date: 2014-04-15 21:27:57 CST

  Contact Information:
  Tony <tingw.liu@gmail.com>
  Home, Qingdao, China.

  From  http://blog.chinaunix.net/uid-20662820-id-4211959.html
*******************************************************************************/

#include "atomic64.h"

atomic64_t num;
long mutexnum = 0;
long maxnum;

struct timeval tv;
long starttime;

//FIXME: gettimeofday is a non-thread safe sysycall
static pthread_mutex_t timelock = PTHREAD_MUTEX_INITIALIZER;
#define TIME_LOCK() pthread_mutex_lock(&timelock)
#define TIME_UNLOCK() pthread_mutex_unlock(&timelock)

static pthread_mutex_t numlock = PTHREAD_MUTEX_INITIALIZER;
#define MUTEX_LOCK() pthread_mutex_lock(&numlock)
#define MUTEX_UNLOCK() pthread_mutex_unlock(&numlock)

static pthread_rwlock_t rwnumlock = PTHREAD_RWLOCK_INITIALIZER;
#define RW_LOCK() pthread_rwlock_wrlock(&rwnumlock)
#define RW_UNLOCK() pthread_rwlock_unlock(&rwnumlock);

static void *add_func(void *arg) {
    long stoptime;
    while(1) {
        atomic64_add(1, &num);
        if (atomic64_read(&num) > maxnum) {
            TIME_LOCK();
            gettimeofday(&tv, 0);
            TIME_UNLOCK();
            stoptime = (long)tv.tv_sec * (long)1000000 + (long)tv.tv_usec;
            printf("Used %ld microseconds\n", stoptime - starttime);
            break;
        }
    }
}


static void *add_func_rwlock(void *arg) {
    long stoptime;
    while(1) {
        RW_LOCK();
        ++mutexnum;
        if (mutexnum > maxnum) {
            RW_UNLOCK();
            TIME_LOCK();
            gettimeofday(&tv, 0);
            TIME_UNLOCK();
            stoptime= (long)tv.tv_sec * (long)1000000 + (long)tv.tv_usec;
            printf("Used %ld microseconds\n", stoptime - starttime);
            break;
        }
        RW_UNLOCK();
    }
}

static void *add_func_mutex(void *arg)
{
    long stoptime;
    while(1) {
        MUTEX_LOCK();
        ++mutexnum;
        if (mutexnum > maxnum) {
            MUTEX_UNLOCK();
            TIME_LOCK();
            gettimeofday(&tv, 0);
            TIME_UNLOCK();
            stoptime= (long)tv.tv_sec * (long)1000000 + (long)tv.tv_usec;
            printf("Used %ld microseconds\n", stoptime - starttime);
            break;
        }
        MUTEX_UNLOCK();
    }
}

#define ATOMIC_TYPE 0
#define MUTEX_TYPE 1
#define RW_TYPE 2
int main(int argc, char **argv)
{
    int threadnum, i, type;
    if (argc != 4) {
        printf("Usage: %s threadnum maxnum type[0-atomic, 1-mutex, 2-rwlock]\n", argv[0]);
        exit(0);
    }
    threadnum = atoi(argv[1]);
    maxnum = atoll(argv[2]);
    type = atoi(argv[3]);
    printf("Use %d threads add num from 0 to %ld\n", threadnum, maxnum);
    gettimeofday(&tv, 0);
    starttime= (long)tv.tv_sec * (long)1000000 + (long)tv.tv_usec;

    atomic64_set(&num, 0);

    pthread_t thread;
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    //pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    for (i = 0; i < threadnum; i++) {
        if (type == ATOMIC_TYPE) {
            pthread_create(&thread, &thread_attr, add_func, 0);
        } else if (type == MUTEX_TYPE) {
            pthread_create(&thread, &thread_attr, add_func_mutex, 0);
        } else if (type == RW_TYPE) {
            pthread_create(&thread, &thread_attr, add_func_rwlock, 0);
        }
    }
    pthread_join(thread, NULL);
    //while(1) {
    //    sleep(10);
    //}
}
