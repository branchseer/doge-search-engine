#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  int v;
  char * name;
  struct node_t * next;
};

struct list_t {
  struct node_t * head;
  struct node_t * tail;
  struct node_t * cur;
};

typedef enum {false, true} bool;

void   list_insert(struct list_t * listptr, char *value);
char * list_remove(struct list_t * listptr);
struct node_t * list_remove_destroy(struct list_t * listptr);
bool   list_is_empty(struct list_t list);
bool   list_has_next(struct list_t list);
char * list_get_next(struct list_t * listptr);

int main(int argc, char* argv[]) {

  char *words[] = { "hello", "world", "how", "are", "you", NULL };
  int i = 0;
  char *aword;
  struct list_t list;

  printf("-- testing insertion ...\n");
  for (aword = words[0]; aword != NULL; i ++) {
    aword = words[i];
    if (aword != NULL) {
      printf(" inserting %s\n", aword);
      list_insert(&list, aword);
    }
  }

  printf("-- testing has_next ...\n");
  while (list_has_next(list) == true) {
    printf("value %s\n", list_get_next(&list));
  }
  /**
  printf("-- testing remove ...\n");
  while (list_is_empty(list) == false) {
    printf("value %s\n", list_remove(&list));
  }
  **/

  printf("-- testing remove destroy...\n");
  while (list_is_empty(list) == false) {
    struct node_t * retval = list_remove_destroy(&list);
    printf("value %s\n", retval->name);
  }

  return 0;
}

void   list_insert(struct list_t * listptr, char *value) {

  struct node_t * newnode = (struct node_t *)malloc(sizeof(struct node_t));
  newnode->next = NULL;
  newnode->name = (char*)malloc(strlen(value) + 1);
  newnode->name = value;
  //  strcpy(newnode->name, value);

  if (list_is_empty(*listptr) == true) {
    listptr->head = newnode;
    listptr->tail = newnode;
    listptr->cur  = listptr->head;
  } else {
    listptr->tail->next = newnode;
    listptr->tail = newnode;
  }
}

bool    list_has_next(struct list_t list) {

  if (list.cur != NULL)
    return true;
  else
    return false;
}

bool    list_is_empty(struct list_t list) {

  if (list.head == NULL) 
    return true;
  else
    return false;
}

char * list_get_next(struct list_t * listptr) {

  char * retval = listptr->cur->name;
  listptr->cur = listptr->cur->next;
  return retval;
}

char * list_remove(struct list_t * listptr) {

  struct node_t * toremove = listptr->head;
  char * retval = listptr->head->name;
  listptr->head = listptr->head->next;
  free(toremove);
  return retval;
}

struct node_t * list_remove_destroy(struct list_t * listptr) {

  struct node_t * toremove = listptr->head;
  listptr->head = listptr->head->next;
  free(toremove);
  return toremove;
}
