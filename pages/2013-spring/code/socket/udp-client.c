/*
 * A simple UDP echo client
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define SERV_PORT 12000
#define MAXLEN    2048
#define SERV_NAME "polaris"
//#define SERV_NAME "134.82.56.104"

int main(int argc, char*argv[]) {

   int s;                   /* socket descriptor                */
   int len;                 /* length of received data          */
   struct sockaddr_in  sa;  /* socket addr. structure, server   */
   struct sockaddr_in  cl;  /* socket addr. structure, client   */
   struct addrinfo *ptrh;
   char msg[MAXLEN+1];
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
    * Get server host information
    */
   getaddrinfo(SERV_NAME, NULL, NULL, &ptrh);

   /*
    * Set up socket address data for the server
    */
   memset((char *)&sa, 0, sizeof(sa));
   memcpy(&sa, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
   sa.sin_family      = AF_INET;
   sa.sin_port        = htons(SERV_PORT);

   /*
    * Set up socket address data for the client
    */
   memset((char *)&cl, 0, sizeof(cl));
   cl.sin_family      = AF_INET;
   cl.sin_addr.s_addr = htonl(INADDR_ANY);
   /* port 0 allows the system to assign any available port */
   cl.sin_port        = htons(0);

   /*
    * Bind the socket to the client
    */
   if (bind(s, (struct sockaddr *)(&cl), sizeof cl) < 0) {
     perror("UDP client bind error.");
     exit(1);
   }

   /*
    * Send the message to the server and receive the echo back
    */
   printf("Type a lower case message : ");
   fgets(msg, MAXLEN, stdin);
   msg_len = strlen(msg);

   sendto(s, msg, msg_len, 0, (struct sockaddr *)&sa, sizeof(sa));
   msg_len = recvfrom(s, msg, MAXLEN, 0, NULL, NULL);
   msg[msg_len] = 0;
   printf(msg);

   return 0;
}

