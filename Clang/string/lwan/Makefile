objects = strbuf.o main.o

main : $(objects)
	cc -o main $(objects)

main.o : strbuf.h main.c
	  cc -c -g main.c

strbuf.o : strbuf.h strbuf.c
	  cc -c -g strbuf.c

clean:
	 trash main $(objects)


