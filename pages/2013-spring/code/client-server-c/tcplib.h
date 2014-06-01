#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>

#define BACKLOG          5          /* # of requests to queue              */
#define MAXHOSTNAME      32         /* maximum host name length            */

/* error codes */
#define NOHOST   -1   /* host does not exist */
#define SOCKFAIL -2   /* socket creation failed */
#define CONNFAIL -3   /* connection failed */
#define BINDFAIL -4   /* binding failed */
#define ACPTFAIL -5   /* accepting connection failed */

#ifndef ON_DEF
#define ON_DEF
extern int ON;    /* turn certain socket option on and off */
extern int OFF;
#endif

int socketClient(char *host, int port);
int socketServer(int port);
int acceptConn(int s);
