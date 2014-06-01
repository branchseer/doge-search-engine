/*
 * tcplib.c: this file contains necessary functions to establish TCP
 * connection using socket interface under Solaris.
 * Xiannong Meng
 * apr-22-1999
 * modified dec-15-2000, make socket reusable, call setsockopt();
 */

#include "tcplib.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * global constants 
 */
int ON = 1;
int OFF = -1;

/*
 * int socketClient(char *host, int port)
 *  socketClient returns to the calling function a socket descriptor, it
 * returns a negative value if failed.
 *  host : Internet host name of the remote server
 *  port : the port number on which the server is listening
 * return code:
 *   -1  remote host does not exist
 *   -2  socket creation failed
 *   -3  remote host connection failed
 */
int socketClient(char *host, int port)
{
   int s;                   /* socket descriptor                */
   int len;                 /* length of received data          */
   struct sockaddr_in  sa;  /* socket addr. structure           */
   struct hostent * hp;     /* host entry                       */
   struct servent * sp;     /* service entry                    */

   /* 
    * Look up the specified hostname
    */
   if ((hp = gethostbyname(host)) == NULL)
       return NOHOST;  /* host does not exist? */
  
   /*
    * Put host's address and address type into socket structure
    */
   bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
   sa.sin_family = hp->h_addrtype;
   
   /*
    * Put the whois socket number into the socket structure.
    */
   sa.sin_port = htons(port);
   bzero(&(sa.sin_zero), 8);  /* initialize */

   /*
    * Allocate an open socket.
    */
   if ((s = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0)
       return SOCKFAIL;

   /* set socket option to be reusable */
   if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void*)&ON, sizeof(int)) < 0)
     return SOCKFAIL;

   /*
    * Connect to the remote server.
    */
   if (connect(s,(struct sockaddr *)(&sa),sizeof(struct sockaddr)) < 0)
       return CONNFAIL;
   
   return s;
}


/*
 * int socketServer(int port)
 * socketServer returns to the calling function a socket descriptor, it
 * returns a negative value if failed.
 *  port : the port number on which the server is listening
 * return code:
 *   -1  host does not exist
 *   -2  socket creation failed
 */
int socketServer(int port)
{
   int s;                   /* socket descriptor                */
   struct sockaddr_in  sa;  /* socket addr. structure           */
   struct hostent * hp;     /* host entry                       */
   struct servent * sp;     /* service entry                    */
   char localhost[MAXHOSTNAME+1];   /* local host name as character string */


   /*
    * Get our own host information
    */
   gethostname(localhost,MAXHOSTNAME);
   if ((hp = gethostbyname(localhost)) == NULL)
       return NOHOST;
   
   sa.sin_port = htons(port);
   bcopy((char *)hp->h_addr,(char *)&sa.sin_addr,hp->h_length);
   sa.sin_family = hp->h_addrtype;

   /*
    * The following line is added by X. Meng
    * without it, bind() fails.
    */
   sa.sin_addr.s_addr = htonl(INADDR_ANY);

   /*
    * Allocate an open socket for the incoming connections
    */
   if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)
       return SOCKFAIL;

   /* set socket option to be reusable */
   if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void*)&ON, sizeof(int)) < 0)
     return SOCKFAIL;

   /*
    * Bind the socket to the service port so
    * we can hear incoming connection
    */
   if (bind(s,(struct sockaddr *)(&sa),sizeof sa) < 0)
	return BINDFAIL;

    /*
     * Set maximum connections we will fall behind
     */
   listen(s, BACKLOG);
   
   return s;
}


/*
 * int acceptConn(int s)
 *  accepts connection from a client and returns a socket ready for
 *  communications.
 */
int acceptConn(int s)
{
   struct sockaddr_in sa;      /* internet socket structure  */
   int i, t;

   i = sizeof sa;
   /*
    * We hang in accept() while waiting for new customers
    */
   if ((t = accept(s,(struct sockaddr *)(&sa),(socklen_t*)&i)) < 0)
       return ACPTFAIL;
   return t;
}


