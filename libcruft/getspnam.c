#include <shadow.h>
#include <string.h>
#include <stdlib.h>

struct spwd *getspnam(const char * name) {
  struct spwd *tmp;
  setspent();
  for (;;) {
    tmp=getspent();
    if (!tmp) return 0;
    if (!strcmp(tmp->sp_namp,name))
      return tmp;
  }
}
