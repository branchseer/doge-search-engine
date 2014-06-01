/* readln.c - readln, recvln */

#include <stdio.h>

/*------------------------------------------------------------------------
 * readln - read from stdin until newline or EOF, or buffer is full.
 * Flush to newline or EOF and return on full buffer. Returns data length.
 *------------------------------------------------------------------------
 */
int
readln(char *buff, int buffsz)
{
	char	*bp = buff, c;
	int	n;

	while(bp - buff < buffsz && 
	      (n = read(0, bp, 1)) > 0) {
		if (*bp++ == '\n')
			return (bp - buff);
	}

	if (n < 0)
		return -1;

	if (bp - buff == buffsz)
		while (read(0, &c, 1) > 0 && c != '\n');

	return (bp - buff);
}

/*------------------------------------------------------------------------
 * recvln - recv from socket until newline or EOF is encountered
 * Flush to newline or EOF and return on full buffer. Returns data length.
 *------------------------------------------------------------------------
 */
int
recvln(int conn, char *buff, int buffsz)
{
	char	*bp = buff, c;
	int	n;

	while(bp - buff < buffsz && 
	      (n = recv(conn, bp, 1, 0)) > 0) {
		if (*bp++ == '\n')
			return (bp - buff);
	}

	if (n < 0)
		return -1;

	if (bp - buff == buffsz)
		while (recv(conn, &c, 1, 0) > 0 && c != '\n');

	return (bp - buff);
}
