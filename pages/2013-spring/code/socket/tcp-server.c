/*
 * A simple TCP echo server
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define SERV_PORT 12000
#define MAXLEN    2048
#define BACKLOG   5

int main(int argc,char *argv[])    {

  int s, t;
  int msg_len, i;
  char msg[MAXLEN];
  struct sockaddr_in  sa;  /* socket addr. structure           */
  struct sockaddr_in  cl;  /* socket addr. structure           */

   /*
    * Allocate an open socket.
    */
   if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("TCP socket error.");
     exit(1);
   }

   /*
    * Set up socket address data for the server
    */
   memset((char *)&sa, 0, sizeof(sa));
   sa.sin_family      = AF_INET;
   sa.sin_addr.s_addr = htonl(INADDR_ANY);
   sa.sin_port        = htons(SERV_PORT);

   /*
    * Bind the socket to the service
    */
   if (bind(s, (struct sockaddr *)(&sa), sizeof sa) < 0) {
     perror("TCP bind error.");
     exit(1);
   }

   /*
    * Set maximum connections we will fall behind
    */
   listen(s, BACKLOG);
   
   i = sizeof(cl);
   while (1)    {
     if (s > 0)
       t = accept(s,(struct sockaddr *)(&cl),(socklen_t*)&i);
     else  {
	perror("socket error\n");
	exit(1);
     }
     if (t > 0)  {
       msg_len = read(t, msg, MAXLEN);
       for (i = 0; i < msg_len; i ++)
	 msg[i] = toupper(msg[i]);
       write(t, msg, msg_len);
     }  else {
       perror(" connection error\n");
       exit(2);
     }
     close(t);
    } /* while(1) */

  close(s);
  return 0;
}
