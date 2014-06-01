/*
 * Defines a list of structures, emulating the vectors from c++
 *
 */
#ifndef LIST_H
#define LIST_H

#define MAXSTRLEN 256    /* maximum string length */

typedef enum { false, true } bool;
struct List;

struct Node {
  char * var;
  char * val;
  int    num;
  struct Node * next;
  struct List * links;
};

struct List {
  struct Node * head, * tail;
};

//void list_add(struct List *thisList, char *var, char* val);
struct Node * list_makeNode(char* var, char* val, int n);
struct Node * list_copyNode(struct Node *src);
struct List * list_copyList(struct List *src);
void list_add(struct List *thisList, struct Node *anode);
char* list_get(struct List thisList, char *var);
int list_len(struct List thisList);
struct Node * list_getNth(struct List thisList, int loc);
void list_traverse(struct List thisList);
void list_init(struct List * thisList);
bool list_isEmpty(struct List thisList);
struct Node * list_contains(struct List thisList, struct Node anode);
bool list_isNodeSame(struct Node a, struct Node b);
struct Node * list_remove(struct List *thisList);
void list_addList(struct List *result, struct List *toadd); 
void list_updateLinks(struct List *visited, struct Node *current, 
		      struct List *pointedBy);

#endif    /* #ifdef LIST_H */
