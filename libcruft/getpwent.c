#include <pwd.h>

static struct passwd pw;
static char buf[1000];

struct passwd *getpwent(void) {
  struct passwd* tmp;
  if (getpwent_r(&pw,buf,sizeof(buf),&tmp)==0)
    return tmp;
  return 0;
}
