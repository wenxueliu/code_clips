/*************************************************************************
	> File Name: aio_rwfundef.h
	> Author: keyaming
	> Mail: keyamingcuc@gmail.com 
	> Created Time: 2013年11月30日 星期六 19时03分45秒
 ************************************************************************/
#ifndef AIO_RWFUNDEF_H
#define AIO_RWFUNDEF_H
#include"aio_comdef.h"
struct afdata{
    struct iocb **piocb;
	struct iocb *iocb;
	aio_context_t *ctx; /*iocb context*/
	int fd; /*file descrption*/
	int afd;/*eventfd*/
};

#endif/*AIO_RWFUNDEF_H*/
