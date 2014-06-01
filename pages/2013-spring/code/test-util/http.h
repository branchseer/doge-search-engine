#ifndef HTTP_H
#define HTTP_H

#define HTTP_OK 200    // okay
#define MOVEDPM 301    // moved
#define BAD_REQ 400    // bad request
#define UN_AUTH 401    // not authorized
#define FORBIDD 403    // forbidden
#define NOT_FOU 404    // not found

struct HttpHead {
  char * host;   // name of the host, e.g., 'www.xxx.com'
  int    code;   // response code, e.g., 200
  char * date;   // date accessed, e.g. Sun, 30 Dec 2012 19:59:45 GMT
};

struct HttpHead * http_retrieveHead( char* page, char *host );

#endif
