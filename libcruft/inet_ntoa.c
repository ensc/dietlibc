#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int i2a(char* dest,unsigned int x) {
  register unsigned int tmp=x;
  register int len=0;
  if (x>=100) { *dest++=tmp/100+'0'; tmp=tmp%100; ++len; }
  if (x>=10) { *dest++=tmp/10+'0'; tmp=tmp%10; ++len; }
  *dest++=tmp+'0';
  return len+1;
}

char *inet_ntoa(struct in_addr in) {
  static char buf[20];
  int len;
  unsigned char *ip=(unsigned char*)&in;
  len=i2a(buf,ip[0]); buf[len]='.';
  len+=i2a(buf+ ++len,ip[1]); buf[len]='.';
  len+=i2a(buf+ ++len,ip[2]); buf[len]='.';
  len+=i2a(buf+ ++len,ip[3]); buf[len]=0;
  return buf;
}
