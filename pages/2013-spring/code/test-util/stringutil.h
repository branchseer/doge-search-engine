#ifndef STRINGUTIL_H
#define STRINGUTIL_H
#include <ctype.h>
#include "list.h"
int findloc(char * str, char c, int start);
int findloc_r(char * str, char c, int start);
int findStrLoc(char * str, char* pattern, int i);
int findStrLoc_r(char * str, char* pattern, int i);
struct List * string_split( char * );
char* lowerString(char * str);
char* upperString(char * str);
char* removeExtraSpace(char* src);
char* str_trim(char* src);
char* removeExtraSlash(char* inStr);
bool str_endWith(char * src, char * ext);
char* str_dup(char* src);

#endif
