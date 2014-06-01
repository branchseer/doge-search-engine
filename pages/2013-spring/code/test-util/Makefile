TCPLIB = .
#CC     = gcc -std=c99 #g++
CC     = gcc
LIB    = #-lsocket -lnsl  # uncomment the line if solaris
G      = -g
INCL   = 
TESTO  = testutil.o stringutil.o htmllinks.o list.o url.o http.o
EXE    = test

all: $(EXE)
list.o: list.c list.h
	$(CC) $G -c list.c
stringutil.o: stringutil.c stringutil.h
	$(CC) $G -c stringutil.c
htmllinks.o: htmllinks.c htmllinks.h
	$(CC) $G -c htmllinks.c
url.o: url.c url.h
	$(CC) $G -c url.c
http.o: http.c http.h stringutil.h
	$(CC) $G -c http.c
testutil.o: testutil.c stringutil.h list.h htmllinks.h url.h
	$(CC) $G -c testutil.c
test:   $(TESTO)
	$(CC) -o test $(TESTO) $(LIB)

clean:
	/bin/rm -f *.o core
realclean:
	/bin/rm -f *.o core $(EXE) *~
