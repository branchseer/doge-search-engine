#ifndef HTML_H
#define HTML_H

#include <stdio.h>
#include "list.h"

struct List * extractURLs( char* pageSrc, char* baseUrl, struct List *l );
void analyze(char* param, struct List *l, char* base );
void analyzeAnchor(char* anchor, struct List *l, char* base );
char* extract(char* line, char* key);
char* fixUrl(char* inUrl);

#endif



