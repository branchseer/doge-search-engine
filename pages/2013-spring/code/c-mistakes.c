/*234567890123456789
 * This program demonstrates some common C mistakes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFSIZE 128

struct node_t {
  int     i;
  char    w[32];
  double  d;
  struct node_t * next;
};

struct node_t2 {
  int     i;
  char    *w;
  double  d;
  struct node_t2 * next;
};

void off_by_one_1() {
  
  char * src = "hello";
  char * dst = (char*)malloc(strlen(src));

  // the 'dst' has one fewer byte needed
  strcpy(dst, src);

  // but C prints it happily anyway
  printf("source -> |%s|  destination -> |%s|\n", src, dst); 

  // use 'strncpy' has the same problem
  strncpy(dst, src, strlen(src));

  // but C prints it happily anyway
  printf("source -> |%s|  destination -> |%s|\n", src, dst); 
}

void off_by_one_2() {

  int    max = 16;
  char   buf[max];
  int    f;
  
  f = open("c-mistakes.c", O_RDONLY);
  int size_read = read(f, buf, max);
  if (size_read < max)
    buf[size_read] = 0; // terminate the string
  else
    buf[max] = 0;  // terminate the string

  printf("content read from file |%s|\n", buf);

  close(f);
}

void size_of() {

  struct node_t x, *y;

  printf("size of long    %d\n", sizeof(long));
  printf("size of int     %d\n", sizeof(int));
  printf("size of double  %d\n", sizeof(double));
  printf("size of float   %d\n", sizeof(float));
  printf("size of char    %d\n", sizeof(char));
  printf("size of float   %d\n", sizeof(float));
  printf("size of struct  %d\n", sizeof(struct node_t));
  printf("sizeof i %d + w %d + d %d + next %d = %d\n", sizeof(x.i), sizeof(x.w), sizeof(x.d), sizeof(x.next), sizeof(x.i) + sizeof(x.w) + sizeof(x.d) + sizeof(x.next));
  printf("size of struct* %d\n", sizeof(struct node_t *));
}

char * remove_in_use() {

  char * word = "hello world";
  struct node_t2 * head = (struct node_t2 *)malloc(sizeof(struct node_t2));
  head->w = (char*)malloc(strlen(word) + 1);
  strcpy(head->w, word);

  struct node_t2 * toremove = head;  
  char * retval = head->w;  
  head = head->next;  
  free(toremove);  
  return retval;
}

char * remv_half(char * in_buf) {
  
  int  len = strlen(in_buf);
  char *out_buf = (char*)malloc(len + 1);
  int  i;
  int  out = 0;
  for (i = 0; i < len; i ++) {
    out_buf[out] = in_buf[i];
    out ++;
    i ++;  // skip next char
  }
  out_buf[out] = 0; // terminate the string
  return out_buf;   // memory leak here
}

void memalloc() {

  char * buf = (char*) malloc(BUFFSIZE);

  printf("type a string (or 'quit') : ");
  scanf("%s", buf);
  while (strcmp(buf, "quit") != 0) {
    buf = remv_half(buf);
    printf("processed string: |%s|\n", buf);
    printf("type a string (or 'quit') : ");
    scanf("%s", buf);
  }
}

int main(int argc, char* argv[]) {

  off_by_one_1();
  off_by_one_2();
  size_of();
  char * val = remove_in_use();
  printf("returned value |%s|\n", val);
  memalloc();
  
  return 0;
}
