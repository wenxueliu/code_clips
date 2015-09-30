
objects = thread_specific.o pthread_affinity.o
target = specific affinity

ALL: affinity specific

specific: thread_specific.o
	cc -o specific thread_specific.o -lpthread

thread_specific.o: thread_specific.c
	cc -c -g thread_specific.c -lpthread

affinity: pthread_affinity.o
	cc -o affinity pthread_affinity.o -lpthread

pthread_affinity.o: pthread_affinity.c
	cc -c -g pthread_affinity.c -lpthread

clean:
	 trash $(target) $(objects) *.log

