/*
 * Copyright (c) 2012 Bucknell University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: L. Felipe Perrone (perrone@bucknell.edu)
 */

#ifndef __ETHER_TRACE_H__
#define __ETHER_TRACE_H__

#define int32 unsigned int
#define int16 unsigned short
#define uint32 unsigned int
#define uint16 unsigned short

#define MAXFRMLEN 2000   // ethernet frame size (actually 1500 suffices
#define BUFSIZE   256    // general buffer size

#define TCP_PROTO 6
#define UDP_PROTO 17

void process_tcp(char * infile_name);
void process_udp(char * infile_name);

char * parse_udp_header(struct udphdr *uh);
char * parse_tcp_header(struct tcphdr *th);
char * read_udp_data(int fid);
char * read_tcp_data(int fid);


 
#endif /* __ETHER_TRACE_H__ */
