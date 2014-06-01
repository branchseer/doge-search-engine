# this is for a test chat-client/chat-server program
G  = -g      ## debug option
CC = gcc     ## compiler
SRCS = tcp-client.c tcp-server.c udp-client.c udp-server.c
EXES = tcp-client tcp-server udp-client udp-server 
OBJS = tcp-client.o tcp-server.o udp-client.o udp-server.o
CFLG = 
LFLG =
LIB = #-lsocket -lnsl -lpthread

all: $(EXES)

%.o: %.c
	$(CC) -c $G $(CFLG) $<

tcp-client: tcp-client.o
	$(CC) -o tcp-client tcp-client.o $(LIB) $(LFLG)

tcp-server: tcp-server.o
	$(CC) -o tcp-server tcp-server.o $(LIB) $(LFLG)

udp-client: udp-client.o
	$(CC) -o udp-client udp-client.o $(LIB) $(LFLG)

udp-server: udp-server.o
	$(CC) -o udp-server udp-server.o $(LIB) $(LFLG)

clean:
	/bin/rm -f core *.o $(EXES) *~
