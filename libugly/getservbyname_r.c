#include <string.h>
#include <netdb.h>

extern int getservbyname_r(const char* name,const char* proto,
			   struct servent *res, char *buf, size_t buflen,
			   struct servent **res_sig) {
  int _res=-1;
  while (!getservent_r(res,buf,buflen,res_sig)) {
    int i;
    if (strcmp(res->s_proto,proto)) continue;
    if (!strcmp(res->s_name,name)) { _res=0; break; }
    for (i=0; res->s_aliases[i]; ++i)
      if (!strcmp(res->s_aliases[i],name)) { _res=0; goto found; /* C sucks */ }
  }
found:
  endservent();
  return _res;
}
