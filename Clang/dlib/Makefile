CC = gcc
CPPFLAGS = -g

all: main libtick.so

main: main.o 
	$(CC) -o $@ $< -ldl -lpthread

libtick.so: tick.c
	$(CC) $(CPPFLAGS) -o $@ -shared -fPIC $+

clean :
	rm -f main *.so *.o core.*

