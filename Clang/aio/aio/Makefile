objects = epoll.o aio_fun.o aio_rwfun.o

epoll : $(objects)
	cc -o epoll $(objects)

epoll.o:aio_rwfun.h
	  cc -c -g epoll.c 

aio_rwfun.o:aio_rwfun.c
			cc -c -g aio_rwfun.c 

aio_fun.o:aio_fundef.h aio_comdef.h
		  cc -c -g aio_fun.c

clean:
	 rm epoll $(objects)
