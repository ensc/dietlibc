#include <string.h>
#include <netdb.h>

extern int getprotobyname_r(const char* name,
			    struct protoent *res, char *buf, size_t buflen,
			    struct protoent **res_sig) {
  int _res=-1;
  while (!getprotoent_r(res,buf,buflen,res_sig)) {
    int i;
    if (!strcmp(res->p_name,name)) { _res=0; break; }
    for (i=0; res->p_aliases[i]; ++i)
      if (!strcmp(res->p_aliases[i],name)) { _res=0; goto found; /* C sucks */ }
  }
found:
  endprotoent();
  return _res;
}
