#include <grp.h>
#include <sys/types.h>
#include <string.h>

int initgroups(const char *user, gid_t group) {
  int n=1;
  gid_t grouplist[32];
  struct group *g;
  grouplist[0]=group;
  setgrent();
  while ((g=getgrent())) {
    char **duh=g->gr_mem;
    while (*duh) {
      if (!strcmp(*duh,user)) {
	grouplist[n]=g->gr_gid;
	if (++n>=32)
	  break;
      }
      duh++;
    }
  }
  return setgroups(n,grouplist);
}

