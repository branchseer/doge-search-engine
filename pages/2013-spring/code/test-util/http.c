#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "http.h"
#include "stringutil.h"

/**
 * Extract http response information from the returning page
 */
struct HttpHead * http_retrieveHead( char * page, char* host ) {

  int len = strlen(page);
  if (len == 0)
    return NULL;

  struct HttpHead * localHead = (struct HttpHead *)malloc(sizeof(struct HttpHead));

  char* lines = (char*)malloc(len + 1);
  char* end = strstr(page, "\n\n");
  if (end == NULL)
    end = strstr(page, "\r\n\r\n");
  if (end == NULL)
    return NULL;

  len = (end - page);
  strncpy(lines, page, len);
  lines[len] = 0; // terminate the string
  
  // now that we have the header portion, let's extract the 
  // response code and the date
  lines = lowerString(lines);
  char * first = strstr(lines, "http");
  if (first == NULL)
    return NULL;  // something gone wrong!

  // extract the response code
  char * value = (char*)malloc(4);
  strncpy(value, (first+9), 3);  // try to copy the digits
  value[3] = 0;
  localHead->code = atoi(value);
  if (localHead->code < 100 || localHead->code > 600)
    localHead->code = BAD_REQ;

  // copy the host
  localHead->host = (char*)malloc(strlen(host) + 1);
  strcpy(localHead->host, host);

  // extract the date
  first = strstr(lines, "date:");
  localHead->date = (char*)malloc(32);
  if (first == NULL)
    strcpy(localHead->date, "None");
  else {
    strncpy(localHead->date, (first+6), 30);
    localHead->date[30] = 0;
    localHead->date = str_trim(localHead->date);
    localHead->date = upperString(localHead->date);
  }

  return localHead;
}
