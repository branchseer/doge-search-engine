/*
 * client.c - main 
 */

#include <stdio.h>
#include "tcplib.h"
#define ECHOPORT     2500 /* a non-privilege port so all can use */
#define MAX          512

int main(int argc, char *argv[])   {

  int s;                   /* socket descriptor                */
  int len;                 /* length of received data          */
  char buf[MAX];    /* buffer to read whois info        */
  char * message = "hello network world!\n";

  if (argc != 2)   {
    fprintf(stderr,"usage %s hostname\n",argv[0]);
    exit(1);
  }
  s = socketClient(argv[1], ECHOPORT);
  /* 
   * Send a message
   */
  len = strlen(message);
  if (write(s, message, len) != len)  {
    perror("write");
    exit(1);
  }

  /*
   * Read the reply and put to user's output
   */
  len = read(s,buf,MAX);
  write(1,buf,len);

  close(s);
  return 0;
}
