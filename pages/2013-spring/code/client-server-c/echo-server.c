/*
 * server.c - main
 */
#include <stdio.h>
#include "tcplib.h"
#define ECHOPORT        2500       /* a non-privilege port so all can use */
#define MAX             512


int main(int argc,char *argv[])    {

  int s, t;
  void doEcho(int);

  s = socketServer(ECHOPORT);
  while (1)    {
    if (s > 0)
      t = acceptConn(s);
    else	{
      fprintf(stderr," socket error\n");
      exit(1);
    }
    if (t > 0)  {
      doEcho(t);
    }  else {
      fprintf(stderr," connection error\n");
      exit(2);
    }
  } /* while(1) */
  close(s);
  return 0;
}

void doEcho(int t)   {

  char buf[MAX];
  char echo[MAX] = "Echo --> ";
  int size;
    
  if ((size = read(t, buf, MAX)) < 0)  {
    fprintf(stderr," read error\n");
    exit(3);
  }
  buf[size] = 0;
  strcat(echo, buf);
  size = strlen(echo);
  if (write(t, echo, size) < 0) {
    fprintf(stderr," write error\n");
    exit(4);
  }
  close(t);
}
