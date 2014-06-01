#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "stringutil.h"

int findloc(char * str, char c, int i) {
  /* find and return the location of 'c' in 'str' */

  while (str[i] != 0 && str[i] != c) {
    i ++;
  }
  if (str[i] == c)
    return i;
  else
    return -1;   /* not found */
}

int findloc_r(char * str, char c, int i) {
  /* find and return the location of 'c' in 'str' in reverse direction */

  while (i >= 0 && str[i] != 0 && str[i] != c) {
    i --;
  }
  if (i >= 0 && str[i] == c)
    return i;
  else
    return -1;   /* not found */
}

int findStrLoc(char * str, char* pattern, int i) {
  /* find and return the location of 'pattern' in 'str' */

  int len = strlen(pattern);
  int found = -1;

  while (str[i] != 0 && found == -1) {
    if (str[i] == pattern[0] && strncmp(&(str[i]), pattern, len) == 0)
      found = i;
    i ++;
  }
  return found;
}

int findStrLoc_r(char * str, char* pattern, int i) {
  /* find and return the location of 'pattern' in 'str' in reverse direction */

  int len = strlen(pattern);
  int found = -1;

  while (i >= 0 && str[i] != 0 && found == -1) {
    if (str[i] == pattern[0] && strncmp(&(str[i]), pattern, len) == 0)
      found = i;
    i --;
  }
  return found;
}

struct List * string_split( char * instr ) {
  /* split the input string 'instr' into a list of tokens */
  struct List * thislist = (struct List *)malloc(sizeof(struct List));
  struct Node * newNode;
  char *buf;
  int i = 0;
  int c = 0;
  int len = strlen(instr);

  buf  = (char*)malloc(len + 1);
  thislist->head = thislist->tail = NULL;
  while (i < len) {
    if (isdigit(instr[i]) || isalpha(instr[i]))
      buf[c++] = instr[i];
    else {
      buf[c] = 0;   /* terminate the buffer */
      struct Node *anode = list_makeNode(buf, buf, -1);
      list_add(thislist, anode);
      c = 0;        /* start a new buffer */
      /* skip over until next letter/digit */
      while (i < len && !isdigit(instr[i]) && !isalpha(instr[i]))
	i ++;
      if (i < len && i > 0) {
	buf[c++] = instr[i];
      }
    }  // end of 'else'
    i ++;
  }
  if (c > 0) {
    buf[c] = 0;   /* very last word */
    struct Node *anode = list_makeNode(buf, buf, -1);
    list_add(thislist, anode);
  }

  return thislist;
}

char* lowerString(char* instr) {
  /* convert all char in 'instr' to lower case */
  int len = strlen(instr);
  char * out = (char*)malloc(len + 1);
  int i;
  for (i = 0; i < len; i ++) {
    if (instr[i] <= 'Z' && instr[i] >= 'A')
      out[i] = (instr[i] - 'A') + 'a';
    else
      out[i] = instr[i];
  }
  return out;
}

char* upperString(char* instr) {
  /* convert all char in 'instr' to upper case */
  int len = strlen(instr);
  char * out = (char*)malloc(len + 1);
  int i;
  for (i = 0; i < len; i ++) {
    if (instr[i] <= 'z' && instr[i] >= 'a')
      out[i] = (instr[i] - 'a') + 'A';
    else
      out[i] = instr[i];
  }
  return out;
}

char* removeExtraSpace(char* src) {
  /* remove extra white space in the 'src' */
  int i = 0;
  int o = 0;
  int first = 1;
  int len = strlen(src);
  char* out = (char*)malloc(len + 1);
  while (i < len) {
    if (!isspace(src[i])) {
      out[o++] = src[i];
      first = 1;
    } else if (isspace(src[i]) && first == 1) {
      out[o++] = src[i];
      first = 0;
    }
    i ++;
  }
  return out;
}

char* str_trim(char * src) {
  /* remove leading and trailing spaces */
  if (src == NULL) return NULL;
  int len = strlen(src);
  char* result = (char*)malloc(len + 1);
  int i = 0;
  int r = 0;
  int b = len - 1;
  while (i < len && isspace(src[i]))
    i ++;
  while (b >= 0 && isspace(src[b]))
    b --;
  for (i; i <= b; i ++) {
    result[r] = src[i];
    r ++;
  }
  result[r] = 0; /* terminate the string */
  return result;
}

char* removeExtraSlash(char* inStr)   {

  int len = strlen(inStr);
  char* result = (char*)malloc(len + 1);
  int slashLoc = findStrLoc(inStr, "://", 0);
  int i;
  int c = 0;
  bool first = true;
  
  if (slashLoc == -1) // not existing
    slashLoc = 0;
  else {
    slashLoc = slashLoc + 3;
    strncpy(result, inStr, slashLoc);
    c = slashLoc;
  }
  for (i = slashLoc; i < len; i ++) {
    if (inStr[i] == '/' && first == true) {
      first = false;
      result[c++] = inStr[i];
    } else if (inStr[i] != '/') {
      result[c++] = inStr[i];
      first = true;
    }
  }
  result[c] = 0; // terminate the string
  return result;
  /**
  int l = strlen(inStr);
  int hold = slashLoc + 3;
  slashLoc = findStrLoc(inStr, "//", hold);
  while (slashLoc > 0 && slashLoc < l) {
      hold = slashLoc;
      while (hold < l && inStr[hold] == '/')
	hold ++;
      if (hold >= l)  {
	strncpy(inStr, inStr, slashLoc+1);
	inStr[slashLoc+1] = 0;
	break;
      } else {
	strncpy(inStr, inStr, slashLoc+1);
	inStr[slashLoc+1] = 0;
	strcat(inStr, &(inStr[hold]));
      }
      slashLoc = findStrLoc(inStr, "//", hold);
      l = strlen(inStr);
  }

  return inStr;
  **/
}

bool str_endWith(char * src, char * ext) {
  
  char * r = strstr(src, ext);
  if (r == NULL)
    return false;
  if (strcmp(r, ext) == 0)
    return true;
  else
    return false;
}

char* str_dup(char* src) {

  char* ret = (char*)malloc(strlen(src) + 1);
  strcpy(ret, src);
  return ret;
}
