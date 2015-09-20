objects = list.o main.o

main : $(objects)
	cc -o main $(objects)

main.o : list.h main.c
	  cc -c -g main.c

list.o : list.h list.c
	  cc -c -g list.c

clean:
	 trash main $(objects)


