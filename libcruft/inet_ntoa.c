#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern char *inet_ntoa_r(struct in_addr in,char* buf);

char *inet_ntoa(struct in_addr in) {
  static char buf[20];
  return inet_ntoa_r(in,buf);
}
