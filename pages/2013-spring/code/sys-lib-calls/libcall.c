#include <stdio.h>
#include <stdlib.h>  // for exit(3)
#define BUFFSIZE 512

int main(int argc, char* argv[]) {

  char c;
  FILE * f = fopen("libcall.c", "r");

  if (f == NULL) {
    fprintf(stderr, "open failed.\n");
    exit(1);
  } else {
    fscanf(f, "%c", &c);
    while (!feof(f)) {
      fprintf(stdout, "%c", c);
      fscanf(f, "%c", &c);
    }  // end of while
  }  // end of if-else 
  fclose(f);
  return 0;
}
