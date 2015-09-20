object = hash.o main.o murmur3.o reallocarray.o

ALL: main

main: $(object)
	cc -o main $(object)

main.o : hash.h main.c
	cc -g -c main.c

hash.o: hash.h murmur3.h reallocarray.h hash.c
	cc -g -c hash.c

murmur3.o: murmur3.h murmur3.c
	cc -g -c murmur3.c

reallocarray.o: reallocarray.h reallocarray.c
	cc -g -c reallocarray.c

clean:
	trash $(object) main
