#include <grp.h>
#include <string.h>
#include <stdlib.h>

struct group *getgrnam(const char* name) {
  struct group *tmp;
  setgrent();
  for (;;) {
    tmp=getgrent();
    if (!tmp) return 0;
    if (!strcmp(tmp->gr_name,name))
      return tmp;
  }
}
