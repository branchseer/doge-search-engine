#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool list_isEmpty(struct List thisList) {

  return (thisList.head == NULL);
}

struct Node * list_contains(struct List thisList, struct Node anode) {

  if (thisList.head == NULL)
    return NULL;

  bool found = false;
  struct Node * work = thisList.head;
  while (found == false && work != NULL) {
    if (list_isNodeSame(anode, *work) == true)
      found = true;
    else
      work = work->next;
  }
  return work;
}

bool list_isNodeSame(struct Node a, struct Node b) {
  
  bool same = true;
  if (strcmp(a.var, b.var) != 0)
    same = false;
  else if (strcmp(a.val, b.val) != 0)
    same = false;
  else if (a.num != b.num)
    same = false;

  return same;
}

void list_addList(struct List *result, struct List *toadd) {
  
  struct Node * thisNode;
  struct Node * current = toadd->head;
  while (current != NULL) {
    thisNode = list_makeNode( current->var, current->val, current->num);
    list_add(result, thisNode);
    current = current->next;
  }
}

/**
 * Add the url in the 'currentNode' to the "pointedby" list.
 * The 'currentNode' is the web page URL that contains a list of URLs
 * ('pointeBy'), thus, each in 'pointedBy' is pointed by 'currentNode'.
 *
 */
void list_updateLinks(struct List * pointTo,
		      struct Node * currentNode, 
		      struct List * pointedBy) {

  struct Node *work = pointedBy->head;
  while (work != NULL) {
    struct Node *at = list_contains(*pointTo, *work);
    if (at == NULL)  { // '*work' node not in the list
      struct Node *newNode = list_copyNode(work);
      list_add(pointTo, newNode);
      at = pointTo->tail;    // 'at' should be the newly added node
    }
    if (at->links == NULL) {
      at->links = (struct List *)malloc(sizeof(struct List));
      list_init(at->links);
    }
    struct Node *newNode = list_copyNode(currentNode);
    list_add(at->links, newNode);
    
    work = work->next;
  } // end of while
}

struct Node * list_remove(struct List *thisList) {
  
  struct Node *anode = thisList->head;

  if (thisList->head != NULL)
    thisList->head = thisList->head->next;

  if (anode != NULL)
    anode->next = NULL;
  return anode;
}

void list_init(struct List * mylist) {
  mylist->head = mylist->tail = NULL;
}

/**
 * Make a new node with given information
 */
struct Node * list_makeNode(char *var, char *val, int n) {

  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  newNode->var = (char*)malloc(strlen(var) + 1);
  strcpy(newNode->var, var);
  newNode->val = (char*)malloc(strlen(val) + 1);
  strcpy(newNode->val, val);
  newNode->num = n;
  newNode->next = NULL;
  newNode->links = NULL;

  return newNode;
}

struct List * list_copyList(struct List *src) {

  if (src == NULL)
    return NULL;

  struct List *newList = (struct List *)malloc(sizeof(struct List));
  struct Node *work = src->head;
  while (work != NULL) {
    struct Node *newNode = list_copyNode(work);
    list_add(newList, newNode);
    work = work->next;
  }

  return newList;
}

struct Node * list_copyNode(struct Node *src) {

  if (src == NULL)
    return NULL;

  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  newNode->var = (char*)malloc(strlen(src->var) + 1);
  strcpy(newNode->var, src->var);
  newNode->val = (char*)malloc(strlen(src->val) + 1);
  strcpy(newNode->val, src->val);
  newNode->num = src->num;
  newNode->next = NULL;
  newNode->links = NULL;

  return newNode;
}

/**
 * Add a new node to the list
 */
//void list_add(struct List *thisList, char *var, char* val) {
void list_add(struct List *thisList, struct Node * newNode) {
  
  if (thisList->head == NULL) {  // new list
    thisList->head = newNode;
    thisList->tail = newNode;
  } else {   // list already exist
    thisList->tail->next = newNode;
    thisList->tail = newNode;
  }
};
    
char* list_get(struct List thisList, char *var) {

  struct Node * current = thisList.head;
  bool found = false;
  while (current != NULL && !found) {
    if (strcmp(current->var, var) == 0) {
      found = true;
    } else {
      current = current->next;
    }
  }  /* end of while */
  if (current != NULL) { // found it!
    return current->val;
  } else {
    return "";
  }

};

int list_len(struct List thisList)   {

  struct Node * current = thisList.head;
  bool found = false;
  int c = 0;
  while (current != NULL) {
    c ++;
    current = current->next;
  }  /* end of while */
  return c;
};

struct Node * list_getNth(struct List thisList, int loc)  {
  /* get nth node of the list */
  struct Node * current = thisList.head;
  int c = 0;
  while (current != NULL && c < loc) {
    c ++;
    current = current->next;
  }  /* end of while */

  if (c == loc)
    return current;
  else
    return NULL;
  
};

void list_traverse(struct List thisList) {

  struct Node * current = thisList.head;
  printf("%s\n", "var/val pairs ...");
  while (current != NULL) {
    printf("[%s|-|%s] ", current->var, current->val);
    current = current->next;
  }  /* end of while */
  printf("\n");
};

