#include <string.h>
#include <grp.h>

int getgrgid_r(gid_t gid,
	       struct group *res, char *buf, size_t buflen,
	       struct group **res_sig) {
  int _res=-1;
  while (!getgrent_r(res,buf,buflen,res_sig)) {
    if (gid==res->gr_gid) {
      _res=0;
      break;
    }
  }
  endgrent();
  return _res;
}
