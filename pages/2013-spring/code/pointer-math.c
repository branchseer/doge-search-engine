/*
 * Demonstrate characteristics of pointer math
 *
 * Phil Stahlfeld
 *
 * Revised by Xiannong Meng
 * for CSCI 363
 */
#include <stdio.h>
#include <stdlib.h>

struct node_t {
  int  v;
  char c;
};

int main(){

  struct node_t * nodep = (struct node_t *)malloc(sizeof(struct node_t));
  int a = 5;
  int *b = &a;
  char *c = (char *) &a;
  printf("b is an integer pointer and c is a character pointer\n");
  printf("Size of integer: %u\n", sizeof(int));
  printf("Size of character: %u\n", sizeof(char));
  printf("Size of structure node_t : %u\n", sizeof(struct node_t));

  printf("    b: 0x%x\n", b);
  printf("b + 1: 0x%x\n", b + 1);

  printf("    c: 0x%x\n", c);
  printf("c + 1: 0x%x\n", c + 1);

  printf("    nodep : 0x%x\n", nodep);
  printf("nodep + 1: 0x%x\n", (nodep + 1));

  unsigned char *i = (unsigned char *)nodep;
  i = i + 1;
  printf("nodep + 1 as a char*: 0x%x\n", i);

  unsigned int *j = (unsigned int *)nodep;
  j = j + 1;
  printf("nodep + 1 as a int*: 0x%x\n", j);
  

  return 0;
}
