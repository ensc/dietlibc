#include <grp.h>

extern struct group __group_pw;
extern char __group_buf[1000];

struct group *getgrent(void) {
  struct group* tmp;
  if (getgrent_r(&__group_pw,__group_buf,sizeof(__group_buf),&tmp)==0)
    return tmp;
  return 0;
}
