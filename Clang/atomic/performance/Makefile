objects = main.o

main : $(objects)
	cc -o main $(objects) -lpthread

main.o : atomic64.h main.c
	  cc -c -g main.c -lpthread

clean:
	 trash main $(objects)


