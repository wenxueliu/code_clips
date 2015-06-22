/*************************************************************************
	> File Name: aio_fundef.h
	> Author: keyaming
	> Mail: keyamingcuc@gmail.com 
	> Created Time: 2013年11月30日 星期六 18时57分54秒
 ************************************************************************/


#ifndef AIO_FUNDEF_H
#define AIO_FUNDEF_H

#define _GNU_SOURCE
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

/*
 * This were good at the time of 2.6.21-rc5.mm4 ...
 */
#ifndef __NR_eventfd
#if defined(__x86_64__)
#define __NR_eventfd 284
#elif defined(__i386__)
#define __NR_eventfd 323
#else
#error Cannot detect your architecture!
#endif
#endif

#define BUILD_BUG_IF(c) ((void) sizeof(char[1 - 2 * !!(c)]))

#define TESTFILE_SIZE (4096 * 2)
#define IORTX_SIZE (1024 * 4)
#define NUM_EVENTS 128


typedef unsigned long aio_context_t;

enum {
	IOCB_CMD_PREAD = 0,
	IOCB_CMD_PWRITE = 1,
	IOCB_CMD_FSYNC = 2,
	IOCB_CMD_FDSYNC = 3,
	/* These two are experimental.
	 * IOCB_CMD_PREADX = 4,
	 * IOCB_CMD_POLL = 5,
	 */
	IOCB_CMD_NOOP = 6,
	IOCB_CMD_PREADV = 7,
	IOCB_CMD_PWRITEV = 8,
};

#if defined(__LITTLE_ENDIAN)
#define PADDED(x,y)	x, y
#elif defined(__BIG_ENDIAN)
#define PADDED(x,y)	y, x
#else
#error edit for your odd byteorder.
#endif

#define IOCB_FLAG_RESFD		(1 << 0)

/*
 * we always use a 64bit off_t when communicating
 * with userland.  its up to libraries to do the
 * proper padding and aio_error abstraction
 */
struct iocb {
	/* these are internal to the kernel/libc. */
	u_int64_t	aio_data;	/* data to be returned in event's data */
	u_int32_t	PADDED(aio_key, aio_reserved1);
	/* the kernel sets aio_key to the req # */

	/* common fields */
	u_int16_t	aio_lio_opcode;	/* see IOCB_CMD_ above */
	int16_t	aio_reqprio;
	u_int32_t	aio_fildes; /*file describe*/

	u_int64_t	aio_buf;
	u_int64_t	aio_nbytes;
	int64_t	aio_offset;

	/* extra parameters */
	u_int64_t	aio_reserved2;	/* TODO: use this for a (struct sigevent *) */

	u_int32_t	aio_flags;
	/*
	 * If different from 0, this is an eventfd to deliver AIO results to
	 */
	u_int32_t	aio_resfd;
}; /* 64 bytes */

struct io_event {
	u_int64_t           data;           /* the data field from the iocb */
	u_int64_t           obj;            /* what iocb this event came from */
	int64_t           res;            /* result code for this event */
	int64_t           res2;           /* secondary result */
};

#endif/*AIO_FUNDEF_H*/
