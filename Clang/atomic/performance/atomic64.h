#ifndef __HI_ATOMIC64_H__
#define __HI_ATOMIC64_H__
#include <stdio.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>



/* Learn from kernel */
#ifdef __x86_64__

#define LOCK_PREFIX "lock ;"
typedef struct {
        long long counter;
} atomic64_t;

/**
 * atomic64_read - read atomic64 variable
 * @v: pointer of type atomic64_t
 *
 * Atomically reads the value of @v.
 * Doesn't imply a read memory barrier.
 */
static inline long atomic64_read(const atomic64_t *v)
{
        return (*(volatile long *)&(v)->counter);
}

/**
 * atomic64_set - set atomic64 variable
 * @v: pointer to type atomic64_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
static inline void atomic64_set(atomic64_t *v, long i)
{
        v->counter = i;
}

/**
 * atomic64_add - add integer to atomic64 variable
 * @i: integer value to add
 * @v: pointer to type atomic64_t
 *
 * Atomically adds @i to @v.
 */
static inline void atomic64_add(long i, atomic64_t *v)
{
        asm volatile(LOCK_PREFIX "addq %1,%0"
                     : "=m" (v->counter)
                     : "er" (i), "m" (v->counter));
}

/**
 * atomic64_sub - subtract the atomic64 variable
 * @i: integer value to subtract
 * @v: pointer to type atomic64_t
 *
 * Atomically subtracts @i from @v.
 */
static inline void atomic64_sub(long i, atomic64_t *v)
{
        asm volatile(LOCK_PREFIX "subq %1,%0"
                     : "=m" (v->counter)
                     : "er" (i), "m" (v->counter));
}
#else /* __x86_64__ */
/*FIXME:
 * This program will run on x86_64 machine in the expected future, we
 * do _not_ need to care other cpu architecture.
 */
#endif

#endif
