#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

int main() {
  struct in_addr bar;
  struct hostent *foo;
//  foo=gethostbyname("zeit.fu-berlin.de");
  foo=gethostbyname("knuth");
  if (foo)
    printf("%s -> %s\n",foo->h_name,inet_ntoa(*(struct in_addr*)foo->h_addr));
/*  printf("%g %g\n",1e-10,1e10); */
}
