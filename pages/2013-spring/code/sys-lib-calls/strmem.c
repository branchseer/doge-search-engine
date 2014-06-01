#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

  char * src = "hello";
  char * dst = (char*)malloc(strlen(src));
  strcpy(dst, src);
  strncpy(dst, src, strlen(src));

  return 0;
}
