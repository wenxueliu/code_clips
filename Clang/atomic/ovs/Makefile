objects = main.o

main : $(objects)
	cc -o main $(objects)

main.o : ovs-atomic.h main.c
	  cc -c -g main.c

clean:
	 trash main $(objects)


