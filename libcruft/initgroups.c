#include <grp.h>
#include <sys/types.h>
#include <string.h>

int initgroups(const char *user, gid_t group) {
  int n=0;
  gid_t grouplist[32];
  struct group *g;
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

