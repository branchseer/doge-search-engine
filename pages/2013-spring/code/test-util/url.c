#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "url.h"
#include "stringutil.h"

char* url_genURL(struct URL aurl) {
  /* return string form of a url */
  int len = strlen(aurl.thisProt) + strlen(aurl.thisHost) 
    + strlen(aurl.thisPath) + strlen(aurl.thisFile) + sizeof(int) + 12;
  char* completeURL = (char*)malloc(len);
  strcpy(completeURL, aurl.thisProt);
  strcat(completeURL, "://");
  strcat(completeURL, aurl.thisHost);
  if (aurl.thisPort != 80) {
    char*port = (char*)malloc(6);
    sprintf(port, "%d", aurl.thisPort);
    strcat(completeURL, ":");
    strcat(completeURL, port);
  }
  strcat(completeURL, "/");
  strcat(completeURL, aurl.thisPath);
  strcat(completeURL, "/");
  strcat(completeURL, aurl.thisFile);
  completeURL = removeExtraSlash(completeURL);
  return completeURL;
}

char* url_connectURL(char * prot, char * host, 
		     char * path, char * file, int port) {
  /* return string form of a url */
  int len = strlen(prot) + strlen(host) 
    + strlen(path) + strlen(file) + sizeof(int) + 12;
  char* completeURL = (char*)malloc(len);
  strcpy(completeURL, prot);
  strcat(completeURL, "://");
  strcat(completeURL, host);
  if (port != 80) {
    char*ports = (char*)malloc(6);
    sprintf(ports, "%d", port);
    strcat(completeURL, ":");
    strcat(completeURL, ports);
  }
  strcat(completeURL, "/");
  strcat(completeURL, path);
  strcat(completeURL, "/");
  strcat(completeURL, file);
  completeURL = removeExtraSlash(completeURL);
  return completeURL;
}

struct URL * url_buildURL( char* prot, char* host, 
		       char* path, char* file, int port) {
  /* build a URL object with given infor */
  struct URL * thisUrl = (struct URL *)malloc(sizeof(struct URL));

  int len = strlen(prot);
  thisUrl->thisProt = (char*)malloc(len + 1);
  strcpy(thisUrl->thisProt, prot);

  len = strlen(host);
  thisUrl->thisHost = (char*)malloc(len + 1);
  strcpy(thisUrl->thisHost, host);

  thisUrl->thisPath = (char*)malloc(MAXSTRLEN);
  strcpy(thisUrl->thisPath, path);

  thisUrl->thisFile = (char*)malloc(MAXSTRLEN);
  strcpy(thisUrl->thisFile, file);

  thisUrl->thisPort = port;

  return thisUrl;
}

char* url_getProt(struct URL aurl) {
  return aurl.thisProt;
}

char* url_getHost(struct URL aurl) {
  return aurl.thisHost;
}

char* url_getPath(struct URL aurl) {
  return aurl.thisPath;
}

char* url_getFile(struct URL aurl) {
  return aurl.thisFile;
}

int url_getPort(struct URL aurl) {
  return aurl.thisPort;
}

int url_getsize(struct URL aurl) {
  int total = 0;
  total = strlen(aurl.thisProt) + strlen(aurl.thisHost)
    + strlen(aurl.thisPath) + strlen(aurl.thisFile) + sizeof(int) + 1;
  return total;
}

char* url_fixURL(char* inUrl, char* baseUrl) {

  int maxlen = strlen(inUrl) + strlen(baseUrl) + 12; // may need "index.html"
  char* host = (char*)malloc(maxlen);
  char* path = (char*)malloc(maxlen);
  struct URL *thisUrl = (struct URL *)malloc(sizeof(struct URL));
  char * completeURL = (char*)malloc(maxlen);
  char * workBuf = (char*)malloc(maxlen);
  
  char* prot = url_extractProtocol(inUrl);

  //  printf("maxlen in url_fixURL: %d\n", maxlen);

  if (prot != NULL) { /* not a relative url */
    thisUrl = url_extractURL(inUrl);
  } else { /* this is a relative url */
    thisUrl = url_extractURL(baseUrl);
    strcpy(thisUrl->thisFile, inUrl);
  }

  /* by now, the protocol and host have been fixed */
  path = url_getPath(*thisUrl);
  if (inUrl[0] == '/')  // an absolute path
    strcpy(path, inUrl);
  else
    strcat(path, url_getFile(*thisUrl));

  /* 1. convert relative path */
  path = url_convertRelPath(path, thisUrl);  /* converting relative path */
  /* 2. remove sectional tags */
  char *tag = rindex(path, '#');
  if (tag != NULL) {
    strncpy(path, path, (tag - path));
    path[(tag-path)] = 0;
  }

  /* 'path' now is complete '/path/file.ext' */

  /* based on the URL information, reconstruct a complete URL */
  /* 3. build protocol and host */
  strcpy(completeURL, url_getProt(*thisUrl));
  strcat(completeURL, "://");
  strcat(completeURL, url_getHost(*thisUrl));

  /* 4. fix port */
  if (url_getPort(*thisUrl) != 80) {
    strcat(completeURL, ":");
    sprintf(workBuf, "%d", url_getPort(*thisUrl));
    strcat(completeURL, workBuf);
  }

  /* 5. attach the file/path */
  strcat(completeURL, path);
  /*
  int len = strlen(completeURL);
  if (completeURL[len-1] == '/') // ending with a path '/'
    strcat(completeURL, "index.html");
  */
  /* 6. remove extra '//' in the middle of a url */
  completeURL = removeExtraSlash(completeURL);

  return completeURL;
}

