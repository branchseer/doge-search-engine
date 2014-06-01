/* send_eof.c - send_eof */

/*------------------------------------------------------------------------
 * send_eof - signal EOF by shutting down send side of connection
 *------------------------------------------------------------------------
 */
int
send_eof(int conn)
{
	return shutdown(conn, 1);
}
