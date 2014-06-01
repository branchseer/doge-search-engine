# this is for a test chat-client/chat-server program
G  = -g      ## debug option
CC = gcc     ## compiler
SRC = echo-client.c echo-server.c webclient.c webclient-http10.c
EXE = echo-client echo-server webclient webclient-http10
LIB = #-lsocket -lnsl -lpthread

all: echo-client echo-server webclient webclient-http10

echo-client: echo-client.o tcplib.o
	$(CC) -o echo-client echo-client.o tcplib.o $(LIB)

echo-client.o: echo-client.c
	$(CC) $G -c echo-client.c

echo-server: echo-server.o tcplib.o
	$(CC) -o echo-server echo-server.o tcplib.o $(LIB)

echo-server.o: echo-server.c
	$(CC) $G -c echo-server.c

webclient: webclient.o tcplib.o
	$(CC) -o webclient webclient.o tcplib.o

webclient-http10: webclient-http10.o tcplib.o
	$(CC) -o webclient-http10 webclient-http10.o tcplib.o

webclient.o: webclient.c tcplib.h
	$(CC) $G -c webclient.c

webclient-http10.o: webclient-http10.c tcplib.h
	$(CC) $G -c webclient-http10.c

tcplib.o: tcplib.c
	$(CC) $G -c tcplib.c

clean:
	/bin/rm -f core *.o $(EXE) *~
