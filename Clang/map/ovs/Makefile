object = main.o hmap.o

ALL: main

main: $(object)
	cc -o main $(object)

main.o : main.c
	cc -g -c main.c

hmap.o: hmap.h hmap.c
	cc -g -c hmap.c

clean:
	trash $(object) main

