#include <grp.h>
#include <string.h>

extern struct group __group_pw;
extern char __group_buf[1000];

struct group *getgrgid(gid_t gid) {
  struct group *tmp;
  if (getgrgid_r(gid,&__group_pw,__group_buf,sizeof(__group_buf),&tmp)==0)
    return tmp;
  return 0;
}
