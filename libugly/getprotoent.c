#include <netdb.h>

extern struct protoent __protoent_pw;
extern char __protoent_buf[1000];

struct protoent *getprotoent(void) {
  struct protoent* tmp;
  if (getprotoent_r(&__protoent_pw,__protoent_buf,sizeof(__protoent_buf),&tmp)==0)
    return tmp;
  return 0;
}
