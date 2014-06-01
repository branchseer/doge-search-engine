/* webclient.c */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tcplib.h"

#define BUFFSIZE	512
#define HTTPPORT        80

/*-----------------------------------------------------------------------
 *
 * Program: webclient
 * Purpose: fetch page from webserver and dump to stdout with headers
 * Usage:   webclient <compname> <path> [appnum]
 * Note:    Appnum is optional. If not specified the standard www appnum
 *          (80) is used.
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	int	app;
	int	conn;
	char    * host;
	char    * path;
	char	buff[BUFFSIZE];
	int	len;

	if (argc < 3 || argc > 4) {
		(void) fprintf(stderr, "%s%s%s", "usage: ", argv[0], 
			       " <compane> <path> [appnum]\n");
		exit(1);
	}

	/* convert arguments to binary computer and appnum */

	host = argv[1];
	path = argv[2];

	if (argc == 4) 
		app = atoi(argv[3]);
	else
	        app = HTTPPORT;
	
	/* contact the web server */

	conn = socketClient(host, app);
	if (conn < 0) 
		exit(1);

	/* send an HTTP/1.1 request to the webserver */

	sprintf(buff, "GET %s HTTP/1.1\r\nHost: %s\r\n", path, host);
	strcat(buff, "User-Agent: self-module\r\n");
	strcat(buff, "Accept: text/html,applicaiton/xhtml+xml\r\n");
	strcat(buff, "Accept-Language: en-us,en;q=0.5\r\n");
	//	strcat(buff, "Accept-Encoding: gzip,defalte\r\n");
	strcat(buff, "Accept-Charset: ISO-8859-1,utf-8;q=0.7\r\n");
	strcat(buff, "Keep-Alive: 115\r\n");
	strcat(buff, "Connection: keep-alive\r\n");
	strcat(buff, "\r\n");

	len = strlen(buff);
	(void) send(conn, buff, len, 0);

	/* dump all data received from the server to stdout */

	while((len = recv(conn, buff, BUFFSIZE, 0)) > 0)
		(void) write(1, buff, len);

	return 0;
}
