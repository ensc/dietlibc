#include <grp.h>
#include <string.h>
#include <stdlib.h>

struct group *getgrgid(gid_t gid) {
  struct group *tmp;
  setgrent();
  for (;;) {
    tmp=getgrent();
    if (!tmp) return 0;
    if (tmp->gr_gid == gid)
      return tmp;
  }
}
