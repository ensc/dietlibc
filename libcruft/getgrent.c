#include <grp.h>

static struct group gr;
static char buf[1000];

struct group *getgrent(void) {
  struct group* tmp;
  if (getgrent_r(&gr,buf,sizeof(buf),&tmp)==0)
    return tmp;
  return 0;
}
