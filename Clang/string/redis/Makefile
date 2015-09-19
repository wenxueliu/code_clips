objects = sds.o main.o zmalloc.o

main : $(objects)
	cc -o main $(objects)

main.o : sds.h main.c
	  cc -c -g main.c

sds.o : sds.h sds.c
	  cc -c -g sds.c

zmalloc.o : zmalloc.h zmalloc.c
	  cc -c -g zmalloc.c

clean:
	 trash main $(objects)


