CC       = gcc
HDRS     = # add your header files here when needed
OBJS     = libcall.o syscall.o strmem.o linkedlist.o
CFLAGS   = -Wall -g
EXECS    = libcall syscall strmem linkedlist

LFLAGS   = # add your loader flags when needed

all: $(EXECS)

# The following line will compile all *.c code to corresponding *.o file
%.o: %.c # $(HDRS)
	$(CC) -c $(CFLAGS) $<

libcall: libcall.o
	$(CC) -o $@ libcall.o $(LFLAGS)

syscall: syscall.o
	$(CC) -o $@ syscall.o $(LFLAGS)

strmem: strmem.o
	$(CC) -o $@ strmem.o $(LFLAGS)

linkedlist: linkedlist.o
	$(CC) -o $@ linkedlist.o $(LFLAGS)
clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
