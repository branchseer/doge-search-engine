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
 *
 * Modified by: X. Meng (xmeng at bucknell.edu)
 * Extract and analyze Transport Layer packets only (TCP and UDP)
 * Spring 2013 for CSCI 363
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <fcntl.h>   /* needed for open(2) */

#include "trsprt_header.h"

int main(int argc, char *argv[]){

  //  process_tcp("tcppkt_headers.dat");
  process_udp("udppkt_headers-dhcp-umass.dat");

  return(0);
}

//************************************************************************

void process_tcp(char * infile_name) {

  struct tcphdr tcp_header;
  int size_read;
  int count = 0;      // packet count
  int tcpf = open(infile_name, O_RDONLY);

  if (tcpf < 0) {
    fprintf(stderr, "tcp file open error...\n");
    exit(1);
  }
  /* process tcp */
  size_read = read(tcpf, &tcp_header, sizeof(struct tcphdr));
  while (size_read > 0) {
    printf(" === tcp packet count %d === \n", count);
    parse_tcp_header(&tcp_header);
    count ++;
    printf("\n");
    
    size_read = read(tcpf, &tcp_header, sizeof(struct tcphdr));
  }
  close(tcpf);
}

//************************************************************************

void process_udp(char * infile_name) {

  struct udphdr udp_header;
  int size_read;
  int count = 0;      // packet count
  int udpf = open(infile_name, O_RDONLY);

  if (udpf < 0) {
    fprintf(stderr, "udp file open error...\n");
    exit(1);
  }

  /* process udp */
  size_read = read(udpf, &udp_header, sizeof(struct udphdr));
  while (size_read > 0) {

    printf(" === packet count %d === \n", count);
    parse_udp_header(&udp_header);
    count ++;
    printf("\n");
    size_read = read(udpf, &udp_header, sizeof(struct udphdr));
  }
  close(udpf);
}

//************************************************************************

char *parse_udp_header(struct udphdr *uhdr) { // see /usr/include/netinet/udp.h

  printf("UDP packet ->\n");
  printf("    source port %u\n", ntohs(uhdr->source));
  printf("    destination port %u\n", ntohs(uhdr->dest));
  printf("    packet length %u\n", ntohs(uhdr->len));
  printf("    check-sum 0x%0x\n", ntohs(uhdr->check));
  
  return NULL;
}

//************************************************************************

char *parse_tcp_header(struct tcphdr *thdr) { // see /usr/include/netinet/tcp.h

  printf("TCP packet ->\n");
  printf("    source port %u\n", ntohs(thdr->source));
  printf("    destination port %u\n", ntohs(thdr->dest));
  printf("    sequence number %u\n", ntohl(thdr->seq));
  printf("    acknowledgement number %u\n", ntohl(thdr->ack_seq));
  printf("    acknowledgement true/false %u\n", thdr->ack);

  return NULL;
}
