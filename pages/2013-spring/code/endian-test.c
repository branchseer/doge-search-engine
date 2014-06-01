#include <stdio.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

int am_big_endian()
{
  // from
  // http://unixpapa.com/incnote/byteorder.html
  // 03/23/2013
  long one = 1;
  return !(*((char *)(&one)));
}

int am_little_endian()
{
  long one = 1;
  return !(*((char *)(&one)));
}

void endian_test_by_blinn() {
  // from
  // http://www.bruceblinn.com/linuxinfo/ByteOrder.html
  // 03/23/2013
  union {
    int i;
    char c[sizeof(int)];
  } foo;

  foo.i = 1;
  if (foo.c[0] == 1)
    printf("Little endian\n");
  else
    printf("Big endian\n");
}

int main(int argc, char*argv[]) {

    printf("Endian of this host : %u\n", __BYTE_ORDER);
    printf("Big Endian : %u\n", __BIG_ENDIAN);
    printf("Little Endian : %u\n", __LITTLE_ENDIAN);

    printf("Use am_big_endia() function %u\n", am_big_endian());

    endian_test_by_blinn();

    unsigned short a = 1; // 0000 0000 0000 0001 or 0x 00 01
                          // ntohs(a) makes it to be
                          // 0000 0001 0000 0000 or 256
    printf("u_short = %u  ntohs(a) = %u htons(a) = %u ntohs(a) shift right by 8 = %u\n", a, ntohs(a), htons(a), (ntohs(a)>>8));

    unsigned short b = 32; // 0000 0000 0010 0000 or 0x 00 a0
                           // ntohs(b) makes it to be
                           // 0010 0000 0000 0000 or 0x a0 00, or 8196 2^13
    printf("u_short = %u  ntohs(b) = %u htons(b) = %u ntohs(b) shift right by 8 = %u\n", b, ntohs(b), htons(b), (ntohs(b)>>8));

    unsigned short c = 3;  // 0000 0000 0000 0011 or 0x 00 03
                           // ntohs(c) makes it to be
                           // 0000 0011 0000 0000 or 0x 03 00, or 768 2^8+2^9
    printf("u_short = %u  ntohs(c) = %u htons(c) = %u ntohs(c) shift right by 8 = %u\n", c, ntohs(c), htons(c), (ntohs(c) >> 8));

    return 0;
}
