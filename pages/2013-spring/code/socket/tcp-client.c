/*
 * A simple TCP echo client
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

int main(int argc, char *argv[])   {

  int s;                   /* socket descriptor                */
  int len;                 /* length of received data          */
  char msg[MAXLEN+1];      /* buffer to read info              */
  int  msg_len;
  struct sockaddr_in  sa;  /* socket addr. structure, server   */
  struct addrinfo *ptrh;


   /*
    * Allocate an open socket.
    */
   if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("TCP socket error.");
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
    * Connect to the remote server.
    */
   if (connect(s,(struct sockaddr *)(&sa),sizeof(struct sockaddr)) < 0) {
     perror("TCP connection error.");
     exit(1);
   }

  /* 
   * Send a message
   */
   /*
    * Send the message to the server and receive the echo back
    */
   printf("Type a lower case message : ");
   fgets(msg, MAXLEN, stdin);
   msg_len = strlen(msg);

   write(s, msg, msg_len);
   msg_len = read(s, msg, MAXLEN);
   msg[msg_len] = 0;
   printf(msg);

  close(s);
  return 0;
}
