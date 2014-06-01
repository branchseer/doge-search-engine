/* webserver.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "tcplib.h"

#define SOLARIS

#if defined(LINUX) || defined(SOLARIS)
#include <sys/time.h>
#endif

#define BUFFSIZE	256
#define SERVER_NAME	"polaris.eg.bucknell.edu"

#define ERROR_400	"<html><head></head><body><h1>Error 400</h1><p>Th\
e server couldn't understand your request.</body></html>\n"

#define ERROR_404	"<html><head></head><body><h1>Error 404</h1><p>Do\
cument not found.</body></html>\n"

#define HOME_PAGE	"<html><head></head><body><h1>Welcome to the CNAI\
 Demo Server</h1><p>Why not visit: <ul><li><a href=\"http://netbook.cs.pu\
rdue.edu\">Netbook Home Page</a><li><a href=\"http://www.comerbooks.com\"\
>Comer Books Home Page<a></ul></body></html>\n"

#define TIME_PAGE	"<html><head></head><body><h1>The current date is\
: %s</h1></body></html>\n"

int	recvln(int, char *, int);
void	send_head(int, int, int);

/*-----------------------------------------------------------------------
 *
 * Program: webserver
 * Purpose: serve hard-coded webpages to web clients
 * Usage:   webserver <appnum>
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{

        int 	        conn;
	int             sock;
	int             port;
	int		n;
	char		buff[BUFFSIZE], cmd[16], path[64], vers[16];
	char		*timestr;
#if defined(LINUX) || defined(SOLARIS)
	struct timeval	tv;
#elif defined(WIN32)
	time_t          tv;
#endif

	if (argc != 2) {
		(void) fprintf(stderr, "usage: %s <appnum>\n", argv[0]);
		exit(1);
	}
	
	port = atoi(argv[1]);
	sock = socketServer(port);

	while(1) {

		/* wait for contact from a client on specified appnum */

		conn = acceptConn(sock);
		if (conn < 0)
			exit(1);

		/* read and parse the request line */

		n = recvln(conn, buff, BUFFSIZE);
		buff[n] = 0;
		fprintf(stderr,"cmd from browser: %s\n", buff);
		sscanf(buff, "%s %s %s", cmd, path, vers);

		/* skip all headers - read until we get \r\n alone */

		while((n = recvln(conn, buff, BUFFSIZE)) > 0) {
		     buff[n] = 0;
		     fprintf(stderr,"cmd from browser: %s\n", buff);
			if (n == 2 && buff[0] == '\r' && buff[1] == '\n')
				break;
		}

		/* check for unexpected end of file */

		if (n < 1) {
			(void) send_eof(conn);
			continue;
		}
		
		/* check for a request that we cannot understand */

		if (strcmp(cmd, "GET") || (strcmp(vers, "HTTP/1.0") &&
					   strcmp(vers, "HTTP/1.1"))) {
			send_head(conn, 400, strlen(ERROR_400));
			(void) send(conn, ERROR_400, strlen(ERROR_400),0);
			(void) send_eof(conn);
			continue;
		}

		/* send the requested web page or a "not found" error */

		if (strcmp(path, "/") == 0) {
			send_head(conn, 200, strlen(HOME_PAGE));
			(void) send(conn, HOME_PAGE, strlen(HOME_PAGE),0);
		} else if (strcmp(path, "/time") == 0) {
#if defined(LINUX) || defined(SOLARIS)
			gettimeofday(&tv, NULL);
			timestr = ctime(&tv.tv_sec);
#elif defined(WIN32)
			time(&tv);
			timestr = ctime(&tv);
#endif
			(void) sprintf(buff, TIME_PAGE, timestr);
			send_head(conn, 200, strlen(buff));
			(void) send(conn, buff, strlen(buff), 0);
		} else { /* not found */
			send_head(conn, 404, strlen(ERROR_404));
			(void) send(conn, ERROR_404, strlen(ERROR_404),0);
		}
		(void) send_eof(conn);
	}
}

/*-----------------------------------------------------------------------
 * send_head - send an HTTP 1.0 header with given status and content-len
 *-----------------------------------------------------------------------
 */
void
send_head(int conn, int stat, int len)
{
	char	*statstr, buff[BUFFSIZE];

	/* convert the status code to a string */

	switch(stat) {
	case 200:
		statstr = "OK";
		break;
	case 400:
		statstr = "Bad Request";
		break;
	case 404:
		statstr = "Not Found";
		break;
	default:
		statstr = "Unknown";
		break;
	}
	
	/*
	 * send an HTTP/1.0 response with Server, Content-Length,
	 * and Content-Type headers.
	 */

	(void) sprintf(buff, "HTTP/1.0 %d %s\r\n", stat, statstr);
	(void) send(conn, buff, strlen(buff), 0);

	/** try out cookie! **/
/**
	(void) sprintf(buff, "Set-Cookie: new=value; version=1; ");
	(void) send(conn, buff, strlen(buff), 0);
	(void) sprintf(buff, "expires=Wed, 01-December-2004 16:00:00 GMT; ");
	(void) send(conn, buff, strlen(buff), 0);
	(void) sprintf(buff, "path=/time; domain=.eg.bucknell.edu\n");
	(void) send(conn, buff, strlen(buff), 0);
**/
/*
	(void) sprintf(buff, "Server: %s", SERVER_NAME);
	(void) send(conn, buff, strlen(buff), 0);
*/
	(void) sprintf(buff, "Content-Length: %d", len);
	(void) send(conn, buff, strlen(buff), 0);

	(void) sprintf(buff, "Content-Type: text/html\n");
	(void) send(conn, buff, strlen(buff), 0);

	(void) sprintf(buff, "\n");
	(void) send(conn, buff, strlen(buff), 0);
}
