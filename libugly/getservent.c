#include <netdb.h>

extern struct servent __servent_pw;
extern char __servent_buf[1000];

struct servent *getservent(void) {
  struct servent* tmp;
  if (getservent_r(&__servent_pw,__servent_buf,sizeof(__servent_buf),&tmp)==0)
    return tmp;
  return 0;
}
