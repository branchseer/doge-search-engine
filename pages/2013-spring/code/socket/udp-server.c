/*
 * A simple UDP echo server
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define SERV_PORT 12000
#define MAXLEN    2048

int main(int argc, char*argv[]) {

   int s;                   /* socket descriptor                */
   int len;                 /* length of received data          */
   struct sockaddr_in  sa;  /* socket addr. structure, server   */
   struct sockaddr_in  cl;  /* socket addr. structure, client   */
   char msg[MAXLEN];
   int  msg_len;
   int  i;

   /*
    * Allocate an open socket.
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
     perror("UDP socket error.");
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
     perror("UDP bind error.");
     exit(1);
   }

   /*
    * Wait for client message and echo the message back
    */
   while (1) {
     len = sizeof(struct sockaddr_in);
     msg_len = recvfrom(s, msg, MAXLEN, 0, (struct sockaddr *)&cl, &len);
     for (i = 0; i < msg_len; i ++)
       msg[i] = toupper(msg[i]);
     sendto(s, msg, msg_len, 0, (struct sockaddr *)&cl, len);
   }

   return 0;
}