/**
 * Extract various pieces of information from a given url in its
 * string format.<br>
 *
 * @param inUrl String form of a given url.
 * 
 * Precondition:  the 'inUrl' is expected to have the form of
 * http://host.domain:port/dir<br>
 * Postcondition: an URL object is constructed and returned
 * from the given string format of url.<br>
 */
struct URL *url_extractURL(char* inUrl) {
  
  struct URL *result;
  int protLoc = findStrLoc(inUrl, "://", 0);
  int hostLoc = -1;
  int portLoc = -1;
  int pathLoc = -1;

  int len = strlen(inUrl);
  /* set up default values */
  char *localHost = (char*)malloc(MAXSTRLEN);
  char *localPath = (char*)malloc(MAXSTRLEN);
  char *localProt = (char*)malloc(MAXSTRLEN);
  char *localFile = (char*)malloc(MAXSTRLEN);
  int   localPort = 80;

  char *workBuf   = (char*)malloc(MAXSTRLEN);

  strcpy(localHost, "");
  strcpy(localPath, "/");
  strcpy(localProt, "http");
  strcpy(localFile, "/");

  if (protLoc >= 0)	{
    hostLoc = protLoc + 3;  /* skip over '://' */
    strncpy(localProt, inUrl, protLoc);
    localProt[protLoc] = 0;
  }  else
    hostLoc = 0;

  portLoc = findloc(inUrl, ':', hostLoc); /* find ':' after protocol */
  
  pathLoc = findloc(inUrl, '/', hostLoc); /* find where '/' is */

  if (portLoc >= 0 && pathLoc > portLoc && pathLoc <= portLoc + 6)  {
    strncpy(localHost, &(inUrl[hostLoc]), (portLoc-hostLoc));
    localHost[(portLoc-hostLoc)] = 0;
    strncpy(workBuf, &(inUrl[portLoc+1]), (pathLoc-portLoc-1));
    workBuf[(pathLoc-portLoc-1)] = 0;
    workBuf = str_trim(workBuf);
    localPort = atoi(workBuf);
  }  else  {
    if (pathLoc >= 0) {
      strncpy(localHost, &(inUrl[hostLoc]), (pathLoc-hostLoc));
      localHost[(pathLoc-hostLoc)] = 0;
    }  else
      strcpy(localHost, &(inUrl[hostLoc]));
  }

  if (pathLoc >= 0) {// need to remove the trailing file name, /in.html
    if (inUrl[len - 1] == '/')  {
      strcpy(localPath, &(inUrl[pathLoc]));
      strcpy(localFile, "/");
    }  else  {
      workBuf = rindex(inUrl, '/');
      len = (workBuf-&(inUrl[pathLoc]))+1;
      strncpy(localPath, &(inUrl[pathLoc]), len );
      localPath[len] = 0;
      strcpy(localFile, (workBuf+1));
    }
  }
	
  result = url_buildURL(localProt, localHost, localPath, localFile, localPort);
  return result;
}

char* url_extractProtocol(char* inUrl) {

  if (inUrl == NULL) return NULL;
  int len = strlen(inUrl);
  //  printf("strlen in extractProtocol %d\n", len);
  //  printf("inUrl in extractProtocol %s\n", inUrl);
  char* proto = (char*)malloc(strlen(inUrl)+1);
  int loc = findloc(inUrl, ':', 0);  /* look for the first ':' */
  if (loc > 0)  { /* the url includes a protocol */
    strncpy(proto, inUrl, loc);
    proto[loc] = 0;  /* terminate the string */
  } else {
    proto = NULL;
  }
  return proto;
}

char* url_convertRelPath(char * path, struct URL * thisUrl) {

  char * completePath = (char*)malloc(MAXSTRLEN);
  char * workBuf = (char*)malloc(MAXSTRLEN);

  if (path == NULL) return NULL;
  if (path[0] != '/') { /* a relative path */
    strcpy(completePath, url_getPath(*thisUrl));
    strcat(completePath, path);
  } else 
    strcpy(completePath, path);

  /* collapse all "/./" */
  int changed = 0;
  int i = findStrLoc(path, "/./", 0);
  while (i >= 0) {
    changed = 1;
    strncpy(workBuf, path, i);  /* copy the first part */
    workBuf[i] = 0;
    strcat(workBuf, &(path[i+2]));  /* then the second part */
    strcpy(path, workBuf);
    i = findStrLoc(path, "/./", 0);
  }

  /* collapse all "/../" */
  i = findStrLoc(path, "/../", 0);
  while (i >= 0) {
    changed = 1;
    int i2 =findloc_r(path, '/', i-1);
    if (i2 < 0) i2 = i;
    strncpy(workBuf, path, i2);  /* copy the first part */
    workBuf[i2] = 0;
    strcat(workBuf, &(path[i+3]));  /* then the second part */
    strcpy(path, workBuf);
    i = findStrLoc(path, "/../", 0);
  }

  if (changed == 1)
    strcpy(completePath, path);

  return completePath;
}

struct Node * url_makeNode(struct URL inUrl) {

  struct Node *retNode = (struct Node *)malloc(sizeof(struct Node));
  retNode->var = (char*)malloc(strlen(inUrl.thisHost) + 1);
  strcpy(retNode->var, inUrl.thisHost);
  retNode->val = (char*)malloc(strlen(inUrl.thisPath) + strlen(inUrl.thisFile) + 2);
  strcpy(retNode->val, inUrl.thisPath);
  strcat(retNode->val, inUrl.thisFile);
  retNode->num = inUrl.thisPort;

  return retNode;
}
