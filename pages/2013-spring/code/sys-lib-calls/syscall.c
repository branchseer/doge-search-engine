#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>  // for exit(3)
#include <stdio.h>
#include <unistd.h>

#define BUFFSIZE 512

int main(int argc, char* argv[]) {

  int  f = open("syscall.c", O_RDONLY);
  char buf[BUFFSIZE + 1];
  int  read_size = 0;
  int  stdout = 1;

  if (f < 0) {
    perror("Open failed -- ");
    exit(1);     // abnormal exit
  } else {
    read_size = read(f, buf, BUFFSIZE);
    while (read_size > 0) {
      buf[read_size] = 0;   // terminate the buffer
      write(stdout, buf, read_size);   // print it to the screen
      read_size = read(f, buf, BUFFSIZE);
    }  // end of while
  }  // end of if-else
  close(f);
  return 0;
}
