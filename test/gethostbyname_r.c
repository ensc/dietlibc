#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

int main() {
  struct in_addr bar;

  struct hostent host,*res;
  char buf[2048];
  int fnord;
  if (gethostbyname_r("knuth",&host,buf,2048,&res,&fnord))
    return 2;
  printf("%s -> %s\n",res->h_name,inet_ntoa(*(struct in_addr*)res->h_addr));
}
