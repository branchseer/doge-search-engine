/* webclient.c */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tcplib.h"

#define BUFFSIZE	256
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
	char	buff[BUFFSIZE];
	int	len;

	if (argc < 3 || argc > 4) {
		(void) fprintf(stderr, "%s%s%s", "usage: ", argv[0], 
			       " <compane> <path> [appnum]\n");
		exit(1);
	}

	/* convert arguments to binary computer and appnum */

	host = argv[1];

	if (argc == 4) 
		app = atoi(argv[3]);
	else
	        app = HTTPPORT;
	
	/* contact the web server */

	conn = socketClient(host, app);
	if (conn < 0) 
		exit(1);

	/* send an HTTP/1.0 request to the webserver */

	len = sprintf(buff, "GET %s HTTP/1.0\r\n\r\n", argv[2]);
	(void) send(conn, buff, len, 0);

	/* dump all data received from the server to stdout */

	while((len = recv(conn, buff, BUFFSIZE, 0)) > 0)
		(void) write(1, buff, len);

	return 0;
}
