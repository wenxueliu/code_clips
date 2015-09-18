objects = dynamic-string.o main.o

main : $(objects)
	cc -o main $(objects)

main.o : dynamic-string.h main.c
	  cc -c -g main.c

dynamic-string.o : dynamic-string.h dynamic-string.c
	  cc -c -g dynamic-string.c

clean:
	 trash main $(objects)

