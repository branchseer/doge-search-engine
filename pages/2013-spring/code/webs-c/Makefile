HOME = /newhome/faculty/meng
#TCPLIB = $(HOME)/lib/C
TCPLIB = .
CC  = gcc
LIB = #-lsocket -lnsl
G   = -g
#INCL = $(HOME)/lib/C
INCL = 

all: webserver webclient
webserver: webserver.o readln.o tcplib.o send_eof.o
	$(CC) -o webserver webserver.o readln.o send_eof.o tcplib.o $(LIB)
webclient: webclient.o readln.o tcplib.o send_eof.o
	$(CC) -o webclient webclient.o readln.o send_eof.o tcplib.o $(LIB)
webserver.o: webserver.c
	$(CC) $G -c webserver.c
webclient.o: webclient.c
	$(CC) $G -c webclient.c
readln.o: readln.c
	$(CC) $G -c readln.c
send_eof.o: send_eof.c
	$(CC) $G -c send_eof.c
tcplib.o: tcplib.c
	$(CC) $G -c tcplib.c
clean:
	/bin/rm -f *.o core *~
realclean:
	/bin/rm -f *.o core *~ webserver webclient
