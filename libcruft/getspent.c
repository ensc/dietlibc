#include <shadow.h>

static struct spwd pw;
static char buf[1000];

struct spwd *getspent(void) {
  struct spwd* tmp;
  if (getspent_r(&pw,buf,sizeof(buf),&tmp)==0)
    return tmp;
  return 0;
}
