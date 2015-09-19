CFLAGS = -g
ALL: echo client

echoServer = echoServer.o ae.o anet.o zmalloc.o
cliObj = ae.o net.o zmalloc.o client.o

echo : $(echoServer)
	cc -o echo -lpthread $(echoServer)

client: $(cliObj)
	cc -o client -lpthread $(cliObj)

echoServer.o : echoServer.c anet.h ae.h
	cc -c -g echoServer.c

client.o : client.c ae.h net.h
	cc -c -g client.c 

ae.o : ae.c ae.h
	cc -c -g ae.c

zmalloc.o : zmalloc.c zmalloc.h
	cc -c -g zmalloc.c

anet.o : anet.h anet.c
	cc -c -g anet.c

net.o : net.h net.c
	cc -c -g net.c

clean:
	@rm *.o
	@rm echo client

