#ifndef URL_H
#define URL_H
#include <strings.h>
#include "list.h"

struct URL {
  char* thisProt;    /* protocol */
  char* thisHost;    /* host */
  char* thisPath;    /* path */
  char* thisFile;    /* file */
  int   thisPort;    /* port number */
};

char* url_genURL(struct URL);
char* url_connectURL(char * prot, char * host, char * path, 
		     char * file, int port);
struct URL * url_buildURL( char* prot, char* host, 
		       char* path, char* file, int port);

char* url_getProt(struct URL aurl);
char* url_getHost(struct URL aurl);
char* url_getPath(struct URL aurl);
char* url_getFile(struct URL aurl);
int   url_getPort(struct URL aurl);

char* url_extractProtocol(char * inurl);
struct URL * url_extractURL(char * inurl);
char* url_fixURL(char* inUrl, char* baseUrl);
char* url_convertRelPath(char *path, struct URL * url);
int url_getsize(struct URL);
struct Node * url_makeNode(struct URL);

#endif
